#include <iostream>
#include "raw_processing.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/photo.hpp"

using namespace std;

#define RAW_WIDTH_DEFAULT 5312
#define RAW_HEIGHT_DEFAULT 3984
#define RAW_BITDEPTHS_DEFAULT 12
#define RAW_FILE_NAME "./res/raw1.buffer"

#define USE_OPENCV_TEST false
#define PROCESSING_BAYER_16 true
#define PROCESSING_BAYER_8 true
#define PROCESSING BAYER_14 true
//#define RAW_FILE_NAME "./res/RawCapture0_20160526_163445.raw"
int main(int argc, char**argv) {
    int height = RAW_HEIGHT_DEFAULT;
    int width = RAW_WIDTH_DEFAULT;
    const char *file = RAW_FILE_NAME;
    int bitdepths = RAW_BITDEPTHS_DEFAULT;
    //int pattern = bayer_buffer::BAYER_RGrGbB;
    int pattern = bayer_buffer::BAYER_GrRBGb;
    //int pattern = bayer_buffer::BAYER_BGbGrR;
    //int pattern = bayer_buffer::BAYER_GbBRGr;
    raw_processing & raw = raw_processing::getInstance();
    raw.selectRawFile(file,
            height,
            width,
            bitdepths);
    const Mat & bayerMat = raw.getBayerMat();
    cout <<"finish the raw parser" << endl;

#if PROCESSING_BAYER_8
    Mat1w bayer8(height, width);
    for (int row = 0; row < bayerMat.rows; ++row) {
        for (int col = 0; col < bayerMat.cols; ++col) {
            bayer8(row, col) = (ushort)(bayerMat.at<ushort>(row, col)>>4  & 0xFF);
        }
    }

    FILE *fp8 = fopen("res/bayer8.raw", "wb");
    fwrite(bayer8.data, 1, height * width * sizeof(uchar), fp8);
    fclose(fp8);

    raw.deNoise(bayer8, noise_reduction::AVERAGE_DENOISE, pattern);

    Mat3w rgb8(height, width);
    raw.demosaicing(bayer8, &rgb8, pattern);
    raw.applyCcm(rgb8, 255);
    raw.applyGamma(rgb8, 128, 0xFF + 1);

    Mat3b rgb8b(height, width);
    for (int row = 0; row < rgb8.rows; ++row) {
        for (int col = 0; col < rgb8.cols; ++col) {
            rgb8b(row, col)[0] = rgb8(row, col)[0] > 255 ? 255 : rgb8(row, col)[0];
            rgb8b(row, col)[1] = rgb8(row, col)[1] > 255 ? 255 : rgb8(row, col)[1];
            rgb8b(row, col)[2] = rgb8(row, col)[2] > 255 ? 255 : rgb8(row, col)[2];
        }
    }
    imwrite("res/rgb8.jpg", rgb8b);
    resize(rgb8b, rgb8b, Size(800, 600));
    imshow("rgb8", rgb8b);
    waitKey();

#if USE_OPENCV_TEST
    Mat1w imgGray(height, width);
    switch (pattern) {
        case bayer_buffer::BAYER_RGrGbB:
            cvtColor(bayer8, imgGray, COLOR_BayerRG2GRAY);
            break;
        case  bayer_buffer::BAYER_GrRBGb:
            cvtColor(bayer8, imgGray, COLOR_BayerGB2GRAY);
            break;
        case bayer_buffer::BAYER_BGbGrR:
            cvtColor(bayer8, imgGray, COLOR_BayerBG2GRAY);
            break;
        case  bayer_buffer::BAYER_GbBRGr:
            cvtColor(bayer8, imgGray, COLOR_BayerGR2GRAY);
            break;
        default:
            break;
    }

    Mat1b gray8(height, width);
    for (int row = 0; row < gray8.rows; ++row) {
        for (int col = 0; col < gray8.cols; ++col) {
            gray8(row, col) = imgGray(row, col) > 0xFF ? 0xFF : imgGray(row, col);
        }
    }

    imwrite("res/gray.jpg", gray8);
    resize(gray8, gray8, Size(800, 600));
    imshow("imgGray", gray8);
    waitKey();
    Mat3w imgColor(height, width);
    switch (pattern) {
        case bayer_buffer::BAYER_RGrGbB:
            cvtColor(bayer8, imgColor, COLOR_BayerRG2BGR);
            break;
        case  bayer_buffer::BAYER_GrRBGb:
            cvtColor(bayer8, imgColor, COLOR_BayerGB2BGR);
            break;
        case bayer_buffer::BAYER_BGbGrR:
            cvtColor(bayer8, imgColor, COLOR_BayerBG2BGR);
            break;
        case  bayer_buffer::BAYER_GbBRGr:
            cvtColor(bayer8, imgColor, COLOR_BayerGR2BGR);
            break;
        default:
            break;
    }
    raw.applyCcm(imgColor, 255);
    raw.applyGamma(imgColor, 128, 0xFF + 1);

    Mat3b rgb8bb(height, width);
    for (int row = 0; row < rgb8.rows; ++row) {
        for (int col = 0; col < rgb8.cols; ++col) {
            rgb8bb(row, col)[0] = imgColor(row, col)[0] > 255 ? 255 : imgColor(row, col)[0];
            rgb8bb(row, col)[1] = imgColor(row, col)[1] > 255 ? 255 : imgColor(row, col)[1];
            rgb8bb(row, col)[2] = imgColor(row, col)[2] > 255 ? 255 : imgColor(row, col)[2];
        }
    }
    imwrite("res/rgb8bb.jpg", rgb8bb);
    resize(rgb8bb, rgb8bb, Size(800, 600));
    imshow("rgb8bb.jpg", rgb8bb);
    waitKey();
#endif
#endif

#if PROCESSING_BAYER_16
    Mat1w bayer16(height, width);
    for (int row = 0; row < bayerMat.rows; ++row) {
        for (int col = 0; col < bayerMat.cols; ++col) {
            bayer16(row, col) = (ushort)(bayerMat.at<ushort>(row, col) << 4);
        }
    }

    FILE *fp16 = fopen("res/bayer16.raw", "wb");
    fwrite(bayer16.data, 1, height * width * sizeof(ushort), fp16);
    fclose(fp16);
    raw.deNoise(bayer16, noise_reduction::AVERAGE_DENOISE, pattern);

#if USE_OPENCV_TEST
    Mat1w imgGray(height, width);
    switch (pattern) {
        case bayer_buffer::BAYER_RGrGbB:
            cvtColor(bayer16, imgGray, COLOR_BayerRG2GRAY);
            break;
        case  bayer_buffer::BAYER_GrRBGb:
            cvtColor(bayer16, imgGray, COLOR_BayerGB2GRAY);
            break;
        case bayer_buffer::BAYER_BGbGrR:
            cvtColor(bayer16, imgGray, COLOR_BayerBG2GRAY);
            break;
        case  bayer_buffer::BAYER_GbBRGr:
            cvtColor(bayer16, imgGray, COLOR_BayerGR2GRAY);
            break;
        default:
            break;
    }
    imwrite("res/gray.jpg", imgGray);
    resize(imgGray, imgGray, Size(800, 600));
    imshow("imgGray", imgGray);
    waitKey();
#endif
    Mat3w rgb16(height, width);
    raw.demosaicing(bayer16, &rgb16, pattern);
    raw.applyCcm(rgb16, 0xFFFF);
    raw.applyGamma(rgb16, 128, 0xFFFF + 1);
    imwrite("res/rgb16.jpg", rgb16);
    resize(rgb16, rgb16, Size(800, 600));
    imshow("rgb16.jpg", rgb16);
    waitKey();
#endif
    return 0;
}
