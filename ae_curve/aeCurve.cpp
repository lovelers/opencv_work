#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <iomanip>

using namespace std;
using namespace cv;
double ratio_max = 1.3f;
double ratio_target1 = 1.005;
int detla1 = 30;
int target1 = 600;
double para_curve_coff1 = 1/5.f;

double ratio_min = 0.1f;
double ratio_target2 = 0.995f;
double para_curve_coff2 = 1.3f;
int para_curve_detla2 = 100;
int detla2 = 50;
int target2 = 5000 - target1;

#define MIN_VALUE 0
#define MAX_VALUE 5000
#define STABLE_VALUE 500
#define EXPONENT 0

int g_convergent_table_index[] = {
    0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500,
    550, 600, 650, 700, 750, 800, 850, 900, 950, 1000,
    1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000,
    2200, 2400, 2600, 2800, 3000, 3300, 3600, 3900, 4200, 4500, 5000
};

#define LOG2 (0.693147f)                                                                          
#define LOG3 (1.098612f)
#define EXIF_LOG2(x) (log((float)(x))/LOG3) // log2(x)   
int main(int argc, char **argv) {
    // get the curve of low light.
    // y = a * log2(x) + b;
    // x = 25, y = 1.5;
    // x = 500, y = 1.005;
    double a = (ratio_max -ratio_target1) / (pow(detla1, para_curve_coff1) - pow(target1, para_curve_coff1));
    double b = ratio_max - a * pow(detla1, para_curve_coff1);
    int count = 0;
    printf("a = %6lf, b = %6lf, detla = %d, count = %d\n", a, b, detla1, target1 / detla1);
    for (int i = detla1; i <= target1; i+= detla1) {
        count++;
        printf("{%d, %6lf},\n", i,  a * pow(i, para_curve_coff1) + b);
        if (count % 10 == 0) {
            printf("\n");
        }
    }

    a = (ratio_target2 - ratio_min) / (pow (detla2, para_curve_coff2) - pow(target2, para_curve_coff2));
    b = ratio_min - a * pow(target2, para_curve_coff2);
    printf("a = %6lf, b = %6lf, detla = %d, count = %d\n", a, b, detla2, target2 / detla2);
    count = 0;
    for (int i = detla2; i <= target2; i+= detla2) {
        count++;
        printf("{%d, %6lf},\n", i + target1, a * pow(i, para_curve_coff2) + b);
        if (count % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    return 0;
}
#if 0
int main(int argc, char **argv) {
    // y = a * pow (x, para_curve_coff) + b

    // from MIN_VALUE to STABLE_VALUE.
    double bb = ratio_max;
    double aa = (1.005f - bb) / pow(STABLE_VALUE, para_curve_coff1);


    // from STABLE_VALUE to MAX_VALUE
    double a = (0.995 - ratio_min) / (pow(STABLE_VALUE, para_curve_coff2) - pow(MAX_VALUE, para_curve_coff2));
    double b = 0.995 - pow (STABLE_VALUE, para_curve_coff2) * a;

    cout << "aa = " << aa << endl;
    cout << "bb = " << bb << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

#if 0
    for (int i = 0; i <= STABLE_VALUE; i += para_curve_detla1) {
        cout << " " << i << ",";
    }

    cout << endl;

    for (int i = STABLE_VALUE; i <= MAX_VALUE; i += para_curve_detla2) {
        cout << " " << i << ",";
    }
    cout << endl;
#endif

    int hist_w = MAX_VALUE;
    int hist_h = ratio_max * 1000;

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255,255,255));
    vector<Mat> hist_planes;
    split(histImage, hist_planes);

    int index = 0;
    for (int i = 0; i < sizeof (g_convergent_table_index) / sizeof (int); i++) {
        printf("%6d, ", g_convergent_table_index[i]);
        if (i % 5 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    for (int i = 0; i <= MAX_VALUE; i++) {
        double tmp = 0.f;
        if (i <= STABLE_VALUE) {
            tmp = aa * pow(STABLE_VALUE - i, para_curve_coff1) + bb;
            tmp = ratio_max - tmp +1;
        } else {
            tmp = a * pow(i, para_curve_coff2) + b;
        }
        if (i == g_convergent_table_index[index]) {
            printf("%.5f, ", tmp);
            if (index % 5 == 0) {
                printf("\n");
            }
            index++;
        }

    }
    printf("\n");

    return 0;
}

#endif
