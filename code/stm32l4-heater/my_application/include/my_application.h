/**
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#ifndef MY_APPLICATION_H
#define MY_APPLICATION_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "pid_controller.h"
#include "interfaces_used.h"
#include "heater.h"
#include "adt7420.h"

#define STOP_MODE_DURATION_SEC	( 2 /* hours */ * 60 /* minutes */ * 60 /* seconds */ )
#define OPER_MODE_DURATION_MS	((uint32_t)( 1 /* hours */ * 60 /* minutes */ * 60 /* seconds */ * 1000 /* milliseconds */ ))
#define TIMER_FREQ				( /* TBD */ )
#define TIMER_TICKS_REQUIRED	( STOP_MODE_DURATION_SEC / TIMER_FREQ )

/**
 * \brief: Initialize the application with a given start time and target temperature value.
 * 
 *      Using this abstraction, application logic and board configurations can be separated.
 *      This way, multiple boards can run the same code with minimal modifications.

 * @param time: The initialization time of the application. Can be considered as the same time the first
                temperature reading was taken. Important for PID controller
 * @param target_value: The target temperature value that the application should try to maintain.
 */
void my_application_init(float time, float target_value);

/**
 * \brief: This function will be called periodically by an Interrupt Service Routine (ISR). 
 *          More specifically, since we need a zero crossing detector, every time it is detected, this
 *          function will be called.
 *
 *          Based on the schematic, whenever a zero cross is detected, we will have a low voltage
 *          in the ZC_DEC pin, hence, ZC_DEC is configured as an external interrupt to detect
 *          falling edges.
 */
void my_application_isr_handler();

bool time_to_sleep();

void stop_mode();

#endif // MY_APPLICATION_H
