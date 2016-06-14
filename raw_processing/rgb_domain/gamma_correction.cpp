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

const float samLut256[] = {
    0, 0.00392157, 0.0156863, 0.0352941, 0.0627451, 0.0980392, 0.141176, 0.192157, 0.25098, 0.317647, 0.392157, 0.47451, 0.564706, 0.662745, 0.768627, 0.882353,
    1.00392, 1.13333, 1.27059, 1.41569, 1.56863, 1.72941, 1.89804, 2.07451, 2.25882, 2.45098, 2.65098, 2.85882, 3.07451, 3.29804, 3.52941, 3.76863,
    4.01569, 4.27059, 4.53333, 4.80392, 5.08235, 5.36863, 5.66275, 5.96471, 6.27451, 6.59216, 6.91765, 7.25098, 7.59216, 7.94118, 8.29804, 8.66275,
    9.03529, 9.41569, 9.80392, 10.2, 10.6039, 11.0157, 11.4353, 11.8627, 12.298, 12.7412, 13.1922, 13.651, 14.1176, 14.5922, 15.0745, 15.5647,
    16.0627, 16.5686, 17.0824, 17.6039, 18.1333, 18.6706, 19.2157, 19.7686, 20.3294, 20.898, 21.4745, 22.0588, 22.651, 23.251, 23.8588, 24.4745,
    25.098, 25.7294, 26.3686, 27.0157, 27.6706, 28.3333, 29.0039, 29.6824, 30.3686, 31.0627, 31.7647, 32.4745, 33.1922, 33.9176, 34.651, 35.3922,
    36.1412, 36.898, 37.6627, 38.4353, 39.2157, 40.0039, 40.8, 41.6039, 42.4157, 43.2353, 44.0627, 44.898, 45.7412, 46.5922, 47.451, 48.3176,
    49.1922, 50.0745, 50.9647, 51.8627, 52.7686, 53.6824, 54.6039, 55.5333, 56.4706, 57.4157, 58.3686, 59.3294, 60.298, 61.2745, 62.2588, 63.251,
    64.251, 65.2588, 66.2745, 67.298, 68.3294, 69.3686, 70.4157, 71.4706, 72.5333, 73.6039, 74.6824, 75.7686, 76.8628, 77.9647, 79.0745, 80.1922,
    81.3177, 82.451, 83.5922, 84.7412, 85.898, 87.0628, 88.2353, 89.4157, 90.6039, 91.8, 93.0039, 94.2157, 95.4353, 96.6628, 97.898, 99.1412,
    100.392, 101.651, 102.918, 104.192, 105.475, 106.765, 108.063, 109.369, 110.682, 112.004, 113.333, 114.671, 116.016, 117.369, 118.729, 120.098,
    121.475, 122.859, 124.251, 125.651, 127.059, 128.475, 129.898, 131.329, 132.769, 134.216, 135.671, 137.133, 138.604, 140.082, 141.569, 143.063,
    144.565, 146.075, 147.592, 149.118, 150.651, 152.192, 153.741, 155.298, 156.863, 158.435, 160.016, 161.604, 163.2, 164.804, 166.416, 168.035,
    169.663, 171.298, 172.941, 174.592, 176.251, 177.918, 179.592, 181.275, 182.965, 184.663, 186.369, 188.082, 189.804, 191.533, 193.271, 195.016,
    196.769, 198.529, 200.298, 202.075, 203.859, 205.651, 207.451, 209.259, 211.075, 212.898, 214.729, 216.569, 218.416, 220.271, 222.133, 224.004,
    225.882, 227.769, 229.663, 231.565, 233.475, 235.392, 237.318, 239.251, 241.192, 243.141, 245.098, 247.063, 249.035, 251.016, 253.004, 255,
};

void gamma_correction::applySimpleGamma(Mat3w &_rgb) {
    for (int row = 0; row < _rgb.rows; ++row) {
        for (int col = 0; col < _rgb.cols; ++col) {
            _rgb(row, col)[0] = gammaLut2_2[_rgb(row,col)[0]];
            _rgb(row, col)[1] = gammaLut2_2[_rgb(row,col)[1]];
            _rgb(row, col)[2] = gammaLut2_2[_rgb(row,col)[2]];
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
    // workaround method

    float C = (GAMMA_MAX_VALUE - _baseOffset + _endOffset) /pow (GAMMA_MAX_VALUE, _gamma);
    int result = 0;
    for (int i = 0; i < GAMMA_TABLE_COUNT; ++i) {
        result  = C * pow (tuning->base_config.gamma_table_x[i], _gamma) + _baseOffset;
        _table[i] = (result < 0) ? 0 : (result > GAMMA_MAX_VALUE) ? GAMMA_MAX_VALUE : result;
    }
}

void gamma_correction::applyGamma(Mat3w& _rgb, int _indoorOutdoorWeight, int _intensityMax) {
    U32 gammaResult[GAMMA_TABLE_COUNT];
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

    float localGammaResult[GAMMA_MAX_VALUE];
    localGammaResult[0] = getGammaResult(gammaResult, 0, true);
    for (int i = 1; i < GAMMA_MAX_VALUE; ++i) {
        localGammaResult[i] = getGammaResult(gammaResult, i, false);
    }

    int base = GAMMA_MAX_VALUE / (_intensityMax + 1);

    float b, g ,r;
    cout << " goto here" << endl;
    for (int row = 0; row < _rgb.rows; ++row) {
        for (int col = 0; col < _rgb.cols; ++col) {
            if (_intensityMax == 0xFF) {
                b = samLut256[_rgb(row, col)[0]];
                g = samLut256[_rgb(row, col)[1]];
                r = samLut256[_rgb(row, col)[2]];
            } else {
                b = getSamLut(_rgb(row, col)[0], _intensityMax);
                g = getSamLut(_rgb(row, col)[1], _intensityMax);
                r = getSamLut(_rgb(row, col)[2], _intensityMax);
            }
            _rgb(row, col)[0] = (ushort)(localGammaResult[static_cast<int>(b * base + 0.5f)] / base + 0.5f);
            _rgb(row, col)[1] = (ushort)(localGammaResult[static_cast<int>(g * base + 0.5f)] / base + 0.5f);
            _rgb(row, col)[2] = (ushort)(localGammaResult[static_cast<int>(r * base + 0.5f)] / base + 0.5f);
        }
    }
    cout << " goto here" << endl;
}

float gamma_correction::getSamLut(ushort value, int _intensityMax) {
    return _intensityMax * pow( value / (float)_intensityMax, 2.f);
}

float gamma_correction::getGammaResult(U32 *_table, int _x, bool _isFirst) {
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
