#include <iostream>
#include <math.h>
using namespace std;
int main() {
    int g_user_gamma_x[32] = {
        0,     128,   256,   384,   512,   640,   768,   896,       
        1024,  1024+128, 1280, 1536,  1792,  2048,  2304,  2560,    
        2816,3072,  3072+256,  3584, 4096,  4608,  5120,  5120+512, 
        6144, 6144+512, 7168, 8192, 10240, 12288, 14336, 16384      
    };

    float g_user_gamma_y[32] = {
        1.f, 0.97f, 0.94f, 0.91f, 0.88f, 0.85f, 0.82f, 0.79f, 
        0.76f, 0.76f, 0.79f, 0.85f, 0.91f, 0.96f, 1.01f, 1.f, 
        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,               
        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,               
    };

    double C = 256 / pow (256, 0.4);
    for (int i = 0; i < 256; i++) {
        if (i % 8 == 0) cout << endl;
        cout << i << " " << (C * pow (i , 0.4));
    }

    cout << endl << "2.2 gamma" << endl;
    for (int i = 0; i < 32; i++) {
        if (i % 8 == 0) cout << endl;
        cout << " " << (C * pow (g_user_gamma_x[i]/64, 0.4));
    }

    cout << endl << "2.2 gamma + user control" << endl;
    for (int i = 0; i < 32; i++) {
        if (i % 8 == 0) cout << endl;
        cout << " " << (C * pow (g_user_gamma_x[i]/64, 0.4) * g_user_gamma_y[i]);
    }
    cout << endl;
    return 0;
}
