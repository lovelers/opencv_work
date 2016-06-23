/**
 * Copyright (C) 2015, Meizu Technology Co., Ltd. All rights reserved.
 *  Guanlin Liang <liangguanlin@meizu.com>
 */

#ifndef __MEIZU_AUTO_FOCUS_TUNING_PARAMETERS_H__
#define __MEIZU_AUTO_FOCUS_TUNING_PARAMETERS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
###
### AF Concepts
###
AutoFocus (AF) is the process of controlling focusing equipments, including Lens motor, ISP which generates contrast value (focus value),
and focusing assist equipments, e.g. laser and phase detection, to focus automatically or on a manually selected point or area.
The algorithm of AutoFocus collects focus values from each frame which captured at different Lens positions and finds the maximum focus value
which represents the best focus position.
Focusing assist equipments, such as laser and phase detection can speed up AF function by measuring the distance of focus target or target's image,
which reduces the searching range of contrast AF dramatically.

1.  AF range, the Lens position range from infinity to macro.
2.  Focus value, focus value calculating is performed by ISP on the AF floating window.
3.  AF filter types, the filter types used by ISP to calculate focus value, they are Big filter (3x9...), Row filter (1x17...), Gradient filter.
4.  Range measurement, measuring the distance of focus target or target's image, performed by laser or phase detection and giving Lens positions as output.
5.  AF search, the AF algorithm search the whole Lens position range or part of it, to find the best focus position.
6.  AF Search mode:
    a. Fine search mode, used when the confidence of Range-Measurement results are high;
    b. Speculate search mode, used when the distance is out of the coverage of Range-Measurement instrument (for now it is Laser).
    c. Normal search mode, when range measurement is unavailable or is not used.
    d. Predict search mode, when only low confidence range measurement results are available, e.g. only PDAF is available.
7.  AF search pause, pause the current AF search when the scene changes significantly until it's stable.
8.  AF scene type, there are 5 AF scene types. Scene indices are: Normal = 0, Low Contrast = 1, High Contrast = 2, Low Light = 3, Low Light Low Contrast = 4

###
### AF process steps
###
1.  Scene detection (classification), AF algorithm sets up search profile based on current scene type.
2.  AF search, search Lens position range to find the best focus position.
3.  Scene change detection, re-trigger AF search if scene change is detected.
 */

#define AF_SCENE_TYPE_NUM       5
#define AF_SEARCH_MODE_NUM      4
#define MAX_AF_WINDOW_SIZE      32768


/**
 * AF Range tuning
 *
 * infinityOffset, Offset of Infinity-position based from calibration data, that allow to step over infinity position.
 * macroOffset, Offset of Macro-position based from calibration data, that allow to step over macro position.
 * infinityFocusOffset, Offset of Infinity focus position (the real focused position) based from calibration data.
 * macroRangeStartOffset, search range of Macro mode is controlled by macro range start offset and macro range end offset.
 * macroRangeEndOffset, macro range end offset.
 * isManulRangeStartFromInfinity, search range of manual mode can be selected starting from 0 or infinity position,
 *   but the maximum position is fixed to 1000.
 */
typedef struct AF_RangeTune {
    int16_t infinityOffset; // Valid value range of offsets is [-1000 ~ 1000]
    int16_t macroOffset;
    int16_t infinityFocusOffset;
    int16_t macroRangeStartOffset;
    int16_t macroRangeEndOffset;
    int8_t isManualRangeStartFromInfinity;  // 0 or 1
    int8_t reserved[1];
} __attribute__((__packed__)) AF_RangeTune;



/**
 * Scene classification
 *
 * lowContrastThreshold, Low Contrast Scene threshold
 * highContrastThreshold, High Contrast Scene threshold
 * lowLightThreshold, Low Light Scene threshold
 */
typedef struct AF_SceneClassifyTune {
    int64_t lowContrastThreshold;   // [0 ~ ]
    int64_t highContrastThreshold;
    int64_t lowLightThreshold;
} __attribute__((__packed__)) AF_SceneClassifyTune;


/**
 * AF Filter types
 *
 * Weights of filters under different scenes, to reduce computation, weights of filters are limited to (1 / power of 2),
 *   that is shift operation, controlling by filter weight shift.
 *   (focus value) = (big filter value >> big filter shift) + (row filter value >> row filter shift) + (gradient filter value >> gradient filter shift)
 *
 * weightShiftBigFilter, weight shift of big filter.
 * weightShiftRowFilter, weight shift of row filter.
 * weightShiftGradientFilter, weight shift of gradient filter.
 */
typedef struct AF_FilterTypeWeightsTune {
    int8_t weightShiftBigFilter[AF_SCENE_TYPE_NUM]; // [0 ~ 64]
    int8_t weightShiftRowFilter[AF_SCENE_TYPE_NUM];
    int8_t weightShiftGradientFilter[AF_SCENE_TYPE_NUM];
        int8_t reserved[1];
} __attribute__((__packed__)) AF_FilterTypeWeightsTune;


/**
 * AF move step size
 *
 * Step size under various AF search mode.
 * AF search mode indices: NORMAL = 0, FINE = 1, SPECULATE = 2, PREDICT = 3
 */
typedef struct AF_MoveStepTune {
    int16_t step[AF_SEARCH_MODE_NUM];   // [1 ~ 1000]
} __attribute__((__packed__)) AF_MoveStepTune;


/**
 * AF window setting
 *
 * AF window size setting for Touch-AF and Continuous-AF mode.
 */
typedef struct AF_WindowSizeTune {
    int16_t sizeOfTouch[2]; // width and height, valid value range [0, MAX_AF_WINDOW_SIZE]
    int16_t sizeOfContinuous[2];
} __attribute__((__packed__)) AF_WindowSizeTune;


/**
 * Frame delay setting.
 *
 * startWarmingTime, AF start up Warming-Time.
 * isDelayOneFrameEachMove, Delay one frame after each Lens-move.
 * firstMoveFrameDelay, Delay of the first Lens-move of one AF-search.
 * infinityDelayOffset, Range scope near infinity that needs moving delay, controlled by a position offset to infinity.
 * dataTriggerFrameDelay, Frame delay of data triggering.
 * gyroTriggerFrameDelay, Frame delay of gyroscope triggering.
 * rangeTriggerFrameDelay, Frame delay of range triggering.
 */
typedef struct AF_DelayTune {
    int32_t startWarmingTime;   // millisecond, [0 ~ ]
    int8_t isDelayOneFrameEachMove; // 0 or 1
    int8_t firstMoveFrameDelay;     // [0 ~ 127]
    int16_t infinityDelayOffset;    // [0 ~ 1000]
    int8_t dataTriggerFrameDelay;   // [0 ~ 127]
    int8_t gyroTriggerFrameDelay;   // [0 ~ 127]
    int8_t rangeTriggerFrameDelay;  // [0 ~ 127]
        int8_t reserved[1];
} __attribute__((__packed__)) AF_DelayTune;


/**
 * Scene change detection
 *
 * dataTriggerRatioThreshold, Data triggering ratio threshold
 * gyroTriggerThreshold, Gyroscope triggering threshold
 * rangeTriggerThreshold, Range triggering threshold, trigger by range measurements (now they're Laser or PDAF).
 * dataStableRatioThreshold, Data stability ratio threshold.
 * gyroStableThreshold, Gyroscope stability threshold.
 * rangeStableThreshold, Range stability threshold.
 * isGyroTriggerEnabled, Enable/Disable Gyroscope triggering.
 */
typedef struct AF_SceneChangeTune {
    float dataTriggerRatioThreshold;    // (0 ~ 1)
    int gyroTriggerThreshold;       // [1 ~ ]
    int rangeTriggerThreshold;      // [1 ~ 1000]
    float dataStableRatioThreshold; // (0 ~ 1)
    int gyroStableThreshold;        // [1 ~ ]
    int rangeStableThreshold;       // [1 ~ 1000]
    int8_t isGyroTriggerEnabled;    // 0 or 1
        int8_t reserved[3];
} __attribute__((__packed__)) AF_SceneChangeTune;


/**
 * AF search pause
 *
 * gyroPauseThreshold, Gyroscope pause threshold.
 * rangePauseThreshold, Range pause threshold, pause by range measurement instrument.
 * gyroStableThreshold, Gyroscope stability threshold.
 * rangeStableThreshold, Range stability threshold.
 */
typedef struct AF_SearchPauseTune {
    int gyroPauseThreshold;     // [1 ~ ]
    int rangePauseThreshold;    // [1 ~ 1000]
    int gyroStableThreshold;    // [1 ~ ]
    int rangeStableThreshold;   // [1 ~ 1000]
} __attribute__((__packed__)) AF_SearchPauseTune;


/**
 * AF Tuning parameters
 */
typedef struct AF_TuningParameters {
    AF_RangeTune rangeTune;
    AF_SceneClassifyTune sceneClassifyTune;
    AF_FilterTypeWeightsTune filterWeightsTune;
    AF_MoveStepTune moveStepTune;
    AF_WindowSizeTune windowSizeTune;
    AF_DelayTune delayTune;
    AF_SceneChangeTune sceneChangeTune;
    AF_SearchPauseTune pauseTune;
} __attribute__((__packed__)) AF_TuningParameters;

const AF_TuningParameters* meizu_autofocus_get_tuning_parameters(void);


#ifdef __cplusplus
}
#endif

#endif // __MEIZU_AUTO_FOCUS_TUNING_PARAMETERS_H__
