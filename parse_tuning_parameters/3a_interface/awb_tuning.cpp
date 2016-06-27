#include "awb_tuning.h"

#define WHITE_POINT_GAIN  { \
    2717, 1250, 2124  \
}

#define WHITE_POINT_RATIO { \
    0.246f, 0.485f, 0.269f \
}

#define DEEP_BLUE {	\
    {1/10.f, 1/2.f},	\
    {0.5f, 1.f},	\
    {0.3f/0.5f, 1.f},	\
    {1.f, 0.7f},	\
    WB_DEEP_BLUE_SCENE,	\
    {'d', 'e', 'e', 'p', 'b', 'l', 'u', 'e', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define D75_DAYLIGHT {	\
    {1/2.f, 1/1.4f},	\
    {1.f, 0.8f},	\
    {0.37f / 0.42f, 1.f},	\
    {1.f, 0.5f},	\
    WB_D75_DAYLIGHT_SCENE,	\
    {'d', '7', '5', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define D65_DAYLIGHT {	\
    {1/1.4f, 1/1.05f},	\
    {0.8f, 0.8f},	\
    {0.37f / 0.42f, 1.f},	\
    {1.f, 0.3f},	\
    WB_D65_DAYLIGHT_SCENE,	\
    {'d', '6', '5', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define D50_DAYLIGHT {	\
    { 1/1.05f, 1.01f},	\
    {0.f, 0.f},	\
    {1/1.05f, 1.01f},	\
    {0.f, 0.f},	\
    WB_D50_DAYLIGHT_SCENE,	\
    {'d', '5', '0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define YELLOWISH {	\
    {1.01f, 1.2f},	\
    {0.5f, 0.8f},	\
    {1.f, 1.05f},	\
    {0.5f, 1.f},	\
    WB_YELLOWISH_SCENE,	\
    {'y', 'e', 'l', 'l', 'o', 'w', 'i', 's', 'h', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define COLD_WHITE {	\
    {1.2f, 1.4f},	\
    {0.8f, 1.f},	\
    {1.f, 1.08f},	\
    {0.5f, 1.0f},	\
    WB_COLD_WHITE_SCENE,	\
    {'c', 'o', 'l', 'd', 'w', 'h', 'i', 't', 'e', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define TL84 {	\
    {1.4f, 1.8f},	\
    {1.f, 0.8f},	\
    {1.f, 1.1f},	\
    {0.5f, 1.f},	\
    WB_TL84_SCENE,	\
    {'t', 'l', '8', '4', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define A_LIGHT {	\
    {1.8f, 3.f},	\
    {0.8f, 0.8f},	\
    {1.f, 1.2f},	\
    {0.5f, 1.f},	\
    WB_ALIGHT_SCENE,	\
    {'a', 'l', 'i', 'g', 'h', 't', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define HORIZON {	\
    {3.f, 8.f},	\
    {0.8f, 0.8f},	\
    {1.f, 2.f},	\
    {0.6f, 1.f},	\
    WB_HORIZON_SCENE,   \
    {'h', 'o', 'r', 'i', 'z', 'o', 'n', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define UNKNOWN { \
    {0.f, 0.f},     \
    {0.5f, 0.5f},   \
    {0.f, 0.f},     \
    {0.8f, 0.8f},   \
    WB_UNKNOWN_SCENE,\
    {'u', 'n', 'k', 'n', 'o', 'w', 'n', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'} \
}

#define WB_SCENE  {	\
    UNKNOWN,    \
    DEEP_BLUE,	\
    D75_DAYLIGHT,	\
    D65_DAYLIGHT,	\
    D50_DAYLIGHT,	\
    YELLOWISH,	\
    COLD_WHITE,	\
    TL84,	\
    A_LIGHT,	\
    HORIZON,	\
}

const awb_tuning_parameters* meizu_tuning_get_awb(void) {
    static const awb_tuning_parameters data = {
        WHITE_POINT_GAIN,
        WHITE_POINT_RATIO,
        0, // manual mode.
        {1024, 1024, 1024}, // gain settings for manual mode.
        0.3f, // macadam default settings.
        1.05f, 0.95f, // the color sampling range.
        0.f, // surpressr.
        0, // default saturation settings.
        100, // delight color
        0.9f, // chromatic adaptation.
        25, // g norm off compensation
        WB_SCENE,
    };
    return &data;
}
