/**
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#ifndef MY_APPLICATION_H
#define MY_APPLICATION_H

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

#endif // MY_APPLICATION_H
