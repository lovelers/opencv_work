#include <iostream>
#include <stdio.h>
#include "bayer_buffer.h"
using namespace std;
using namespace cv;

bayer_buffer::bayer_buffer() {
    width = -1;
    height = -1;
    bitdepths = 0;
    pattern = BAYER_UNKNOWN;
}


const static int width_max = 8192;
const static int height_max = 8192;
const static int bitdepths_max = 16;

bool bayer_buffer::init(const char *_file, int _height, int _width, int _bitdepths) {
    if (_file == NULL
            || _width < 0 || _width > width_max
            || _height < 0 || _height > height_max
            || _bitdepths < 0 || _bitdepths > bitdepths_max) {
        cout << __func__ << "parameters invalid" << endl;
        return false;
    }

    FILE *fp = fopen(_file, "rb");
    if (fp == NULL) {
        cout << __func__ << "open " << _file << "failed" << endl;
        return false;
    }

    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    if (fileSize < (_width * _height * _bitdepths + 7) / 8) {
        cout << __func__ << "file size:" << fileSize << "doesn't match the settings: "
            << _width << " x " << _height << ",bitdepths:" << _bitdepths << endl;
        fclose(fp);
        return false;
    }

    rewind(fp);
    uchar *bayerBuffer = new uchar[fileSize];
    int rc = fread(bayerBuffer, 1, fileSize, fp);
    cout << "rc = " << rc << endl;

    bayer.create(_height, _width, CV_16UC1);
    if (_bitdepths == 12) {
        fastParserBayer12(bayerBuffer, bayer);
    } else {
        cout << "can't support bitdepths:" << _bitdepths << endl;
    }

    cout << "fileSize = " << fileSize << ",seekto = " << ftell(fp)<< endl;

    fclose(fp);
    width = _width;
    height = _height;
    bitdepths = _bitdepths;
}

void bayer_buffer::uninit() {
}

void bayer_buffer::setPattern(Bayer_Pattern_Type _pattern) {
    pattern  = _pattern;
}

void bayer_buffer::fastParserBayer12(uchar *_bayerBuffer, Mat _bayer) {
    MatIterator_<ushort> it = _bayer.begin<ushort>(),
        it_end = _bayer.end<ushort>();
    two_pixel * pixel = (two_pixel *)_bayerBuffer;
    while (it!=it_end) {
        *it++ = (ushort)(pixel->a);
        *it++ = (ushort)(pixel->b);
        ++pixel;
    }
}

void bayer_buffer::makeSamBayer(int _height, int _width, const char *_file) {
    //_bayer.create(_height, _width, CV_16UC1);
    two_pixel pixel;
    FILE *fp = fopen(_file, "wb+");
    for (int row = 0; row < _height;  ++row) {
        unsigned int value = static_cast<unsigned int> (row / 128 * 128);
        for (int col = 0; col < _width; col+=2) {
            pixel.a = pixel.b = value;
            fwrite(&pixel, 1, sizeof (two_pixel), fp);
        }
    }
    fclose(fp);
}
