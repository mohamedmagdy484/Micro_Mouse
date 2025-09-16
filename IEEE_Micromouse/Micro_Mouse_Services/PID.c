#include "PID.h"

/* Initialize PID controller parameters */
void PID_Init(PID_Controller_t *pid,
                        float Kp, float Ki, float Kd,
                        float Ts,
                        float out_min, float out_max)
{
    if (pid == 0 || Ts <= 0.0f) {
        
    }
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->Ts = Ts;

    pid->integral = 0.0f;
    pid->prev_error = 0.0f;

    pid->out_min = out_min;
    pid->out_max = out_max;    
}

/* Compute PID output */
float PID_Compute(PID_Controller_t *pid, float setpoint, float measured)
{
   

    float error = setpoint - measured;

    /* Integral term (with accumulation) */
    pid->integral += error * pid->Ts;

    /* Anti-windup: clamp integral */
    if (pid->integral * pid->Ki > pid->out_max) {
        pid->integral = pid->out_max / pid->Ki;
    } else if (pid->integral * pid->Ki < pid->out_min) {
        pid->integral = pid->out_min / pid->Ki;
    }
    /* Derivative term */
    float derivative = (error - pid->prev_error) / pid->Ts;

    /* PID output */
    float u = (pid->Kp * error) +
              (pid->Ki * pid->integral) +
              (pid->Kd * derivative);

    /* Clamp output */
    if (u > pid->out_max) {
        u = pid->out_max;
    } else if (u < pid->out_min) {
        u = pid->out_min;
    }

    /* Save error for next iteration */
    pid->prev_error = error;

    return u;
}

/* Reset PID state */
void PID_Reset(PID_Controller_t *pid)
{   
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;   
}
