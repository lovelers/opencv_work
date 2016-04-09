#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <stdio.h>
#include <iomanip>

using namespace std;
using namespace cv;

typedef unsigned int U32;
// copy from the gamma_tuning.h
typedef struct __gamma_base_config {
    float normalGamma;
    int normalBaseOffset;
    int normalEndOffset;
    U32 normalLinearityWeight;
    float indoorGamma;
    int indoorBaseOffset;
    int indoorEndOffset;
    U32 indoorLinearityWeight;
    float outdoorGamma;
    int outdoorBaseOffset;
    int outdoorEndOffset;
    U32 outdoorLinearityWeight;
    const U32 *gamma_table_x;
    const float *user_gamma;
} gamma_base_config;


// convert from the gamma_tuning.cpp.
const U32 gamma_index[] =  {
    0,     128,   256,   384,   512,   640,   768,   896,
    1024,  1024+128, 1280, 1536,  1792,  2048,  2304,  2560,
    2816,3072,  3072+256,  3584, 4096,  4608,  5120,  5120+512,
    6144, 6144+512, 7168, 8192, 10240, 12288, 14336, 16384 
};

const float user_gamma[] = {
    0.1f, 0.52f, 0.55f, 0.58f, 0.61f, 0.64f, 0.67f, 0.7f,
    0.73f, 0.76f, 0.79f, 0.85f, 0.91f, 0.96f, 1.01f, 1.02f,
    1.03f, 1.04f, 1.05f, 1.06f, 1.07f, 1.08f, 1.09f, 1.10f,
    1.09f, 1.08f, 1.07f, 1.06f, 1.05f, 1.04f, 1.03f, 1.02f,
};

int gamma_w = 16384;
int gamma_h = 16384;
#define GAMMA_MAX_VALUE 16384
#define GAMMA_TABLE_COUNT 32

void dot(vector<Mat> gamma_planes, int x, int y, int detla, int b, int g, int r) {
    int x1 = x - detla;
    int x2 = x + detla;
    int y1 = y - detla;
    int y2 = y + detla;
    if (x1 < 0) x1 = 0;
    if (x2 > gamma_w -1) x2 = gamma_w -1;
    if (y1 < 0) y1 = 0;
    if (y2 > gamma_h -1) y2 = gamma_h -1;

    for (int x3 = x1; x3 < x2; x3++) {
        for (int y3 = y1; y3 < y2; y3++) {
            gamma_planes[0].at<uchar>(y3, x3) = b;
            gamma_planes[1].at<uchar>(y3, x3) = g;
            gamma_planes[2].at<uchar>(y3, x3) = r;
        }
    }
}

void Gamma(double R, int w, int h, vector<Mat> &gamma_planes) {
    double C = h / pow(h, R);
    cout << "willis" << C << endl;
    for (int x = 0; x < w; x++) {
        int y = C * pow(x, R);
        dot(gamma_planes, x, y, 20, 0, 0, 0);
    }
}

void initGamma(U32 *gamma_table_y, float gamma, int base_offset, int end_offset, int linearity) {
    float C = (GAMMA_MAX_VALUE - base_offset + end_offset) /pow (GAMMA_MAX_VALUE, gamma);
    int result = 0;
    for (int i = 0; i < GAMMA_TABLE_COUNT; i++) {
        result  = C * pow (gamma_index[i], gamma) + base_offset;
        gamma_table_y[i] = (result < 0) ? 0 : (result > GAMMA_MAX_VALUE) ? GAMMA_MAX_VALUE : result;
    }
}

gamma_base_config baseConfig;

int main(int arg, char **argv) {
    Mat gamma(gamma_h, gamma_w, CV_8UC3, Scalar(255, 255,255));
    vector<Mat> gamma_planes;
    split(gamma, gamma_planes);


    baseConfig.normalGamma = 1/2.2f;
    baseConfig.normalBaseOffset = -128;
    baseConfig.normalEndOffset = 0;
    baseConfig.normalLinearityWeight = 0;

    baseConfig.indoorGamma = 1/1.8f;
    baseConfig.indoorBaseOffset = 0;
    baseConfig.indoorEndOffset = 0;
    baseConfig.indoorLinearityWeight = 0;

    baseConfig.outdoorGamma = 1/2.4f;
    baseConfig.outdoorBaseOffset = -384;
    baseConfig.outdoorEndOffset = 384;
    baseConfig.outdoorLinearityWeight = 4;

    baseConfig.gamma_table_x = gamma_index;
    baseConfig.user_gamma = user_gamma;

    // normal gamma.
    U32 gamma1_table[32];
    initGamma(gamma1_table, baseConfig.normalGamma, baseConfig.normalBaseOffset, baseConfig.normalEndOffset, baseConfig.normalLinearityWeight);

    for (int i = 0; i < 32; i++) {
        gamma1_table[i] *= baseConfig.user_gamma[i];
    }

    for (int i = 0; i < 31; i++) {
        int x1 = gamma_index[i];
        int x2 = gamma_index[i+1];
        int y1 = gamma1_table[i];
        int y2 = gamma1_table[i+1];

        double a = (y2 -y1) / (double)(x2 - x1);
        double b =  y1 - a * x1;
        cout << a << "," << b << endl;

        for (int x = x1; x < x2; x++) {
            int y = a * x + b;
            dot(gamma_planes, x, y, 10, 255, 0, 0);
        }
    }


    // indoor gamma.
    U32 gamma2_table[32];
    initGamma(gamma2_table, baseConfig.indoorGamma, baseConfig.indoorBaseOffset, baseConfig.indoorEndOffset, baseConfig.indoorLinearityWeight);

    for (int i = 0; i < 32; i++) {
        gamma2_table[i] *= baseConfig.user_gamma[i];
    }
    for (int i = 0; i < 31; i++) {
        int x1 = gamma_index[i];
        int x2 = gamma_index[i+1];
        int y1 = gamma2_table[i];
        int y2 = gamma2_table[i+1];

        double a = (y2 -y1) / (double)(x2 - x1);
        double b =  y1 - a * x1;
        cout << a << "," << b << endl;

        for (int x = x1; x < x2; x++) {
            int y = a * x + b;
            dot(gamma_planes, x, y, 10, 0, 255, 0);
        }
    }


    // outdoor gamma.
    U32 gamma3_table[32];
    initGamma(gamma3_table, baseConfig.outdoorGamma, baseConfig.outdoorBaseOffset, baseConfig.outdoorEndOffset, baseConfig.outdoorLinearityWeight);

    for (int i = 0; i < 32; i++) {
        gamma3_table[i] *= baseConfig.user_gamma[i];
    }
    for (int i = 0; i < 31; i++) {
        int x1 = gamma_index[i];
        int x2 = gamma_index[i+1];
        int y1 = gamma3_table[i];
        int y2 = gamma3_table[i+1];

        double a = (y2 -y1) / (double)(x2 - x1);
        double b =  y1 - a * x1;
        cout << a << "," << b << endl;

        for (int x = x1; x < x2; x++) {
            int y = a * x + b;
            dot(gamma_planes, x, y, 10, 0, 0, 255);
        }
    }

    // blue sshape.
    U32 gamma4_table[32];
    initGamma(gamma4_table, 1/5.f, 0, -2048, 0);

    for (int i = 0; i < 31; i++) {
        int x1 = gamma_index[i];
        int x2 = gamma_index[i+1];
        int y1 = gamma4_table[i];
        int y2 = gamma4_table[i+1];

        double a = (y2 -y1) / (double)(x2 - x1);
        double b =  y1 - a * x1;
        cout << a << "," << b << endl;

        for (int x = x1; x < x2; x++) {
            int y = a * x + b;
            dot(gamma_planes, x, y, 10, 128, 128, 128);
        }
    }
    cout << "goto here" << endl;

    merge(gamma_planes, gamma);
    flip(gamma, gamma, 0);

    Mat resize_gamma;
    resize(gamma, resize_gamma, Size(640, 640));
    //imshow("gamma", gamma);
    //waitKey();
    imwrite("willis.jpg", resize_gamma);
    return 0;
}

