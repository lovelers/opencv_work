#ifndef __RAW_PROCESSING_H
#define __RAW_PROCESSING_H
#include "bayer_domain/inc/bayer_buffer.h"
#include "bayer_domain/inc/color_demosaicing_interpolation.h"

using namespace cv;
using namespace std;

class raw_processing {
    public:
        static raw_processing& getInstance() {
            static raw_processing raw;
            return raw;
        }
        bool selectRawFile(const char *_file, int _width, int _height, int _bitdepths);
        const Mat& getBayerMat();
        void demosaicing(const Mat& bayer, Mat *rgbMat, int bayerPattern);
        ~raw_processing();

    private:
        bayer_buffer *bayer;
        raw_processing();
        raw_processing(const raw_processing&);
        raw_processing & operator = (const raw_processing &);
};

#endif// __RAW_PROCESSING_H
