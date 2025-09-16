/* 
 * File:   PID.h
 * Author: hamma
 *
 * Created on 26 August 2025, 21:34
 */

#ifndef PID_H
#define PID_H

#include <stdint.h>


/* PID Controller structure */
typedef struct {
    float Kp;
    float Ki;
    float Kd;

    float Ts;            /* sample time*/
    float integral;      /* accumulated integral */
    float prev_error;    /* previous error */

    float out_min;       
    float out_max;       
} PID_Controller_t;

/* pid Functions */
void PID_Init(PID_Controller_t *pid,
                        float Kp, float Ki, float Kd,
                        float Ts,
                        float out_min, float out_max);

float PID_Compute(PID_Controller_t *pid,
                           float setpoint, float measured);

void PID_Reset(PID_Controller_t *pid);



#endif	/* PID_H */

