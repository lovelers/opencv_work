#include "color_correction.h"
#include <iostream>

using namespace std;

static int simpleCcmMatrix[3][3] = {
    400, -112, -32,
    -78, 405, -71,
    -27, -131, 414,
};

void color_correction::applySimpleCCM(Mat3w &_rgb, ushort _max) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << simpleCcmMatrix[i][j] << endl;
        }
    }
    for (int row = 0; row < _rgb.rows; ++row) {
        for (int col = 0; col < _rgb.cols; ++col) {
            int b = _rgb(row, col)[0];
            int g = _rgb(row, col)[1];
            int r = _rgb(row, col)[2];

            r = (simpleCcmMatrix[0][0] * r +
                    simpleCcmMatrix[0][1] * g +
                    simpleCcmMatrix[0][2] * b) / 255;

            g = (simpleCcmMatrix[1][0] * r +
                    simpleCcmMatrix[1][1] * g +
                    simpleCcmMatrix[1][2] * b) / 255;

            b = (simpleCcmMatrix[2][0] * r +
                    simpleCcmMatrix[2][1] * g +
                    simpleCcmMatrix[2][2] * b) / 255;

            _rgb(row, col)[0] = (ushort)(b < 0 ? 0 : (b > _max ? _max : b));
            _rgb(row, col)[1] = (ushort)(g < 0 ? 0 : (g > _max ? _max : g));
            _rgb(row, col)[2] = (ushort)(r < 0 ? 0 : (r > _max ? _max : r));
        }
    }
}

color_correction::color_correction() {
    m.rr = 255;
    m.rg = 0;
    m.rb = 0;

    m.gr = 0;
    m.gg = 255;
    m.gb = 0;

    m.br = 0;
    m.bg = 0;
    m.bb = 255;

    saturation = 0;
}

void  color_correction::setSaturation(int _saturation) {
    saturation = _saturation;
}

void color_correction::setCcm(ccm *_m) {
    memcpy(&m, _m, sizeof(ccm));
}

void color_correction::doCcm(Mat3w &_rgb, ushort _max) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << simpleCcmMatrix[i][j] << endl;
        }
    }
    cout << "------->CCM<----------" << endl;
    cout << m.rr << ", " << m.rg << ", " << m.rb << endl;
    cout << m.gr << ", " << m.gg << ", " << m.gb << endl;
    cout << m.br << ", " << m.bg << ", " << m.bb << endl;
    cout << ">CCM End<" << endl;

    for (int row = 0; row < _rgb.rows; ++row) {
        for (int col = 0; col < _rgb.cols; ++col) {
            int b = _rgb(row, col)[0];
            int g = _rgb(row, col)[1];
            int r = _rgb(row, col)[2];

            r = (m.rr * r + m.rg * g + m.rb * b) / 255;
            g = (m.gr * r + m.gg * g + m.gb * b) / 255;
            b = (m.br * r + m.bg * g + m.bb * b) / 255;

            _rgb(row, col)[0] = (ushort)(b < 0 ? 0 : (b > _max ? _max : b));
            _rgb(row, col)[1] = (ushort)(g < 0 ? 0 : (g > _max ? _max : g));
            _rgb(row, col)[2] = (ushort)(r < 0 ? 0 : (r > _max ? _max : r));
        }
    }
}

