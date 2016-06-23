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

        json_value * ae = get_object("af", value);
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

    print_json_value(value, 0);

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
    return 0;
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
