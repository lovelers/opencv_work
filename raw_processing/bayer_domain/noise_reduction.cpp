#include "noise_reduction.h"
#include <iostream>
using namespace cv;
using namespace std;

void noise_reduction::deNoise(Mat _bayer8, int _denoiseType) {
    switch(_denoiseType) {
        case AVERAGE_DENOISE:
            noise_reduction::averageDeNoise(_bayer8);
            break;
        case MEDIUM_DENOISE:
            noise_reduction::mediumDeNoise(_bayer8);
            break;
        default:
            cout << "unsupport denoise type :" << _denoiseType << endl;
            break;
    }
}


void noise_reduction::averageDeNoise(Mat _bayer8) {
    int border = 2;
    Mat padded;
    copyMakeBorder(_bayer8, padded, border, border, border, border, BORDER_REFLECT_101);

    for(int row = 0; row < _bayer8.rows; row+=2) {
        for (int col = 0; col < _bayer8.cols; col+=2) {
            Mat crop(padded, Rect(col, row, 6, 6));
            _bayer8.at<uchar>(row, col) =
                (crop.at<uchar>(0, 0) + crop.at<uchar>(0, 2) + crop.at<uchar>(0,4)
                 + crop.at<uchar>(2, 0) + crop.at<uchar>(2, 2) + crop.at<uchar>(2,4)
                 + crop.at<uchar>(4, 0) + crop.at<uchar>(4, 2) + crop.at<uchar>(4,4)) / 9; 

            _bayer8.at<uchar>(row, col+1) =
                _bayer8.at<uchar>(row+1, col) =
                (crop.at<uchar>(0, 1) + crop.at<uchar>(0, 3) + crop.at<uchar>(0,5)
                 + crop.at<uchar>(2, 1) + crop.at<uchar>(2, 3) + crop.at<uchar>(2,5)
                 + crop.at<uchar>(4, 1) + crop.at<uchar>(4, 3) + crop.at<uchar>(4,5)

                 +crop.at<uchar>(1, 0) + crop.at<uchar>(1,2) + crop.at<uchar>(1,4)
                 + crop.at<uchar>(3, 0) + crop.at<uchar>(3,2) + crop.at<uchar>(3,4)
                 + crop.at<uchar>(5, 0) + crop.at<uchar>(5,2) + crop.at<uchar>(5,4)) / 18;

            _bayer8.at<uchar>(row+1, col+1) =
                (crop.at<uchar>(1, 1) + crop.at<uchar>(1, 3) + crop.at<uchar>(1,5)
                 + crop.at<uchar>(3,1) + crop.at<uchar>(3,3) + crop.at<uchar>(3,5)
                 + crop.at<uchar>(5,1) + crop.at<uchar>(5,3) + crop.at<uchar>(5,5)) / 9; 
        }
    }
}

void noise_reduction::mediumDeNoise(Mat _bayer8) {
}

void noise_reduction::newMethod(Mat _bayer8) {
    int border = 2;
    Mat padded;
    copyMakeBorder(_bayer8, padded, border, border, border, border, BORDER_REFLECT_101);

    for(int row = 0; row < _bayer8.rows; row+=2) {
        for (int col = 0; col < _bayer8.cols; col+=2) {
            int avg1, avg2, avg3, avg4, avg;
            Mat crop(padded, Rect(col, row, 6, 6));
            avg1 = 
                (crop.at<uchar>(0, 0) + crop.at<uchar>(0, 2) + crop.at<uchar>(0,4)
                 + crop.at<uchar>(2, 0) + crop.at<uchar>(2, 2) + crop.at<uchar>(2,4)
                 + crop.at<uchar>(4, 0) + crop.at<uchar>(4, 2) + crop.at<uchar>(4,4)) / 9; 

            avg2 = 
                (crop.at<uchar>(0, 1) + crop.at<uchar>(0, 3) + crop.at<uchar>(0,5)
                 + crop.at<uchar>(2, 1) + crop.at<uchar>(2, 3) + crop.at<uchar>(2,5)
                 + crop.at<uchar>(4, 1) + crop.at<uchar>(4, 3) + crop.at<uchar>(4,5)) / 9; 

            avg3 = 
                (crop.at<uchar>(1, 0) + crop.at<uchar>(1,2) + crop.at<uchar>(1,4)
                 + crop.at<uchar>(3, 0) + crop.at<uchar>(3,2) + crop.at<uchar>(3,4)
                 + crop.at<uchar>(5, 0) + crop.at<uchar>(5,2) + crop.at<uchar>(5,4)) / 9; 

            avg4 =
                (crop.at<uchar>(1, 1) + crop.at<uchar>(1, 3) + crop.at<uchar>(1,5)
                    + crop.at<uchar>(3,1) + crop.at<uchar>(3,3) + crop.at<uchar>(3,5)
                    + crop.at<uchar>(5,1) + crop.at<uchar>(5,3) + crop.at<uchar>(5,5)) / 9; 
            avg = ((avg1+avg2+avg3+avg4) >> 2);
        }
    }
}
