/**
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#include "my_application.h"

// ---------------------------------------------
// PID related
#define PID_Kp 1.0f
#define PID_Ki 0.5f
#define PID_Kd 0.2f

#define CONTROL_SIGNAL_MAX 100
#define CONTROL_SIGNAL_MIN 0

static PID_Controller pid;
static float last_time_read;
// ---------------------------------------------

static uint32_t operation_init_time;

// ---------------------------------------------
/**
 * Redefinition of GPIO EXTI Callback in order to support the application specific
 * interrupt handlers, this will handle the most part of the processing, when the MCU
 * is not in the STOP mode
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

    operation_init_time = HAL_GetTick();
}

bool time_to_sleep(){
	// FIXME: In a real world application a timer would be a preferable choice
	// it is only used for the easy of use as a PoC solution, a trade off,
	// since it is the only calculation that is done in the while loop
	if( (HAL_GetTick() - operation_init_time ) < OPER_MODE_DURATION_MS ){
		HAL_Delay(10);		// Prevent "instantaneous" re-checking
		return false;
	}

	return true;
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
    sprintf(data, "Current temperature: %.2f\n", current_temperature);				// Create message
    HAL_UART_Transmit(&UART_USED, (uint8_t*)&data, strlen(data), HAL_MAX_DELAY);	// Send message

    // ------------------------------------------------------------
    // Related to Question 3
    // ------------------------------------------------------------
    // Time step from the previous read in ms
    float dt = ( last_time_read - HAL_GetTick() );

    // Calculate PID output      
    float control_signal = PID_update(&pid, current_temperature, dt);

    // Control heater based on the PID output
    control_heater_set(control_signal);
}

void stop_mode(){
	// FIXME: What is missing from this implementation?
	// To set LPTIM or another timer to generate an event every
	// TIMER_TICKS_REQUIRED ticks in order to resume from STOP mode

	// Make sure that the heating element is turned off
	control_heater_reset();

	// Zero crossing control is achieved using an interrupt, hence, it is mandatory,
	// in order to not alter the implementation, to disable interrupts before
	// entering stop mode. We will resume from the STOP mode using Events
	// Disable all interrupts
	__disable_irq();

	// Suspend the systick before going into stop mode
	HAL_SuspendTick();

	// Enter Stop Mode
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);

	// Now we are in stop mode
	// ...
	// The code will continue
	// its execution at the time we wake up

	// Resume clocks
	SystemClock_Config();
	// Resume systick
	HAL_ResumeTick();

	// Enable all interrupts again
	__enable_irq();

	// To calculate the duration we are in operation mode
	operation_init_time = HAL_GetTick();
}
