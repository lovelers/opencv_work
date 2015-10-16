#include <stdio.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include "common.h"

using namespace cv;
using namespace std;

enum neighborhood_operations {
    AVERAGE,
    MAXIMUM,
    MINIMUM,
};

void neighborhood_average(const vector<Mat> &src_planes, vector<Mat> &dst_planes, int rows, int cols){
    int x, y, x1, x2, y1, y2, plane;
    uint average;
    for (y = 0; y < rows; ++y) {
        for (x = 0; x < cols; ++x) {
            x1 = x - 1;
            x2 = x + 2;
            y1 = y - 1;
            y2 = y + 2;
            if (x1 < 0) x1 = 0;
            if (y1 < 0) y1 = 0;
            if (x2 > cols) x2 = cols;
            if (y2 > rows) y2 = rows;
            for( plane = 0; plane < src_planes.size(); ++plane) {
                Mat mat = src_planes[plane](Range(y1, y2), Range(x1, x2));
                MatConstIterator_<uchar> it = mat.begin<uchar>(), it_end = mat.end<uchar>();
                average = 0;
                for (; it != it_end; it++) {
                    average += (uint)*it;
                }
                dst_planes[plane].at<uchar>(y, x) = (uchar)(average / mat.total());
#if 1
                if (x == 0 && y == 0) {
                    cout << mat <<endl;
                    cout << mat.total() << endl;
                    cout << average << endl;
                    cout << (uint)dst_planes[plane].at<uchar>(y, x) << endl;
                }
#endif
            }
        }
    }
}

void neighborhood_maximum(const vector<Mat> &src_planes, vector<Mat> &dst_planes, int rows, int cols){
    int x, y, x1, x2, y1, y2, plane;
    uchar maximum;
    for (y = 0; y < rows; ++y) {
        for (x = 0; x < cols; ++x) {
            x1 = x - 1;
            x2 = x + 2;
            y1 = y - 1;
            y2 = y + 2;
            if (x1 < 0) x1 = 0;
            if (y1 < 0) y1 = 0;
            if (x2 > cols) x2 = cols;
            if (y2 > rows) y2 = rows;
            for( plane = 0; plane < src_planes.size(); ++plane) {
                Mat mat = src_planes[plane](Range(y1, y2), Range(x1, x2));
                MatConstIterator_<uchar> it = mat.begin<uchar>(), it_end = mat.end<uchar>();
                maximum = 0;
                for (; it != it_end; it++) {
                    if (maximum < *it) {
                        maximum = *it;
                    }
                }
                dst_planes[plane].at<uchar>(y, x) = maximum;
#if 1
                if (x == 0 && y == 0) {
                    cout << mat <<endl;
                    cout << mat.total() << endl;
                    cout << maximum << endl;
                    cout << (uint)dst_planes[plane].at<uchar>(y, x) << endl;
                }
#endif
            }
        }
    }
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
    vector<Mat> src_planes;
    split(img_yuv, src_planes);
    vector<Mat> dst_planes;
    int rows = img_yuv.rows;
    int cols = img_yuv.cols;
    dst_planes.push_back(Mat(rows, cols, CV_8U, Scalar(0)));
    dst_planes.push_back(Mat(rows, cols, CV_8U, Scalar(128)));
    dst_planes.push_back(Mat(rows, cols, CV_8U, Scalar(128)));

    neighborhood_operations neighborhood = MAXIMUM;

    switch(neighborhood) {
        case AVERAGE:
            neighborhood_average(src_planes, dst_planes, rows, cols);
            merge(dst_planes, img_yuv);
            cvtColor(img_yuv, img, COLOR_YCrCb2BGR);
            namedWindow("image with neighborhood average", WINDOW_AUTOSIZE);
            imshow("image with neighborhood average", img);
            waitKey();
            break;
        case MAXIMUM:
            neighborhood_maximum(src_planes, dst_planes, rows, cols);
            merge(dst_planes, img_yuv);
            cvtColor(img_yuv, img, COLOR_YCrCb2BGR);
            namedWindow("image with neighborhood maximum", WINDOW_AUTOSIZE);
            imshow("image with neighborhood maximum", img);
            waitKey();
        default:
            break;
    }
    return 0;
}

