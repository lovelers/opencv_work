#ifndef __RAW_PROCESSING_H
#define __RAW_PROCESSING_H
#include "opencv2/core.hpp"

#include "bayer_domain/bayer_buffer.h"
#include "bayer_domain/color_demosaicing_interpolation.h"
#include "bayer_domain/white_balance.h"
#include "bayer_domain/noise_reduction.h"

#include "rgb_domain/color_correction.h"
#include "rgb_domain/gamma_correction.h"
using namespace cv;
using namespace std;

class raw_processing {
    public:
        static raw_processing& getInstance() {
            static raw_processing raw;
            return raw;
        }
        bool selectRawFile(const char *_file, int _height, int _width, int _bitdepths);
        const Mat& getBayerMat();
        void deNoise(Mat1w & _bayer, int _denoiseType, int _bayerPattern);
        void applyWBGains(Mat1w& _bayer, int _rGains, int _gGains, int _bGains, int _pattern);
        void demosaicing(const Mat1w& _bayer, Mat3w *_rgb, int _bayerPattern);

        void applyCcm(Mat3w &_rgb, ushort _max);
        void applyGamma(Mat3w &_rgb, int _indoorOutdoor, int _intensityMax);
        ~raw_processing();

    private:
        bayer_buffer *bayer;
        raw_processing();
        raw_processing(const raw_processing&);
        raw_processing & operator = (const raw_processing &);
};

#endif// __RAW_PROCESSING_H
