/*
 * encoder.c
 *
 *  Created on: Sep 16, 2025
 *      Author: SIGMA
 */

#include "encoder.h"
#define PPR 200
#define Ts 0.02f

// تعريف المتغيرات هنا فقط!
volatile long encoder_count1 = 0;
volatile long encoder_count2 = 0;

// دالة حساب RPM
float Encoder_GetRPM(volatile long *count)
{
    static long prev_count1 = 0;
    static long prev_count2 = 0;

    long diff;
    if (count == &encoder_count1) {
        diff = encoder_count1 - prev_count1;
        prev_count1 = encoder_count1;
    } else {
        diff = encoder_count2 - prev_count2;
        prev_count2 = encoder_count2;
    }

    float revolutions = (float)diff / PPR;
    return (revolutions / Ts) * 60.0f;
}
