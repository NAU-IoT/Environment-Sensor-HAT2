#ifndef __BME280_H__
#define __BME280_H__
#include "addrs.h"

typedef struct BMEData {
    // file descriptor
    int file;
    // data array
    char *data;
    // b1, block data
    char *block_data;
    // t_fine, part of temperature offset calculations,
    // used in pressure and humidity functions
    float t_fine;
} BMEData;

void bme280(const HWInfo *hw_info);

float temperature(BMEData *bme_data);

float pressure(BMEData *bme_data);

float humidity();

float altitude();

#endif
