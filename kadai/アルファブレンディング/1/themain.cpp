#include "ppmload.h"
#include "stdio.h"
#include "math.h"

void AlphaBlend(struct ppmimg *front, struct ppmimg *gray, struct ppmimg *back, struct ppmimg *synthesis);


int main(){
	struct ppmimg *front = NULL, *gray = NULL,  *back = NULL,  *synthesis = NULL;

	front = makeimagestruct(front);
	gray = makeimagestruct(gray);
	back = makeimagestruct(back);
	synthesis = makeimagestruct(synthesis);

	loadppmimage("saboten.ppm", front);
	loadppmimage("nyanGray0.ppm", gray);
	loadppmimage("nikaidouman.ppm", back);

	synthesis = createppmimage(synthesis, back->iwidth, back->iheight, back->cmode);

	AlphaBlend(front, gray, back, synthesis);

	saveppmimage(synthesis, "AlphaBlend.ppm");

	deleteppmimg(front);
	deleteppmimg(gray);
	deleteppmimg(back);
	deleteppmimg(synthesis);
}

void AlphaBlend(struct ppmimg *front, struct ppmimg *gray, struct ppmimg *back, struct ppmimg *synthesis) {
	struct RGBColor f;	//前面画像の画素値
	struct RGBColor g;	//アルファ値用のグレースケール画像の画素値
	struct RGBColor b;	//背面画像の画素値
	struct RGBColor s;	//合成画像の画素値
	double alpha;

	for (int i = 0; i < back->iwidth; i++) {
		for (int j = 0; j < back->iheight; j++) {

			if (front->iwidth > i && front->iheight > j) {
				f = getPnmPixel(front, i, j);
				g = getPnmPixel(gray, i, j);
			}
			else {
				f.R = 0;
				f.G = 0;
				f.B = 0;
				g.dens = 0;
			}

			b = getPnmPixel(back, i, j);
			alpha = (double)g.dens / 255;

			s.R = (unsigned char)((1 - alpha) * b.R + alpha * f.R);
			s.G = (unsigned char)((1 - alpha) * b.G + alpha * f.G);
			s.B = (unsigned char)((1 - alpha) * b.B + alpha * f.B);

			setPnmPixel(synthesis, i, j, s);
		}
	}
}