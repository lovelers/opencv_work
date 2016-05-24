#include "inc/color_demosaicing_interpolation.h"
#include "inc/bayer_buffer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

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

void demosaicing::convert(const Mat& bayerMat, Mat *rgbMat, int bayerPattern) {
    Mat padded;
    int border = 2;
    copyMakeBorder(bayerMat, padded, border, border, border, border, BORDER_REFLECT_101, Scalar::all(0));
    if (rgbMat == NULL) {
        return;
        //rgbMat = new Mat(bayerMat.rows(), bayerMat.cols(), CV_8UC3);
    }

    if (bayerPattern == bayer_buffer::BAYER_RGrGbB) {
        convertRGrGbB(padded, rgbMat, bayerMat.rows, bayerMat.cols, border);
    }
}

void demosaicing::convertRGrGbB(const Mat& padded, Mat *rgbMat, int _height, int _width, int border) {
    vector<Mat> rgbPlanes;
    split(*rgbMat, rgbPlanes);
    for (int h = 0; h < _height; ++h) {
        for (int w = 0; w < _width; w+=2) {
            if (h % 2 == 0) {
                Mat bayerRMat(padded, Rect(h, w, h+2*border+1, w+2*border+1));
                Mat bayerGrMat(padded, Rect(h, (w+1), h+2*border+1, (w+1)+2*border+1));
                rgbPlanes[0].at<ushort>(h, w) = calcBayerWithPattern(bayerRMat, RR_PATTERN); 
                rgbPlanes[1].at<ushort>(h, w) = calcBayerWithPattern(bayerRMat, GR_PATTERN); 
                rgbPlanes[2].at<ushort>(h, w) = calcBayerWithPattern(bayerRMat, BR_PATTERN); 

                rgbPlanes[0].at<ushort>(h, w+1) = calcBayerWithPattern(bayerGrMat, RGr_PATTERN); 
                rgbPlanes[1].at<ushort>(h, w+1) = calcBayerWithPattern(bayerGrMat, GGr_PATTERN); 
                rgbPlanes[2].at<ushort>(h, w+1) = calcBayerWithPattern(bayerGrMat, BGr_PATTERN); 
            } else {
                Mat bayerGbMat(padded, Rect(h, w, h+2*border+1, w+2*border+1));
                Mat bayerBMat(padded, Rect(h, (w+1), h+2*border+1, (w+1)+2*border+1));
                rgbPlanes[0].at<ushort>(h, w) = calcBayerWithPattern(bayerGbMat, RGb_PATTERN); 
                rgbPlanes[1].at<ushort>(h, w) = calcBayerWithPattern(bayerGbMat, GGb_PATTERN); 
                rgbPlanes[2].at<ushort>(h, w) = calcBayerWithPattern(bayerGbMat, BGb_PATTERN); 

                rgbPlanes[0].at<ushort>(h, w+1) = calcBayerWithPattern(bayerBMat, RB_PATTERN); 
                rgbPlanes[1].at<ushort>(h, w+1) = calcBayerWithPattern(bayerBMat, GB_PATTERN); 
                rgbPlanes[2].at<ushort>(h, w+1) = calcBayerWithPattern(bayerBMat, BB_PATTERN); 
            }
        }
    }
}

ushort demosaicing::calcBayerWithPattern(const Mat& bayerMat, int pattern) {
    int result = 0;
    int offset = pattern * 25;
    if (bayerMat.rows == bayerMat.cols || bayerMat.rows == 5) {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                result += bayerMat.at<ushort>(i, j) * sDemosaicingPattern[pattern + 5 * i + j];
            }
        }
        return  (ushort)result * sDemosaicingCofficient[pattern];
    }
    return 0;
}
