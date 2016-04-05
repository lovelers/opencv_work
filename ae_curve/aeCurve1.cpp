#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <iomanip>

using namespace std;
using namespace cv;
double ratio_max = 1.5;
double para_curve_coff1 = 3;
int    para_curve_detla1 = 50;

double ratio_min = 0.3f;
double para_curve_coff2 = 1.5;
int    para_curve_detla2 = 100;

#define MIN_VALUE 0
#define MAX_VALUE 5000
#define STABLE_VALUE 500
#define EXPONENT 0
int main(int argc, char **argv) {

#if EXPONENT
#else
    // y = a * pow (x, para_curve_coff) + b
    
    // from MIN_VALUE to STABLE_VALUE.
    double bb = ratio_max;
    double aa = (1.f - bb) / pow(STABLE_VALUE, para_curve_coff1);


    // from STABLE_VALUE to MAX_VALUE
    double a = (1 - ratio_min) / (pow(STABLE_VALUE, para_curve_coff2) - pow(MAX_VALUE, para_curve_coff2));
    double b = 1 - pow (STABLE_VALUE, para_curve_coff2) * a;

    cout << "aa = " << aa << endl;
    cout << "bb = " << bb << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    for (int i = 0; i <= STABLE_VALUE; i += para_curve_detla1) {
        cout << " " <<setw(8) << i << ",";
    }

    for (int i = STABLE_VALUE; i <= MAX_VALUE; i += para_curve_detla2) {
        cout << " " <<setw(8) << i << ",";
    }

    int hist_w = MAX_VALUE;
    int hist_h = ratio_max * 1000;

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255,255,255));
    vector<Mat> hist_planes;
    split(histImage, hist_planes);

    for (int i = 0; i <= MAX_VALUE; i++) {
        double tmp = 0.f;
        if (i <= STABLE_VALUE) {
            tmp = aa * pow(i, para_curve_coff1) + bb;
            if (i % para_curve_detla1 == 0) {
                cout << " " << setw(8) << tmp << ",";
            }
        } else {
            tmp = a * pow(i, para_curve_coff2) + b;
            if (i % para_curve_detla2 == 0) {
            cout << " " << setw(8) << tmp << ",";
        }
    }

    return 0;
}

