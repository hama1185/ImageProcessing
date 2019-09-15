#include "ppmload.h"
#define _CRT_SECURE_NO_WARNINGS

void main(void){
	struct ppmimg *image1=NULL,*image2=NULL,*imageR=NULL, *imageG = NULL, *imageB = NULL;;;

	image1 = makeimagestruct(image1);
	image2 = makeimagestruct(image2);
	imageR = makeimagestruct(imageR);
	imageG = makeimagestruct(imageG);
	imageB = makeimagestruct(imageB);
	loadppmimage("nikaidouman.ppm",image1);
	cloneppmimage(image1,image2);
	imageR = createppmimage(imageR, image1 -> iwidth, image1 -> iheight, image1 -> cmode);
	imageG = createppmimage(imageG, image1->iwidth, image1->iheight, image1->cmode);
	imageB = createppmimage(imageB, image1->iwidth, image1->iheight, image1->cmode);
	for(int j = 0; j < image1 -> iheight; j++){
		for(int i=0; i < image1 -> iwidth; i++){
			struct RGBColor trgb = getPnmPixel(image1,i,j);
			struct RGBColor Ronly = getPnmPixel(image1, i, j);
			struct RGBColor Gonly = getPnmPixel(image1, i, j);
			struct RGBColor Bonly = getPnmPixel(image1, i, j);
			if(image1->cmode == 1){
				trgb.dens = trgb.dens;
			}else{
				Ronly.R = trgb.R;//Rのみの抽出のときtrgb.R
				Ronly.G = 0;//Gのみの抽出のときtrgb.G
				Ronly.B = 0;//Bのみの抽出のときtrgb.B

				Gonly.R = 0;//Rのみの抽出のときtrgb.R
				Gonly.G = trgb.G;//Gのみの抽出のときtrgb.G
				Gonly.B = 0;//Bのみの抽出のときtrgb.B

				Bonly.R = 0;//Rのみの抽出のときtrgb.R
				Bonly.G = 0;//Gのみの抽出のときtrgb.G
				Bonly.B = trgb.B;//Bのみの抽出のときtrgb.B
			}
			setPnmPixel(imageR,i,j,Ronly);
			setPnmPixel(imageG, i, j, Gonly);
			setPnmPixel(imageB, i, j, Bonly);
		}
	}
	saveppmimage(imageR,"R_only.ppm");
	saveppmimage(imageG, "G_only.ppm");
	saveppmimage(imageB, "B_only.ppm");
	deleteppmimg(image1);
	deleteppmimg(image2);
	deleteppmimg(imageR);
}