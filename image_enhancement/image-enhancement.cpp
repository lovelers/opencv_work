#include <stdio.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include "../common.h"

using namespace cv;
using namespace std;

void laplacian(const vector<Mat> &src_planes, vector<Mat> dst_planes, const int rows, const int cols);
void sobel(const vector<Mat> &src_planes, vector<Mat> dst_planes, const int rows, const int cols);
void average_smooth(const vector<Mat> &src_planes, vector<Mat> dst_planes, const int rows, const int cols);
void unsharp_mask(const vector<Mat> &src_planes1, const vector<Mat> &src_planes2, vector<Mat> dst_planes, const int rows, const int cols);
uchar gamma_correction(uint value, const float gamma_r, const float gamma_c);
int main(int argc, char **argv) {
    const char *imagename = (argc > 2) ? argv[1] : SKELETON_ORIG;
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
    vector<Mat> src_planes;
    split(img_yuv, src_planes);
    vector<Mat> laplacian_planes;
    split(img_yuv, laplacian_planes);
    int rows = img_yuv.rows;
    int cols = img_yuv.cols;

    //laplacian sharpnening
    laplacian(src_planes, laplacian_planes, rows, cols);
    Mat laplacian_mat;
    merge(laplacian_planes, laplacian_mat);
    cvtColor(laplacian_mat, dstimg, COLOR_YCrCb2BGR);
    imwrite("laplacian.jpg", dstimg);
    //laplacian_planes.clear();
    //laplacian_mat.release();

    //sobel sharpneing
    vector<Mat> sobel_planes;
    split(img_yuv, sobel_planes);
    sobel(src_planes, sobel_planes, rows, cols);
    Mat sobel_mat;
    merge(sobel_planes, sobel_mat);
    cvtColor(sobel_mat, dstimg, COLOR_YCrCb2BGR);
    imwrite("sobel.jpg", dstimg);

    //average smooth
    vector<Mat> smooth_planes;
    split(img_yuv, smooth_planes);
    average_smooth(sobel_planes, smooth_planes, rows, cols);
    Mat smooth_mat;
    merge(smooth_planes, smooth_mat);
    cvtColor(smooth_mat, dstimg, COLOR_YCrCb2BGR);
    imwrite("smooth.jpg", dstimg);

    //unsharp mask
    vector<Mat> unsharp_planes;
    unsharp_planes.push_back(Mat(rows, cols, CV_32S, Scalar(0)));
    unsharp_mask(laplacian_planes, smooth_planes, unsharp_planes, rows, cols);

    // add unsharp mask to original image
    vector<Mat> sum_unsharp_planes;
    split(img_yuv, sum_unsharp_planes);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int sum = sum_unsharp_planes[0].at<uchar>(y, x) + unsharp_planes[0].at<int>(y, x);
            sum_unsharp_planes[0].at<uchar>(y, x) = sum < 0 ? 0 : sum > 255 ? 255 : sum;
        }
    }
    Mat sum_unsharp_mat;
    merge(sum_unsharp_planes, sum_unsharp_mat);
    cvtColor(sum_unsharp_mat, dstimg, COLOR_YCrCb2BGR);
    imwrite("sum_unsharp_mask.jpg", dstimg);


    // gamma correction with low-power.
    // y = gamma_c * pow(x, gamma_r);
    float gamma_r = 0.5;
    float gamma_c = 255.f / pow(255, gamma_r);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            sum_unsharp_planes[0].at<uchar>(y, x) = gamma_correction(sum_unsharp_planes[0].at<uchar>(y, x), gamma_r, gamma_c);
        }
    }
    Mat result_mat;
    merge(sum_unsharp_planes, result_mat);
    cvtColor(result_mat, dstimg, COLOR_YCrCb2BGR);
    imwrite("result.jpg", dstimg);
    imshow("result.jpg", dstimg);
    waitKey();
    return 0;
}

void laplacian(const vector<Mat> &src_planes, vector<Mat> dst_planes, const int rows, const int cols) {
    /**
     * laplacian: the second derivation for image sharpneing.
     * [-1, -1, -1]
     * [-1,  8, -1]
     * [1-, -1, -1]
     */
    Matx<int, 3, 3> filter(-1, -1, -1, -1, 8, -1, -1, -1, -1);
    const int C = 1;
    Matx<int, 3, 3> f, g;
    int intensity;
    int s=0, t=0;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            for (int y1 = -1; y1 < 2; ++y1) {
                for (int x1 = -1; x1 < 2; ++x1) {
                    s = x + x1;
                    t = y + y1;
                    if (s < 0 || t < 0 || s-1 > cols || t-1 > rows) {
                        s = x;
                        t = y;
                    }
                    f(y1+1, x1+1) = src_planes[0].at<uchar>(t, s);
                }
            }

            intensity = f.dot(filter) * C + (uint) src_planes[0].at<uchar>(y, x);
            dst_planes[0].at<uchar>(y, x) = intensity > 255 ? 255 : intensity < 0 ? 0 :(uchar)intensity;
#if 0
            cout << f.t() << endl;
            cout << f.dot(filter) << endl;
            cout << (uint)dst_planes[0].at<uchar>(y, x) << endl;
#endif
        }
    }
    return;
}



void sobel(const vector<Mat> &src_planes, vector<Mat> dst_planes, const int rows, const int cols) {

    /**
     * sobel operator.
     * [-1, 0, 1]
     * [-2, 0, 2]
     * [-1, 0, 1]
     * 
     * [-1, -2, -1]
     * [ 0,  0,  0]
     * [ 1,  2,  1]
     */
    Matx<int, 3, 3> filter1(-1, 0, 1, -2, 0, 2, -1, 0, 1);
    Matx<int, 3, 3> filter2(-1, -2, -1, 0, 0, 0, 1, 2, 1);
    const int coefficient = 1;

    Matx<int ,3 ,3> f, g;
    int s=0, t=0;
    int intensity = 0;
    for (int y = 0; y < rows; ++y){
        for (int x = 0; x < cols; ++x) {
            for (int y1 = -1; y1 < 2; ++y1) {
                for (int x1 = -1; x1 < 2; ++x1) {
                    s = x + x1;
                    t = y + y1;
                    if (s < 0 || t < 0 || s-1 > cols || t-1 > rows) {
                        s = x;
                        t = y;
                    }
                    f(y1+1, x1+1) = src_planes[0].at<uchar>(t, s);
                }
            }

            intensity = (f.dot(filter1) + f.dot(filter2)) * coefficient + (uint) src_planes[0].at<uchar>(y, x);
            dst_planes[0].at<uchar>(y, x) = intensity > 255 ? 255 : intensity < 0 ? 0 :(uchar)intensity;
#if 0
            cout << f.t() << endl;
            cout << f.dot(filter1) << f.dot(filter2) << endl;
            cout << (uint)dst_planes[0].at<uchar>(y, x) << endl;
#endif
        }
    }
    return;
}


void average_smooth(const vector<Mat> &src_planes, vector<Mat> dst_planes, const int rows, const int cols) {
    int s=0, t=0;
    uint sum;
    for (int y = 0; y < rows; ++y){
        for (int x = 0; x < cols; ++x) {
            sum = 0; 
            for (int y1 = -1; y1 < 2; ++y1) {
                for (int x1 = -1; x1 < 2; ++x1) {
                    s = x + x1;
                    t = y + y1;
                    if (s < 0 || t < 0 || s-1 > cols || t-1 > rows) {
                        s = x;
                        t = y;
                    }
                    sum += (uint)src_planes[0].at<uchar>(t, s);
                }

            }
            sum /= 9;
            dst_planes[0].at<uchar>(y, x) = sum;
        }
    }
    return;
}

void unsharp_mask(const vector<Mat> &src_planes1, const vector<Mat> &src_planes2, vector<Mat> dst_planes, const int rows, const int cols) {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            dst_planes[0].at<int>(y, x) = src_planes1[0].at<uchar>(y, x) - src_planes2[0].at<uchar>(y, x);
        }
    }
    return;
}
uchar gamma_correction(uint value, const float gamma_r, const float gamma_c) {
    uint result = gamma_c * pow(value, gamma_r);
    return (result > 255) ? 255 : (uchar) result;
}
