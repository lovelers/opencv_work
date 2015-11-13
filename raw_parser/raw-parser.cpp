#include <stdio.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace std;

// output is 12bit raw; need transfer to 8bit raw
void getTwoPixel(FILE *fp, int *first, int *second) {
    int a = fgetc(fp);
    int b = fgetc(fp);
    int c = fgetc(fp);

#if 0
    *first = (((a & 0xFF) << 4) | ((b & 0xFF) >> 4)) >> 4;
    *second = (((b & 0xFF) << 8) | (c & 0xFF)) >> 4;
#endif
    *first = (a & 0xFF);
    *second = ((b & 0xFF) << 4) | ( c & 0xFF) >> 4;
}

int main( int argc, char ** argv) {
    FILE *fp = fopen("raw.buffer", "r");
    FILE *save = fopen("imx230.raw", "w+");
    int sensorW = 5312;
    int sensorH = 3984;
    Mat img(sensorH, sensorW, CV_8UC1, Scalar(0));
    {
        int pixel_1, pixel_2;
        MatIterator_<uchar> it = img.begin<uchar>(), it_end = img.end<uchar>();
        int i = 0;
        int total = sensorH  * sensorW;
        while (i < total) {
            getTwoPixel(fp, &pixel_1, &pixel_2);
            *it++ = (uchar)pixel_1 ;
            fputc(pixel_1, save);
            *it++ = (uchar)pixel_2 ;
            fputc(pixel_2, save);
            i += 2;
        }
        fclose(fp);
        fclose (save);
    }

    if (0){
        FILE *resave = fopen("imx230_800x600.raw", "w+");
        Mat resize_img;
        resize(img, resize_img, Size(800, 600));
        MatIterator_<uchar> it = img.begin<uchar>(), it_end = img.end<uchar>();
        while (it != it_end) {
            fputc(*it, resave);
            it++;
        }
        fflush(resave);
        fclose(resave);
    }

    //Mat img_rgb(sensorW, sensorH, CV_8UC3, Scalar(0, 0, 0));
    Mat img_gray(sensorH, sensorW, CV_8UC1, Scalar(0));
    cvtColor(img, img_gray, COLOR_BayerGB2GRAY);

    Mat resize_gray;
    resize(img_gray, resize_gray, Size(800, 600));
    cvNamedWindow("rgb");
    namedWindow("rgb", WINDOW_AUTOSIZE);
    imshow("rgb", resize_gray);
    waitKey();
    return 0;
}

