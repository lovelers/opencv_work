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

void raw_processing::demosaicing(const Mat1w& bayer, Mat3w *rgb, int bayerPattern) {
    demosaicing::convert(bayer, rgb, bayerPattern);
}


void raw_processing::applyWBGains(Mat1w &bayer, int rGains, int gGains, int bGains, int pattern) {
    //white_balance::applyWBGains(bayer, rGains, gGains, bGains, pattern);
}

void raw_processing::deNoise(Mat1w& _bayer, int _denoiseType, int _bayerPattern) {
    noise_reduction::deNoise(_bayer, _denoiseType, _bayerPattern);
}

void raw_processing::applyCcm(Mat3w &_rgb, ushort _max) {
    color_correction::applySimpleCCM(_rgb, _max);
}

void raw_processing::applyGamma(Mat3w& _rgb, int _indoorOutdoor, int _intensityMax) {
    //gamma_correction::applySimpleGamma(_rgb);
    gamma_correction::getInstance().applyGamma(_rgb, _indoorOutdoor, _intensityMax);
}
