#include "ppmload.h"


struct CMYColor {
	unsigned char C, M, Y;
};

struct CMYKColor {
	unsigned char C, M, Y, K;
};

struct HSVColor {
	int H;
	unsigned char S, V;
};

unsigned char Min(unsigned char a, unsigned char b, unsigned char c) {
	unsigned char min = a;
	if(b < min){
		min = b;
	}
	if(c < min){
		min = c;
	}	
	return min;
}

unsigned char Max(unsigned char a, unsigned char b, unsigned char c) {
	unsigned char max = a;
	if(max < b){
		max = b;
	}
	if(max < c){ 
		max = c;
	}
	return max;
}

void fromRGBtoCMY(struct RGBColor rgb, struct CMYColor *cmy){
	cmy->C = 255 - rgb.R;
	cmy->M = 255 - rgb.G;
	cmy->Y = 255 - rgb.B;
}

void fromCMYtoRGB(struct CMYColor cmy, struct RGBColor *rgb){
	rgb->R = 255 - cmy.C;
	rgb->G = 255 - cmy.M;
	rgb->B = 255 - cmy.Y;
}

void fromRGBtoCMYK(struct RGBColor rgb, struct CMYKColor *cmyk){
	cmyk->K = Min(255 - rgb.R, 255 - rgb.G, 255 - rgb.B);
	cmyk->C = 255 - rgb.R - cmyk->K;
	cmyk->M = 255 - rgb.G - cmyk->K;
	cmyk->Y = 255 - rgb.B - cmyk->K;
}

void fromCMYKtoRGB(struct CMYKColor cmyk, struct RGBColor *rgb){
	struct RGBColor argb;
	argb.R = 255 - cmyk.C - cmyk.K;
	argb.G = 255 - cmyk.M - cmyk.K;
	argb.B = 255 - cmyk.Y - cmyk.K;

	if(argb.R > 0){
		rgb->R = argb.R;
	}
	else{
		rgb->R = 0; 
	}
	if(argb.G > 0){
		rgb->G = argb.G;
	}
	else{
		rgb->G = 0;
	}
	if(argb.B > 0){
		rgb->B = argb.B;
	}	
	else{
		rgb->B = 0;
	} 

}
void fromRGBtoHSV(struct RGBColor rgb, struct HSVColor *hsv) {
	unsigned char max, min;
	max = Max(rgb.R, rgb.G, rgb.B);
	min = Min(rgb.R, rgb.G, rgb.B);

	hsv->S = (max - min) / max;
	hsv->V = max;

	if(rgb.R == rgb.G && rgb.R == rgb.B){
		hsv->H = 0;
	}
	else if(max == rgb.R){
		hsv->H = 60 * (rgb.G - rgb.B) / (max - min);
	}
	else if(max == rgb.G){
		hsv->H = 60 * (rgb.B - rgb.R) / (max - min) + 120;
	}
	else if(max == rgb.B){
		hsv->H = 60 * (rgb.R - rgb.G) / (max - min) + 240;
	}


	if(hsv->H < 0){
		hsv->H += 360; 
	}

}

void fromHSVtoRGB(struct HSVColor hsv, struct RGBColor *rgb) {
	double max, min;
	max = hsv.V;
	min = max - (((double)hsv.S / 255) * max);


	if (hsv.H >= 0 && hsv.H < 60) {
		rgb->R = (unsigned char)max;
		rgb->G = (unsigned char)(((double)hsv.H / 60) * (max - min) + min);
		rgb->B = (unsigned char)min;
	}
	else if (hsv.H >= 60 && hsv.H < 120) {
		rgb->R = (unsigned char)(((120 - (double)hsv.H) / 60) * (max - min) + min);
		rgb->G = (unsigned char)max;
		rgb->B = (unsigned char)min;
	}
	else if (hsv.H >= 120 && hsv.H < 180) {
		rgb->R = (unsigned char)min;
		rgb->G = (unsigned char)max;
		rgb->B = (unsigned char)((((double)hsv.H - 120) / 60) * (max - min) + min);
	}
	else if (hsv.H >= 180 && hsv.H < 240) {
		rgb->R = (unsigned char)min;
		rgb->G = (unsigned char)(((240 - (double)hsv.H) / 60) * (max - min) + min);
		rgb->B = (unsigned char)max;
	}
	else if (hsv.H >= 240 && hsv.H < 300) {
		rgb->R = (unsigned char)((((double)hsv.H - 240) / 60) * (max - min) + min);
		rgb->G = (unsigned char)min;
		rgb->B = (unsigned char)max;
	}
	else if (hsv.H >= 300 && hsv.H < 360) {
		rgb->R = (unsigned char)max;
		rgb->G = (unsigned char)min;
		rgb->B = (unsigned char)(((360 - (double)hsv.H) / 60) * (max - min) + min);
	}
	else {
		rgb->R = (unsigned char)min;
		rgb->G = (unsigned char)min;
		rgb->B = (unsigned char)min;
	}


}

void rot(struct HSVColor *hsv) {
	hsv->H = hsv->H + 180;
	if(hsv->H > 360){
		hsv->H -= 360;
	}
}


void main(void){
	struct ppmimg *image1 = NULL, *image2 = NULL, *image3 = NULL, *image4 = NULL;

	image1 = makeimagestruct(image1);
	image2 = makeimagestruct(image2);
	image3 = makeimagestruct(image3);
	image4 = makeimagestruct(image4);


	loadppmimage("Balloon.ppm", image1);
	image2 = createppmimage(image2, image1->iwidth, image1->iheight, image1->cmode);
	image3 = createppmimage(image3, image1->iwidth, image1->iheight, image1->cmode);
	image4 = createppmimage(image4, image1->iwidth, image1->iheight, image1->cmode);

	for (int j = 0; j < image1->iheight; j++) {
		for (int i = 0; i < image1->iwidth; i++) {
			struct RGBColor trgb;
			struct HSVColor thsv;
			unsigned char copy;

			trgb = getPnmPixel(image1, i, j);
			fromRGBtoHSV(trgb, &thsv);
			copy = thsv.S;
			thsv.S = 255;
			fromHSVtoRGB(thsv, &trgb);
			setPnmPixel(image2, i, j, trgb);

			thsv.S = copy;
			rot(&thsv);
			fromHSVtoRGB(thsv, &trgb);
			setPnmPixel(image3, i, j, trgb);

			rot(&thsv);

			if (thsv.H > 45 && thsv.H < 280) {
				if (thsv.S < 8 || thsv.S > 50) {
					if (thsv.V < 70 || thsv.V > 95) {
						thsv.H = 0;
						thsv.S = 0;
						thsv.V = 0;
					}
				}
			}
			fromHSVtoRGB(thsv, &trgb);
			setPnmPixel(image4, i, j, trgb);
		}
	}

	saveppmimage(image2, "A.ppm");
	saveppmimage(image3, "B.ppm");
	saveppmimage(image4, "C.ppm");

	deleteppmimg(image1);
	deleteppmimg(image2);
	deleteppmimg(image3);
	deleteppmimg(image4);

	
}