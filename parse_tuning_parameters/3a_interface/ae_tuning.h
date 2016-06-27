#ifndef __AE_TUNING_H__
#define __AE_TUNING_H__

typedef unsigned int U32;
typedef unsigned char U8;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * AE_METERING_COFIG
 * centerWidth: this value should be the change according the 3A Chian Block. AE Luminance Averge block width. 
 * Fixed Value: 32 for Exynos7420
 *
 * centerHeight: this value should be the change according the 3A Chian Block. AE Luminance Averge block height. 
 * Fixed Value: 32 for Exynos7420
 *
 * spotWidth: the spot metering mode width.
 * Value Range: [1, 32]
 *
 * spotHeight: the spot metering mode height.
 * Value Range: [1,32]
 */
typedef struct __ae_metering_config {
    U32 centerWidth;
    U32 centerHeight;
    U8 centerMap[1024];
    U32 spotWidth;
    U32 spotHeight;
    U8 spotMap[81];
} __attribute__((__packed__)) ae_metering_config;


/**
 * AE_BASE_CONFIG
 * here the brightnees indicate the AE statistic block output.
 * (targetBrMin, targetBrMax): the target brightness range when AE is done.
 * Typical Value:[475, 525], targetBrMin Range[300, 600], targetBrMax[350, 650]
 *
 * (stableBrMin, stableBrMax): the out of stalbe range, when current average brightness is out
 * of the stable range, we start do the ae convergent again.
 * Typical Value:[450, 550], targetBrMin Range[300, 600], targetBrMax[350, 650]
 * Note That: stableBrMin <= targetBrMin. stableBrMax <= targetBrMax
 *
 * (brLimitMin, brLimitMax): the limit range of average AE brightness output.
 * Typical Value:(5, 5000).
 * brLimitMin Range:[0, 10], brLimitMax Range[4096-5000]
 *
 * anaGainMax: the max analog gain.
 * anaGainMax Range:[1.f, 16.f] eg: 8.f means 8x gain
 *
 * digGainMax: the max digital gain.
 * digGainMax Range:[1.f, 4.f] eg:1.f means 1x.
 * Note that: digital Gain should not be too big, because it will cause much more noise, and the Gain changing is not smooth.
 *
 * aperture: the aperture value of camera lens.
 * aperture typical value 220 for exynos7420. it's modify according to the camera lens hardware.
 *
 * expTimeDefault: the default exposure value setting to sensor.
 * expTimeDefault typical value is 1000(us). Unit:us. range:(10, 33000).
 *
 *
 * gainDefault: the default gain value setting to sensor/isp.
 * gainDetault typical value is 1.f. means 1x. range(1, anaGainMax).
 *
 * expTimeMin: the minium exposure value setting to sensor.
 * expTimeMin value = one line exposure time. about 12us for IMX230.
 */
typedef struct __ae_base_config {
    U32 targetBrMin;
    U32 targetBrMax;
    U32 stableBrMin;
    U32 stableBrMax;
    U32 brLimitMin;
    U32 brLimitMax;
    float anaGainMax;
    float digGainMax;
    U32 aperture;
    U32 expTimeDefault;
    float gainDefault;
    U32 expTimeMin;
} __attribute__((__packed__)) ae_base_config;

typedef struct __ae_table {
    U32 luminance;
    float convergent_ratio;
} __attribute__((__packed__)) ae_table;

/**
 * AE_CONVERGENT_TABLE_CONFIG
 * targetBr: the value = (targetBrMin + targetBrMax) / 2.
 * Note: it's shoulb always equal (targetBrMin + targetBrMax) / 2
 *
 * table: the ae convergent table ratio.
 * the table count should be equal the underExpCount + overExpCount
 */

typedef struct __ae_convergent_table {
    U32 targetBr;

    U32 tableCount1;
    ae_table table1[24];

    U32 tableCount2;
    ae_table table2[108];

    U32 flickerAutoCount1;
    float flickerAuto1[24];
    U32 flickerAutoCount2;
    float flickerAuto2[24];

    U32 flicker50HzTableCount1;
    float flicker50Hz1[24];
    U32 flicker50HzTableCount2;
    float flicker50Hz2[24];

    U32 flicker60HzTableCount1;
    float flicker60Hz1[30];
    U32 flicker60HzTableCount2;
    float flicker60Hz2[30];
} __attribute__((__packed__)) ae_convergent_table;

typedef struct __ae_bv_range {
    float exposure1;
    float gain1;
    float y1;

    float exposure2;
    float gain2;
    float y2;
} __attribute__((__packed__)) ae_bv_range;

typedef struct __ae_real_environment_info {
    ae_bv_range indoorBvRange;
    ae_bv_range normalBvRange;
    ae_bv_range outdoorBvRange;
    ae_bv_range underExposureBvRange;
    ae_bv_range overExposureBvRange;
    ae_bv_range totalBvRange;
} __attribute__((__packed__)) ae_real_environment_info;

typedef struct __ae_dynamic_compensation {
    int dynamic_ae_enabled;
    float pureWhiteThreshold;
    float pureWhiteEnhanceMaxRatio;
    float lowLightWeakRatio;
} __attribute__((__packed__)) ae_dynamic_compensation;

typedef struct {
    ae_metering_config  metering_config;
    ae_base_config      base_config;
    ae_convergent_table convergent_table;
    ae_real_environment_info environment_info;
    ae_dynamic_compensation dynamic_compensation;
} __attribute__((__packed__)) ae_tuning_parameters;

const ae_tuning_parameters* meizu_tuning_get_ae(void);

#ifdef __cplusplus
}
#endif

#endif // __AE_TUNING_H__
