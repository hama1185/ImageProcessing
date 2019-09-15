#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning(disable:4996)

struct RGBColor{
	union {
		struct {
			unsigned char R,G,B;
		};
		unsigned char dens;
	};
};


struct ppmimg{
	int iwidth;
	int iheight;
	int depth;
	int cmode;
	
	char magicnumber[10];//テキスト形式カラーとグレイスケールのみ(P3,P2)
	unsigned char* dat;
};


struct ppmimg *makeimagestruct(struct ppmimg* simg);
void loadppmimage(const char *imagename,struct ppmimg* simg);
void saveppmimage(struct ppmimg* simg,const char *imagename);

void setPnmPixel(struct ppmimg* simg,int x,int y,struct RGBColor _rgb);
struct RGBColor getPnmPixel(struct ppmimg* simg,int x,int y);
struct ppmimg *cloneppmimage(struct ppmimg* simg,struct ppmimg *dimage);
struct ppmimg *createppmimage(struct ppmimg* cimage,int width,int height,int mode);
void deleteppmimg(struct ppmimg *img);
void quick(int a[], int left, int right);
