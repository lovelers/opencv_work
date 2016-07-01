#include "rgb_analyzer.h"
#include <math.h>
#include <iostream>

using namespace std;
float rgb_analyzer::PSNR(const Mat3w &_rgb1, const Mat3w &_rgb2, int _max) {
    uint64_t variance = 0;
    float mse = 0.f;
    float psnr = 0.f;
    for (int row = 0; row < _rgb1.rows; ++row) {
        for (int col = 0; col < _rgb1.cols; ++col) {
            variance += pow(_rgb1(row, col)[0] - _rgb2(row, col)[0], 2);
            variance += pow(_rgb1(row, col)[1] - _rgb2(row, col)[1], 2);
            variance += pow(_rgb1(row, col)[2] - _rgb2(row, col)[2], 2);
        }
    }
    
    mse = variance  / (_rgb1.rows * _rgb1.cols * 3.f);

    if (mse != 0.f) {
        psnr = 10 * log10(_max * _max / mse);
    }
    cout << "variance:" << variance << ", MSE:" << mse << ", PSNR:" << psnr << endl;
    return psnr;
}
