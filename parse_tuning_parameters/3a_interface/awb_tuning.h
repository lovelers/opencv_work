#ifndef __AWB_TUNING_H
#define __AWB_TUNING_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * white balance will do r, g, b gain compensation in linearity space.
 * the unit base gain is 1024. so, 1024 means the 1x gain settings.
 */
typedef struct __awb_color_gain_t {
    int rGain;
    int gGain;
    int bGain;
} __attribute__((__packed__)) awb_color_gain;

/**
 * awb_color_ratio is used to show the r, g, b ratio calculated by awb statistics.
 */
typedef struct __awb_color_ratio_t {
    float rRatio;
    float gRatio;
    float bRatio;
} __attribute__((__packed__)) awb_color_ratio;

#define WB_SCENE_NAME_COUNT 20
typedef enum __wb_scene_type_t {
    WB_UNKNOWN_SCENE=0,
    WB_DEEP_BLUE_SCENE,
    WB_D75_DAYLIGHT_SCENE,
    WB_D65_DAYLIGHT_SCENE,
    WB_D50_DAYLIGHT_SCENE,
    WB_YELLOWISH_SCENE,
    WB_COLD_WHITE_SCENE,
    WB_TL84_SCENE,
    WB_ALIGHT_SCENE,
    WB_HORIZON_SCENE,
    WB_SCENE_MAX,
} __attribute__((__packed__))AWB_SCENE_TYPE;

typedef struct __awb_ratio_range_t {
    float min;
    float max;
} __attribute__((__packed__))awb_ratio_range;

/**
 * AWB_WHITE_BALANCE
 * we seperate the real color world into differnt scene according the rgb color space. each color world, we use below struture to distinguish them.
 * and then, awbalgo will generate the r, g, b ratio, according to the r/b ratio, we seperate different scene and then adjust the awb output.
 *
 * gray_rb_ratio: the r/b ratio range.
 * Ratio Range: min <= max.it's depends on the AwbAlgo outpus logs. *
 *
 * gray_fb_ratio: we will get the D50 scene at first. and then we need convert to different scene.
 * according to the algorithm ratio and gain settings.
 * Ratio Range: min <= max. min:[0, 1] max:[0, 1]
 *
 * wp_rb_ratio: AwbAlgo will calculate the white point r,g,b ratio. according the r,g,b ratio, we will adjust the real r, g, b gains.
 * Ratio Range: min <= max. it's depends on the AwbAlgo outpus logs.
 *
 * wp_fb_ratio: if white point ratio is good enouth, we will not do convernt to different scene.
 * otherwise, we need fit the real ratio compensation base on the D50 gains output.
 * Ratio Range: min <= max. min:[0, 1] max:[0, 1]
 *
 * scene_type: the index of current color scene.
 * scene_type value: belong the ENUM: AWB_SCENE_TYPE.
 *
 * scene_name: the nickname of current color scene.
 * scene_name: it's the string of char[WB_SCENE_NAME_COUNT]
 */
typedef struct __awb_white_balance_t {
    awb_ratio_range gray_rb_ratio;
    awb_ratio_range gray_fb_ratio;
    awb_ratio_range wp_rb_ratio;
    awb_ratio_range wp_fb_ratio;
    AWB_SCENE_TYPE scene_type;
    char scene_name[WB_SCENE_NAME_COUNT];
} __attribute__((__packed__))awb_white_balance;

typedef struct __awb_tuning_parameters_t {
    awb_color_gain white_point_gain;
    awb_color_ratio white_point_ratio;
    int manual_gain_mode;
    awb_color_gain manual_gain;
    float macadam_ratio;
    float color_sampling_high;
    float color_sampling_low;
    float surpressr;
    float saturation_default;
    int delight_color;
    float chromatic_adaptation;
    int g_norm_off_compensation;
    awb_white_balance wb[WB_SCENE_MAX];
} __attribute__((__packed__)) awb_tuning_parameters;

const awb_tuning_parameters* meizu_tuning_get_awb(void);
#ifdef __cplusplus
}
#endif

#endif// __AWB_TUNING_H
