#include "color_correction.h"
#include <iostream>

using namespace std;

int simpleCcmMatrix[3][3] = {
    400, -112, -32,
    -78, 405, -71,
    -27, -131, 414,
};

void color_correction::applySimpleCCM(Mat &_rgb) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << simpleCcmMatrix[i][j] << endl;
        }
    }
    typedef Point3_<uchar> Pixel;
    for (int row = 0; row < _rgb.rows; ++row) {
        Pixel* bgr = _rgb.ptr<Pixel>(row);
        for (int col = 0; col < _rgb.cols; ++col) {
            int b = bgr[col].x;
            int g = bgr[col].y;
            int r = bgr[col].z;

            r = (simpleCcmMatrix[0][0] * r +
                simpleCcmMatrix[0][1] * g +
                simpleCcmMatrix[0][2] * b) / 255;

            g = (simpleCcmMatrix[1][0] * r +
                simpleCcmMatrix[1][1] * g +
                simpleCcmMatrix[1][2] * b) / 255;

            b = (simpleCcmMatrix[2][0] * r +
                simpleCcmMatrix[2][1] * g +
                simpleCcmMatrix[2][2] * b) / 255;

            bgr[col].x = (b > 255) ? 255 : (b < 0) ?  0 : (uchar)b;
            bgr[col].y = (g > 255) ? 255 : (g < 0) ?  0 : (uchar)g;
            bgr[col].z = (r > 255) ? 255 : (r < 0) ?  0 : (uchar)r;
        }
    }
}





