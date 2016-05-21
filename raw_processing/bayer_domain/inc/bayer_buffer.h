#ifndef __BAYER_BUFFER_H
#define __BAYER_BUFFER_H


#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

class bayer_buffer {
    public:
        typedef unsigned short U16;
        enum Bayer_Pattern_Type {
            BAYER_UNKNOWN = -1,
            BAYER_RGrGbB = 0,
        };
        bayer_buffer();
        bool init(const char *_file, int _weight, int _height, int _bitdepths);
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
};

#endif // __BAYER_BUFFER_H
