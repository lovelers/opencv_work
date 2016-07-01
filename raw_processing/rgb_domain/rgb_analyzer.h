#ifndef __RGB_ANALYZER_H
#define __RGB_ANALYZER_H
#include "opencv2/core.hpp"
using namespace cv;
class rgb_analyzer {
    public:
        static float PSNR(const Mat3w &_rgb1, const Mat3w &_rgb2, int _max);
    private:
        rgb_analyzer();
};
#endif// __RGB_ANALYZER_H
