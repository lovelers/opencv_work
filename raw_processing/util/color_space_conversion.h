#ifndef __COLOR_SPACE_CONVERSION_H
#define __COLOR_SPACE_CONVERSION_H
#include "opencv2/core.hpp"
using namespace cv;

class color_conversion {
    public:
        static void rgb_2_yuv(const Mat3w & _rgb, Mat3w & _yuv);
        static void yuv_2_rgb(const Mat3w & _yuv, Mat3w & _rgb);
        static void bayer_2_rgb(const Mat1w& _bayer, Mat3w *_rgb, int _bayerPattern, int _max);
        /*
         * Gr, R, Gr, R, Gr, R, Gr, R, Gr, R, Gr
         *  B, Gb, B, Gb, B, Gb, B, Gb, B, Gb, B
         * Gr, R, Gr, R, Gr, R, Gr, R, Gr, R, Gr
         *  B, Gb, B, Gb, B, Gb, B, Gb, B, Gb, B
         * Gr, R, Gr, R, Gr, R, Gr, R, Gr, R, Gr
         *  B, Gb, B, Gb, B, Gb, B, Gb, B, Gb, B
         * Gr, R, Gr, R, Gr, R, Gr, R, Gr, R, Gr
         *  B, Gb, B, Gb, B, Gb, B, Gb, B, Gb, B
         * Gr, R, Gr, R, Gr, R, Gr, R, Gr, R, Gr
         *  B, Gb, B, Gb, B, Gb, B, Gb, B, Gb, B
         * Gr, R, Gr, R, Gr, R, Gr, R, Gr, R, Gr
         *  B, Gb, B, Gb, B, Gb, B, Gb, B, Gb, B
         */
        enum {
            RR_PATTERN = 0, // R at R location
            GR_PATTERN,     // G at R location
            BR_PATTERN,     // B at R location

            RGr_PATTERN,
            GGr_PATTERN,
            BGr_PATTERN,

            RGb_PATTERN,
            GGb_PATTERN,
            BGb_PATTERN,

            RB_PATTERN,
            GB_PATTERN,
            BB_PATTERN,

            RGB_PATTERN_MAX,
        };

    private:
        static float sDemosaicingCofficient[RGB_PATTERN_MAX];

        static int sDemosaicingPattern[];

        static void convertRGrGbB(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max);
        static void convertGrRBGb(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max);
        static void convertBGbGrR(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max);
        static void convertGbBRGr(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max);
        static ushort calcBayerWithPattern(const Mat1w& _bayer, int _pattern, int _max);

        color_conversion();
};
#endif// __COLOR_SPACE_CONVERSION_H
