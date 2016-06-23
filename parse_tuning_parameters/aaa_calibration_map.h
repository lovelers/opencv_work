#ifndef __AAA_CALIBRATION_MAP_H
#define __AAA_CALIBRATION_MAP_H
#include "3a_interface/meizu_auto_focus_tuning_parameters.h"
#include "3a_interface/ae_tuning.h"
#include "3a_interface/gamma_tuning.h"
#include "3a_interface/awb_tuning.h"
#include "json/json-parser/json.h"
#include "json/json-builder/json-builder.h"

typedef struct __aaa_calibration_map {
    AF_TuningParameters af;
    ae_tuning_parameters ae;
    gamma_tuning_parameters gamma;
    awb_tuning_parameters awb;
} __attribute__((__packed__)) aaa_calibration_map;

int saveCalibrationMapToBin(const char * _file, const aaa_calibration_map * _map);
int parseCalibrationMapFromBin(const char *_file, const aaa_calibration_map * _map);

int parseCalibrationMapFromJson(const char *_file, aaa_calibration_map *_map);
int saveCalibrationMapToJson(const char * _file, const aaa_calibration_map * _map);

int parse_af_tuning_parameters(json_value *value, AF_TuningParameters *af);
int parse_ae_tuning_parameters(json_value *value, ae_tuning_parameters *ae);
int parse_gamma_tuning_parameters(json_value *value, gamma_tuning_parameters *gamma);
int parse_awb_tuning_parameters(json_value *value, awb_tuning_parameters *awb);

void print_depth_shift(int depth);
void print_json_value(const json_value *value, int depth);
void print_object(const json_value *value,  int depth);
void print_array(const json_value *value, int depth);

json_value *get_object(const char * key, json_value *value);
json_value *get_array(const char *key, json_value *value);

#endif// __AAA_CLIBRATION_MAP_H
