#include "aaa_calibration_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define DEFAULT_FILE_PATH "mz_rcam_cali.bin.tmp"
int saveCalibrationMapToBin(const char *_file, const aaa_calibration_map *_map) {
    if (_file == NULL) {
        _file = DEFAULT_FILE_PATH;
    }

    FILE *fp = fopen(_file, "wb+");
    if (!fp) {
        printf("saveCalibrationMapToFile failed. open %s failed\n", _file);
        return -1;
    }
    int n = sizeof(aaa_calibration_map);
    int offset = 0;
    int cnt = 0;
    do {
        cnt = fwrite((const void *) _map+offset, 1, n-offset, fp);
        offset += cnt;
    } while (offset < n && cnt > 0);

    fclose(fp);

    printf("save %s success. file length: %lu \n", _file, sizeof(aaa_calibration_map));
    return 0;
}

int parseCalibrationMapFromBin(const char *_file, const aaa_calibration_map * _map) {
    if (_file == NULL) {
        _file = DEFAULT_FILE_PATH;
    }

    FILE *fp = fopen(_file, "rb");
    int n = 0;
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    int requestSize = sizeof(aaa_calibration_map);

    if (fileSize < requestSize) {
        printf("bad file %s, fileSize = %d, request size = %d \n",
                _file, fileSize, requestSize);
        fclose(fp);
        return -1;
    }

    rewind(fp);

    int cnt = 0;
    int offset = 0;
    do {
        cnt = fread((void *)_map, 1, requestSize - offset, fp);
        offset += cnt;
    } while (offset < requestSize && cnt > 0);

    printf ("parse %s success. file length: %d, request length: %d \n",
            _file, fileSize, requestSize);
    fclose(fp);
    return 0;
}

int parseCalibrationMapFromJson(const char * _file, aaa_calibration_map * _map) {
    struct stat status;
    if (stat(_file, &status) != 0) {
        printf("File %s not found!!!\n", _file);
        return -1;
    }
    int size = status.st_size;
    char *contents = (char *)malloc(size);

    FILE *fp = fopen(_file, "rb");
    if (fp == NULL) {
        printf("Unable to open %s \n", _file);
        free(contents);
        return -1;
    }

    if (fread(contents, size, 1, fp) != 1) {
        printf ("Unable to read %s \n" , _file);
        fclose(fp);
        free(contents);
        return -1;
    }

    fclose(fp);

    printf ("%s\n", contents);

    json_value *value = json_parse((json_char *)contents, size);

    if (value == NULL) {
        printf("Unable to parse data");
        return -1;
    }


    // check the header of json value.
    {
        json_value * af = get_object("af", value);
        parse_af_tuning_parameters(af, &_map->af);

        json_value * ae = get_object("ae", value);
        parse_ae_tuning_parameters(ae, &_map->ae);

        json_value * gamma = get_object("gamma", value);
        parse_gamma_tuning_parameters(gamma, &_map->gamma);

        json_value * awb = get_object("awb", value);
        parse_awb_tuning_parameters(awb, &_map->awb);
    }
    json_value_free(value);
    free(contents);
    return 0;
}

int saveCalibrationMapToJson(const char * _file, const aaa_calibration_map * _map) {
    return 0;
}

void print_depth_shift(int depth) {
    int j;
    for (j=0; j < depth; j++) {
        printf(" ");
    }
}

void print_json_value(const json_value * value, int depth) {
    int j;
    if (value == NULL) {
        return;
    }
    if (value->type != json_object) {
        print_depth_shift(depth);
    }
    switch (value->type) {
        case json_none:
            printf("none\n");
            break;
        case json_object:
            print_object(value, depth+1);
            break;
        case json_array:
            print_array(value, depth+1);
            break;
        case json_integer:
            printf("int: %10" PRId64 "\n", value->u.integer);
            break;
        case json_double:
            printf("double: %f\n", value->u.dbl);
            break;
        case json_string:
            printf("string: %s\n", value->u.string.ptr);
            break;
        case json_boolean:
            printf("bool: %d\n", value->u.boolean);
            break;
    }
}

void print_object(const json_value *value,  int depth) {
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++) {
        print_depth_shift(depth);
        printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
        print_json_value(value->u.object.values[x].value, depth+1);
    }
}

void print_array(const json_value *value, int depth) {
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.array.length;
    printf("array\n");
    for (x = 0; x < length; x++) {
        print_json_value(value->u.array.values[x], depth);
    }
}

int parse_af_tuning_parameters(json_value *value, AF_TuningParameters *af) {
    int size, i;

    json_value *range = get_object("range", value);
    if (range) {
        af->rangeTune.infinityOffset = (int16_t)range->u.array.values[0]->u.integer;
        af->rangeTune.macroOffset = (int16_t)range->u.array.values[1]->u.integer;
        af->rangeTune.infinityFocusOffset = (int16_t)range->u.array.values[2]->u.integer;
        af->rangeTune.macroRangeStartOffset = (int16_t)range->u.array.values[3]->u.integer;
        af->rangeTune.macroRangeEndOffset = (int16_t)range->u.array.values[4]->u.integer;
        af->rangeTune.isManualRangeStartFromInfinity = (int8_t)range->u.array.values[5]->u.integer;
        af->rangeTune.reserved[0] = 0;
    }

    json_value *scene_classify = get_object("scene_classify", value);
    if (scene_classify) {
        af->sceneClassifyTune.lowContrastThreshold = (int64_t)scene_classify->u.array.values[0]->u.integer;
        af->sceneClassifyTune.highContrastThreshold = (int64_t)scene_classify->u.array.values[1]->u.integer;
        af->sceneClassifyTune.lowLightThreshold = (int64_t)scene_classify->u.array.values[2]->u.integer;
    }

    json_value *filter_weights = get_object("filter_weights", value);
    if (filter_weights) {
        json_value *weight_shift_big_filter = get_object("weight_shift_big_filter", filter_weights);
        if (weight_shift_big_filter) {
            for (i = 0; i < 5; ++i) {
                af->filterWeightsTune.weightShiftBigFilter[i] =(int8_t) weight_shift_big_filter->u.array.values[i]->u.integer;
            }
        }

        json_value *weight_shift_row_filter = get_object("weight_shift_row_filter", filter_weights);
        if (weight_shift_row_filter) {
            for (i = 0; i < 5; ++i) {
                af->filterWeightsTune.weightShiftRowFilter[i] = (int8_t) weight_shift_row_filter->u.array.values[i]->u.integer;
            }
        }

        json_value *weight_shift_gradient_filter = get_object("weight_shift_gradient_filter", filter_weights);
        if (weight_shift_gradient_filter && weight_shift_gradient_filter->type == json_array) {
            for (i = 0; i < 5; ++i) {
                af->filterWeightsTune.weightShiftGradientFilter[i] = (int8_t) weight_shift_gradient_filter->u.array.values[i]->u.integer;
            }
        }
    }

    json_value *step = get_object("step", value);
    if (step && step->type == json_array) {
        for (i = 0; i < AF_SEARCH_MODE_NUM; ++i) {
            af->moveStepTune.step[i] = (int16_t) step->u.array.values[i]->u.integer;
        }
    }

    json_value *window_size = get_object("window_size", value);
    if (window_size &&  window_size->type == json_array) {
        af->windowSizeTune.sizeOfTouch[0] = (int16_t) window_size->u.array.values[0]->u.integer;
        af->windowSizeTune.sizeOfTouch[1] = (int16_t) window_size->u.array.values[1]->u.integer;
        af->windowSizeTune.sizeOfContinuous[0] = (int16_t) window_size->u.array.values[2]->u.integer;
        af->windowSizeTune.sizeOfContinuous[1] = (int16_t) window_size->u.array.values[3]->u.integer;
    }

    json_value *delay = get_object("delay", value);
    if (delay && delay->type == json_array) {
        af->delayTune.startWarmingTime = (int32_t)delay->u.array.values[0]->u.integer;
        af->delayTune.isDelayOneFrameEachMove = (int8_t)delay->u.array.values[1]->u.integer;
        af->delayTune.firstMoveFrameDelay = (int8_t)delay->u.array.values[2]->u.integer;
        af->delayTune.infinityDelayOffset = (int16_t)delay->u.array.values[3]->u.integer;
        af->delayTune.dataTriggerFrameDelay = (int8_t)delay->u.array.values[4]->u.integer;
        af->delayTune.gyroTriggerFrameDelay = (int8_t)delay->u.array.values[5]->u.integer;
        af->delayTune.rangeTriggerFrameDelay = (int8_t)delay->u.array.values[6]->u.integer;
        af->delayTune.reserved[0] = 0;
    }

    json_value *scene_change = get_object("scene_change", value);
    if (scene_change && scene_change->type == json_array) {
        af->sceneChangeTune.dataTriggerRatioThreshold = (float) delay->u.array.values[0]->u.dbl;
        af->sceneChangeTune.gyroTriggerThreshold = (int) delay->u.array.values[1]->u.integer;
        af->sceneChangeTune.rangeTriggerThreshold = (int) delay->u.array.values[2]->u.integer;
        af->sceneChangeTune.dataStableRatioThreshold = (float) delay->u.array.values[3]->u.dbl;
        af->sceneChangeTune.gyroStableThreshold = (int) delay->u.array.values[4]->u.integer;
        af->sceneChangeTune.rangeStableThreshold = (int) delay->u.array.values[5]->u.integer;
        af->sceneChangeTune.isGyroTriggerEnabled = (int8_t) delay->u.array.values[6]->u.integer;
        af->sceneChangeTune.reserved[0] = 0;
        af->sceneChangeTune.reserved[1] = 0;
        af->sceneChangeTune.reserved[2] = 0;
    }
    json_value *search_pause = get_object("search_pause", value);
    if (search_pause && search_pause->type == json_array) {
        af->pauseTune.gyroPauseThreshold = (int) delay->u.array.values[0]->u.integer;
        af->pauseTune.rangePauseThreshold = (int) delay->u.array.values[1]->u.integer;
        af->pauseTune.gyroStableThreshold = (int) delay->u.array.values[2]->u.integer;
        af->pauseTune.rangeStableThreshold = (int) delay->u.array.values[3]->u.integer;
    }
    return 0;
}

int parse_ae_tuning_parameters(json_value *value, ae_tuning_parameters *ae) {
    int size, i;
    print_json_value(value, 0);
    json_value *metering_config = get_object("metering_config", value);
    // metering_config
    {
        json_value *centerWidth = get_object("centerWidth", metering_config);
        if (centerWidth && centerWidth->type == json_integer) {
            ae->metering_config.centerWidth = (U32)centerWidth->u.integer;
        }

        json_value *centerHeight = get_object("centerHeight", metering_config);
        if (centerHeight && centerHeight->type == json_integer) {
            ae->metering_config.centerHeight = (U32)centerHeight->u.integer;
        }

        json_value *centerMap = get_object("centerMap", metering_config);
        if (centerMap && centerMap->type == json_array) {
            for (i = 0; i < ae->metering_config.centerHeight * ae->metering_config.centerWidth; ++i) {
                ae->metering_config.centerMap[i] = (U8) centerMap->u.array.values[i]->u.integer;
            }
        }

        json_value *spotWidth = get_object("spotWidth", metering_config);
        if (spotWidth && spotWidth->type == json_integer) {
            ae->metering_config.spotWidth = (U32)spotWidth->u.integer;
        }

        json_value *spotHeight = get_object("spotHeight", metering_config);
        if (spotHeight && spotHeight->type == json_integer) {
            ae->metering_config.spotHeight = (U32)spotHeight->u.integer;
        }

        json_value *spotMap = get_object("spotMap", metering_config);
        if (spotMap && spotMap->type == json_array) {
            for (i = 0; i < ae->metering_config.spotHeight * ae->metering_config.spotWidth; ++i) {
                ae->metering_config.spotMap[i] = (U8) spotMap->u.array.values[i]->u.integer;
            }
        }
    }

    //base config.
    {
        json_value *base_config = get_object("base_config", value);
        
        json_value *targetBrMin = get_object("targetBrMin", base_config);
        if (targetBrMin && targetBrMin->type == json_integer) {
            ae->base_config.targetBrMin = (U32) targetBrMin->u.integer;
        }

        json_value *targetBrMax = get_object("targetBrMax", base_config);
        if (targetBrMax && targetBrMax->type == json_integer) {
            ae->base_config.targetBrMax = (U32) targetBrMax->u.integer;
        }

        json_value *stableBrMin = get_object("stableBrMin", base_config);
        if (stableBrMin && stableBrMin->type == json_integer) {
            ae->base_config.stableBrMin = (U32) stableBrMin->u.integer;
        }

        json_value *stableBrMax = get_object("stableBrMax", base_config);
        if (stableBrMax && stableBrMax->type == json_integer) {
            ae->base_config.stableBrMax = (U32) stableBrMax->u.integer;
        }

        json_value *brLimitMin = get_object("brLimitMin", base_config);
        if (brLimitMin && brLimitMin->type == json_integer) {
            ae->base_config.brLimitMin = (U32) brLimitMin->u.integer;
        }

        json_value *brLimitMax = get_object("brLimitMax", base_config);
        if (brLimitMax && brLimitMax->type == json_integer) {
            ae->base_config.brLimitMax = (U32) brLimitMax->u.integer;
        }

        json_value *anaGainMax = get_object("anaGainMax", base_config);
        if (anaGainMax && anaGainMax->type == json_double) {
            ae->base_config.anaGainMax = (float) anaGainMax->u.dbl;
        }

        json_value *digGainMax = get_object("digGainMax", base_config);
        if (digGainMax && digGainMax->type == json_double) {
            ae->base_config.digGainMax = (float) digGainMax->u.dbl;
        }

        json_value *aperture = get_object("aperture", base_config);
        if (aperture && aperture->type == json_integer) {
            ae->base_config.aperture = (U32) aperture->u.integer;
        }

        json_value *expTimeDefault = get_object("expTimeDefault", base_config);
        if (expTimeDefault && expTimeDefault->type == json_integer) {
            ae->base_config.expTimeDefault = (U32) expTimeDefault->u.integer;
        }

        json_value *gainDefault = get_object("gainDefault", base_config);
        if (gainDefault && gainDefault->type == json_double) {
            ae->base_config.gainDefault = (float) gainDefault->u.dbl;
        }

        json_value *expTimeMin = get_object("expTimeMin", base_config);
        if (expTimeMin && expTimeMin->type == json_integer) {
            ae->base_config.expTimeMin = (U32) expTimeMin->u.integer;
        }
    }

    // convergent_table.
    {
        json_value *convergent_table = get_object("convergent_table", value);

        json_value *targetBr = get_object("targetBr", convergent_table);
        if (targetBr && targetBr->type == json_integer) {
            ae->convergent_table.targetBr = (U32) targetBr->u.integer;
        }

        json_value *tableCount1 = get_object("tableCount1", convergent_table);
        if (tableCount1 && tableCount1->type == json_integer) {
            ae->convergent_table.tableCount1 = (U32) convergent_table1->u.integer;
        }

        json_value *table1 = get_object("table1", convergent_table);
        if (table1 && table1->type == json_array) {
            size = ae->convergent_table.tableCount1 * 2;
            for (i = 0; i < size; i+= 2) {
                ae->convergent_table.table1[i] = (U32) table1->u.array.values[i]->u.integer;
                ae->convergent_table.table1[i+1] = (float) table1->u.array.values[i]->u.dbl;
            }
        }

        json_value *tableCount2 = get_object("tableCount2", convergent_table);
        if (tableCount2 && tableCount2->type == json_integer) {
            ae->convergent_table.tableCount2 = (U32) convergent_table2->u.integer;
        }
        
        json_value *table2 = get_object("table2", convergent_table);
        if (table2 && table2->type == json_array) {
            size = ae->convergent_table.tableCount2 * 2;
            for (i = 0; i < size; i+= 2) {
                ae->convergent_table.table2[i] = (U32) table2->u.array.values[i]->u.integer;
                ae->convergent_table.table2[i+1] = (float) table2->u.array.values[i]->u.dbl;
            }
        }

        json_value *flickerAutoCount = get_object("flickerAutoCount", convergent_table);
        if (flickerAutoCount && flickerAutoCount->type == json_integer) {
            ae->convergent_table.flickerAutoCount1 = (U32) flickerAutoCount->u.integer;
        }

        json_value *flickerAuto = get_object("flickerAuto", convergent_table);
        if (flickerAuto && flickerAuto->type == json_array) {
            size = ae->convergent_table.flickerAutoCount1 ;
            for (i = 0; i < size; i++) {
                ae->convergent_table.flickerAuto1[i] = (float) flickerAuto->u.array.values[i]->u.dbl;
            }
        }

        json_value *flicker50HzCount = get_object("flicker50HzCount", convergent_table);
        if (flicker50HzCount && flicker50HzCount->type == json_integer) {
            ae->convergent_table.flicker50HzTableCount1 = (U32) flicker50HzCount->u.integer;
        }

        json_value *flicker50Hz = get_object("flicker50Hz", convergent_table);
        if (flicker50Hz && flicker50Hz->type == json_array) {
            size = ae->convergent_table.flicker50HzTableCount1 ;
            for (i = 0; i < size; i++) {
                ae->convergent_table.flicker50Hz1[i] = (float) flicker50Hz->u.array.values[i]->u.dbl;
            }
        }


        json_value *flicker60HzCount = get_object("flicker60HzCount", convergent_table);
        if (flicker60HzCount && flicker60HzCount->type == json_integer) {
            ae->convergent_table.flicker60HzTableCount1 = (U32) flicker60HzCount->u.integer;
        }

        json_value *flicker60Hz = get_object("flicker60Hz", convergent_table);
        if (flicker60Hz && flicker60Hz->type == json_array) {
            size = ae->convergent_table.flicker60HzTableCount1 ;
            for (i = 0; i < size; i++) {
                ae->convergent_table.flicker60Hz1[i] = (float) flicker60Hz->u.array.values[i]->u.dbl;
            }
        }
    }

    //environment info.
    {
        json_value *environment_info = get_object("enviroment_info", value);

        if (environment_info && environment_info->type == json_object) {
            json_value *indoorBvRange = get_object("indoorBvRange", environment_info);
            parse_ae_tuning_parameters_bv_range(indoorBvRange, &ae->environment_info.indoorBvRange);

            json_value *normalBvRange = get_object("normalBvRange", environment_info);
            parse_ae_tuning_parameters_bv_range(normalBvRange, &ae->environment_info.normalBvRange);

            json_value *outdoorBvRange = get_object("outdoorBvRange", environment_info);
            parse_ae_tuning_parameters_bv_range(outdoorBvRange, &ae->environment_info.outdoorBvRange);

            json_value *underExposureBvRange = get_object("underExposureBvRange", environment_info);
            parse_ae_tuning_parameters_bv_range(underExposureBvRange, &ae->environment_info.underExposureBvRange);

            json_value *overExposureBvRange = get_object("overExposureBvRange", environment_info);
            parse_ae_tuning_parameters_bv_range(overExposureBvRange, &ae->environment_info.overExposureBvRange);

            json_value *totalBvRange = get_object("totalBvRange", environment_info);
            parse_ae_tuning_parameters_bv_range(totalBvRange, &ae->environment_info.totalBvRange);
        }
    }

    // dynamic_compensation
    {
        json_value *dynamic_compensation = get_object("dynamic_compensation", value);
        if (dynamic_compensation & dynamic_compensation->type == json_object) {
            json_value * dynamic_ae_enabled = get_object("dynamic_ae_enabled", dynamic_compensation);
            if (dynamic_ae_enabled && dynamic_ae_enabled->type == json_integer) {
                ae->dynamic_compensation.dynamic_ae_enabled = (int) dynamic_ae_enabled->u.integer;
            }

            json_value * pureWhiteThreshold = get_object("pureWhiteThreshold", dynamic_compensation);
            if (pureWhiteThreshold && pureWhiteThreshold->type == json_double) {
                ae->dynamic_compensation.pureWhiteThreshold = (float) pureWhiteThreshold->u.dbl;
            }

            json_value * pureWhiteEnhanceMaxRatio = get_object("pureWhiteEnhanceMaxRatio", dynamic_compensation);
            if (pureWhiteEnhanceMaxRatio && pureWhiteEnhanceMaxRatio->type == json_double) {
                ae->dynamic_compensation.pureWhiteEnhanceMaxRatio = (float) pureWhiteEnhanceMaxRatio->u.dbl;
            }

            json_value * lowLightWeakRatio = get_object("lowLightWeakRatio", dynamic_compensation);
            if (lowLightWeakRatio && lowLightWeakRatio->type == json_double) {
                ae->dynamic_compensation.lowLightWeakRatio = (float) lowLightWeakRatio->u.dbl;
            }
        }
    }
    return 0;
}

void parse_ae_tuning_parameters_bv_range(json_value *value, ae_bv_range * range) {
    if (value && value->type == json_array) {
        range->exposure1 = (float) value->u.array.values[0]->u.dbl;
        range->gain1 = (float) value->u.array.values[1]->u.dbl;
        range->y1 = (float) value->u.array.values[2]->u.dbl;

        range->exposure2 = (float) value->u.array.values[3]->u.dbl;
        range->gain2 = (float) value->u.array.values[4]->u.dbl;
        range->y2 = (float) value->u.array.values[5]->u.dbl;
    }
}

int parse_gamma_tuning_parameters(json_value *value, gamma_tuning_parameters *gamma) {
    return 0;
}

int parse_awb_tuning_parameters(json_value *value, awb_tuning_parameters *awb) {
    return 0;
}

json_value *get_object(const char *key, json_value *value) {
    if (value->type != json_object) {
        printf("bad type for %s. \n", key);
        return NULL;
    }
    int size = value->u.object.length;
    int i;
    for (i = 0; i < size; ++i) {
        if (strcmp(key, value->u.object.values[i].name) == 0) {
            return value->u.object.values[i].value;
        }
    }
    return NULL;
}

json_value *get_array(const char *key, json_value *value) {
}
