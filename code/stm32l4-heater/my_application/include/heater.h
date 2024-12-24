/**
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#ifndef HEATER_H
#define HEATER_H

#include "interfaces_used.h"

#define AC_FREQ	50

#define HEATER_ACTIVATE() 	HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, GPIO_PIN_SET)
#define HEATER_DEACTIVATE() HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, GPIO_PIN_RESET)

/**
 * \brief:
 */
void control_heater_reset(void);

/**
 * \brief: Control
 */
void control_heater_set(float control_signal);

/**
 * \brief:
 */
void control_heater_activate_isr(void);

#endif // HEATER_H
