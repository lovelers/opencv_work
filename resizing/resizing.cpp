#include <stdio.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include "../common.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    const char *imagename = (argc > 1) ? argv[1] : LENA_JPG;
    Ptr<IplImage> iplimg(cvLoadImage(imagename));
    if (!iplimg) {
        cout << "can't load image" << imagename << endl;
        return -1;
    }

    Mat img = cv::cvarrToMat(iplimg);
    if (img.empty()) return -1;
    Mat dstimg;

    Mat img_yuv;
    cvtColor(img, img_yuv, COLOR_BGR2YCrCb);
    int rows = img_yuv.rows;
    int cols = img_yuv.cols;

    vector<Mat> yuv_planes;
    split(img_yuv, yuv_planes);
    cout << " ima_yuv planes : " << yuv_planes.size() << endl;

    vector<Mat> resize_planes;
    resize_planes.push_back(Mat(rows/2, cols/2, CV_8U, Scalar(0)));
    resize_planes.push_back(Mat(rows/2, cols/2, CV_8U, Scalar(0)));
    resize_planes.push_back(Mat(rows/2, cols/2, CV_8U, Scalar(0)));

    int x1=0,y1=0;
    for (int y = 0; y < rows; y+=2) {
        for (int x = 0; x < cols; x+=2) {
            for (int i = 0; i < 3; ++i) {
                resize_planes[i].at<uchar>(y1, x1) = yuv_planes[i].at<uchar>(y, x);
            }
            x1++;
        }
        y1++;
        x1 = 0;
    }

    Mat resize_mat;
    merge(resize_planes, resize_mat);
    cvtColor(resize_mat, dstimg, COLOR_YCrCb2BGR);
    imshow("resize.jpg", dstimg);
    waitKey();
    return 0;
}
