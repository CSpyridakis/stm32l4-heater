/**
 * \brief: Structure representing a PID controller
 * \details: This structure contains all necessary parameters and variables for a PID controller.
 * 
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct {
    float Kp;               // Proportional gain
    float Ki;               // Integral gain
    float Kd;               // Derivative gain

    float setpoint;         // The desired value of the controlled plant
    float max_value;        // The maximum value the the output can take
    float min_value;        // The minimum value the the output can take

    float integral;         
    float previous_error;


} PID_Controller;

/**
 * \brief: Initializes the PID controller structure with given gains
 * 
 * \param[in] pid   Pointer to the PID controller structure
 * \param[in] Kp    Proportional gain
 * \param[in] Ki    Integral gain
 * \param[in] Kd    Derivative gain
 */
void PID_init(PID_Controller *pid, float Kp, float Ki, float Kd);

/** 
 * \brief: Sets the desired output value for the controller
 * 
 * \param[in] pid   Pointer to the PID controller structure
 * \param[in] setpoint Desired output value
*/
void PID_set_setpoint(PID_Controller *pid, float setpoint);

/**
 * \brief: Sets the maximum and minimum output values for the controller
 * 
 * \param[in] pid   Pointer to the PID controller structure
 * \param[in] max_value Maximum output value
 * \param[in] min_value Minimum output value
 */
void PID_set_limits(PID_Controller *pid, float min_value, float max_value);

/**
 * \brief: Updates the PID controller and returns the current control signal
 * 
 * \param[in] pid   Pointer to the PID controller structure
 * \param[in] measurement Current sensor/measurement value
 * \param[in] dt     Time step between updates (in seconds)
 * 
 * \return Updated control signal
 */
float PID_update(PID_Controller *pid, float measurement, float dt);

#endif // PID_CONTROLLER_H
