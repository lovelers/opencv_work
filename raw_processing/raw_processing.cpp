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

void raw_processing::demosaicing(const Mat& bayer, Mat *rgbMat, int bayerPattern) {
    demosaicing::convert(bayer, rgbMat, bayerPattern);
}


void raw_processing::applyWBGains(Mat &bayer8, int rGains, int gGains, int bGains, int pattern) {
    white_balance::applyWBGains(bayer8, rGains, gGains, bGains, pattern);
}

void raw_processing::deNoise(Mat& _bayer8, int _denoiseType) {
    noise_reduction::deNoise(_bayer8, _denoiseType);
}

void raw_processing::applyCcm(Mat &_rgb) {
    color_correction::applySimpleCCM(_rgb);
}

void raw_processing::applyGamma(Mat& _rgb) {
    gamma_correction::applySimpleGamma(_rgb);
}
