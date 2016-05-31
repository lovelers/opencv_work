#ifndef __BAYER_BUFFER_H
#define __BAYER_BUFFER_H

#include "opencv2/core.hpp"
using namespace cv;
using namespace std;

class bayer_buffer {
    public:
        enum Bayer_Pattern_Type {
            BAYER_UNKNOWN = -1,
            BAYER_RGrGbB = 0,
            BAYER_GrRBGb = 1,
            BAYER_BGbGrR,
            BAYER_GbBRGr
        };

        typedef struct __tow_pixel {
            unsigned int a : 12;
            unsigned int b : 12;
        } __attribute__((__packed__)) two_pixel;
        bayer_buffer();
        bool init(const char *_file, int _height, int _width, int _bitdepths);
        void uninit();
        void setPattern(Bayer_Pattern_Type _pattern);
        inline Bayer_Pattern_Type getPattern() const { return pattern;};
        inline const Mat& getMat() const { return bayer;};
    private:
        Mat bayer;
        int width;
        int height;
        int bitdepths;
        Bayer_Pattern_Type pattern;
        void fastParserBayer12( uchar *_bayerBuffer, Mat _bayer);
};

#endif // __BAYER_BUFFER_H
