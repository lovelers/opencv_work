#ifndef __yuv_processing_H
#define __yuv_processing_H

#include "opencv2/core.hpp"
using namespace cv;

class yuv_processing {
    public:
        static yuv_processing& getInstance();
        static void getHistInfoYuv(const Mat3w & _yuv, Mat1i & _hist);
        static void equalizationYuv(Mat3w & _yuv, int _maxValue);
        static void cutLowLevel(Mat3w & _yuv, int _cut, int _max);
        static int getContrast(const Mat3w & _yuv, int _avgBrightness);
        static int getAverageBrightness(const Mat3w & _yuv);
        static void denoiseYuvDomain(const Mat3w & _src, Mat3w & _dst);
    private:
        yuv_processing();
};
#endif // __yuv_processing_H
