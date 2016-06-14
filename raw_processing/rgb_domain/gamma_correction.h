#ifndef __GAMMA_CORRECTION_H
#define __GAMMA_CORRECTION_H
#include "opencv2/core.hpp"
#include "../tuning/gamma_tuning.h"

using namespace cv;

typedef unsigned int U32;

class gamma_correction {
    public:
        static void applySimpleGamma(Mat3w& _rgb);

        static gamma_correction& getInstance() {
            static gamma_correction gamma;
            return gamma;
        }

        void applyGamma(Mat3w& _rgb, int _indoorOutdoorWeight = 128, int _intensityMax = 0xFFFF);

        void setGammaMaxIntensity(int _max);
    private:
        gamma_correction();
        const gamma_tuning_parameters* tuning;
        U32 gammaIndoor[GAMMA_TABLE_COUNT];
        U32 gammaNormal[GAMMA_TABLE_COUNT];
        U32 gammaOutdoor[GAMMA_TABLE_COUNT];
        void initGamma(U32 *_table, float _gamma, int _baseOffset, int _endOffset, int _linearity);
        float getGammaResult(U32 *_table, int _x, bool _isFirst);

        float getSamLut(ushort value, int _intensityMax);
};
#endif// __GAMMA_CORRECTION_H
