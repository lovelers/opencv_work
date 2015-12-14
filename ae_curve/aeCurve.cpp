#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <iomanip>

using namespace std;
using namespace cv;

#define EXPONENT 0
int detla = 50;
double ratio_min = 0.3f;
double ratio_max = 1.5f;

double base = 1.02f;
double para_curve_coff1 = 3;// 0-10
double para_curve_coff2 = 1.5;// 10-100
int main( int argc, char **argv) {

#if EXPONENT
    // y = a * pow(base, x) + b;
    double aa = (ratio_max - 1.f) / ( pow(base, 0) - pow(base, 10));
    double bb = ratio_max - aa * pow(base, 0);
    double a = (1.f - ratio_min) / (pow(base, 10) - pow(base, 100));
    double b = 1.f - a * pow(base, 10);
#else
    // y = a * pow(x, para_curve_coff) + b;
    double bb = ratio_max;
    double aa = (1.f - ratio_max) / pow(10, para_curve_coff1);

    double a = ( 1 - ratio_min) / (double)(pow(10, para_curve_coff2) - pow(100, para_curve_coff2));
    double b =  1 - pow(10, para_curve_coff2) * a;
    cout << "aa = " << aa << endl;
    cout << "bb = " << bb << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
#endif
    {
        for (int i = 0; i <= 20; i++) {
            if (i %10 == 0) {
                cout << endl;
            }
            cout << " " <<setw(8) << i * 25 << ",";
        }
        for (int i = 10; i <= 100; i++) {
            if (i %10 == 0) {
                cout << endl;
            }
            cout << " " <<setw(8) << i * 50 << ",";
        }
        cout << endl;
#if 0
        for (int i = 0; i < 10; i++) {
            cout << " " << setw(8) << aa * pow(base, i) + bb  << ",";
        }
        for (int i = 10; i <= 100; i++) {
            if (i % 10 == 0) cout << endl;
            cout << " " << setw(8) << a * pow(base, i) + b << ",";
        }
        cout << endl;
#endif
    }
    int hist_h = ratio_max * 100;
    const int weight_coff = 5;
    int hist_w = 101 * weight_coff;
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255,255,255));
    vector<Mat> hist_planes;
    split(histImage, hist_planes);

    for (int i = 0; i <= 20; i++) {
        float j = (float) i / 2.f;
#if EXPONENT
        double tmp = aa * pow(base, 10 - j) + bb;
#else
        double tmp = aa * pow(10-j, para_curve_coff1) + bb;
#endif
        if (i%10 == 0) cout << endl;

        cout << " " << setw(8) << double(ratio_max - tmp + 1) << ",";
        int k = (ratio_max - tmp + 1) * 100;
        hist_planes[0].at<uchar>(k, j * weight_coff) = 255;
        hist_planes[1].at<uchar>(k, j * weight_coff) = 0;
        hist_planes[2].at<uchar>(k, j * weight_coff) = 0;
    }

    for (int i = 10; i < hist_w / weight_coff; i++) {
#if EXPONENT
        double tmp = a * pow(base, i) + b;
#else
        double tmp = a * pow(i, para_curve_coff2) + b;
#endif
        if (i % 10 == 0) cout << endl;
        cout << " " << setw(8) << tmp << ",";

        int k = tmp * 100;
        hist_planes[0].at<uchar>(k, i * weight_coff) = 255;
        hist_planes[1].at<uchar>(k, i * weight_coff) = 0;
        hist_planes[2].at<uchar>(k, i * weight_coff) = 0;
    }

    cout << endl;
    Mat flipHistImage(hist_h, hist_w, CV_8UC3, Scalar(255,255,255));
    vector<Mat> flip_planes;
    split(flipHistImage, flip_planes);
    for (int i = 0; i < hist_w; i++) {
        for (int j = 0; j < hist_h; j++) {
            flip_planes[0].at<uchar>(j, i) = hist_planes[0].at<uchar>(hist_h - j -1, i);
            flip_planes[1].at<uchar>(j, i) = hist_planes[1].at<uchar>(hist_h - j -1, i);
            flip_planes[2].at<uchar>(j, i) = hist_planes[2].at<uchar>(hist_h - j -1, i);
        }
    }
    merge(flip_planes, flipHistImage);

    namedWindow("histogram", WINDOW_AUTOSIZE);
    imshow("histogram", flipHistImage);
    waitKey(0);
    return 0;
}
