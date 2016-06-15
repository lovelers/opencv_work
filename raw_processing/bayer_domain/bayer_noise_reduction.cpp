#include "bayer_noise_reduction.h"
#include <iostream>
#include "bayer_buffer.h"
using namespace cv;
using namespace std;

void bayer_noise_reduction::deNoise(const Mat1w& _bayer, Mat1w & _dst, int _denoiseType, int _bayerPattern) {
    Mat1w padded8;
    int border = 2;
    copyMakeBorder(_bayer, padded8, border, border, border, border, BORDER_REFLECT_101);
    switch(_denoiseType) {
        case AVERAGE_DENOISE:
            bayer_noise_reduction::averageDeNoise(padded8, _dst, _bayerPattern, border);
            break;
        case MEDIUM_DENOISE:
            bayer_noise_reduction::mediumDeNoise(padded8, _dst, _bayerPattern, border);
            break;
        default:
            cout << "unsupport denoise type :" << _denoiseType << endl;
            break;
    }
}

void bayer_noise_reduction::averageDeNoise(const Mat1w& _bayer, Mat1w & _dst, int _bayerPattern, int _border) {
    switch (_bayerPattern) {
        case bayer_buffer::BAYER_RGrGbB:
        case bayer_buffer::BAYER_BGbGrR:
            {
                for(int row = 0; row < _dst.rows; row+=2) {
                    for (int col = 0; col < _dst.cols; col+=2) {
#if 1
                        Mat1w crop(_bayer, Rect(col, row, 6, 6));
#else
                        Mat1w crop(6, 6);
                        for (int i = 0; i < 6; ++i) {
                            for (int j = 0; j < 6; ++j) {
                                crop(i, j) = _bayer(row+i, col+j);
                            }
                        }
#endif
                        _dst(row, col) =
                            (crop(0, 0) + crop(0, 2) + crop(0,4)
                             + crop(2, 0) + crop(2, 2) + crop(2,4)
                             + crop(4, 0) + crop(4, 2) + crop(4,4)) / 9;

                        _dst(row, col+1) =
                            _dst(row+1, col) =
                            (crop(0, 1) + crop(0, 3) + crop(0,5)
                             + crop(2, 1) + crop(2, 3) + crop(2,5)
                             + crop(4, 1) + crop(4, 3) + crop(4,5)

                             +crop(1, 0) + crop(1,2) + crop(1,4)
                             + crop(3, 0) + crop(3,2) + crop(3,4)
                             + crop(5, 0) + crop(5,2) + crop(5,4)) / 18;

                        _dst(row+1, col+1) =
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
                for(int row = 0; row < _dst.rows; row+=2) {
                    for (int col = 0; col < _dst.cols; col+=2) {
#if 1
                        Mat1w crop(_bayer, Rect(col, row, 6, 6));
#else
                        Mat1w crop(6, 6);
                        for (int i = 0; i < 6; ++i) {
                            for (int j = 0; j < 6; ++j) {
                                crop(i, j) = _bayer(row+i, col+j);
                            }
                        }
#endif
                        _dst(row, col) =
                            _dst(row+1, col+1) =
                            (crop(0, 0) + crop(0, 2) + crop(0,4)
                             + crop(2, 0) + crop(2, 2) + crop(2,4)
                             + crop(4, 0) + crop(4, 2) + crop(4,4)
                             + crop(1, 1) + crop(1, 3) + crop(1,5)
                             + crop(3,1) + crop(3,3) + crop(3,5)
                             + crop(5,1) + crop(5,3) + crop(5,5)) / 18;


                        _dst(row, col+1) =
                            (crop(0, 1) + crop(0, 3) + crop(0,5)
                             + crop(2, 1) + crop(2, 3) + crop(2,5)
                             + crop(4, 1) + crop(4, 3) + crop(4,5)) / 9;

                        _dst(row+1, col) =
                            (crop(1, 0) + crop(1,2) + crop(1,4)
                             + crop(3, 0) + crop(3,2) + crop(3,4)
                             + crop(5, 0) + crop(5,2) + crop(5,4)) / 9;

                    }
                }
            }
            break;
        default:
            {
                for(int row = 0; row < _dst.rows; row+=2) {
                    for (int col = 0; col < _dst.cols; col+=2) {
                        Mat1w crop(_bayer, Rect(col, row, 6, 6));
                        _dst(row, col) =
                            (crop(0, 0) + crop(0, 2) + crop(0,4)
                             + crop(2, 0) + crop(2, 2) + crop(2,4)
                             + crop(4, 0) + crop(4, 2) + crop(4,4)) / 9;

                        _dst(row, col+1) =
                            (crop(0, 1) + crop(0, 3) + crop(0,5)
                             + crop(2, 1) + crop(2, 3) + crop(2,5)
                             + crop(4, 1) + crop(4, 3) + crop(4,5)) / 9;

                        _dst(row+1, col) =
                            (crop(1, 0) + crop(1,2) + crop(1,4)
                             + crop(3, 0) + crop(3,2) + crop(3,4)
                             + crop(5, 0) + crop(5,2) + crop(5,4)) / 9;

                        _dst(row+1, col+1) =
                            (crop(1, 1) + crop(1, 3) + crop(1,5)
                             + crop(3,1) + crop(3,3) + crop(3,5)
                             + crop(5,1) + crop(5,3) + crop(5,5)) / 9;
                    }
                }
            }
            break;
    }
}

void bayer_noise_reduction::mediumDeNoise(const Mat1w& _bayer, Mat1w & _dst, int _bayerPattern, int _border) {
    int border = 2;
    ushort rgb[4][5];
    for (int row = 0; row < _dst.rows; row+=2) {
        for (int col = 0; col < _dst.cols; col+=2) {
            int k = 0;
            int row1 = row + _border;
            int col1 = col + _border;
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j,++k) {
                    rgb[k][0] = _bayer(row1+i-2, col1+j);
                    rgb[k][1] = _bayer(row1+i, col1+j-2);
                    rgb[k][2] = _bayer(row1+i, col1+j);
                    rgb[k][3] = _bayer(row1+i, col1+j+2);
                    rgb[k][4] = _bayer(row1+i+2, col1+j);
                }
            }

            int value = _bayer(row1, col1);
            quickSelection(rgb[0], 0, 4, 2, &value);
            _dst(row, col) = value;

            value = _bayer(row1, col1+1);
            quickSelection(rgb[1], 0, 4, 2, &value);
            _dst(row, col+1) = value;

            value = _bayer(row1+1, col1);
            quickSelection(rgb[2], 0, 4, 2, &value);
            _dst(row+1, col) = value;

            value = _bayer(row1+1, col1+1);
            quickSelection(rgb[3], 0, 4, 2, &value);
            _dst(row+1, col+1) = value;
        }
    }
}

void bayer_noise_reduction::quickSelection(ushort *_array, int _start, int _end, int _element, int *value) {
    if (_start < _end) {
        int povit = _array[_end];
        int threshold = quickPartition(_array, _start, _end, povit);
        if (threshold == _element) {
            *value = _array[_element];
        } else if (threshold > _element) {
            quickSelection(_array, _start, threshold -1, _element, value);
        } else {
            quickSelection(_array, threshold + 1, _end, _element, value);
        }
    } else if (_start == _end) {
        *value = _array[_start];
    } else {
        return;
    }
}

int bayer_noise_reduction::quickPartition(ushort *_array, int _start, int _end, int povit) {
    int i = _start;
    for (int index = _start; index < _end; ++index) {
        if (_array[index] <= povit) {
            // swap (index, i).
            ushort tmp = _array[index];
            _array[index] = _array[i];
            _array[i] = tmp;
            ++i;
        }
    }
    // swap (i, _end).
    if (i != _end) {
        ushort tmp = _array[i];
        _array[i] = _array[_end];
        _array[_end] = tmp;
    }
    return i;
}
