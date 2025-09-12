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
 * Motor Right Configuration (Timer1 - OC1B -> PD4)
 * ================================================== */
#define DC_MOTOR_RIGHT_PORT      PORTD
#define DC_MOTOR_RIGHT_DDR       DDRD
#define DC_MOTOR_RIGHT_IN1       PD0
#define DC_MOTOR_RIGHT_IN2       PD1

#define PWM_RIGHT_DDR            DDRD
#define PWM_RIGHT_PIN            PD4   // OC1B (Right Motor PWM)


/* ==================================================
 * Motor Left Configuration (Timer1 - OC1A -> PD5)
 * ================================================== */
#define DC_MOTOR_LEFT_PORT       PORTD
#define DC_MOTOR_LEFT_DDR        DDRD
#define DC_MOTOR_LEFT_IN1        PD2
#define DC_MOTOR_LEFT_IN2        PD3

#define PWM_LEFT_DDR             DDRD
#define PWM_LEFT_PIN             PD5   // OC1A (Left Motor PWM)


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
void Motors_init(void);

void DC_RIGHT(DcMotor_State state, uint8 speed);   // speed: 0 - 100%
void DC_LEFT(DcMotor_State state, uint8 speed);    // speed: 0 - 100%


#endif /* DC_MOTOR_H_ */
