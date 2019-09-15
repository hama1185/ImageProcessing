#pragma warning(disable: 4996)
#include "ppmload.h"
#define _CRT_SECURE_NO_WARNINGS


void cvtGray(struct ppmimg *src, struct ppmimg *dst, int sw);
void makeTable(int *table, int num);
int Max(int R, int G, int B);
int Min(int R, int G, int B);


void main(void){
	struct ppmimg *image1=NULL,*image2=NULL,*image3=NULL;
	int num = 0,sw = 0;
	printf("sw ?(range is from 0 to 3)\n");
	scanf("%d", &sw);

	image1 = makeimagestruct(image1);
	image2 = makeimagestruct(image2);
	image3 = makeimagestruct(image3);
	loadppmimage("nyan01.ppm",image1);
	cloneppmimage(image1,image2);
	image3 = createppmimage(image3, image1 -> iwidth, image1 -> iheight, image1 -> cmode);
	cvtGray(image1, image3, sw);

	deleteppmimg(image1);
	deleteppmimg(image2);
	deleteppmimg(image3);
	
}
int Max(int R, int G, int B) {
	int max;
	max = R;
	if (G > max) {
		max = G;
	}
	if (B > max) {
		max = B;
	}
	return max;
}

int Min(int R, int G, int B) {
	int min;
	min = R;
	if (G < min) {
		min = G;
	}
	if (B < min) {
		min = B;
	}
	return min;
}

void makeTable(int *table,int num){
	int j = 0;
	int *threshold = (int *)malloc(num * sizeof(int));
	for (int x = 0; x < num; x++) {
		if (x == 0) {
			threshold[x] = 0;
		}
		else if (x == (num - 1)) {
			threshold[x] = 255;
		}
		else {
			threshold[x] = x * (int)((256 / (num - 1)));
		}
	}
	for (int i = 0; i < 256; i++) {
		if (i == ((256 / num) * (j + 1))) {
			j++;
		}
		table[i] = threshold[j];
	}
		free(threshold);
}
	


void cvtGray(struct ppmimg *src, struct ppmimg *dst, int sw) {
	int *table;
	table = (int *)malloc(256 * sizeof(int));
	int num;
	printf("look up table range ?(range is from 2 to 128)\n");
	scanf("%d", &num);
	makeTable(table, num);
	switch (sw){
		case 0:
			int Y;
			for (int j = 0; j < src->iheight; j++) {
				for (int i = 0; i < src->iwidth; i++) {
					struct RGBColor trgb = getPnmPixel(src, i, j);
					if (src->cmode == 1) {
						trgb.dens = trgb.dens;
					}
					else {
						Y = trgb.G;
						trgb.R = table[Y];
						trgb.G = table[Y];
						trgb.B = table[Y];
					}
					setPnmPixel(dst, i, j, trgb);
				}
			}
			saveppmimage(dst, "nyanGray0.ppm");
			free(table);
			break;
		case 1:
			int g;
			for (int j = 0; j < src->iheight; j++) {
				for (int i = 0; i < src->iwidth; i++) {
					struct RGBColor trgb = getPnmPixel(src, i, j);
					if (src->cmode == 1) {
						trgb.dens = trgb.dens;
					}
					else {
						g = (int)((trgb.G + trgb.B + trgb.R)/ 3);
						trgb.R = table[g];
						trgb.G = table[g];
						trgb.B = table[g];
					}
					setPnmPixel(dst, i, j, trgb);
				}
			}
			saveppmimage(dst, "nyanGray1.ppm");
			free(table);
			break;
		case 2:
			int max, min;
			for (int j = 0; j < src->iheight; j++) {
				for (int i = 0; i < src->iwidth; i++) {
					struct RGBColor trgb = getPnmPixel(src, i, j);
					if (src->cmode == 1) {
						trgb.dens = trgb.dens;
					}
					else {
						max = Max(trgb.R, trgb.G, trgb.B);
						min = Min(trgb.R, trgb.G, trgb.B);
						Y = (int)((min + max) / 2);
						trgb.R = table[Y];
						trgb.G = table[Y];
						trgb.B = table[Y];
					}
					setPnmPixel(dst, i, j, trgb);
				}
			}
			saveppmimage(dst, "nyanGray2.ppm");
			free(table);
			break;
		case 3:
			for (int j = 0; j < src->iheight; j++) {
				for (int i = 0; i < src->iwidth; i++) {
					struct RGBColor trgb = getPnmPixel(src, i, j);
					if (src->cmode == 1) {
						trgb.dens = trgb.dens;
					}
					else {
						Y = (int)(0.298912 * trgb.R + 0.586611 * trgb.G + 0.114478 * trgb.B);
						trgb.R = table[Y];
						trgb.G = table[Y];
						trgb.B = table[Y];
					}
					setPnmPixel(dst, i, j, trgb);
				}
			}
			saveppmimage(dst, "nyanGray3.ppm");
			free(table);
			break;
		default:
			break;
	}
}