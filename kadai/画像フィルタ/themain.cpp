#pragma source_character_set
#include "ppmload.h"
#include "stdio.h"
#include "math.h"

void Gaussian(struct ppmimg *image, struct ppmimg *changeImage);
void Sharpening(struct ppmimg *image, struct ppmimg *changeImage);
void Rank(struct ppmimg *image, struct ppmimg *changeImage, int rank);
void InitMatrix(int a[], int num);
void quick(int a[], int left, int right);
void Median(struct ppmimg *image, struct ppmimg *changeImage);
void Mode(struct ppmimg *image, struct ppmimg *changeImage);
int rtMode(int a[], int num, int current);

void main(void) {
	int rank;
	struct ppmimg *image1 = NULL, *image2 = NULL, *gaussianImage = NULL, *sharpeningImage = NULL, *rankImage = NULL, *medianImage = NULL,
		*modeImage = NULL;
	
	printf("input rank 1~9\n");
	scanf("%d", &rank);

	image1 = makeimagestruct(image1);
	image2 = makeimagestruct(image2);
	gaussianImage = makeimagestruct(gaussianImage);
	sharpeningImage = makeimagestruct(sharpeningImage);
	rankImage = makeimagestruct(rankImage);
	medianImage = makeimagestruct(medianImage);
	modeImage = makeimagestruct(modeImage);

	loadppmimage("inussg.pgm", image1);
	loadppmimage("inunoise.pgm", image2);

	gaussianImage = createppmimage(gaussianImage, image1->iwidth, image1->iheight, image1->cmode);
	sharpeningImage = createppmimage(sharpeningImage, image1->iwidth, image1->iheight, image1->cmode);
	rankImage = createppmimage(rankImage, image1->iwidth, image1->iheight, image1->cmode);
	medianImage = createppmimage(medianImage, image2->iwidth, image2->iheight, image2->cmode);
	modeImage = createppmimage(modeImage, image1->iwidth, image1->iheight, image1->cmode);
		
	Gaussian(image1, gaussianImage);
	Sharpening(image1, sharpeningImage);
	Rank(image1, rankImage, rank);
	Median(image2, medianImage);
	Mode(image1, modeImage);
	

	deleteppmimg(image1);
	deleteppmimg(image2);
	deleteppmimg(gaussianImage);
	deleteppmimg(sharpeningImage);
	deleteppmimg(rankImage);
	deleteppmimg(medianImage);
	deleteppmimg(modeImage);
	
}
void Gaussian(struct ppmimg *image, struct ppmimg *changeImage) {
	int matrix[3][3];
	matrix[0][0] = 1;
	matrix[1][0] = 2;
	matrix[2][0] = 1;
	matrix[0][1] = 2;
	matrix[1][1] = 4;
	matrix[2][1] = 2;
	matrix[0][2] = 1;
	matrix[1][2] = 2;
	matrix[2][2] = 1;

	struct RGBColor before, after;
	double densSum = 0;
	for (int h = 0; h < image->iheight; h++) {
		for (int w = 0; w < image->iwidth; w++) {
			densSum = 0;
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {
					if ((h != 0 && w != 0) && (h < image->iheight - 1 && w < image ->iwidth - 1)) {
						before = getPnmPixel(image, (w + x), (h + y));
						densSum += before.dens * matrix[1 + x][1 + y] / 16.0;
					}
				}
			}
			if (densSum > 255) {
				densSum = 255;
			}
			after.dens = (unsigned char)densSum;
			setPnmPixel(changeImage, w, h, after);
		}
	}
	saveppmimage(changeImage, "gaussian.pgm");
}

void Sharpening(struct ppmimg *image, struct ppmimg *changeImage) {
	int matrix[3][3];
	matrix[0][0] = 0;
	matrix[1][0] = -1;
	matrix[2][0] = 0;
	matrix[0][1] = -1;
	matrix[1][1] = 5;
	matrix[2][1] = -1;
	matrix[0][2] = 0;
	matrix[1][2] = -1;
	matrix[2][2] = 0;

	struct RGBColor before, after;
	int densSum = 0;
	for (int h = 0; h < image->iheight; h++) {
		for (int w = 0; w < image->iwidth; w++) {
			densSum = 0;
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {
					if ((h != 0 && w != 0) && (h < image->iheight - 1 && w < image->iwidth - 1)) {
						before = getPnmPixel(image, (w + x), (h + y));
						densSum += before.dens * matrix[1 + x][1 + y];
					}
				}
			}
			if (densSum > 255) {
				densSum = 255;
			}
			else if (densSum < 0) {
				densSum = 0;
			}
			after.dens = (unsigned char)densSum;
			setPnmPixel(changeImage, w, h, after);
		}
	}
	saveppmimage(changeImage, "sharpening.pgm");
}

void Rank(struct ppmimg *image, struct ppmimg *changeImage, int rank) {
	int matrix[9];
	int num;
	struct RGBColor before, after;
	int hmax = image->iheight - 1, wmax = image->iheight - 1;
	for (int h = 0; h < image->iheight; h++) {
		for (int w = 0; w < image->iwidth; w++) {
			InitMatrix(matrix, 9);
			num = 0;
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {
						if((w + x) < 0 || (h + y) < 0 || (w + x) > wmax || (h + y) > hmax){
							matrix[num] = 0;
						}
						else {
							before = getPnmPixel(image, (w + x), (h + y));
							matrix[num] = before.dens;
						}
					num++;
				}
			}
			quick(matrix, 0, 8);
			after.dens = matrix[rank - 1];
			setPnmPixel(changeImage, w, h, after);
		}
	}
	saveppmimage(changeImage, "rank.pgm");
}

void InitMatrix(int a[], int num) {
	for (int i = 0; i < num; i++) {
		a[i] = 0;
	}
}

void quick(int a[], int left, int right) {
	int pl = left;
	int pr = right;
	int pivot = a[(left + right) / 2];

	do {
		while (a[pl] < pivot) pl++;
		while (a[pr] > pivot) pr--;
		if (pl <= pr) {
			int k = a[pl];
			a[pl] = a[pr];
			a[pr] = k;
			pl++;
			pr--;
		}
	} while (pl <= pr);

	if (pr > left)	quick(a, left, pr);
	if (pl < right) quick(a, pl, right);

}

void Median(struct ppmimg *image, struct ppmimg *changeImage) {
	int matrix[9];
	int num;
	struct RGBColor before, after;
	int hmax = image->iheight - 1, wmax = image->iheight - 1;
	for (int h = 0; h < image->iheight; h++) {
		for (int w = 0; w < image->iwidth; w++) {
			InitMatrix(matrix, 9);
			num = 0;
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {
					if ((w + x) < 0 || (h + y) < 0 || (w + x) > wmax || (h + y) > hmax) {
						matrix[num] = 0;
					}
					else {
						before = getPnmPixel(image, (w + x), (h + y));
						matrix[num] = before.dens;
					}
					num++;
				}
			}
			quick(matrix, 0, 8);
			after.dens = matrix[4];
			setPnmPixel(changeImage, w, h, after);
		}
	}
	saveppmimage(changeImage, "median.pgm");
}

void Mode(struct ppmimg *image, struct ppmimg *changeImage) {
	int hist[256];
	int num;
	struct RGBColor before, after, current;
	int hmax = image->iheight - 1, wmax = image->iheight - 1;
	for (int h = 0; h < image->iheight; h++) {
		for (int w = 0; w < image->iwidth; w++) {
			InitMatrix(hist, 256);
			num = 0;
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {
					if ((w + x) < 0 || (h + y) < 0 || (w + x) > wmax || (h + y) > hmax) {
						hist[0]++;
					}
					else {
						before = getPnmPixel(image, (w + x), (h + y));
						hist[before.dens]++;
					}		
				}
			}
			current = getPnmPixel(image, w, h);
			after.dens = rtMode(hist, 256, (int)current.dens);
			setPnmPixel(changeImage, w, h, after);
		}
	}
	saveppmimage(changeImage, "mode.pgm");
}

int rtMode(int a[], int num, int current) {
	int mode = 0;
	int max = 0;
	for (int i = 0; i < num;i++) {
		if (a[i] > max) {
			max = a[i];
			mode = i;
		}
	}
	if (max == 1) {
		mode = current;
	}
	return mode;
}


