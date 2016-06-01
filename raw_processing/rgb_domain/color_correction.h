#ifndef __COLOR_CORRECTION_H
#define __COLOR_CORRECTION_H
#include "opencv2/core.hpp"

using namespace cv;

#define CCM_X 3
#define CCM_Y 3
#define CCM_R 0
#define CCM_G 1
#define CCM_B 2
class color_correction {
    public:
        typedef struct __ccm {
            int rr;
            int rg;
            int rb;
            int gr;
            int gg;
            int gb;
            int br;
            int bg;
            int bb;
        } ccm;
        static void applySimpleCCM(Mat3w &_rgb, ushort _max);

        color_correction &getInstance() {
            static color_correction ccm;
            return ccm;
        };

        void setSaturation(int _saturation);
        void setCcm(ccm *_ccm);
        void doCcm(Mat3w &_rgb, ushort _max);
    private:
        color_correction();
        ccm m;
        int saturation;

};
#endif // __COLOR_CORRECTION_H

