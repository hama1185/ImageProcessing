#include "ppmload.h"
#define _CRT_SECURE_NO_WARNINGS 1

int main(void){
	FILE *fp;
	fp = fopen("outputData.txt", "wt");

	struct ppmimg *image1 = NULL;
	double sum = 0, sqsum = 0,ave = 0,/*濃度平均*/sqave = 0,/*二乗平均*/var = 0;/*分散*/

	int max = 0, min = 255,mode = 0,/*最頻値*/x = 0, med2;/*中央値*/

	int hist[256] = {0};
	int *med;

	image1 = makeimagestruct(image1);
	loadppmimage("pic2.ppm", image1);
	med = (int*)calloc(image1->iwidth * image1->iheight, sizeof(int));

	for(int i = 0; i < image1->iwidth; i++){
		for(int j = 0; j < image1->iheight; j++){
			struct RGBColor data = getPnmPixel(image1, i, j);
			sum += data.R;
			sqsum += data.R * data.R;
			if(max < data.R){
                max = data.R;
            }
			if(min > data.R){
                min = data.R;
            }
			med[x] = data.R;
			hist[data.R]++;
			x++;
		}
	}

	ave = sum / x;
	sqave = sqsum / x;
	var = sqave - ave * ave;
	quick(med, 0, image1->iwidth * image1->iheight - 1);//クイックソート
	if(image1->iwidth * image1->iheight % 2){
        med2 = med[(image1->iwidth * image1->iheight) / 2];
    }
	else{
        med2 = (med[(image1->iwidth * image1->iheight) / 2] + med[(image1->iwidth * image1->iheight) / 2 - 1]) / 2;
    } 


	fprintf(fp, "濃度平均:%5d\n", (int)ave);
	fprintf(fp, "分散:%5d\n", (int)var);
	fprintf(fp, "最大値:%5d\n", max);
	fprintf(fp, "最小値:%5d\n", min);
	fprintf(fp, "中央値:%5d\n", med2);
	fprintf(fp, "\nヒストグラム\n");
	for(int i = 0; i < 256; i++){
		fprintf(fp, "%3d:", i);
		for(int j = 0; j < hist[i]; j++){
            fprintf(fp, "*");
        }
		fprintf(fp, "\n");
	}

	for(int i = 1; i < 256; i++){
		if(hist[mode] < hist[i]){
            mode = i;
        }
	}
	fprintf(fp, "モード:%5d\n\n", mode);

	free(med);
	deleteppmimg(image1);
	fclose(fp);

	return 0;
}