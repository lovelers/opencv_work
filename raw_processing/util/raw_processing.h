#ifndef __RAW_PROCESSING_H
#define __RAW_PROCESSING_H
#include "opencv2/core.hpp"

#include "color_space_conversion.h"

#include "bayer_domain/bayer_buffer.h"
#include "bayer_domain/white_balance.h"
#include "bayer_domain/bayer_noise_reduction.h"

#include "rgb_domain/color_correction.h"
#include "rgb_domain/gamma_correction.h"
#include "rgb_domain/rgb_analyzer.h"

#include "yuv_domain/yuv_processing.h"

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
        void deNoiseBayerDomain(const Mat1w & _bayer, Mat1w _dst, int _denoiseType, int _bayerPattern);
        void applyWBGains(Mat1w& _bayer, int _rGains, int _gGains, int _bGains, int _pattern);
        void bayer2rgb(const Mat1w& _bayer, Mat3w *_rgb, int _bayerPattern, int _max);

        void applyCcm(Mat3w &_rgb, ushort _max);
        void applyGamma(Mat3w &_rgb, int _indoorOutdoor, int _intensityMax);
        void histEqualization(Mat3w & _rgb, int _maxValue);
        void rgb2yuv(const Mat3w & _rgb, Mat3w & _yuv);
        void yuv2rgb(const Mat3w & _yuv, Mat3w & _rgb);
        void makeSamBayer(int _height, int _width, const char *_file);

        void denoiseYuvDomain(const Mat3w & _src, Mat3w & _dst);

        float PSNR(const Mat3w & _rgb1, const Mat3w & _rgb2, int _max);
        ~raw_processing();
    private:
        bayer_buffer *bayer;
        raw_processing();
        raw_processing(const raw_processing&);
        raw_processing & operator = (const raw_processing &);
};

#endif// __RAW_PROCESSING_H
