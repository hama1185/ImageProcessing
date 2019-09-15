#pragma source_character_set
#include "ppmload.h"

void NN(struct ppmimg* before, struct ppmimg* after, double ratio);
void BL(struct ppmimg* before, struct ppmimg* after, double ratio);
void BC(struct ppmimg* before, struct ppmimg* after, double ratio);
double rt(double t);

void main(void) {
	FILE *fp;
	fp = fopen("out.txt", "wt");
	clock_t start, end;
	start = clock();
	struct ppmimg *image1 = NULL, *image2 = NULL, *image3 = NULL;

	image1 = makeimagestruct(image1);
	image2 = makeimagestruct(image2);
	image3 = makeimagestruct(image3);

	loadppmimage("twitter.ppm", image1);

	image2 = createppmimage(image2, (int)(image1->iwidth * 0.3), (int)(image1->iheight * 0.3), image1->cmode);
	image3 = createppmimage(image3, (int)(image1->iwidth * 2.7), (int)(image1->iheight * 2.7), image1->cmode);

	//NN(image1, image2, 0.3);
	//NN(image1, image3, 2.7);
	//BL(image1, image2, 0.3);
	//BL(image1, image3, 2.7);
	BC(image1, image2, 0.3);
	BC(image1, image3, 2.7);

	//saveppmimage(image2, "twiNN1.ppm");
	//saveppmimage(image3, "twiNN2.ppm");
	//saveppmimage(image2, "twiBL1.ppm");
	//saveppmimage(image3, "twiBL2.ppm");
	saveppmimage(image2, "twiBC1.ppm");
	saveppmimage(image3, "twiBC2.ppm");

	deleteppmimg(image1);
	deleteppmimg(image2);
	deleteppmimg(image3);

	end = clock();
	fprintf(fp, "計算時間: %.2fms", (double)end - (double)start);

	fclose(fp);

}

void NN(struct ppmimg* before, struct ppmimg* after, double ratio) {
	int i, j;

	for (i = 0; i < after->iwidth; i++) {
		for (j = 0; j < after->iheight; j++) {
			struct RGBColor rgb;
			if (before->iwidth > floor(i / ratio) && before->iheight > floor(j / ratio) && ratio >= 1) {
				rgb = getPnmPixel(before, floor(i / ratio), floor(j / ratio));
			}
			else if (before->iwidth > floor(i / ratio) && before->iheight > floor(j / ratio) && ratio < 1) {
				rgb = getPnmPixel(before, floor(i / ratio), floor(j / ratio));
			}
			else {
				rgb = getPnmPixel(before, 0, 0);
			}
			setPnmPixel(after, i, j, rgb);
		}
	}
}


void  BL(struct ppmimg* before, struct ppmimg* after, double ratio) {
	struct RGBColor I[2][2];
	double fx, fy, dx, dy, rj, gj, bj;
	int i, j;

	for (i = 0; i < after->iwidth; i++) {
		for (j = 0; j < after->iheight; j++) {
			struct RGBColor rgb;

			fx = i / ratio;
			fy = j / ratio;

			if (floor(fx) + 1 < before->iwidth &&  floor(fy) + 1 < before->iheight) {

				I[0][0] = getPnmPixel(before, floor(fx), floor(fy));
				I[1][0] = getPnmPixel(before, floor(fx) + 1, floor(fy));
				I[0][1] = getPnmPixel(before, floor(fx), floor(fy) + 1);
				I[1][1] = getPnmPixel(before, floor(fx) + 1, floor(fy) + 1);
			}
			else {
				I[0][0] = getPnmPixel(before, 0, 0);
				I[1][0] = getPnmPixel(before, 0, 0);
				I[0][1] = getPnmPixel(before, 0, 0);
				I[1][1] = getPnmPixel(before, 0, 0);
			}

			dx = fx - floor(fx);
			dy = fy - floor(fy);


			rj = ((1 - dx) * (1 - dy) * I[0][0].R + dx * (1 - dy) * I[1][0].R + (1 - dx) * dy * I[0][1].R + dx * dy * I[1][1].R);
			gj = ((1 - dx) * (1 - dy) * I[0][0].G + dx * (1 - dy) * I[1][0].G + (1 - dx) * dy * I[0][1].G + dx * dy * I[1][1].G);
			bj = ((1 - dx) * (1 - dy) * I[0][0].B + dx * (1 - dy) * I[1][0].B + (1 - dx) * dy * I[0][1].B + dx * dy * I[1][1].B);

			if ((rj < 256 && rj >= 0) && (gj < 256 && gj >= 0) && (bj < 256 && bj >= 0)) {
				rgb.R = (unsigned char)rj;
				rgb.G = (unsigned char)gj;
				rgb.B = (unsigned char)bj;
			}
			else {
				rgb.R = 0;
				rgb.G = 0;
				rgb.B = 0;
			}

			setPnmPixel(after, i, j, rgb);
		}
	}
}


void BC(struct ppmimg* before, struct ppmimg* after, double ratio) {
	double x, y;
	int X, Y;
	double weight;
	double dens;
	int count = 0;

	for (int j = 0; j < after->iwidth; j++) {
		for (int i = 0; i < after->iheight; i++) {
		
			struct RGBColor rgb;

			x = i / ratio;
			y = j / ratio;

			int int_x = (int)x;
			int int_y = (int)y;

			int R, G, B;
			R = G = B = 0;
			weight = 0;

			for (int dx = -1; dx <= 2; dx++) {
				for (int dy = -1; dy <= 2; dy++) {
					X = int_x + dx;
					Y = int_y + dy;
					if (X < 0) {
						X = 0;
					}
					if (X >= before->iheight) {
						X = before->iheight - 1;
					}
					if (Y < 0) {
						Y = 0;
					}
					if (Y >= before->iwidth) {
						Y = before->iwidth - 1;
					}
					weight = rt(X - x);
					weight *= rt(Y - y);

					rgb = getPnmPixel(before, X, Y);

					R += (double)(rgb.R * weight);
					G += (double)(rgb.G * weight);
					B += (double)(rgb.B * weight);
				}
			}

			if (R > 255) {
				R = 255;
			}
			else if (R < 0) {
				R = 0;
			}
			if (G > 255) {
				G = 255;
			}
			else if (G < 0) {
				G = 0;
			}
			if (B > 255) {
				B = 255;
			}
			else if (B < 0) {
				B = 0;
			}

			rgb.R = (unsigned char)R;
			rgb.G = (unsigned char)G;
			rgb.B = (unsigned char)B;
			setPnmPixel(after, i, j, rgb);
		}
	}
}

double rt(double t) {

	double a = -1.0;
	t = fabs(t);

	if (t <= 1.0) {
		return 1.0 - (a + 3.0) * pow(t, 2) + (a + 2) * pow(t, 3);
	}
	else if (t <= 2.0) {
		return -4.0 * a + 8.0 * a * t - 5.0 * a * pow(t, 2) + a * pow(t, 3);
	}
	else {
		return 0;
	}
}