#ifndef __COLOR_SPACE_CONVERSION_H
#define __COLOR_SPACE_CONVERSION_H
#include "opencv2/core.hpp"
using namespace cv;

class color_conversion {
    public:
        static void rgb_2_yuv(const Mat3w & _rgb, Mat3w & _yuv);
        static void yuv_2_rgb(const Mat3w & _yuv, Mat3w & _rgb);
    private:
        color_conversion();
};
#endif// __COLOR_SPACE_CONVERSION_H
