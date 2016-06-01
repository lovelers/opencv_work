#include "noise_reduction.h"
#include <iostream>
#include "bayer_buffer.h"
using namespace cv;
using namespace std;

void noise_reduction::deNoise(Mat1w& _bayer, int _denoiseType, int _bayerPattern) {
    switch(_denoiseType) {
        case AVERAGE_DENOISE:
            noise_reduction::averageDeNoise(_bayer, _bayerPattern);
            break;
        case MEDIUM_DENOISE:
            noise_reduction::mediumDeNoise(_bayer, _bayerPattern);
            break;
        default:
            cout << "unsupport denoise type :" << _denoiseType << endl;
            break;
    }
}

void noise_reduction::averageDeNoise(Mat1w& _bayer, int _bayerPattern) {
    int border = 2;
    Mat1w padded;
    copyMakeBorder(_bayer, padded, border, border, border, border, BORDER_REFLECT_101);


    switch (_bayerPattern) {
        case bayer_buffer::BAYER_RGrGbB:
        case bayer_buffer::BAYER_BGbGrR:
            {
                for(int row = 0; row < _bayer.rows; row+=2) {
                    for (int col = 0; col < _bayer.cols; col+=2) {
                        Mat1w crop(padded, Rect(col, row, 6, 6));
                        _bayer(row, col) =
                            (crop(0, 0) + crop(0, 2) + crop(0,4)
                             + crop(2, 0) + crop(2, 2) + crop(2,4)
                             + crop(4, 0) + crop(4, 2) + crop(4,4)) / 9;

                        _bayer(row, col+1) =
                            _bayer(row+1, col) =
                            (crop(0, 1) + crop(0, 3) + crop(0,5)
                             + crop(2, 1) + crop(2, 3) + crop(2,5)
                             + crop(4, 1) + crop(4, 3) + crop(4,5)

                             +crop(1, 0) + crop(1,2) + crop(1,4)
                             + crop(3, 0) + crop(3,2) + crop(3,4)
                             + crop(5, 0) + crop(5,2) + crop(5,4)) / 18;

                        _bayer(row+1, col+1) =
                            (crop(1, 1) + crop(1, 3) + crop(1,5)
                             + crop(3,1) + crop(3,3) + crop(3,5)
                             + crop(5,1) + crop(5,3) + crop(5,5)) / 9;
                    }
                }
            }
            break;
        case bayer_buffer::BAYER_GrRBGb:
        case bayer_buffer::BAYER_GbBRGr:
            {
                for(int row = 0; row < _bayer.rows; row+=2) {
                    for (int col = 0; col < _bayer.cols; col+=2) {
                        Mat1w crop(padded, Rect(col, row, 6, 6));
                        _bayer(row, col) =
                            _bayer(row+1, col+1) =
                            (crop(0, 0) + crop(0, 2) + crop(0,4)
                             + crop(2, 0) + crop(2, 2) + crop(2,4)
                             + crop(4, 0) + crop(4, 2) + crop(4,4)
                             + crop(1, 1) + crop(1, 3) + crop(1,5)
                             + crop(3,1) + crop(3,3) + crop(3,5)
                             + crop(5,1) + crop(5,3) + crop(5,5)) / 18;


                        _bayer(row, col+1) =
                            (crop(0, 1) + crop(0, 3) + crop(0,5)
                             + crop(2, 1) + crop(2, 3) + crop(2,5)
                             + crop(4, 1) + crop(4, 3) + crop(4,5)) / 9;

                        _bayer(row+1, col) =
                            (crop(1, 0) + crop(1,2) + crop(1,4)
                             + crop(3, 0) + crop(3,2) + crop(3,4)
                             + crop(5, 0) + crop(5,2) + crop(5,4)) / 9;

                    }
                }
            }
            break;
        default:
            {
                for(int row = 0; row < _bayer.rows; row+=2) {
                    for (int col = 0; col < _bayer.cols; col+=2) {
                        Mat1w crop(padded, Rect(col, row, 6, 6));
                        _bayer(row, col) =
                            (crop(0, 0) + crop(0, 2) + crop(0,4)
                             + crop(2, 0) + crop(2, 2) + crop(2,4)
                             + crop(4, 0) + crop(4, 2) + crop(4,4)) / 9;

                        _bayer(row, col+1) =
                            (crop(0, 1) + crop(0, 3) + crop(0,5)
                             + crop(2, 1) + crop(2, 3) + crop(2,5)
                             + crop(4, 1) + crop(4, 3) + crop(4,5)) / 9;

                        _bayer(row+1, col) =
                            (crop(1, 0) + crop(1,2) + crop(1,4)
                             + crop(3, 0) + crop(3,2) + crop(3,4)
                             + crop(5, 0) + crop(5,2) + crop(5,4)) / 9;

                        _bayer(row+1, col+1) =
                            (crop(1, 1) + crop(1, 3) + crop(1,5)
                             + crop(3,1) + crop(3,3) + crop(3,5)
                             + crop(5,1) + crop(5,3) + crop(5,5)) / 9;
                    }
                }
            }
            break;
    }
}

void noise_reduction::mediumDeNoise(Mat1w& _bayer, int _bayerPattern) {
    int border = 2;
    Mat1w padded;
    copyMakeBorder(_bayer, padded, border, border, border, border, BORDER_REFLECT_101);
    switch (_bayerPattern) {
        case bayer_buffer::BAYER_RGrGbB:
            break;
        case bayer_buffer::BAYER_GrRBGb:
            break;
        case bayer_buffer::BAYER_BGbGrR:
            break;
        case bayer_buffer::BAYER_GbBRGr:
            break;
        default:
            break;
    }
}

