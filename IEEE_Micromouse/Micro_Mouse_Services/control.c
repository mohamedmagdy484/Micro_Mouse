#include "control.h"
#include "../HAL/encoder.h"
#include <math.h>
#include <avr/interrupt.h>


#define WHEEL_BASE_MM 125.0f   // المسافة بين العجلتين بالمم (عدلها حسب تصميمك)
#define WHEEL_DIAMETER_MM 42.0f // قطر العجلة بالمم
#define PPR 200
#define Ts 0.02f   // 20ms

extern volatile long encoder_count1;
extern volatile long encoder_count2;

ISR(INT1_vect) {

	encoder_count1++;

}

ISR(INT2_vect) {

	encoder_count2++;

}


/*
 * احسب السرعة (مم/ثانية) من RPM الإنكودر
 */
float get_measured_velocity()
{
    float rpm_left = Encoder_GetRPM(&encoder_count1);
    float rpm_right = Encoder_GetRPM(&encoder_count2);
    float velocity_left = (rpm_left * M_PI * WHEEL_DIAMETER_MM) / 60.0f;
    float velocity_right = (rpm_right * M_PI * WHEEL_DIAMETER_MM) / 60.0f;
    return (velocity_left + velocity_right) / 2.0f;
}

/*
 * احسب الزاوية من فرق النبضات بين العجلتين (تقريب بسيط)
 */
float get_measured_angle()
{
    static long prev_left = 0, prev_right = 0;
    long delta_left = encoder_count1 - prev_left;
    long delta_right = encoder_count2 - prev_right;
    prev_left = encoder_count1;
    prev_right = encoder_count2;

    float angle = ((float)(delta_right - delta_left) / PPR) * (WHEEL_DIAMETER_MM / WHEEL_BASE_MM) * 360.0f;
    return angle; // زاوية الدوران خلال العينة (تقريب)
}

/*
 * الفانكشن الرئيسية: تاخد set_velocity و set_angle من الالجو
 * وتحسب measured_velocity و measured_angle من الإنكودر وتطلع pwm
 */
void Control_Update(ControlSystem_t *ctrl,
                    float set_velocity,    // من الالجو
                    float set_angle,       // من الالجو
                    float *leftMotorOut, float *rightMotorOut)
{
    float measured_velocity = get_measured_velocity();
    float measured_angle = get_measured_angle();

    // PID للسرعة الأمامية
    float forwardOut = PID_Compute(&ctrl->forwardPID, set_velocity, measured_velocity);

    // PID للزاوية (heading)
    float rotationOut = PID_Compute(&ctrl->rotationPID, set_angle, measured_angle);

    // تركيبة الإخراج النهائي للموتورين
    *leftMotorOut  = forwardOut - rotationOut;
    *rightMotorOut = forwardOut + rotationOut;
}
