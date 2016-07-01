#include "yuv_processing.h"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

yuv_processing& yuv_processing::getInstance() {
    static yuv_processing equalization;
    return equalization;
}

yuv_processing::yuv_processing() {
}

void yuv_processing::getHistInfoYuv(const Mat3w & _yuv, Mat1i & _hist) {
    for (int row = 0; row < _yuv.rows; ++row) {
        for (int col = 0; col < _yuv.cols; ++col) {
            ++_hist(0, static_cast<int>(_yuv(row, col)[2]));
        }
    }
}


void yuv_processing::equalizationYuv(Mat3w & _yuv, int _maxValue) {
    Mat1i hist(1, _maxValue + 1, int(0));
    getHistInfoYuv(_yuv, hist);
    float pr[_maxValue + 1];
    int exp[_maxValue + 1];
    float sumy = _yuv.cols * _yuv.rows;
    int y = 0;

    int avgBrightness = getAverageBrightness(_yuv);
    int contrast = getContrast(_yuv, avgBrightness);
    cout << "avgBrightness:" << avgBrightness << ", contrast:" << contrast << endl;

    for (int i = 0; i < _maxValue + 1; ++i) {
        pr[i] = hist(0,i) / sumy;
        float tmp = 0;
        for (int j = 0; j < i; ++j) {
            tmp += pr[j] * _maxValue;
        }
        exp[i] = static_cast<int>(tmp + 0.5f);
        cout << i << ":" << pr[i] << "," << exp[i] << endl;
    }

    for (int row = 0; row < _yuv.rows; ++row) {
        for (int col = 0; col < _yuv.cols; ++col) {
            _yuv(row, col)[2] = static_cast<ushort>(exp[_yuv(row, col)[2]]);
        }
    }

}

int yuv_processing::getContrast(const Mat3w & _yuv, int _avgBrightness) {
    uint variance = 0;
    for (int row = 0; row < _yuv.rows; ++row) {
        for (int col = 0; col < _yuv.cols; ++col) {
            variance += pow(_yuv(row, col)[2] - _avgBrightness, 2);
        }
    }
    variance = static_cast<int>(sqrt(variance / (_yuv.rows * _yuv.cols)) + 0.5f);
}

int yuv_processing::getAverageBrightness(const Mat3w & _yuv) {
    uint brightness = 0;
    for (int row = 0; row < _yuv.rows; ++row) {
        for (int col = 0; col < _yuv.cols; ++col) {
            brightness += _yuv(row, col)[2];
        }
    }
    return static_cast<int>(brightness / (_yuv.rows * _yuv.cols) + 0.5f);
}

void yuv_processing::cutLowLevel(Mat3w & _yuv, int _cut, int _max) {
    float a = _max / static_cast<float>(_max - _cut);
    float b = -a *  _cut;
    cout << __func__ << "a:" << a << ",b:" << b << endl;

    for (int row = 0; row < _yuv.rows; ++row) {
        for (int col = 0; col < _yuv.cols; ++col) {
            if (_yuv(row, col)[2] < _cut) {
                _yuv(row, col)[2] = 0;
            } else {
                _yuv(row, col)[2] = static_cast<ushort>
                    (a * _yuv(row, col)[2] + b);
            }
        }
    }
}

void yuv_processing::denoiseYuvDomain(const Mat3w & _src, Mat3w & _dst) {
    // uv smoothing only. 3x3 matrix
    {
        int row = _src.rows;
        int col = _src.cols;
        for (int i = 1; i < row-1; ++i) {
            for (int j = 1; j < col-1; ++j) {
                _dst(i, j)[2] = _src(i, j)[2];
                _dst(i, j)[1] = (_src(i-1,j-1)[1]
                        + _src(i-1, j)[1]
                        + _src(i-1, j+1)[1]
                        + _src(i, j-1)[1]
                        + _src(i, j)[1]
                        + _src(i, j+1)[1]
                        + _src(i+1, j-1)[1]
                        + _src(i+1, j)[1]
                        + _src(i+1, j+1)[1]) / 9;
                _dst(i, j)[0] = (_src(i-1,j-1)[0]
                        + _src(i-1, j)[0]
                        + _src(i-1, j+1)[0]
                        + _src(i, j-1)[0]
                        + _src(i, j)[0]
                        + _src(i, j+1)[0]
                        + _src(i+1, j-1)[0]
                        + _src(i+1, j)[0]
                        + _src(i+1, j+1)[0]) / 9;
            }
        }

        for (int i = 0; i < col; ++i) {
            _dst(0, i)[0] = _src(0, i)[0];
            _dst(0, i)[1] = _src(0, i)[1];
            _dst(0, i)[2] = _src(0, i)[2];

            _dst(row-1, i)[0] = _src(row-1, i)[0];
            _dst(row-1, i)[1] = _src(row-1, i)[1];
            _dst(row-1, i)[2] = _src(row-1, i)[2];
        }

        for (int i = 0; i < row; ++i) {
            _dst(i, 0)[0] = _src(i, 0)[0];
            _dst(i, 0)[1] = _src(i, 0)[1];
            _dst(i, 0)[2] = _src(i, 0)[2];

            _dst(i, col-1)[0] = _src(i, col-1)[0];
            _dst(i, col-1)[1] = _src(i, col-1)[1];
            _dst(i, col-1)[2] = _src(i, col-1)[2];
        }

    }
}
