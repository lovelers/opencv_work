#include <iostream>
#include "raw_processing.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>

using namespace std;

#define RAW_WIDTH_DEFAULT 5312
#define RAW_HEIGHT_DEFAULT 3984
#define RAW_BITDEPTHS_DEFAULT 12
#define RAW_FILE_NAME "./res/raw.buffer"
int main(int argc, char**argv) {

    raw_processing & raw = raw_processing::getInstance();
    raw.selectRawFile(RAW_FILE_NAME,
            RAW_WIDTH_DEFAULT,
            RAW_HEIGHT_DEFAULT,
            RAW_BITDEPTHS_DEFAULT);
    const Mat & bayerMat = raw.getBayerMat();
#if 1
    Mat imgGray(RAW_WIDTH_DEFAULT, RAW_HEIGHT_DEFAULT, CV_16UC1);
    cvtColor(bayerMat, imgGray, COLOR_BayerGB2GRAY);
    resize(imgGray, imgGray, Size(800, 600));
    imshow("imgGray", imgGray);
    waitKey();
#endif
    return 0;
}
