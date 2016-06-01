#include "color_demosaicing_interpolation.h"
#include "bayer_buffer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#if 1
float demosaicing::sDemosaicingCofficient[RGB_PATTERN_MAX] = {
    1/8.f, 1/8.f, 1/8.f,
    1/8.f, 1/8.f, 1/8.f,
    1/8.f, 1/8.f, 1/8.f,
    1/4.f, 1/4.f, 1/4.f,
};

int demosaicing::sDemosaicingPattern[] = {
    // detla R coff = 1/2
    // R at R locations. // coff = 1/8.f
    0, 0, -1, 0, 0,
    0, 0, 0, 0, 0,
    -1, 0, 12, 0, -1,
    0, 0, 0, 0, 0,
    0, 0, -1, 0, 0,

    // detla R coff = 1/2
    // G at R locations.// coff = 1/8.f
    0, 0, -1, 0, 0,
    0, 0, 2, 0, 0,
    -1, 2, 4, 2, -1,
    0, 0, 2, 0, 0,
    0, 0, -1, 0, 0,

    // detla R coff = 1/2
    // B at R locations. // coff = 1/8.f
    0, 0, -1, 0, 0,
    0, 2, 0, 2, 0,
    -1, 0, 4, 0, -1,
    0, 2, 0, 2, 0,
    0, 0, -1, 0, 0,


    //detla Gr coff = 1/2
    // R at Gr locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, -1, 0, -1, 0,
    0, 4, 4, 4, 0,
    0, -1, 0, -1, 0,
    0, 0, 0, 0, 0,

    //detla Gr coff = 1/2
    // G at Gr locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, -1, 0, -1, 0,
    0, 0, 4+8, 0, 0,
    0, -1, 0, -1, 0,
    0, 0, 0, 0, 0,


    // detla Gr coff = 1/2
    // B at Gr locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, -1, 4, -1, 0,
    0, 0, 4, 0, 0,
    0, -1, 4, -1, 0,
    0, 0, 0, 0, 0,


    //detla Gb coff = 1/2
    // R at Gb locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, -1, 4, -1, 0,
    0, 0, 4, 0, 0,
    0, -1, 4, -1, 0,
    0, 0, 0, 0, 0,

    //detla Gb coff = 1/2
    // G at Gb locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, -1, 0, -1, 0,
    0, 0, 4+8, 0, 0,
    0, -1, 0, -1, 0,
    0, 0, 0, 0, 0,


    //detla Gb coff = 1/2
    // B at Gb locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, -1, 0, -1, 0,
    0, 4, 4, 4, 0,
    0, -1, 0, -1, 0,
    0, 0, 0, 0, 0,

    // detla B coff = 1.f
    // R at B locations. // coff = 1/4.f
    0, 0, -1, 0, 0,
    0, 1, 0, 1, 0,
    -1, 0, 4, 0, -1,
    0, 1, 0, 1, 0,
    0, 0, -1, 0, 0,

    // detla B coff = 1.f
    // G at B locations. // coff = 1/4.f
    0, 0, -1, 0, 0,
    0, 0, 1, 0, 0,
    -1, 1, 4, 1, -1,
    0, 0, 1, 0, 0,
    0, 0, -1, 0, 0,

    // detla B coff = 1.f
    // B at B locations. // coff = 1/4.f
    0, 0, -1, 0, 0,
    0, 0, 0, 0, 0,
    -1, 0, 8, 0, -1,
    0, 0, 0, 0, 0,
    0, 0, -1, 0, 0,
};
#else

float demosaicing::sDemosaicingCofficient[RGB_PATTERN_MAX] = {
    1/5.f, 1/4.f, 1/4.f,
    1/2.f, 1/5.f, 1/2.f,
    1/2.f, 1/5.f, 1/2.f,
    1/4.f, 1/4.f, 1/5.f,
};

int demosaicing::sDemosaicingPattern[] = {
    // detla R coff = 1/2
    // R at R locations. // coff = 1/8.f
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
    1, 0, 1, 0, 1,
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,

    // detla R coff = 1/2
    // G at R locations.// coff = 1/8.f
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,

    // detla R coff = 1/2
    // B at R locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,


    //detla Gr coff = 1/2
    // R at Gr locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

    //detla Gr coff = 1/2
    // G at Gr locations. // coff = 1/8.f
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
    1, 0, 1, 0, 1,
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,


    // detla Gr coff = 1/2
    // B at Gr locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,


    //detla Gb coff = 1/2
    // R at Gb locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,

    //detla Gb coff = 1/2
    // G at Gb locations. // coff = 1/8.f
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
    1, 0, 1, 0, 1,
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,


    //detla Gb coff = 1/2
    // B at Gb locations. // coff = 1/8.f
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

    // detla B coff = 1.f
    // R at B locations. // coff = 1/4.f
    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,

    // detla B coff = 1.f
    // G at B locations. // coff = 1/4.f
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,

    // detla B coff = 1.f
    // B at B locations. // coff = 1/4.f
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
    1, 0, 1, 0, 1,
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
};
#endif

void demosaicing::convert(const Mat1w& bayer, Mat3w *rgb, int bayerPattern) {
    int border = 2;
    Mat1w padded;
    copyMakeBorder(bayer, padded, border, border, border, border, BORDER_REFLECT_101);
    cout << "padded. height = " << padded.rows << "width = " << padded.cols << endl;
    cout << "bayer. height = " << bayer.rows << "width = " << bayer.cols << endl;
    if (rgb == NULL) {
        return;
    }

    cout << "bayerPattern = " << bayerPattern << endl;
    switch(bayerPattern) {
        case bayer_buffer::BAYER_RGrGbB:
            convertRGrGbB(padded, rgb, bayer.rows, bayer.cols, border);
            break;
        case bayer_buffer::BAYER_GrRBGb:
            convertGrRBGb(padded, rgb, bayer.rows, bayer.cols, border);
            break;
        case bayer_buffer::BAYER_BGbGrR:
            convertBGbGrR(padded, rgb, bayer.rows, bayer.cols, border);
            break;
        case bayer_buffer::BAYER_GbBRGr:
            convertGbBRGr(padded, rgb, bayer.rows, bayer.cols, border);
            break;
        default:
            cout <<"unsupport bayer pattern type:" << endl;
            break;
    }
}

void demosaicing::convertRGrGbB(const Mat1w& padded, Mat3w *rgb, int _height, int _width, int border) {
    cout << "height x width = " << _height << "x" << _width <<endl;
    typedef Point3_<ushort> Pixel;
    for (int h = 0; h < _height; ++h) {
        Pixel* bgr = rgb->ptr<Pixel>(h);
        //color b g r map to point x, y , z
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 0) {
                Mat1w bayerRMat(padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(bayerRMat, RR_PATTERN);
                bgr[w].y = calcBayerWithPattern(bayerRMat, GR_PATTERN);
                bgr[w].x = calcBayerWithPattern(bayerRMat, BR_PATTERN);

                Mat1w bayerGrMat(padded, Rect((w+1), h, 5, 5));
                bgr[w+1].z = calcBayerWithPattern(bayerGrMat, RGr_PATTERN);
                bgr[w+1].y = calcBayerWithPattern(bayerGrMat, GGr_PATTERN);
                bgr[w+1].x = calcBayerWithPattern(bayerGrMat, BGr_PATTERN);
            } else {
                Mat1w bayerGbMat(padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(bayerGbMat, RGb_PATTERN);
                bgr[w].y = calcBayerWithPattern(bayerGbMat, GGb_PATTERN);
                bgr[w].x = calcBayerWithPattern(bayerGbMat, BGb_PATTERN);

                Mat1w bayerBMat(padded, Rect((w+1), h, 5, 5));
                bgr[w+1].z   = calcBayerWithPattern(bayerBMat, RB_PATTERN);
                bgr[w+1].y   = calcBayerWithPattern(bayerBMat, GB_PATTERN);
                bgr[w+1].x   = calcBayerWithPattern(bayerBMat, BB_PATTERN);
            }
        }
    }
}

void demosaicing::convertGrRBGb(const Mat1w& padded, Mat3w *rgb, int _height, int _width, int border) {
    cout << "height x width = " << _height << "x" << _width <<endl;
    Mat3w bgr = *rgb;
    for (int h = 0; h < _height; ++h) {
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 0) {
                Mat1w bayerGrMat(padded, Rect(w, h, 5, 5));
                bgr(h, w)[2] = calcBayerWithPattern(bayerGrMat, RGr_PATTERN);
                bgr(h, w)[1] = calcBayerWithPattern(bayerGrMat, GGr_PATTERN);
                bgr(h, w)[0] = calcBayerWithPattern(bayerGrMat, BGr_PATTERN);

                Mat1w bayerRMat(padded, Rect(w+1, h, 5, 5));
                bgr(h, w+1)[2] = calcBayerWithPattern(bayerRMat, RR_PATTERN);
                bgr(h, w+1)[1] = calcBayerWithPattern(bayerRMat, GR_PATTERN);
                bgr(h, w+1)[0] = calcBayerWithPattern(bayerRMat, BR_PATTERN);

            } else {
                Mat1w bayerBMat(padded, Rect(w, h, 5, 5));
                bgr(h, w)[2]  = calcBayerWithPattern(bayerBMat, RB_PATTERN);
                bgr(h, w)[1]  = calcBayerWithPattern(bayerBMat, GB_PATTERN);
                bgr(h, w)[0]  = calcBayerWithPattern(bayerBMat, BB_PATTERN);

                Mat1w bayerGbMat(padded, Rect(w+1, h, 5, 5));
                bgr(h, w+1)[2] = calcBayerWithPattern(bayerGbMat, RGb_PATTERN);
                bgr(h, w+1)[1] = calcBayerWithPattern(bayerGbMat, GGb_PATTERN);
                bgr(h, w+1)[0] = calcBayerWithPattern(bayerGbMat, BGb_PATTERN);
            }
        }
    }
}

void demosaicing::convertBGbGrR(const Mat1w& padded, Mat3w *rgb, int _height, int _width, int border) {
    cout << "height x width = " << _height << "x" << _width <<endl;
    typedef Point3_<ushort> Pixel;
    for (int h = 0; h < _height; ++h) {
        Pixel* bgr = rgb->ptr<Pixel>(h);
        //color b g r map to point x, y , z
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 1) {
                Mat1w bayerGrMat(padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(bayerGrMat, RGr_PATTERN);
                bgr[w].y = calcBayerWithPattern(bayerGrMat, GGr_PATTERN);
                bgr[w].x = calcBayerWithPattern(bayerGrMat, BGr_PATTERN);

                Mat1w bayerRMat(padded, Rect(w+1, h, 5, 5));
                bgr[w+1].z = calcBayerWithPattern(bayerRMat, RR_PATTERN);
                bgr[w+1].y = calcBayerWithPattern(bayerRMat, GR_PATTERN);
                bgr[w+1].x = calcBayerWithPattern(bayerRMat, BR_PATTERN);

            } else {
                Mat1w bayerBMat(padded, Rect(w, h, 5, 5));
                bgr[w].z   = calcBayerWithPattern(bayerBMat, RB_PATTERN);
                bgr[w].y   = calcBayerWithPattern(bayerBMat, GB_PATTERN);
                bgr[w].x   = calcBayerWithPattern(bayerBMat, BB_PATTERN);

                Mat1w bayerGbMat(padded, Rect(w+1, h, 5, 5));
                bgr[w+1].z = calcBayerWithPattern(bayerGbMat, RGb_PATTERN);
                bgr[w+1].y = calcBayerWithPattern(bayerGbMat, GGb_PATTERN);
                bgr[w+1].x = calcBayerWithPattern(bayerGbMat, BGb_PATTERN);
            }
        }
    }
}

void demosaicing::convertGbBRGr(const Mat1w& padded, Mat3w *rgb, int _height, int _width, int border) {
    cout << "height x width = " << _height << "x" << _width <<endl;
    typedef Point3_<ushort> Pixel;
    for (int h = 0; h < _height; ++h) {
        Pixel* bgr = rgb->ptr<Pixel>(h);
        //color b g r map to point x, y , z
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 1) {
                Mat1w bayerRMat(padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(bayerRMat, RR_PATTERN);
                bgr[w].y = calcBayerWithPattern(bayerRMat, GR_PATTERN);
                bgr[w].x = calcBayerWithPattern(bayerRMat, BR_PATTERN);

                Mat1w bayerGrMat(padded, Rect((w+1), h, 5, 5));
                bgr[w+1].z = calcBayerWithPattern(bayerGrMat, RGr_PATTERN);
                bgr[w+1].y = calcBayerWithPattern(bayerGrMat, GGr_PATTERN);
                bgr[w+1].x = calcBayerWithPattern(bayerGrMat, BGr_PATTERN);
            } else {
                Mat1w bayerGbMat(padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(bayerGbMat, RGb_PATTERN);
                bgr[w].y = calcBayerWithPattern(bayerGbMat, GGb_PATTERN);
                bgr[w].x = calcBayerWithPattern(bayerGbMat, BGb_PATTERN);

                Mat1w bayerBMat(padded, Rect((w+1), h, 5, 5));
                bgr[w+1].z   = calcBayerWithPattern(bayerBMat, RB_PATTERN);
                bgr[w+1].y   = calcBayerWithPattern(bayerBMat, GB_PATTERN);
                bgr[w+1].x   = calcBayerWithPattern(bayerBMat, BB_PATTERN);
            }
        }
    }
}

ushort demosaicing::calcBayerWithPattern(const Mat1w& bayer, int pattern) {
    int result = 0;
    int *patternStart = sDemosaicingPattern;
    patternStart += pattern * 25;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (patternStart[5*i +j] != 0) {
                result += (int)(bayer(i,j)) * patternStart[5*i + j];
            }
        }
    }
    result = result * sDemosaicingCofficient[pattern];
    result = result < 0 ? 0 : (result > 0xFFFF ? 0xFFFF : result);
    return (ushort) result;
}
