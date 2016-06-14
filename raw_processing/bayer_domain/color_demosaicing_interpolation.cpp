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

void demosaicing::convert(const Mat1w& _bayer, Mat3w *_rgb, int _bayerPattern, int _max) {
    int border = 2;
    Mat1w padded;
    copyMakeBorder(_bayer, padded, border, border, border, border, BORDER_REFLECT_101);
    cout << "_padded. height = " << padded.rows << "width = " << padded.cols << endl;
    cout << "_bayer. height = " << _bayer.rows << "width = " << _bayer.cols << endl;
    if (_rgb == NULL) {
        return;
    }

    cout << "_bayerPattern = " << _bayerPattern << endl;
    switch(_bayerPattern) {
        case bayer_buffer::BAYER_RGrGbB:
            convertRGrGbB(padded, _rgb, _bayer.rows, _bayer.cols, border, _max);
            break;
        case bayer_buffer::BAYER_GrRBGb:
            convertGrRBGb(padded, _rgb, _bayer.rows, _bayer.cols, border, _max);
            break;
        case bayer_buffer::BAYER_BGbGrR:
            convertBGbGrR(padded, _rgb, _bayer.rows, _bayer.cols, border, _max);
            break;
        case bayer_buffer::BAYER_GbBRGr:
            convertGbBRGr(padded, _rgb, _bayer.rows, _bayer.cols, border, _max);
            break;
        default:
            cout <<"unsupport _bayer pattern type:" << endl;
            break;
    }
}

void demosaicing::convertRGrGbB(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border ,int _max) {
    cout << "height x width = " << _height << "x" << _width <<endl;
    typedef Point3_<ushort> Pixel;
    for (int h = 0; h < _height; ++h) {
        Pixel* bgr = _rgb->ptr<Pixel>(h);
        //color b g r map to point x, y , z
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 0) {
                Mat1w _bayerRMat(_padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(_bayerRMat, RR_PATTERN, _max);
                bgr[w].y = calcBayerWithPattern(_bayerRMat, GR_PATTERN, _max);
                bgr[w].x = calcBayerWithPattern(_bayerRMat, BR_PATTERN, _max);

                Mat1w _bayerGrMat(_padded, Rect((w+1), h, 5, 5));
                bgr[w+1].z = calcBayerWithPattern(_bayerGrMat, RGr_PATTERN, _max);
                bgr[w+1].y = calcBayerWithPattern(_bayerGrMat, GGr_PATTERN, _max);
                bgr[w+1].x = calcBayerWithPattern(_bayerGrMat, BGr_PATTERN, _max);
            } else {
                Mat1w _bayerGbMat(_padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(_bayerGbMat, RGb_PATTERN, _max);
                bgr[w].y = calcBayerWithPattern(_bayerGbMat, GGb_PATTERN, _max);
                bgr[w].x = calcBayerWithPattern(_bayerGbMat, BGb_PATTERN, _max);

                Mat1w _bayerBMat(_padded, Rect((w+1), h, 5, 5));
                bgr[w+1].z   = calcBayerWithPattern(_bayerBMat, RB_PATTERN, _max);
                bgr[w+1].y   = calcBayerWithPattern(_bayerBMat, GB_PATTERN, _max);
                bgr[w+1].x   = calcBayerWithPattern(_bayerBMat, BB_PATTERN, _max);
            }
        }
    }
}

void demosaicing::convertGrRBGb(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max) {
    cout << "height x width = " << _height << "x" << _width <<endl;
    Mat3w bgr = *_rgb;
    for (int h = 0; h < _height; ++h) {
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 0) {
                Mat1w _bayerGrMat(_padded, Rect(w, h, 5, 5));
                bgr(h, w)[2] = calcBayerWithPattern(_bayerGrMat, RGr_PATTERN, _max);
                bgr(h, w)[1] = calcBayerWithPattern(_bayerGrMat, GGr_PATTERN, _max);
                bgr(h, w)[0] = calcBayerWithPattern(_bayerGrMat, BGr_PATTERN, _max);

                Mat1w _bayerRMat(_padded, Rect(w+1, h, 5, 5));
                bgr(h, w+1)[2] = calcBayerWithPattern(_bayerRMat, RR_PATTERN, _max);
                bgr(h, w+1)[1] = calcBayerWithPattern(_bayerRMat, GR_PATTERN, _max);
                bgr(h, w+1)[0] = calcBayerWithPattern(_bayerRMat, BR_PATTERN, _max);

            } else {
                Mat1w _bayerBMat(_padded, Rect(w, h, 5, 5));
                bgr(h, w)[2]  = calcBayerWithPattern(_bayerBMat, RB_PATTERN, _max);
                bgr(h, w)[1]  = calcBayerWithPattern(_bayerBMat, GB_PATTERN, _max);
                bgr(h, w)[0]  = calcBayerWithPattern(_bayerBMat, BB_PATTERN, _max);

                Mat1w _bayerGbMat(_padded, Rect(w+1, h, 5, 5));
                bgr(h, w+1)[2] = calcBayerWithPattern(_bayerGbMat, RGb_PATTERN, _max);
                bgr(h, w+1)[1] = calcBayerWithPattern(_bayerGbMat, GGb_PATTERN, _max);
                bgr(h, w+1)[0] = calcBayerWithPattern(_bayerGbMat, BGb_PATTERN, _max);
            }
        }
    }
}

void demosaicing::convertBGbGrR(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max) {
    cout << "height x width = " << _height << "x" << _width <<endl;
    typedef Point3_<ushort> Pixel;
    for (int h = 0; h < _height; ++h) {
        Pixel* bgr = _rgb->ptr<Pixel>(h);
        //color b g r map to point x, y , z
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 1) {
                Mat1w _bayerGrMat(_padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(_bayerGrMat, RGr_PATTERN, _max);
                bgr[w].y = calcBayerWithPattern(_bayerGrMat, GGr_PATTERN, _max);
                bgr[w].x = calcBayerWithPattern(_bayerGrMat, BGr_PATTERN, _max);

                Mat1w _bayerRMat(_padded, Rect(w+1, h, 5, 5));
                bgr[w+1].z = calcBayerWithPattern(_bayerRMat, RR_PATTERN, _max);
                bgr[w+1].y = calcBayerWithPattern(_bayerRMat, GR_PATTERN, _max);
                bgr[w+1].x = calcBayerWithPattern(_bayerRMat, BR_PATTERN, _max);

            } else {
                Mat1w _bayerBMat(_padded, Rect(w, h, 5, 5));
                bgr[w].z   = calcBayerWithPattern(_bayerBMat, RB_PATTERN, _max);
                bgr[w].y   = calcBayerWithPattern(_bayerBMat, GB_PATTERN, _max);
                bgr[w].x   = calcBayerWithPattern(_bayerBMat, BB_PATTERN, _max);

                Mat1w _bayerGbMat(_padded, Rect(w+1, h, 5, 5));
                bgr[w+1].z = calcBayerWithPattern(_bayerGbMat, RGb_PATTERN, _max);
                bgr[w+1].y = calcBayerWithPattern(_bayerGbMat, GGb_PATTERN, _max);
                bgr[w+1].x = calcBayerWithPattern(_bayerGbMat, BGb_PATTERN, _max);
            }
        }
    }
}

void demosaicing::convertGbBRGr(const Mat1w& _padded, Mat3w *_rgb, int _height, int _width, int _border, int _max) {
    cout << "height x width = " << _height << "x" << _width <<endl;
    typedef Point3_<ushort> Pixel;
    for (int h = 0; h < _height; ++h) {
        Pixel* bgr = _rgb->ptr<Pixel>(h);
        //color b g r map to point x, y , z
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 1) {
                Mat1w _bayerRMat(_padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(_bayerRMat, RR_PATTERN, _max);
                bgr[w].y = calcBayerWithPattern(_bayerRMat, GR_PATTERN, _max);
                bgr[w].x = calcBayerWithPattern(_bayerRMat, BR_PATTERN, _max);

                Mat1w _bayerGrMat(_padded, Rect((w+1), h, 5, 5));
                bgr[w+1].z = calcBayerWithPattern(_bayerGrMat, RGr_PATTERN, _max);
                bgr[w+1].y = calcBayerWithPattern(_bayerGrMat, GGr_PATTERN, _max);
                bgr[w+1].x = calcBayerWithPattern(_bayerGrMat, BGr_PATTERN, _max);
            } else {
                Mat1w _bayerGbMat(_padded, Rect(w, h, 5, 5));
                bgr[w].z = calcBayerWithPattern(_bayerGbMat, RGb_PATTERN, _max);
                bgr[w].y = calcBayerWithPattern(_bayerGbMat, GGb_PATTERN, _max);
                bgr[w].x = calcBayerWithPattern(_bayerGbMat, BGb_PATTERN, _max);

                Mat1w _bayerBMat(_padded, Rect((w+1), h, 5, 5));
                bgr[w+1].z   = calcBayerWithPattern(_bayerBMat, RB_PATTERN, _max);
                bgr[w+1].y   = calcBayerWithPattern(_bayerBMat, GB_PATTERN, _max);
                bgr[w+1].x   = calcBayerWithPattern(_bayerBMat, BB_PATTERN, _max);
            }
        }
    }
}

ushort demosaicing::calcBayerWithPattern(const Mat1w& _bayer, int _pattern, int _max) {
    int result = 0;
    int *patternStart = sDemosaicingPattern;
    patternStart += _pattern * 25;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (patternStart[5*i +j] != 0) {
                result += (int)(_bayer(i,j)) * patternStart[5*i + j];
            }
        }
    }
    result = result * sDemosaicingCofficient[_pattern];
    result = result < 0 ? 0 : (result > _max ? _max : result);
    return (ushort) result;
}
