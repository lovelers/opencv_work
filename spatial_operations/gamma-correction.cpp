#include <stdio.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include "common.h"

using namespace cv;
using namespace std;

const float gamma_r = 0.6f;
//y = gamma_c * pow(x, gamma_r);
const float gamma_c = 255.f / pow(255, gamma_r);
uchar gamma_correction(uint value) {
    uint y =  gamma_c * pow(value, gamma_r);
    return (y > 255) ? 255 : (uchar)y;
}
int main( int argc, char ** argv) {
    const char * imagename = argc > 1 ? argv[1] : LENA_JPG;

    Ptr<IplImage> iplimg(cvLoadImage(imagename));
    if (!iplimg) {
        cout << "can't load image" << imagename << endl;
        return -1;
    }

    Mat img = cv::cvarrToMat(iplimg);
    if (img.empty()) return -1;

    Mat img_yuv;
    cvtColor(img, img_yuv, COLOR_BGR2YCrCb);
    vector<Mat> planes;
    split(img_yuv, planes);


    for (int y = 0; y < img_yuv.rows; ++y) {
        for (int x = 0; x < img_yuv.cols; ++x) {
            planes[0].at<uchar>(y, x) = gamma_correction(planes[0].at<uchar>(y, x));
        }
    }

    merge(planes, img_yuv);
    cvtColor(img_yuv, img, COLOR_YCrCb2BGR);

    namedWindow("image with gamma", WINDOW_AUTOSIZE);

#if 0
    cvShotImage("image with gray", iplimg);
#else
    imshow("image with gamma", img);
#endif
    waitKey();
    return 0;
}
