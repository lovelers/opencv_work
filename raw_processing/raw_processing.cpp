#include "raw_processing.h"
using namespace cv;
using namespace std;


raw_processing::raw_processing() {
    bayer = new bayer_buffer();
}

raw_processing::~raw_processing() {
    if (bayer != NULL) {
        bayer->uninit();
        delete bayer;
        bayer = NULL;
    }
}

bool raw_processing::selectRawFile(const char *_file, int _height, int _width, int _bitdepths) {
    if (bayer != NULL) {
        return bayer->init(_file, _height, _width, _bitdepths);
    }
    return false;
}

const Mat& raw_processing::getBayerMat() {
    return bayer->getMat();
}

void raw_processing::demosaicing(const Mat1w& _bayer, Mat3w *_rgb, int _bayerPattern, int _max) {
    demosaicing::convert(_bayer, _rgb, _bayerPattern, _max);
}


void raw_processing::applyWBGains(Mat1w &bayer, int rGains, int gGains, int bGains, int pattern) {
    //white_balance::applyWBGains(bayer, rGains, gGains, bGains, pattern);
}

void raw_processing::deNoiseBayerDomain(const Mat1w& _bayer, Mat1w _dst, int _denoiseType, int _bayerPattern) {
    bayer_noise_reduction::deNoise(_bayer, _dst, _denoiseType, _bayerPattern);
}

void raw_processing::applyCcm(Mat3w &_rgb, ushort _max) {
    color_correction::applySimpleCCM(_rgb, _max);
}

void raw_processing::applyGamma(Mat3w& _rgb, int _indoorOutdoor, int _intensityMax) {
    //gamma_correction::applySimpleGamma(_rgb);
    gamma_correction::getInstance().applyGamma(_rgb, _indoorOutdoor, _intensityMax);
}

void raw_processing::histEqualization(Mat3w & _yuv, int _maxValue) {
    //yuv_processing::getInstance().equalizationYuv(_yuv, _maxValue);
    yuv_processing::getInstance().cutLowLevel(_yuv, 64, _maxValue);
}

void raw_processing::rgb2yuv(const Mat3w & _rgb, Mat3w & _yuv) {
    color_conversion::rgb_2_yuv(_rgb, _yuv);
}

void raw_processing::yuv2rgb(const Mat3w & _yuv, Mat3w & _rgb) {
    color_conversion::yuv_2_rgb(_yuv, _rgb);
}

void raw_processing::makeSamBayer(int _height, int _width, const char *_file) {
    bayer->makeSamBayer(_height, _width, _file);
}
