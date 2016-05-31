#ifndef __NOISE_REDUCTION_H
#define __NOISE_REDUCTION_H
#include "opencv2/core.hpp"
using namespace cv;

class noise_reduction {
    public:
        enum {
            AVERAGE_DENOISE = 0,
            MEDIUM_DENOISE,
        };
        static void deNoise(Mat _bayer8, int _denoiseType, int _bayerPattern);
        static void averageDeNoise(Mat _bayer8, int _bayerPattern);
        static void mediumDeNoise(Mat _bayer8, int _bayerPattern);
    private:
        noise_reduction();
};
#endif // __NOISE_REDUCTION_H
