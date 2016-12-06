#ifndef __BAYER_NOISE_REDUCTION_H
#define __BAYER_NOISE_REDUCTION_H
#include "opencv2/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class bayer_noise_reduction {
    public:
        enum {
            AVERAGE_DENOISE = 0,
            MEDIUM_DENOISE,
        };
        static void deNoise(const Mat1w& _bayer,  Mat1w & _dst, int _denoiseType, int _bayerPattern);
    private:
        static void averageDeNoise(const Mat1w & _bayer, Mat1w & _dst, int _bayerPattern, int _border);
        static void mediumDeNoise(const Mat1w & _bayer, Mat1w & _dst, int _bayerPattern, int _border);

        static void quickSelection(ushort* _array, int _start, int _end, int element, int *value);
        static int quickPartition(ushort *_array, int _start, int _end, int povit);
        bayer_noise_reduction();
};
#endif // __BAYER_NOISE_REDUCTION_H
