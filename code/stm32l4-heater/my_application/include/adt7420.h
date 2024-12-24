/**
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#ifndef ADT7420_H
#define ADT7420_H

#include "interfaces_used.h"

#define ADT_I2C_ADDRESS 0x48

#define TEMP_CONFIG     0x03

#define TEMP_REG	0x00
#define STAT_REG	0x02
#define CONF_REG	0x03
#define ID_REG		0x0B


typedef enum {
    ADT7420_SUCCESS = 0,
    ADT7420_ERROR 	= -1
} ADT7420_STATUS;

ADT7420_STATUS adt7420_verify_setup();

ADT7420_STATUS adt7420_read_temperature(float* temp);

#endif
