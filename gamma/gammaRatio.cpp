#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <iomanip>

using namespace std;
using namespace cv;


// after experiment. the indoor outdoor ratio should be para_curve.

#define INDOOR 168.0
#define OUTDOOR 100.0
int main(int argc, char **argv) {
    // indoor. (1, 0), (300, 1.f);
    // y = a * pow(x, base) + b;
    double indoor_curve_coff = 1.5;
    double a = (1.f - 0) / (pow(INDOOR, indoor_curve_coff) - pow(1, indoor_curve_coff));
    double b = - a * pow(1, indoor_curve_coff);

    for (int i = 1; i <= INDOOR; i++ ) {
        printf(" %.6f,", a * pow(i ,indoor_curve_coff) + b);
        if (i %10 == 0) printf("\n");
    }

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");

    // outdoor. (1, 0), (1000, 1.f);
    double outdoor_curve_coff = 1.5;
    double aa = (1.f - 0) / (pow(OUTDOOR, outdoor_curve_coff) - pow(1, outdoor_curve_coff));
    double bb = - aa * pow (1, outdoor_curve_coff);
    for (int i = 1; i <= OUTDOOR; i ++) {
        printf(" %.6f,", aa * pow (i, outdoor_curve_coff) + bb);
        if (i %10 == 0) printf("\n");
    }

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    return 0;
}
