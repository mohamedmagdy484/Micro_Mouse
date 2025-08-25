/******************************************************************************
 *
 * Module: DC_Motor
 *
 * Author: Mohamed Magdy
 *
 *******************************************************************************/
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "../COMMON/std_types.h"

/* ==================================================
 * Motor Right Configuration (Timer0 - OC0 -> PB3)
 * ================================================== */
#define DC_MOTOR_RIGHT_PORT      PORTB
#define DC_MOTOR_RIGHT_DDR       DDRB
#define DC_MOTOR_RIGHT_IN1       PB0
#define DC_MOTOR_RIGHT_IN2       PB1

#define PWM_RIGHT_DDR            DDRB
#define PWM_RIGHT_PIN            PB3   // OC0


/* ==================================================
 * Motor Left Configuration (Timer2 - OC2 -> PD7)
 * ================================================== */
#define DC_MOTOR_LEFT_PORT       PORTD
#define DC_MOTOR_LEFT_DDR        DDRD
#define DC_MOTOR_LEFT_IN1        PD5
#define DC_MOTOR_LEFT_IN2        PD6

#define PWM_LEFT_DDR             DDRD
#define PWM_LEFT_PIN             PD7   // OC2


/* ==================================================
 * Motor States
 * ================================================== */
typedef enum {
	STOP,
	CW,
	A_CW
} DcMotor_State;


/* ==================================================
 * Functions Prototypes
 * ================================================== */
void DC_RIGHT_INIT(void);
void DC_LEFT_INIT(void);

void DC_RIGHT(DcMotor_State state, uint8 speed);   // speed: 0 - 100%
void DC_LEFT(DcMotor_State state, uint8 speed);    // speed: 0 - 100%


#endif /* DC_MOTOR_H_ */
