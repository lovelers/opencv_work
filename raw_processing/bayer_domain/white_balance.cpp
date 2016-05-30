#include "white_balance.h"
#include "bayer_buffer.h"
using namespace std;
using namespace cv;

void white_balance::applyWBGains(Mat &bayer8, int _rGains, int _gGains, int _bGains, int pattern) {
    // keep gGains = 1024;
    switch (pattern) {
        case bayer_buffer::BAYER_RGrGbB:
            for (int row = 0; row < bayer8.rows; row+=2) {
                for (int col = 0; col < bayer8.cols; col+=2) {
                    int tmp = _rGains * bayer8.at<uchar>(row, col) / 1024;
                    bayer8.at<uchar>(row, col) = (tmp > 255) ? 255 : (uchar)tmp;
                    tmp = _bGains * bayer8.at<uchar>(row+1, col+1) / 1024;
                    bayer8.at<uchar>(row+1, col+1) = (tmp > 255) ? 255 : (uchar)tmp;
                }
            }
            break;
        case bayer_buffer::BAYER_GrRBGb:
            for (int row = 0; row < bayer8.rows; row+=2) {
                for (int col = 0; col < bayer8.cols; col+=2) {
                    int tmp = _rGains * bayer8.at<uchar>(row, col+1) / 1024;
                    bayer8.at<uchar>(row, col+1) = (tmp > 255) ? 255 : (uchar)tmp;
                    tmp = _bGains * bayer8.at<uchar>(row+1, col) / 1024;
                    bayer8.at<uchar>(row+1, col) = (tmp > 255) ? 255 : (uchar)tmp;
                }
            }
            break;
        case bayer_buffer::BAYER_BGbGrR:
            for (int row = 0; row < bayer8.rows; row+=2) {
                for (int col = 0; col < bayer8.cols; col+=2) {
                    int tmp = _bGains * bayer8.at<uchar>(row, col) / 1024;
                    bayer8.at<uchar>(row, col) = (tmp > 255) ? 255 : (uchar)tmp;
                    tmp = _rGains * bayer8.at<uchar>(row+1, col+1) / 1024;
                    bayer8.at<uchar>(row+1, col+1) = (tmp > 255) ? 255 : (uchar)tmp;
                }
            }
            break;
        case bayer_buffer::BAYER_GbBRGr:
            for (int row = 0; row < bayer8.rows; row+=2) {
                for (int col = 0; col < bayer8.cols; col+=2) {
                    int tmp = _bGains * bayer8.at<uchar>(row, col+1) / 1024;
                    bayer8.at<uchar>(row, col+1) = (tmp > 255) ? 255 : (uchar)tmp;
                    tmp = _rGains * bayer8.at<uchar>(row+1, col) / 1024;
                    bayer8.at<uchar>(row+1, col) = (tmp > 255) ? 255 : (uchar)tmp;
                }
            }
            break;
        default:
            break;
    }
}
