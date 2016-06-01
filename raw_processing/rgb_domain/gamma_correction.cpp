#include "gamma_correction.h"
#include <iostream>

using namespace std;
using namespace cv;

const uchar gammaLut2_2[] = {
    0, 20, 28, 33, 38, 42, 46, 49, 52, 55, 58, 61, 63, 65, 68, 70,
    72, 74, 76, 78, 80, 81, 83, 85, 87, 88, 90, 91, 93, 94, 96, 97,
    99, 100, 102, 103, 104, 106, 107, 108, 109, 111, 112, 113, 114, 115, 117, 118,
    119, 120, 121, 122, 123, 124, 125, 126, 128, 129, 130, 131, 132, 133, 134, 135,
    136, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 147, 148, 149,
    150, 151, 152, 153, 153, 154, 155, 156, 157, 158, 158, 159, 160, 161, 162, 162,
    163, 164, 165, 165, 166, 167, 168, 168, 169, 170, 171, 171, 172, 173, 174, 174,
    175, 176, 176, 177, 178, 178, 179, 180, 181, 181, 182, 183, 183, 184, 185, 185,
    186, 187, 187, 188, 189, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 196,
    196, 197, 197, 198, 199, 199, 200, 200, 201, 202, 202, 203, 203, 204, 205, 205,
    206, 206, 207, 208, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 214,
    215, 216, 216, 217, 217, 218, 218, 219, 219, 220, 220, 221, 222, 222, 223, 223,
    224, 224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231,
    232, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239,
    240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247,
    248, 248, 249, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255,
};

void gamma_correction::applySimpleGamma(Mat &_rgb) {
    typedef Point3_<uchar> Pixel;
    for (int row = 0; row < _rgb.rows; ++row) {
        Pixel* bgr = _rgb.ptr<Pixel>(row);
        for (int col = 0; col < _rgb.cols; ++col) {
            bgr[col].x = gammaLut2_2[bgr[col].x];
            bgr[col].y = gammaLut2_2[bgr[col].y];
            bgr[col].z = gammaLut2_2[bgr[col].z];
        }
    }

}

gamma_correction::gamma_correction() {
    tuning = meizu_tuning_get_gamma();
    float gamma = tuning->base_config.normalGamma;
    int baseOffset = tuning->base_config.normalBaseOffset;
    int endOffset = tuning->base_config.normalEndOffset;
    U32 linearity = tuning->base_config.normalLinearityWeight;
    initGamma(gammaNormal, gamma, baseOffset, endOffset, linearity);

    // indoor
    gamma = tuning->base_config.indoorGamma;
    baseOffset = tuning->base_config.indoorBaseOffset;
    endOffset = tuning->base_config.indoorEndOffset;
    linearity = tuning->base_config.indoorLinearityWeight;
    initGamma(gammaIndoor, gamma, baseOffset, endOffset, linearity);

    // outdoor
    gamma = tuning->base_config.outdoorGamma;
    baseOffset = tuning->base_config.outdoorBaseOffset;
    endOffset = tuning->base_config.outdoorEndOffset;
    linearity = tuning->base_config.outdoorLinearityWeight;
    initGamma(gammaOutdoor, gamma, baseOffset, endOffset, linearity);
}

void gamma_correction::initGamma(U32 *_table, float _gamma, int _baseOffset, int _endOffset, int _linearity) {
    float C = (GAMMA_MAX_VALUE - _baseOffset + _endOffset) /pow (GAMMA_MAX_VALUE, _gamma);
    int result = 0;
    for (int i = 0; i < GAMMA_TABLE_COUNT; ++i) {
        result  = C * pow (tuning->base_config.gamma_table_x[i], _gamma) + _baseOffset;
        _table[i] = (result < 0) ? 0 : (result > GAMMA_MAX_VALUE) ? GAMMA_MAX_VALUE : result;
    }
}

void gamma_correction::applyGamma(Mat3w& _rgb, int _indoorOutdoorWeight, int _intensityMax) {
    U32 gammaResult[GAMMA_TABLE_COUNT];
    U32 localGammaResult[_intensityMax];
    if (_indoorOutdoorWeight > 128) {
        float outdoorRatio = (_indoorOutdoorWeight - 128) / 128.f;
        for (int i = 0; i < GAMMA_TABLE_COUNT; ++i) {
            gammaResult[i] = (1.f - outdoorRatio) * gammaNormal[i]
                + outdoorRatio * gammaOutdoor[i];
        }
    } else {
        float indoorRatio = 1 - _indoorOutdoorWeight / 128.f;
        for (int i = 0; i < GAMMA_TABLE_COUNT; ++i) {
            gammaResult[i] = (1.f - indoorRatio) * gammaNormal[i]
                + indoorRatio * gammaIndoor[i];
        }
    }
    for (int i = 0; i < GAMMA_TABLE_COUNT; ++i) {
        gammaResult[i] *= tuning->base_config.user_gamma[i];
    }

    if (_intensityMax <= GAMMA_MAX_VALUE) {
        int base = GAMMA_MAX_VALUE / _intensityMax;
        localGammaResult[0] = getGammaResult(gammaResult, 0, true) / base;
        for (int i = 1; i < _intensityMax; ++i) {
            localGammaResult[i] = getGammaResult(gammaResult, i * base, false) / base;
        }
    } else {
        U32 tmpGammaResult[GAMMA_MAX_VALUE];
        int detla = _intensityMax / GAMMA_MAX_VALUE;
        tmpGammaResult[0] = getGammaResult(gammaResult, 0, true);
        for (int i = 1; i < GAMMA_MAX_VALUE; ++i) {
            tmpGammaResult[i] = getGammaResult(gammaResult, i, false);
        }

        for (int i = 0; i < _intensityMax; ++i) {
            localGammaResult[i] = tmpGammaResult[i/detla] * detla;
        }
    }

    for (int row = 0; row < _rgb.rows; ++row) {
        for (int col = 0; col < _rgb.cols; ++col) {
            _rgb(row, col)[0] = localGammaResult[_rgb(row,col)[0]];
            _rgb(row, col)[1] = localGammaResult[_rgb(row,col)[1]];
            _rgb(row, col)[2] = localGammaResult[_rgb(row,col)[2]];
        }
    }
}

U32 gamma_correction::getGammaResult(U32 *_table, int _x, bool _isFirst) {
    static int s = 0;
    static float a = 0.f;
    static float b = 0.f;
    if (_isFirst) s = 0;
    for (int i = s; i < GAMMA_TABLE_COUNT; i++) {
        if (_x == tuning->base_config.gamma_table_x[i]) {
            return _table[i];
        } else if (_x < tuning->base_config.gamma_table_x[i]) {
            if (i != s) {
                U32 x1 = tuning->base_config.gamma_table_x[i-1];
                U32 y1 = _table[i-1];
                U32 x2 = tuning->base_config.gamma_table_x[i];
                U32 y2 = _table[i];
                // y = ax + b.
                a = (y2 - y1) / (float)(x2 - x1);
                b = y1 - a * x1;
                s = i;
            }
            return a * _x + b;
        }
    }
}
