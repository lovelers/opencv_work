#ifndef __COLOR_CORRECTION_H
#define __COLOR_CORRECTION_H
#include "opencv2/core.hpp"

using namespace cv;

class color_correction {
    public:
        static void applySimpleCCM(Mat &_rgb);
    private:
        color_correction();
};
#endif // __COLOR_CORRECTION_H

