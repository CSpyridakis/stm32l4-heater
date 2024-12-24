/**
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#include <stdio.h>
#include <stdint.h>

#include "my_application.h"
#include "pid_controller.h"
#include "interfaces_used.h"
#include "heater.h"
#include "adt7420.h"

// ---------------------------------------------
// PID related
#define PID_Kp 1.0f
#define PID_Ki 0.5f
#define PID_Kd 0.2f

#define CONTROL_SIGNAL_MAX 100
#define CONTROL_SIGNAL_MIN 0

PID_Controller pid;
float last_time_read;
// ---------------------------------------------

// ---------------------------------------------
/**
 * Redefinition of GPIO EXTI Callback in order to support the application specific
 * interrupt handlers
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == ZC_DEC_Pin){
		my_application_isr_handler();
	}
}

// ---------------------------------------------

void my_application_init(float time, float target_value) {
	// ---------------------------------------------
    // Initialize the application here
	// ---------------------------------------------

    float target_temperature = target_value;                        // Target temperature

    // Setup PID
    PID_init(&pid, PID_Kp, PID_Ki, PID_Kd);                         // Initialize PID controller
    PID_set_setpoint(&pid, target_temperature);                     // Set desired output
    PID_set_limits(&pid, CONTROL_SIGNAL_MIN, CONTROL_SIGNAL_MAX);   // Set limits for output (0-100% of period to keep heater on)

    last_time_read = HAL_GetTick();                                 // Get current time from system, to measure dt for PID

    ADT7420_STATUS status = adt7420_verify_setup();					// Verify ADT7420 operation
    if (status != ADT7420_SUCCESS){
    	// FIXME: determine how the application should behave
    	UNUSED(status);
    }
}

void my_application_isr_handler() {
	// ---------------------------------------------
    // Handle interrupt here every time a zero cross is detected
	// ---------------------------------------------

	// ------------------------------------------------------------
	// Related to Question 3
	// ------------------------------------------------------------
	control_heater_reset();			// Will make sure that the heater goes to its idle state before starting it again

    // ------------------------------------------------------------
    // Related to Question 2
    // ------------------------------------------------------------
    // Read temperature sensor data from I2C
    float current_temperature;
    if ( adt7420_read_temperature(&current_temperature) != ADT7420_SUCCESS ){
    	return;
    }

    // Send temperature data to serial port for debugging
    char data[50];
    sprintf(data, "Current temperature: %.2f\n", current_temperature);	// Create message
    HAL_UART_Transmit(&UART_USED, (uint8_t*)&data, sizeof(data), 1000);	// Send message

    // ------------------------------------------------------------
    // Related to Question 3
    // ------------------------------------------------------------
    // Time step from the previous read
    float dt = ( last_time_read - HAL_GetTick() );

    // Calculate PID output      
    float control_signal = PID_update(&pid, current_temperature, dt);

    // Control heater based on the PID output
    control_heater_set(control_signal);
}
