#ifndef __WHITE_BALANCE_H
#define __WHITE_BALANCE_H
#include "opencv2/core.hpp"
using namespace std;
using namespace cv;
class white_balance {
    public:
        static void applyWBGains(Mat &bayer8, int _rGains, int _gGains, int _bGains, int pattern);
    private:
        white_balance();
};
#endif // __WHITE_BALANCE_H
