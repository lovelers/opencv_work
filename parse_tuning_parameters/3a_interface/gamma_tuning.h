#ifndef __GAMMA_TUNING_H
#define __GAMMA_TUNING_H

typedef unsigned int U32;

#ifdef __cplusplus
extern "C" {
#endif
#define RGB_CHANNEL 3
#define GAMMA_TABLE_COUNT 32
#define GAMMA_MAX_VALUE 16384

/**
 * GAMMA_BASE_CONFIG
 * here interface provide the gamma settings and compensation method.
 *
 * normalGamma: the standard gamma used.
 * normalGamma recommand range: [1.0f, 2.8f]. typical value: 2.05f
 * normalBaseOffset: the offset of normal gamma. offset will adjust the low-light part of gamma.
 * normalBaseOffset recommand range: [-500, 500]. typical value: -128
 * normalEndOffset: the offset of normal gamma. offset will adjust the high-light part of gamma.
 * normalEndOffset recommand range: [-500, 500]. typical value: 0
 * normalLinearityWeight range: [0, 10]. typical value : 4
 * normalLinearityWeight: compensation of low-light part of gamma. it will let the gamma cure become linerity from index[0, normalLinearityWeight].
 * * indoorGamma: the indoor gamma used.  * indoorGamma recommand range: [1.0f, 2.8f]. typical value : 1.8f
 * indoorBaseOffset: the offset of indoor gamma. offset will adjust the low-light part of gamma.
 * indoorBaseOffset recommand range: [-500, 500]. typical value: 256
 * indoorEndOffset: the offset of indoor gamma. offset will adjust the high-light part of gamma.
 * indoorEndOffset recommand range: [-500, 500]. typical value: 256
 * indoorLinearityWeight: compensation of low-light part of gamma. it will let the gamma cure become linerity from index[0, indoorLinearityWeight].
 * indoorLinearityWeight range: [0, 10]. typical value : 4
 *
 * outdoorGamma: the outdoor gamma used.
 * outdoorGamma recommand range: [1.0f, 2.8f]. typical value : 2.5f
 * outdoorBaseOffset: the offset of outdoor gamma. offset will adjust the low-light part of gamma.
 * outdoorBaseOffset recommand range: [-500, 500]. typical value: -500
 * outdoorEndOffset: the offset of outdoor gamma. offset will adjust the high-light part of gamma.
 * outdoorEndOffset recommand range: [-500, 500]. typical value:  -100
 * outdoorLinearityWeight: compensation of low-light part of gamma. it will let the gamma cure become linerity from index[0, outdoorLinearityWeight].
 * outdoorLinearityWeight range: [0, 10]. typical value : 4
 */
typedef struct __gamma_base_config {
    float normalGamma;
    int normalBaseOffset;
    int normalEndOffset;
    U32 normalLinearityWeight;
    float indoorGamma;
    int indoorBaseOffset;
    int indoorEndOffset;
    U32 indoorLinearityWeight;
    float outdoorGamma;
    int outdoorBaseOffset;
    int outdoorEndOffset;
    U32 outdoorLinearityWeight;
    const U32 gamma_table_x[32];
    const float user_gamma[32];
} __attribute__((__packed__)) gamma_base_config;

/**
 * DRC_COMPENSATION
 *
 * drcEnabled: if it's true, drc algo will optimize the gamma curve. if it's false, disable the drc compensation.
 * drcEnable Range:true or false
 *
 * drcStrength: it's indicate the drc amplitude.
 * drcStrength range (0, 0.4f]. typical value : 0.2
 */
typedef struct __drc_compensation {
    int     drcEnabled;
    float   drcStrength;
} __attribute__((__packed__)) drc_compensation;


typedef struct __dark_current_reduce {
    U32 gamma_index;
} __attribute__((__packed__)) dark_current_reduce;

enum {
    // Don't need fit the gamma value. it will use the original gamma algorithm output.
    NORMAL_GAMMA_VALUE_FLAG = 0,
    // we need reverse the gamma value in algorithm like this: value = 16384 - value.
    REVERSE_GAMMA_VALUE_FLAG = 1,
    // we need apply the ratio for new gamma value in algoritm like this: value = value * ratio / 10000. 10000 means the 1x unit ratio.
    GAMMA_RATIO_FLAG = 2,
};

/**
 * GAMMA_FILTER_ITEM
 * the operation struct of different color filter.
 *
 * rgbFlag: the differnt flag will enable the differnt gamma calculate processing at GammaAlgo.
 * r, g, b three channel have the different flags.
 * rgbFlag Value:  NORMAL_GAMMA_VALUE_FLAG or  REVERSE_GAMMA_VALUE_FLAG or  GAMMA_RATIO_FLAG.
 *
 * rgb: the value settings for rgb gamma.
 * rgb Value: then depends on the rgbFlag.
 * if flag == NORMAL_GAMMA_VALUE. the range is: [0, 16384].
 * if flag == REVERSE_GAMMA_VALUE_FLAG, this array is reserved, we just input the 0.
 * if flag == GAMMA_RATIO_FLAG, the value is the positive integer.
 */
typedef struct __gamma_filter_item {
    U32 rgbFlag[RGB_CHANNEL];
    U32 rgb[RGB_CHANNEL][GAMMA_TABLE_COUNT];
} __attribute__((__packed__)) gamma_filter_item;

/**
 * this is used for color filter impelments base on rgb channels gamma.
 */
typedef struct __gamma_filter_table{
    gamma_filter_item blackWhite;
    gamma_filter_item negative;
    gamma_filter_item bluish;
    gamma_filter_item reserved;
    gamma_filter_item reserved1;
    gamma_filter_item reserved2;
    gamma_filter_item reserved3;
    gamma_filter_item reserved4;
} __attribute__((__packed__)) gamma_filter_table;

typedef struct {
    gamma_base_config           base_config;
    drc_compensation            compensation;
    gamma_filter_table          filter_table;
} __attribute__((__packed__)) gamma_tuning_parameters;

const gamma_tuning_parameters* meizu_tuning_get_gamma(void);

#ifdef __cplusplus
}
#endif

#endif// __GAMMA_TUNING_H

