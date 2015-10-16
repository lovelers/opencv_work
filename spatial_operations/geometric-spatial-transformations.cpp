#include <stdio.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include "common.h"

using namespace cv;
using namespace std;

enum geometric_spatial_transformations {
    IDENTITY,
    SCALING,
    ROTATION,
    TRANSLATION,
    SHEAR_VERTICAL,
};

void identity(const vector<Mat> & src_planes, vector<Mat> dst_planes, int src_rows, int src_cols, int dst_rows, int dst_cols) {
    Matx<int, 2, 2> identity_matx(1, 0, 0, 1);
    Matx<int, 1, 2> src_matx, dst_matx;

    int x, y, plane;
    for (y = 0; y < src_rows; ++y) {
        for (x = 0; x < src_cols; ++x) {
            src_matx(0, 0) = x;
            src_matx(0, 1) = y;
            dst_matx = src_matx * identity_matx;
            for (plane = 0; plane < src_planes.size(); ++plane) {
                dst_planes[plane].at<uchar>(dst_matx(0, 1), dst_matx(0, 0)) = src_planes[plane].at<uchar>(y, x);
            }
        }
    }
}

void rotation(const vector<Mat> & src_planes, vector<Mat> dst_planes, int src_rows, int src_cols, int dst_rows, int dst_cols, float coefficient) {
    Matx<float, 2, 2> rotation_matx(cos(coefficient), sin(coefficient), -sin(coefficient), cos(coefficient));
    Matx<float, 1, 2> src_matx, dst_matx;

    int x, y, plane, x1, y1;
    int base_x = src_cols / 2;
    int base_y = src_rows / 2;
    int base_x1 = dst_cols / 2;
    int base_y1 = dst_rows / 2;
    for (y = 0; y < src_rows; ++y) {
        for (x = 0; x < src_cols; ++x) {
            src_matx(0, 0) = base_x - x;
            src_matx(0, 1) = base_y - y;
            dst_matx = src_matx * rotation_matx;
            x1 = int(dst_matx(0, 0) + 0.5f) + base_x1;
            y1 = int(dst_matx(0, 1) + 0.5f) + base_y1;
            for (plane = 0; plane < src_planes.size(); ++plane) {
                dst_planes[plane].at<uchar>(y1, x1) = src_planes[plane].at<uchar>(y, x);
            }
        }
    }
}

void shear_vertical(const vector<Mat> & src_planes, vector<Mat> dst_planes, int src_rows, int src_cols, int dst_rows, int dst_cols, float coefficient) {
    Matx<float, 2, 2> shear_matx(1, 0, coefficient, 1);
    Matx<float, 1, 2> src_matx, dst_matx;

    int x, y, plane, x1, y1;
    for (y = 0; y < src_rows; ++y) {
        for (x = 0; x < src_cols; ++x) {
            src_matx(0, 0) = x;
            src_matx(0, 1) = y;
            dst_matx = src_matx * shear_matx;
            x1 = dst_matx(0, 0);
            y1 = dst_matx(0, 1);
            for (plane = 0; plane < src_planes.size(); ++plane) {
                dst_planes[plane].at<uchar>(y1, x1) = src_planes[plane].at<uchar>(y, x);
            }
        }
    }
}
int main(int argc, char ** argv) {
    const char *imagename = argc > 1 ? argv[1] : LENA_JPG;

    Ptr<IplImage> iplimg(cvLoadImage(imagename));
    if (!iplimg) {
        cout << "can't load image " << imagename << endl;
        return -1;
    }
    Mat img = cv::cvarrToMat(iplimg);
    if (img.empty()) return -1;

    Mat img_yuv;
    cvtColor(img, img_yuv, COLOR_BGR2YCrCb);
    vector<Mat> src_planes, dst_planes;
    split(img_yuv, src_planes);
    int src_rows = img_yuv.rows;
    int src_cols = img_yuv.cols;
    int dst_rows = src_rows;
    int dst_cols = src_cols;
    float coefficient = 0.3f;

    geometric_spatial_transformations transformation = SHEAR_VERTICAL;
    switch (transformation) {
        case IDENTITY:
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(0)));
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(128)));
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(128)));
            identity(src_planes, dst_planes, src_rows, src_cols, dst_rows, dst_cols);
            merge(dst_planes, img_yuv);
            cvtColor(img_yuv, img, COLOR_YCrCb2BGR);
            namedWindow("geometric spatial transformations identity", WINDOW_AUTOSIZE);
            imshow("geometric spatial transformations identity", img);
            waitKey();
            break;
        case ROTATION:
            dst_rows = src_rows * sqrt(2.f) + 1;
            dst_cols = src_cols * sqrt(2.f) + 1;
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(0)));
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(128)));
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(128)));
            rotation(src_planes, dst_planes, src_rows, src_cols, dst_rows, dst_cols, coefficient);
            merge(dst_planes, img_yuv);
            cvtColor(img_yuv, img, COLOR_YCrCb2BGR);
            namedWindow("geometric spatial transformations rotation", WINDOW_AUTOSIZE);
            imshow("geometric spatial transformations rotation", img);
            waitKey();
        case SHEAR_VERTICAL:
            dst_rows = src_rows;
            dst_cols = src_cols + (coefficient * src_rows + 0.5f);
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(0)));
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(128)));
            dst_planes.push_back(Mat(dst_rows, dst_cols, CV_8U, Scalar(128)));
            shear_vertical(src_planes, dst_planes, src_rows, src_cols, dst_rows, dst_cols, coefficient);
            merge(dst_planes, img_yuv);
            cvtColor(img_yuv, img, COLOR_YCrCb2BGR);
            namedWindow("geometric spatial transformations shear", WINDOW_AUTOSIZE);
            imshow("geometric spatial transformations shear", img);
            waitKey();
        default:
            break;
    }
}
