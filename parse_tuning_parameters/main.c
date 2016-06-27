#include "aaa_calibration_map.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_FILE "mz_rcam_cali.json"
void LoadTuningParametersFromAlgoDefaultSettings(aaa_calibration_map * map) {
    memcpy(&map->af, meizu_autofocus_get_tuning_parameters(), sizeof (AF_TuningParameters));
    memcpy(&map->ae, meizu_tuning_get_ae(), sizeof (ae_tuning_parameters));
    memcpy(&map->gamma, meizu_tuning_get_gamma(), sizeof (gamma_tuning_parameters));
    memcpy(&map->awb, meizu_tuning_get_awb(), sizeof (awb_tuning_parameters));
}

void printAeTuningParameters(const aaa_calibration_map *map) {
    printf("---------------------------ae tuning parameters -------------------------\n");
    const ae_tuning_parameters *ae = &map->ae;
    printf("centerWidth = %d, centerWidth = %d \n",
            ae->metering_config.centerWidth, ae->metering_config.centerHeight);
}
int main(int argc, char *argv[]) {
    aaa_calibration_map map;
#if 0
    LoadTuningParametersFromAlgoDefaultSettings(&map);
    saveCalibrationMapToBin("mz_rcam_cali.bin", &map);
#endif
    const char *file = (argc > 1) ? argv[1] : DEFAULT_FILE;
    //parseCalibrationMapFromBin(file, &map);
    parseCalibrationMapFromJson(file, &map);
    saveCalibrationMapToBin("mz_rcam_cali1.bin", &map);

    aaa_calibration_map map1;
    parseCalibrationMapFromBin("mz_rcam_cali1.bin", &map1);

    compareBinMap(&map, &map1);
    // ae_tuning_parameters
    {
        //printAeTuningParameters(&map);
    }

    return 0;
}

