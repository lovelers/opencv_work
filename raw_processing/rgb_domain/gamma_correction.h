#ifndef __GAMMA_CORRECTION_H
#define __GAMMA_CORRECTION_H
#include "opencv2/core.hpp"

using namespace cv;

class gamma_correction {
    public:
        static void applySimpleGamma(Mat& _rgb);
    private:
        gamma_correction();
};
#endif// __GAMMA_CORRECTION_H
