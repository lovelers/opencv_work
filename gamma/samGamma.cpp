#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <stdio.h>
#include <iomanip>

using namespace std;
using namespace cv;

int gamma_table[][2] = {
    {0,0},{64,159},{128,356},{256,841},{384,1429},{512,2033},{640,2611},{768,3126},
    {896,3575},{1024,3990},{1280,4688},{1536,5316},{1792,5870},{2048,6382},{2304,6865},{2560,7328},
    {2816,7782},{3072,8198},{3584,8886},{4096,9474},{4608,10002},{5120,10457},{6144,11260},{7168,11946},
    {8192,12605},{9216,13202},{10240,13717},{11264,14195},{12288,14593},{13312,14978},{14336,15356},{16384,16197}
};

int gamma_table_low_light[][2] = {
{0,0},{64,184},{128,488},{256,1167},{384,1775},{512,2298},{640,2769},{768,3215},
{896,3630},{1024,4016},{1280,4714},{1536,5345},{1792,5903},{2048,6419},{2304,6905},{2560,7360},
{2816,7786},{3072,8183},{3584,8879},{4096,9482},{4608,10008},{5120,10461},{6144,11262},{7168,11947},
{8192,12607},{9216,13204},{10240,13719},{11264,14196},{12288,14594},{13312,14978},{14336,15354},{16384,16193},
};


int gamma_table_albert_low_light[][2] = {
    {0,1024},{128,1757},{256,2376},{384,2939},{512,3472},{640,3983},{768,4479},{896,4963},
    {1024,5437},{1152,5904},{1280,6364},{1408,6819},{1536,7268},{1792,7861},{2048,8440},{2304,9006},
    {2560,9561},{2816,10107},{3072,10646},{3328,11177},{3584,11702},{4096,12443},{4608,13164},{5120,13870},
    {5632,14561},{6144,15239},{7168,16271},{8192,17267},{10240,18881},{12288,20404},{14336,21857},{16384,23259}
};

int gamma_table_albert_indoor[][2] = {
    {0,0},{128,1114},{256,1818},{384,2546},{512,3150},{640,3713},{768,4249},{896,4766},
    {1024,5267},{1152,5754},{1280,6232},{1408,6700},{1536,7161},{1792,7780},{2048,8376},{2304,8955},
    {2560,9519},{2816,10071},{3072,10611},{3328,11143},{3584,11666},{4096,12404},{4608,13118},{5120,13810},
    {5632,14484},{6144,15143},{7168,16135},{8192,17084},{10240,18591},{12288,19993},{14336,21315},{16384,22582},
};

#if 0
int gamma_table_albert_outdoor[][2] = {
    {0,0},{128,997},{256,1639},{384,2518},{512,3154},{640,3734},{768,4280},{896,4803},
    {1024,5309},{1152,5800},{1280,6279},{1408,6749},{1536,7211},{1792,7830},{2048,8427},{2304,9005},
    {2560,9567},{2816,10116},{3072,10654},{3328,11182},{3584,11701},{4096,12434},{4608,13140},{5120,13825},
    {5632,14492},{6144,15142},{7168,16119},{8192,16383},{10240,16383},{12288,16383},{14336,16383},{16384,16383}
};
#endif

int gamma_table_albert_outdoor[][2] = {
    {0,0},{128,848},{256,1464},{384,2109},{512,2668},{640,3199},{768,3714},{896,4215},
    {1024,4706},{1152,5189},{1280,5665},{1408,6136},{1536,6602},{1792,7167},{2048,7719},{2304,8105},
    {2560,8466},{2816,8815},{3072,9158},{3328,9494},{3584,9824},{4096,10292},{4608,10743},{5120,11181},
    {5632,11608},{6144,12024},{7168,12651},{8192,13250},{10240,14203},{12288,15088},{14336,15924},{16384,16383},
};

int gamma_w = 16384;
int gamma_h = 16384;
void dot(vector<Mat> gamma_planes, int x, int y, int detla, int b, int g, int r) {
    int x1 = x - detla;
    int x2 = x + detla;
    int y1 = y - detla;
    int y2 = y + detla;
    if (x1 < 0) x1 = 0;
    if (x2 > gamma_w -1) x2 = gamma_w -1;
    if (y1 < 0) y1 = 0;
    if (y2 > gamma_h -1) y2 = gamma_h -1;

    for (int x3 = x1; x3 < x2; x3++) {
        for (int y3 = y1; y3 < y2; y3++) {
            gamma_planes[0].at<uchar>(y3, x3) = b;
            gamma_planes[1].at<uchar>(y3, x3) = g;
            gamma_planes[2].at<uchar>(y3, x3) = r;
        }
    }
}

void Gamma(double R, int w, int h, vector<Mat> &gamma_planes) {
    double C = h / pow(h, R);
    cout << "willis" << C << endl;
    for (int x = 0; x < w; x++) {
        int y = C * pow(x, R);
        dot(gamma_planes, x, y, 20, 0, 0, 0);
    }
};
int main(int arg, char **argv) {
    Mat gamma(gamma_h, gamma_w, CV_8UC3, Scalar(255, 255,255));
    vector<Mat> gamma_planes;
    split(gamma, gamma_planes);

    cout << "goto here" << endl;
    Gamma(0.45, gamma_w, gamma_h, gamma_planes);
    for (int i = 0; i < 31; i++) {
        int x1 = gamma_table_albert_indoor[i][0];
        int y1 = gamma_table_albert_indoor[i][1];
        int x2 = gamma_table_albert_indoor[i+1][0];
        int y2 = gamma_table_albert_indoor[i+1][1];
        double a = (y2 -y1) / (double)(x2 - x1);
        double b =  y1 - a * x1;
        cout << a << "," << b << endl;

        for (int x = x1; x < x2; x++) {
            int y = a * x + b;
            //dot(gamma_planes, x, y, 10, 255, 0, 0);
        }
    }
    for (int i = 0; i < 31; i++) {
        int x1 = gamma_table_albert_low_light[i][0];
        int y1 = gamma_table_albert_low_light[i][1];
        int x2 = gamma_table_albert_low_light[i+1][0];
        int y2 = gamma_table_albert_low_light[i+1][1];
        double a = (y2 -y1) / (double)(x2 - x1);
        double b =  y1 - a * x1;
        cout << a << "," << b << endl;

        for (int x = x1; x < x2; x++) {
            int y = a * x + b;
            //dot(gamma_planes, x, y, 10, 255, 0, 0);
        }


    }

    for (int i = 0; i < 31; i++) {
        int x1 = gamma_table_low_light[i][0];
        int y1 = gamma_table_low_light[i][1];
        int x2 = gamma_table_low_light[i+1][0];
        int y2 = gamma_table_low_light[i+1][1];
        double a = (y2 -y1) / (double)(x2 - x1);
        double b =  y1 - a * x1;
        cout << a << "," << b << endl;

        for (int x = x1; x < x2; x++) {
            int y = a * x + b;
            dot(gamma_planes, x, y, 10, 0, 0, 255);
        }


    }
    cout << "goto here" << endl;

    merge(gamma_planes, gamma);
    flip(gamma, gamma, 0);

    Mat resize_gamma;
    resize(gamma, resize_gamma, Size(640, 640));
    //imshow("gamma", gamma);
    //waitKey();
    imwrite("sam_gamma_low_light.jpg", resize_gamma);
    return 0;
}

