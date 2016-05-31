#include "noise_reduction.h"
#include <iostream>
#include "bayer_buffer.h"
using namespace cv;
using namespace std;

void noise_reduction::deNoise(Mat _bayer8, int _denoiseType, int _bayerPattern) {
    switch(_denoiseType) {
        case AVERAGE_DENOISE:
            noise_reduction::averageDeNoise(_bayer8, _bayerPattern);
            break;
        case MEDIUM_DENOISE:
            noise_reduction::mediumDeNoise(_bayer8, _bayerPattern);
            break;
        default:
            cout << "unsupport denoise type :" << _denoiseType << endl;
            break;
    }
}


void noise_reduction::averageDeNoise(Mat _bayer8, int _bayerPattern) {
    int border = 2;
    Mat padded;
    copyMakeBorder(_bayer8, padded, border, border, border, border, BORDER_REFLECT_101);

    {
        for(int row = 0; row < _bayer8.rows; row+=2) {
            for (int col = 0; col < _bayer8.cols; col+=2) {
                Mat crop(padded, Rect(col, row, 6, 6));
                _bayer8.at<uchar>(row, col) =
                    (crop.at<uchar>(0, 0) + crop.at<uchar>(0, 2) + crop.at<uchar>(0,4)
                     + crop.at<uchar>(2, 0) + crop.at<uchar>(2, 2) + crop.at<uchar>(2,4)
                     + crop.at<uchar>(4, 0) + crop.at<uchar>(4, 2) + crop.at<uchar>(4,4)) / 9; 

                _bayer8.at<uchar>(row, col+1) =
                    (crop.at<uchar>(0, 1) + crop.at<uchar>(0, 3) + crop.at<uchar>(0,5)
                     + crop.at<uchar>(2, 1) + crop.at<uchar>(2, 3) + crop.at<uchar>(2,5)
                     + crop.at<uchar>(4, 1) + crop.at<uchar>(4, 3) + crop.at<uchar>(4,5)) / 9;

                _bayer8.at<uchar>(row+1, col) =
                     (crop.at<uchar>(1, 0) + crop.at<uchar>(1,2) + crop.at<uchar>(1,4)
                     + crop.at<uchar>(3, 0) + crop.at<uchar>(3,2) + crop.at<uchar>(3,4)
                     + crop.at<uchar>(5, 0) + crop.at<uchar>(5,2) + crop.at<uchar>(5,4)) / 9;

                _bayer8.at<uchar>(row+1, col+1) =
                    (crop.at<uchar>(1, 1) + crop.at<uchar>(1, 3) + crop.at<uchar>(1,5)
                     + crop.at<uchar>(3,1) + crop.at<uchar>(3,3) + crop.at<uchar>(3,5)
                     + crop.at<uchar>(5,1) + crop.at<uchar>(5,3) + crop.at<uchar>(5,5)) / 9; 
            }
        }
        return;
    }
    switch (_bayerPattern) {
        case bayer_buffer::BAYER_RGrGbB:
            {
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
            break;
        case bayer_buffer::BAYER_GrRBGb:
            {
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
            break;
        case bayer_buffer::BAYER_BGbGrR:
            {
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
            break;
        case bayer_buffer::BAYER_GbBRGr:
            {
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
            break;
        default:
            break;
    }
}

void noise_reduction::mediumDeNoise(Mat _bayer8, int _bayerPattern) {
}

