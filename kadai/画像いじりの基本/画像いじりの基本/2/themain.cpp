#include "ppmload.h"

void main(void){
	struct ppmimg *image1=NULL,*image2=NULL,*image3=NULL, *image4 = NULL;
	//ave‚Í‡@
	//pic‚Í‡A
	int ave;
	double pic;
	image1 = makeimagestruct(image1);
	image2 = makeimagestruct(image2);
	image3 = makeimagestruct(image3);
	image4 = makeimagestruct(image4);
	loadppmimage("nikaidouman.ppm",image1);
	cloneppmimage(image1,image2);
	image3 = createppmimage(image3,image1->iwidth,image1->iheight,image1->cmode);
	image4 = createppmimage(image4, image1->iwidth, image1->iheight, image1->cmode);
	for(int j=0;j<image1->iheight;j++){
		for(int i=0;i<image1->iwidth;i++){
			struct RGBColor trgb = getPnmPixel(image1,i,j);
			struct RGBColor one = getPnmPixel(image1, i, j);
			struct RGBColor two = getPnmPixel(image1, i, j);
			if(image1->cmode == 1){
				trgb.dens = trgb.dens;
			}else{
				ave = (trgb.R + trgb.G + trgb.B) / 3;
				pic = trgb.R * 0.2999 + trgb.G * 0.587 + trgb.B * 0.114;
				(int)pic;
				if (pic > 255){
					pic = 255;
				}
				one.R = ave;
				one.G = ave;
				one.B = ave;

				two.R = pic;
				two.G = pic;
				two.B = pic;

				(double)pic;
			}
			setPnmPixel(image3,i,j,one);
			setPnmPixel(image4, i, j, two);
		}
	}
	saveppmimage(image3,"ave1.ppm");
	saveppmimage(image4,"pic2.ppm");
	deleteppmimg(image1);
	deleteppmimg(image2);
	deleteppmimg(image3);
	deleteppmimg(image4);
}