#include "color_space_conversion.h"
#include <iostream>
using namespace std;

/**reference the https://zh.wikipedia.org/wiki/YUV
 * YUV is [ 0.299, 0.587, 0.114; -0.168736, -0.331246,  0.5000; 0.5, -0.418688, -0.081312 ]
 * inverse [ 1.000, 0.000, 1.402;  1.000000, -0.344100, -0.7141; 1.0,  1.772000,  0.000000 ]
 */
#define R 0
#define G 1
#define B 2
#define Y 0
#define U 1
#define V 2
#define RGB_CHANNEL 3


float rgb2yuv[RGB_CHANNEL][RGB_CHANNEL] = {
    0.299f, 0.587f, 0.114f,
    -0.168736f, -0.331246f,  0.5000f,
    0.5f, -0.418688f, -0.081312f,
};

float yuv2rgb[RGB_CHANNEL][RGB_CHANNEL] = {
    1.000, 0.000, 1.402,
    1.000000, -0.344100, -0.7141,
    1.0,  1.772000,  0.000000
};

void color_conversion::rgb_2_yuv(const Mat3w & _rgb, Mat3w & _yuv) {
    for (int row = 0; row < _rgb.rows; ++row) {
        for (int col = 0; col < _rgb.cols; ++col) {
            int r = _rgb(row, col)[2];
            int g = _rgb(row, col)[1];
            int b = _rgb(row, col)[0];
            float y = r * rgb2yuv[R][R] + g * rgb2yuv[R][G] + b * rgb2yuv[R][B];
            float u = r * rgb2yuv[G][R] + g * rgb2yuv[G][G] + b * rgb2yuv[G][B] + 128.f;
            float v = r * rgb2yuv[B][R] + g * rgb2yuv[B][G] + b * rgb2yuv[B][B] + 128.f;
            _yuv(row, col)[2] = (y < 0 ? 0 : (y > 255 ? 255 : static_cast<ushort>(y)));
            _yuv(row, col)[1] = (u < 0 ? 0 : (u > 255 ? 255 : static_cast<ushort>(u)));
            _yuv(row, col)[0] = (v < 0 ? 0 : (v > 255 ? 255 : static_cast<ushort>(v)));
        }
    }
}

void color_conversion::yuv_2_rgb(const Mat3w &_yuv, Mat3w & _rgb) {
    for (int row = 0; row < _yuv.rows; ++row) {
        for (int col = 0; col < _yuv.cols; ++col) {
            int y = _yuv(row, col)[2];
            int u = _yuv(row, col)[1];
            int v = _yuv(row, col)[0];
            float r = y * yuv2rgb[Y][Y] + (u-128) * yuv2rgb[Y][U] + (v-128) * yuv2rgb[Y][V];
            float g = y * yuv2rgb[U][Y] + (u-128) * yuv2rgb[U][U] + (v-128) * yuv2rgb[U][V];
            float b = y * yuv2rgb[V][Y] + (u-128) * yuv2rgb[V][U] + (v-128) * yuv2rgb[V][V];
            _rgb(row, col)[2] = (r < 0 ? 0 : (r > 255 ? 255 : static_cast<ushort>(r)));
            _rgb(row, col)[1] = (g < 0 ? 0 : (g > 255 ? 255 : static_cast<ushort>(g)));
            _rgb(row, col)[0] = (b < 0 ? 0 : (b > 255 ? 255 : static_cast<ushort>(b)));
        }
    }
}
