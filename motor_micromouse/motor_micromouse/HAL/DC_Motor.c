/******************************************************************************
 *
 * Module: DC_Motor
 *
 * Author: Mohamed Magdy
 *
 *******************************************************************************/

#include "dc_motor.h"
#include <avr/io.h>

/* ==================================================
 * Functions Definitions
 * ================================================== */

void Motors_init(void){

	DC_MOTOR_RIGHT_DDR |= (1 << DC_MOTOR_RIGHT_IN1) | (1 << DC_MOTOR_RIGHT_IN2);
	PWM_RIGHT_DDR |= (1 << PWM_RIGHT_PIN);
	DC_MOTOR_RIGHT_PORT &= ~((1 << DC_MOTOR_RIGHT_IN1) | (1 << DC_MOTOR_RIGHT_IN2));
	DC_MOTOR_LEFT_DDR |= (1 << DC_MOTOR_LEFT_IN1) | (1 << DC_MOTOR_LEFT_IN2);
	PWM_LEFT_DDR |= (1 << PWM_LEFT_PIN);
	DC_MOTOR_LEFT_PORT &= ~((1 << DC_MOTOR_LEFT_IN1) | (1 << DC_MOTOR_LEFT_IN2));

	TCCR1A |= (1 << WGM10) | (1 << COM1B1) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS11);
}




/* --------- Control Right Motor --------- */
void DC_RIGHT(DcMotor_State state, uint8 speed)
{
	/* Control direction */
	switch(state)
	{
	case STOP:
		DC_MOTOR_RIGHT_PORT &= ~((1 << DC_MOTOR_RIGHT_IN1) | (1 << DC_MOTOR_RIGHT_IN2));
		break;

	case CW:
		DC_MOTOR_RIGHT_PORT |= (1 << DC_MOTOR_RIGHT_IN1);
		DC_MOTOR_RIGHT_PORT &= ~(1 << DC_MOTOR_RIGHT_IN2);
		break;

	case A_CW:
		DC_MOTOR_RIGHT_PORT |= (1 << DC_MOTOR_RIGHT_IN2);
		DC_MOTOR_RIGHT_PORT &= ~(1 << DC_MOTOR_RIGHT_IN1);
		break;
	}

	/* Set speed (0 - 100%) */
	OCR1B = (uint8)((speed * 255) / 100);
}

/* --------- Control Left Motor --------- */
void DC_LEFT(DcMotor_State state, uint8 speed)
{
	/* Control direction */
	switch(state)
	{
	case STOP:
		DC_MOTOR_LEFT_PORT &= ~((1 << DC_MOTOR_LEFT_IN1) | (1 << DC_MOTOR_LEFT_IN2));
		break;

	case CW:
		DC_MOTOR_LEFT_PORT |= (1 << DC_MOTOR_LEFT_IN1);
		DC_MOTOR_LEFT_PORT &= ~(1 << DC_MOTOR_LEFT_IN2);
		break;

	case A_CW:
		DC_MOTOR_LEFT_PORT |= (1 << DC_MOTOR_LEFT_IN2);
		DC_MOTOR_LEFT_PORT &= ~(1 << DC_MOTOR_LEFT_IN1);
		break;
	}

	/* Set speed (0 - 100%) */
	OCR1A = (uint8)((speed * 255) / 100);
}
