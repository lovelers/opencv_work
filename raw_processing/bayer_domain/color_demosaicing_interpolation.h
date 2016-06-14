#ifndef __COLOR_DEMOSAICING_INTERPOLATION_H
#define __COLOR_DEMOSAICING_INTERPOLATION_H
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
/**
 * the demosaicing intperpolation method is based on the
 * "HIGH-QUALITY LINEAR INTERPOLATION FOR DEMOSAICING OF BAYER-PATTERNED COLOR IMAGES"
 * author :Henrique S. Malvar, Li-wei He, and Ross Cutler
 */
class demosaicing {
    public:
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

        static float sDemosaicingCofficient[RGB_PATTERN_MAX];

        static int sDemosaicingPattern[];

        static void convert(const Mat1w& _bayer, Mat3w *_rgb, int _bayerPattern, int _max);
        static void convertRGrGbB(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max);
        static void convertGrRBGb(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max);
        static void convertBGbGrR(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max);
        static void convertGbBRGr(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max);
        static ushort calcBayerWithPattern(const Mat1w& _bayer, int _pattern, int _max);

    private:
        demosaicing();
};
#endif // __COLOR_DEMOSAICING_INTERPOLATION_H
