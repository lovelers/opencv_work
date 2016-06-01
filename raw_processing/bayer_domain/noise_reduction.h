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
        static void deNoise(Mat1w& _bayer, int _denoiseType, int _bayerPattern);
    private:
        static void averageDeNoise(Mat1w& _bayer, int _bayerPattern);
        static void mediumDeNoise(Mat1w& _bayer, int _bayerPattern);
        noise_reduction();
};
#endif // __NOISE_REDUCTION_H
