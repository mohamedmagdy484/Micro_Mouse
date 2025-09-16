#ifndef CONTROL_H
#define CONTROL_H

#include "PID.h"

//control structure
typedef struct {
    PID_Controller_t forwardPID;     // control for forward velocity
    PID_Controller_t rotationPID;    // control for heading/rotation
} ControlSystem_t;

//Update function
void Control_Update(ControlSystem_t *ctrl,
                    float set_velocity,   // من الالجو
                    float set_angle,      // من الالجو
                    float *leftMotorOut, float *rightMotorOut);

#endif
