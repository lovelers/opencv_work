#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <iomanip>

using namespace std;
using namespace cv;


// after experiment. the indoor outdoor ratio should be para_curve.
#define MAX_GAMMA_VALUE 16384
#define MAX_GAMMA_VALUE 16384
#define NORMAL_GAMMA (1/2.2f)
#define GAMMA_TABLE_X_COUNT 128
#define GAMMA_X_OFFSET 128.0
#define GAMMA_INDEX_COUNT 32
int gamma_h = 10;
int gamma_w = 10;
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
int main(int argc, char **argv) {
    {
        //s = c* pow (r, y); 
        double gamma_ratio = 1 / 2.2f;
        double gamma_c = MAX_GAMMA_VALUE  / pow( MAX_GAMMA_VALUE, gamma_ratio);
        int *table_x = new int(MAX_GAMMA_VALUE);
        int *table_y = new int(MAX_GAMMA_VALUE);
        for (int i = 0; i < MAX_GAMMA_VALUE; i++) {
            table_x[i] = i;
            table_y[i] = gamma_c * pow(i, gamma_ratio);
        }

        //ratio s' = y * pow (r, y-1); y means gamma_ratio.
        // when r = 1, max_s' = gamma_ratio * pow (1, gamma_ratio-1);
        // when r = 16384, min_s' = gamma_ratio * pow( 16384, gamma_ratio-1);
        {
            double max_derivative = gamma_ratio * pow (1, gamma_ratio -1);
            double min_derivative = gamma_ratio * pow (MAX_GAMMA_VALUE, gamma_ratio -1);
            // y = ax + b;
            double detla = (max_derivative - min_derivative) / (GAMMA_INDEX_COUNT -1);
            printf( "detla. %lf", detla);
            double derivative_x[GAMMA_INDEX_COUNT];
            double derivative_y[GAMMA_INDEX_COUNT];
            for (int i = 0; i < GAMMA_INDEX_COUNT; i++) {
                derivative_y[i] = (GAMMA_INDEX_COUNT - 1 -i) * detla + min_derivative;
            }
            int index = 0;
            for (int i = 1; i < MAX_GAMMA_VALUE; i++) {
                double tmp = gamma_ratio * pow ( i, gamma_ratio -1);
                if (tmp <= derivative_y[index]) {
                    derivative_x[index] = i;
                    printf("ratio_average: %d \n", i);
                    index++;
                }

                if (index >= GAMMA_INDEX_COUNT) break;
            }

        }

#if 0
        Mat sgamma(gamma_h, gamma_w, CV_8UC3, Scalar(255, 255,255));
        vector<Mat> sgamma_planes;
        split(sgamma, sgamma_planes);
        for (int i = 0; i < gamma_w; i++) {
            dot(sgamma_planes, table_x[i], table_y[i], 10, 0, 0, 0);
        }
        merge(sgamma_planes, sgamma);
        Mat resize_sgamma;
        flip(sgamma, sgamma, 0);
        resize(sgamma, resize_sgamma, Size(640, 640));
        imwrite("sgamma.jpg", resize_sgamma);
#endif

        float detla = MAX_GAMMA_VALUE / (GAMMA_TABLE_X_COUNT -1);
        int gamma_y[GAMMA_TABLE_X_COUNT];
        for (int i = 0; i < GAMMA_TABLE_X_COUNT; i++) {
            gamma_y[i] = i * detla;
        }

        int index = 0;
        int gamma_x[GAMMA_TABLE_X_COUNT];
        for (int i = 0; i < MAX_GAMMA_VALUE; i++) {
            if (table_y[i] >= gamma_y[index]) {
                gamma_x[index] = i;
                index++;
                if (index >=  GAMMA_TABLE_X_COUNT) break;
            }
        }

        for (int i = 0; i < GAMMA_TABLE_X_COUNT; i++) {
            //printf("%d, %d, %d \n", gamma_x[i], (gamma_x[i] + (int)(GAMMA_X_OFFSET / 2)) / 128 * 128,  gamma_y[i]);
        }
    }


    {
        // gamma_y = gamma_c * pow (gamma_x, gamma_ratio);
        double gamma_ratio = 1 / 2.0f;
        double gamma_c = 32 / pow ( 32,  gamma_ratio);
        for (int i = 0; i < 31; i++) {
            printf("(%d, %d)\n", i, (int)(gamma_c * pow ( i, gamma_ratio)));
        }
    }
    return 0;
}
