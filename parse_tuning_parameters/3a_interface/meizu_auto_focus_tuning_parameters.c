#include "meizu_auto_focus_tuning_parameters.h"

const AF_TuningParameters* meizu_autofocus_get_tuning_parameters(void) {
/*
    const AF_RangeTune rangeTune = {-7, -100, -7, 300, 100, 1};
    const AF_SceneClassifyTune sceneClassifyTune = {4500000, 20000000};
    const AF_FilterTypeWeightsTune filterWeightsTune = {{0, 0, 0}, {2, 2, 2}, {64, 0, 64}};
    const AF_MoveStepTune moveStepTune = {50, 25, 38, 50};
    const AF_WindowSizeTune windowSizeTune = {{4680, 5956}, {4680, 5956}};
    const AF_DelayTune delayTune = {150, 0, 1, 76, 6, 7, 4};
    const AF_SceneChangeTune sceneChangeTune = {0.25f, 5, 50, 0.04f, 3, 14, 1};
    const AF_SearchPauseTune pauseTune = {6, 100, 3, 26};
*/

#define AF_RANGE_TUNE_INIT                  {5, -100, 5, 300, 100, 1}
#define AF_SCENE_CLASSIFY_TUNE_INIT         {6500000, 20000000, 650000000}
#define AF_FILTER_WEIGHTS_TUNE_INIT         {{0, 0, 0, 0, 0}, {1, 0, 1, 1, 0}, {3, 3, 3, 3, 3}}
#define AF_MOVE_STEP_TUNE_INIT              {50, 25, 38, 50}
#define AF_WINDOW_SIZE_TUNE_INIT            {{4680, 5956}, {4680, 5956}}
#define AF_DELAY_TUNE_INIT                  {150, 0, 1, 0, 6, 7, 4}
#define AF_SCENE_CHANGE_TUNE_INIT           {0.25f, 5, 50, 0.04f, 3, 14, 1}
#define AF_PAUSE_TUNE_INIT                  {6, 100, 3, 26}

    static const AF_TuningParameters tune =
            {AF_RANGE_TUNE_INIT, AF_SCENE_CLASSIFY_TUNE_INIT, AF_FILTER_WEIGHTS_TUNE_INIT, AF_MOVE_STEP_TUNE_INIT,
             AF_WINDOW_SIZE_TUNE_INIT, AF_DELAY_TUNE_INIT, AF_SCENE_CHANGE_TUNE_INIT, AF_PAUSE_TUNE_INIT};

    return &tune;
}

