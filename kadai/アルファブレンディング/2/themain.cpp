#include "ppmload.h"
#include "stdio.h"
#include "math.h"

void ChromaKey(struct ppmimg *front, struct ppmimg *back, struct ppmimg *synthesis);
void Green(struct ppmimg *image);

int main(){
	struct ppmimg *front = NULL, *back = NULL,  *synthesis = NULL;

	front = makeimagestruct(front);
	back = makeimagestruct(back);
	synthesis = makeimagestruct(synthesis);

	loadppmimage("nikaidouman.ppm", front);
	loadppmimage("saboten.ppm", back);

	Green(front);

	synthesis = createppmimage(synthesis, back->iwidth, back->iheight, back->cmode);

	ChromaKey(front, back, synthesis);

	saveppmimage(synthesis, "Chromakey.ppm");
	deleteppmimg(front);
	deleteppmimg(back);
	deleteppmimg(synthesis);
}

void ChromaKey(struct ppmimg *front, struct ppmimg *back, struct ppmimg *synthesis) {
	struct RGBColor f;
	struct RGBColor b;
	struct RGBColor set;

	for (int i = 0; i < back->iwidth; i++) {
		for (int j = 0; j < back->iheight; j++) {
			if (front->iwidth > i && front->iheight > j) {
				f = getPnmPixel(front, i, j);
			}
			else {
				f.R = 0;
				f.G = 255;
				f.B = 0;
			}

			b = getPnmPixel(back, i, j);

			if (f.R == 0 && f.G == 255 && f.B == 0) {
				set.R = b.R;
				set.G = b.G;
				set.B = b.B;
			}
			else {
				set.R = f.R;
				set.G = f.G;
				set.B = f.B;
			}

			setPnmPixel(synthesis, i, j, set);
		}
	}
}

void Green(struct ppmimg *image) {
	struct RGBColor copy;
	for (int i = 0; i < image->iwidth; i++) {
		for (int j = 0; j < image->iheight; j++) {
			copy = getPnmPixel(image, i, j);
			if (copy.R > 100 && copy.G > 100 && copy.B > 100) {
				copy.R = 0;
				copy.G = 255;
				copy.B = 0;
			}
			else {
				copy.R = copy.R;
				copy.G = copy.G;
				copy.B = copy.B;
			}
			setPnmPixel(image, i, j, copy);
		}
	}
}