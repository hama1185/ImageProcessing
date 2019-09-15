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
				Ronly.R = trgb.R;//R�݂̂̒��o�̂Ƃ�trgb.R
				Ronly.G = 0;//G�݂̂̒��o�̂Ƃ�trgb.G
				Ronly.B = 0;//B�݂̂̒��o�̂Ƃ�trgb.B

				Gonly.R = 0;//R�݂̂̒��o�̂Ƃ�trgb.R
				Gonly.G = trgb.G;//G�݂̂̒��o�̂Ƃ�trgb.G
				Gonly.B = 0;//B�݂̂̒��o�̂Ƃ�trgb.B

				Bonly.R = 0;//R�݂̂̒��o�̂Ƃ�trgb.R
				Bonly.G = 0;//G�݂̂̒��o�̂Ƃ�trgb.G
				Bonly.B = trgb.B;//B�݂̂̒��o�̂Ƃ�trgb.B
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