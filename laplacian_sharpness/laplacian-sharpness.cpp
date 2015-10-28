#include <stdio.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include "../common.h"

using namespace cv;
using namespace std;

// laplacian filter mask

void laplacian(const vector<Mat> &src_planes, vector<Mat> &dst_planes, int rows, int cols) {
    /**
     * laplacian: the second derivative for image sharpening
     * [1    1   1]
     * [1   -8   1]
     * [1    1   1]
     */
    Matx<int, 3, 3> filter(-1, -1, -1, -1, +8, -1, -1, -1, -1);
    const float C = 1;
    Matx<int ,3 ,3> f, g;
    int intensity;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            for (int y1 = -1; y1 < 2; ++y1) {
                for (int x1 = -1; x1 < 2; ++x1) {
                    if (x + x1 < 0 || y + y1 < 0 || x + x1 >= cols || y + y1 >= rows) {
                        f(y1+1, x1+1) = src_planes[0].at<uchar>(y, x);
                    } else {
                        f(y1+1, x1+1) = src_planes[0].at<uchar>(y1+y , x1+x);
                    }
                }
#if 1
                g = f.mul(filter);
                //cout << g.t() << endl;
                intensity = C* (g(0, 0) + g(0, 1) + g(0, 2) +
                            g(1, 0) + g(1, 1) + g(1, 2) +
                            g(2, 0) + g(2, 1) + g(2, 2)) + (uint) src_planes[0].at<uchar>(y, x);
                //cout << intensity << " " << (uint) src_planes[0].at<uchar>(y, x) << endl;
#else
                intensity = f.dot(filter);
#endif

                dst_planes[0].at<uchar>(y, x) = intensity > 255 ? 255 : intensity < 0 ? 0 :(uchar)intensity;
            }
        }
    }
}

void laplacian1(const vector<Mat> &src_planes, vector<Mat> &dst_planes, int rows, int cols) {
    /**
     * laplacian: the second derivative for image sharpening
     * [0    1   0]
     * [1   -4   1]
     * [0    1   0]
     */
    Matx<int, 3, 3> filter(0, -1, 0, -1, +4, -1, 0, -1, 0);
    const float C = 1 ;

    Matx<int ,3 ,3> f, g;
    int intensity;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            for (int y1 = -1; y1 < 2; ++y1) {
                for (int x1 = -1; x1 < 2; ++x1) {
                    if (x + x1 < 0 || y + y1 < 0 || x + x1 >= cols || y + y1 >= rows) {
                        f(y1+1, x1+1) = src_planes[0].at<uchar>(y, x);
                    } else {
                        f(y1+1, x1+1) = src_planes[0].at<uchar>(y1+y , x1+x);
                    }
                }
#if 0
                g = f.mul(filter);
                //cout << g.t() << endl;
                intensity = C* (g(0, 0) + g(0, 1) + g(0, 2) +
                            g(1, 0) + g(1, 1) + g(1, 2) +
                            g(2, 0) + g(2, 1) + g(2, 2)) + (uint) src_planes[0].at<uchar>(y, x);
                //cout << intensity << " " << (uint) src_planes[0].at<uchar>(y, x) << endl;
#else
                intensity = f.dot(filter) + (uint)src_planes[0].at<uchar>(y, x);
#endif

                dst_planes[0].at<uchar>(y, x) = intensity > 255 ? 255 : intensity < 0 ? 0 :(uchar)intensity;
            }
        }
    }
}
int main( int argc, char ** argv) {
    const char * imagename = argc > 1 ? argv[1] : BLUR_JPG;

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
    split(img_yuv, dst_planes);
    int rows = img_yuv.rows;
    int cols = img_yuv.cols;
    //dst_planes.push_back(Mat(rows, cols, CV_8U, Scalar(0)));
    //dst_planes.push_back(Mat(rows, cols, CV_8U, Scalar(128)));
    //dst_planes.push_back(Mat(rows, cols, CV_8U, Scalar(128)));

    fprintf(stderr, "goto laplacian\n");
    laplacian1(src_planes, dst_planes, rows, cols);
    merge(dst_planes, img_yuv);
    cvtColor(img_yuv, img, COLOR_YCrCb2BGR);
    namedWindow("image with neighborhood average", WINDOW_AUTOSIZE);
    imshow("image with neighborhood average", img);
    waitKey();
    imwrite("sharpness.jpg", img);
    return 0;
}

