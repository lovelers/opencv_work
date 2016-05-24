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

bool raw_processing::selectRawFile(const char *_file, int _width, int _height, int _bitdepths) {
    if (bayer != NULL) {
        return bayer->init(_file, _width, _height, _bitdepths);
    }
    return false;
}

const Mat& raw_processing::getBayerMat() {
    return bayer->getMat();
}

void raw_processing::demosaicing(const Mat& bayer, Mat *rgbMat, int bayerPattern) {
    demosaicing::convert(bayer, rgbMat, bayerPattern);
}
