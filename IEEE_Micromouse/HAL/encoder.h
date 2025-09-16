/*
 * encoder.h
 *
 *  Created on: Sep 16, 2025
 *      Author: SIGMA
 */

#ifndef HAL_ENCODER_H_
#define HAL_ENCODER_H_

extern volatile long encoder_count1;
extern volatile long encoder_count2;

float Encoder_GetRPM(volatile long *count);

#endif /* HAL_ENCODER_H_ */
