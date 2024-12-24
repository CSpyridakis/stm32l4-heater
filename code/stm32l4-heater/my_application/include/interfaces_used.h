/**
 * \brief: Using this abstraction, application logic and board configurations can be separated.
 * 			This way, multiple boards can run the same code with minimal modifications.
 *
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#ifndef INTERFACES_USED_H
#define INTERFACES_USED_H

#include "stm32l4xx_hal.h"

// UART
#define UART_USED huart1
extern UART_HandleTypeDef UART_USED;
// I2C
#define I2C_USED hi2c1
extern I2C_HandleTypeDef I2C_USED;
// Zero cross detection
#define ZC_DEC_Pin GPIO_PIN_11
// Heater output
#define HEATER_GPIO_Port GPIOA
#define HEATER_Pin GPIO_PIN_12

#endif // INTERFACES_USED_H
