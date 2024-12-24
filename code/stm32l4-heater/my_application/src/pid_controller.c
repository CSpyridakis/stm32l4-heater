/**
 * \brief: Structure representing a PID controller
 * \details: This structure contains all necessary parameters and variables for a PID controller.
 * 
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#include <stdio.h>

#include "pid_controller.h"

void PID_init(PID_Controller *pid, float Kp, float Ki, float Kd){
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->setpoint = 0.0f;
    pid->max_value = 0.0f;
    pid->min_value = 0.0f;

    pid->integral = 0.0f;
    pid->previous_error = 0.0f;
}

void PID_set_setpoint(PID_Controller *pid, float setpoint){
    pid->setpoint = setpoint;
}

void PID_set_limits(PID_Controller *pid, float min_value, float max_value){
    pid->min_value = min_value;
    pid->max_value = max_value;
}

float PID_update(PID_Controller *pid, float measurement, float dt){
    // --------------------------------------------
    // Error calculation
    // --------------------------------------------
    float error = pid->setpoint - measurement;
    
    // --------------------------------------------
    // Proportional term: P = Kp * error
    // --------------------------------------------
    float proportional_term = pid->Kp * error;

    // --------------------------------------------
    // Integral term: I = Ki * integral(error * dt)
    // --------------------------------------------
    pid->integral += error * dt;
    
    // Anti-wind-up (Dynamic integral clamping)
    // Find integral limits
    float integrator_max = 0.0f, integrator_min = 0.0f;
    if(pid->max_value > proportional_term){
        integrator_max = pid->max_value - proportional_term;
    }
    if (pid->min_value < proportional_term){
        integrator_min = pid->min_value - proportional_term;
    }
    // Clamp integral value within limits
    if (pid->integral > integrator_max) {
        pid->integral = integrator_max;
    }
    else if(pid->integral < integrator_min){
        pid->integral = integrator_min;
    }

    // Calculate integral
    float integral_term = pid->Ki * pid->integral;

    // --------------------------------------------
    // Derivative term: D = Kd * (error - prev_error) / dt
    // --------------------------------------------
    float derivative_term = pid->Kd * (error - pid->previous_error) / dt;

    // --------------------------------------------
    // PID output calculation
    // --------------------------------------------
    float out = proportional_term + integral_term + derivative_term;

    // Limiting the output to max and min values
    if(out > pid->max_value){
        out = pid->max_value;
    }
    else if(out < pid->min_value){
        out = pid->min_value;
    }

    // --------------------------------------------
    // Update values for the next iteration
    // --------------------------------------------
    pid->previous_error = error;

    // Return the updated output value
    return out;
}
