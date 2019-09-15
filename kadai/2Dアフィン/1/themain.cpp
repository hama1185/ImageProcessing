#include "ppmload.h"
#include "stdio.h"
#include "math.h"
#define PI  3.141592

void BC(struct ppmimg* before, struct ppmimg* after, double ratio);
double rt(double t);
void AfinTransform(struct ppmimg* before, struct ppmimg* after, int x, int y, double ratio, double rot);

int main(){
	struct ppmimg *before = NULL, *after = NULL;
	
	before = makeimagestruct(before);
	after = makeimagestruct(after);
	loadppmimage("twitter.pgm", before);
	
	AfinTransform(before, after, 0, 0, 3, 270);

	saveppmimage(after, "afine.pgm");

	deleteppmimg(before);
	deleteppmimg(after);
	return 0;
}

void BC(struct ppmimg* before, struct ppmimg* after, double ratio) {
	double x, y;
	int X, Y;
	double weight;
	double dens;
	int count = 0;

	for (int j = 0; j < after->iwidth; j++) {
		for (int i = 0; i < after->iheight; i++) {

			struct RGBColor trgb;

			x = i / ratio;
			y = j / ratio;

			int int_x = (int)x;
			int int_y = (int)y;

			int dens = 0;
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

					trgb = getPnmPixel(before, X, Y);

					dens += (double)(trgb.dens * weight);
				}
			}

			if (dens > 255) {
				dens = 255;
			}

			if (dens < 0) {
				dens = 0;
			}
			trgb.dens = (unsigned char)dens;
			setPnmPixel(after, i, j, trgb);
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
void AfinTransform(struct ppmimg* before, struct ppmimg* after, int x, int y, double ratio, double rot) {
	/*パラメータの説明
	　x,yは元画像からの変位(xは横移動,yは縦移動)
	  ratioは拡大縮小比率
	  rotは回転角度(左回り,単位は度数法)
	  */
	struct ppmimg *sub = NULL;
	sub = makeimagestruct(sub);
	sub = createppmimage(sub, (int)(before->iwidth * ratio), (int)(before->iheight * ratio), before->cmode);

	BC(before, sub, ratio);	//BC補間法で画像の拡大縮小

	after = createppmimage(after, sub->iwidth, sub->iheight, sub->cmode);

	int **flag; //参照済みの座標か否かの判定変数
	double a[2][3]; //アフィン変換の行列の要素の値 
	double b[3]; //アフィン変換する座標
	double c[2]; //アフィン変換後の座標
	struct RGBColor gray;
	struct RGBColor init; //画像が存在しないところを黒(GrayScaleの値で0)に変換
	init.dens = 0;
	rot = rot * PI / 180; //弧度法に変換
	a[0][0] = cos(rot);  //アフィン変換の行列式
	a[0][1] = sin(rot);	//軸を右に回転させることで画像を左向きへ相対的に回転
	a[0][2] = (double)x;
	a[1][0] = -sin(rot);
	a[1][1] = cos(rot);
	a[1][2] = (double)y;


	flag = (int **)malloc(sizeof(int *) * after->iwidth);	//フラグ変数の領域確保
	for (int i = 0; i < after->iwidth; i++) {
		flag[i] = (int *)malloc(sizeof(int) * after->iheight);
	}

	for (int i = 0; i < after->iwidth; i++) {	//フラグの初期化
		for (int j = 0; j < after->iheight; j++) {
			flag[i][j] = 0;
		}
	}

	for (int i = 0; i < after->iwidth; i++) {		//アフィン変換処理部
		for (int j = 0; j < after->iheight; j++) {
			gray = getPnmPixel(sub, i, j);

			/*回転軸を画像の中心に移動*/
			b[0] = i - after->iwidth / 2;
			b[1] = j - after->iheight / 2;
			b[2] = 1;

			for (int k = 0; k < 2; k++) {
				c[k] = 0;
			}

			for (int l = 0; l < 2; l++) {
				for (int m = 0; m < 3; m++) {
					c[l] += (a[l][m] * b[m]);
				}
			}

			c[0] = c[0] + after->iwidth / 2;
			c[1] = c[1] + after->iheight / 2;


			if (c[0] < after->iwidth && c[0] >= 0) {
				if (c[1] < after->iheight && c[1] >= 0) {
					setPnmPixel(after, (int)c[0], (int)c[1], gray);
					flag[(int)c[0]][(int)c[1]] = 1;
				}
			}
		}
	}

	/*フラグが立っていない部分を黒に塗りつぶす*/
	for (int i = 0; i < after->iwidth; i++) {
		for (int j = 0; j < after->iheight; j++) {
			if (flag[i][j] == 0) {
				setPnmPixel(after, i, j, init);
			}
		}
	}
}