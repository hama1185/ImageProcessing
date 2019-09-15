#include "ppmload.h"
#define _CRT_SECURE_NO_WARNINGS 1

int main(void) {
	FILE *fp[3];
	fp[0] = fopen("outputData1.txt", "wt");
	fp[1] = fopen("outputData2.txt", "wt");
	fp[2] = fopen("outputData3.txt", "wt");

	struct ppmimg *image1 = NULL;
	double sum[3] = {0},sqsum[3] = {0}, ave[3] = {0}, sqave[3] = {0}, var[3] = {0};
	int max[3] = {0}, min[3] = {255}, mode[3] = {0}, x = 0, med2[3];
	

	int hist[3][256] = {0};//ヒストグラム
	int *medR;
	int *medG;
	int *medB;
	char rgb[3][10] = {"Red", "Green", "Blue"};

	image1 = makeimagestruct(image1);
	
	loadppmimage("Balloon.ppm", image1);

	medR = (int*)calloc(image1->iwidth * image1->iheight, sizeof(int));
	medG = (int*)calloc(image1->iwidth * image1->iheight, sizeof(int));
	medB = (int*)calloc(image1->iwidth * image1->iheight, sizeof(int));

	for(int i = 0; i < image1->iwidth; i++){
		for(int j = 0; j < image1->iheight; j++){
			struct RGBColor data = getPnmPixel(image1, i, j);

			sum[0] += data.R;
			sqsum[0] += data.R * data.R;
			if(max[0] < data.R){
                max[0] = data.R;
            }
			if(min[0] > data.R){
                min[0] = data.R;
            }	
			medR[x] = data.R;
			hist[0][data.R]++;

			sum[1] += data.G;
			sqsum[1] += data.G * data.G;
			if(max[1] < data.G){
                max[1] = data.G;
            }
			if(min[1] > data.G){
                min[1] = data.G;
            }
			medG[x] = data.G;
			hist[1][data.G]++;

			sum[2] += data.B;
			sqsum[2] += data.B * data.B;
			if(max[2] < data.B){
                max[2] = data.B;
            }
			if(min[2] > data.B){
                min[2] = data.B;
            }
			medB[x] = data.B;
			hist[2][data.B]++;

			x++;
		}
	}

	for (int i = 0; i < 3; i++){
		ave[i] = sum[i] / x;
		sqave[i] = sqsum[i] / x;
		var[i] = sqave[i] - ave[i] * ave[i];
	}
	
	quick(medR, 0, image1->iwidth * image1->iheight - 1);
	quick(medG, 0, image1->iwidth * image1->iheight - 1);
	quick(medB, 0, image1->iwidth * image1->iheight - 1);

	if(image1->iwidth * image1->iheight % 2){
        med2[0] = medR[(image1->iwidth * image1->iheight) / 2];
    }
	else{
        med2[0] = (medR[(image1->iwidth * image1->iheight) / 2] + medR[(image1->iwidth * image1->iheight) / 2 - 1]) / 2;
    }

	if(image1->iwidth * image1->iheight % 2){
        med2[1] = medG[(image1->iwidth * image1->iheight) / 2];
    }
	else{
        med2[1] = (medG[(image1->iwidth * image1->iheight) / 2] + medG[(image1->iwidth * image1->iheight) / 2 - 1]) / 2;
    }

	if(image1->iwidth * image1->iheight % 2){
        med2[2] = medB[(image1->iwidth * image1->iheight) / 2];
    }
	else{
        med2[2] = (medB[(image1->iwidth * image1->iheight) / 2] + medB[(image1->iwidth * image1->iheight) / 2 - 1]) / 2;
    }

	for(int i = 0; i < 3; i++){
		fprintf(fp[i], "%s\n", rgb[i]);
		fprintf(fp[i], "濃度平均:%5d\n", (int)ave[i]);
		fprintf(fp[i], "分散:%5d\n", (int)var[i]);
		fprintf(fp[i], "最大値:%5d\n", max[i]);
		fprintf(fp[i], "最小値:%5d\n", min[i]);
		fprintf(fp[i], "中央値:%5d\n", med2[i]);
		fprintf(fp[i], "\nヒストグラム\n");

		for(int j = 0; j < 256; j++){
			fprintf(fp[i], "%3d:", j);

			for(int m = 0; m < hist[i][j]; m++){
                fprintf(fp[i], "*");
            }
			fprintf(fp[i], "\n");

			for(int m = 0; m < 256; m++){
				if (hist[i][mode[i]] < hist[i][m]){
                    mode[i] = m;
                }
			}
		}
		fprintf(fp[i], "最頻値:%5d\n\n", mode[i]);
	}

	free(medR);
	free(medG);
	free(medB);

	deleteppmimg(image1);
	fclose(fp[0]);
	fclose(fp[1]);
	fclose(fp[2]);

	return 0;
}