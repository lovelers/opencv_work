#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

#define LENA_JPG "../data/lena.jpg"

int main(int argc, char ** argv) {
    const char *imgname = argc > 1 ? argv[1] : LENA_JPG;
    Mat src = imread(imgname); // use the IMREAD_COLOR default at imgcodecs.hpp. convert image to the 3 channel BGR color image

    if (src.empty()) {
        cout << "read" << imgname << "fail" << endl;
        return -1;
    }
    vector<Mat> bgr_planes;
    split(src, bgr_planes);

    const int histSize = 256;
    const int channel = 3;
    
    int bin[channel][histSize] = {0};

    int y, x, c;

    for (y = 0; y < src.rows; ++y) {
        for (x = 0; x < src.cols; ++x) {
            for (c = 0; c < channel; ++c) {
                ++bin[c][(int)bgr_planes[c].at<uchar>(y, x)];
            }
        }
    }

    int count = src.rows * src.cols;

    int total = 0;
    for (int i = 0; i < histSize; i++) {
        total += bin[2][i];
    }
    if (total != count) {
        fprintf(stderr, "total ; %d, count %d", total, count );
        return -1;
    }
    fprintf(stderr, "total ; %d, count %d", total, count );
    float bin_ratio[channel][histSize] = { 0.f};
    float max_ratio[channel] = {0.f};
    for (int i = 0; i < histSize; ++i) {
        for (int j = 0; j < channel; ++j) {
            bin_ratio[j][i] = (float)bin[j][i] / count;
            if (max_ratio[j] < bin_ratio[j][i])
                max_ratio[j] = bin_ratio[j][i];
        }
    }

    fprintf(stderr, "max_ratio. b:g:r %f, %f, %f \n", max_ratio[0], max_ratio[1], max_ratio[2]);
    int hist_w = 512; int hist_h = 400;
    int multi_ratio[channel] = {1};
    for (int i = 0; i < channel; ++i) {
        multi_ratio[i] = (int)hist_h / max_ratio[i];
    }
    fprintf(stderr, "multi_ratio. b:g:r %f, %f, %f \n", multi_ratio[0], multi_ratio[1], multi_ratio[2]);

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255 ,255));
    vector<Mat> hist_planes;
    split(histImage, hist_planes);

    for (int i = 0; i < 256; i++) {
        hist_planes[0].at<uchar>((int)(bin_ratio[0][i] * multi_ratio[0]), (int)(i * hist_w / 256)) = 255;
        hist_planes[0].at<uchar>((int)(bin_ratio[0][i] * multi_ratio[0]), (int)(i * hist_w / 256)) = 0;
        hist_planes[0].at<uchar>((int)(bin_ratio[0][i] * multi_ratio[0]), (int)(i * hist_w / 256)) = 0;

        hist_planes[1].at<uchar>((int)(bin_ratio[1][i] * multi_ratio[1]), (int)(i * hist_w / 256)) = 0;
        hist_planes[1].at<uchar>((int)(bin_ratio[1][i] * multi_ratio[1]), (int)(i * hist_w / 256)) = 255;
        hist_planes[1].at<uchar>((int)(bin_ratio[1][i] * multi_ratio[1]), (int)(i * hist_w / 256)) = 0;

        hist_planes[2].at<uchar>((int)(bin_ratio[2][i] * multi_ratio[2]), (int)(i * hist_w / 256)) = 0;
        hist_planes[2].at<uchar>((int)(bin_ratio[2][i] * multi_ratio[2]), (int)(i * hist_w / 256)) = 0;
        hist_planes[2].at<uchar>((int)(bin_ratio[2][i] * multi_ratio[2]), (int)(i * hist_w / 256)) = 0;
    }
    merge(hist_planes, histImage);

    namedWindow("histogram", WINDOW_AUTOSIZE);
    imshow("histogram", histImage);
    waitKey(0);
    return 0;
}
