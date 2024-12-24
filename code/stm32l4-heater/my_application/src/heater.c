/**
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#include "heater.h"

#include "interfaces_used.h"

void control_heater_activate(void){
	HEATER_ACTIVATE();
}

void control_heater_reset(void){
	HEATER_DEACTIVATE();
}

void control_heater_set(float control_signal){
	// Control_signal is a percentage of time (0-100%) to keep the
	// heating element active in a half cycle duration
	int control = (int)control_signal;

	// Calculate half cycle duration
	//
	// Half cycle duration
	//   ||
	//   \/
	//	|   |
	/*	  _     _     _     _
		 / \   / \   / \   / \
		 	\_/	  \_/   \_/	  \_/
	*/
	uint8_t half_cycle_duration_ms = (uint8_t)((1.0f / AC_FREQ) / 2.0f * 1000.0f);

	// Calculate the duration to keep active the heater, based on the control_signal
	// and the overall duration of a half cycle
	//   Active time
	//    ||
	//    \/
	//	  _       _       _       _
	/*	 | \     | \     | \     | \
		 	 |_/	 |_/     |_/	 |_/
	*/
	uint8_t heater_active_time_ms = half_cycle_duration_ms * (uint8_t)control;


	// Finally, calculate the actual delay in ms before activating the heating element for
	// the rest of the cycle
	//
	// Delay
	// ||
	// \/
	//	  _       _       _       _
	/*	 | \     | \     | \     | \
		 	 |_/	 |_/     |_/	 |_/
	*/
	uint8_t delay_until_heater_activation_ms = half_cycle_duration_ms - heater_active_time_ms;

	// FIXME: This is not recommended! Since we are already in a ISR, and it MUST be as short as
	// possible, it is only used for the easy of use to demonstrate the implementation that is followed
	HAL_Delay(delay_until_heater_activation_ms);

	// Finally, for the rest time, activate the heater
	control_heater_activate();
}
