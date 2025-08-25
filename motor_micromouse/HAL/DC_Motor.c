
 /******************************************************************************
 *
 * Module: DC_Motor
 *
 *
 *
 *
 *
 * Author: Mohamed Magdy
 *
 *******************************************************************************/
#include <avr/io.h>
#include "DC_Motor.h"

void DC_RIGHT_INIT(void)
{
    // Set IN1 and IN2 (motor right direction pins) as output
    DC_MOTOR_RIGHT_DDR |= (1 << DC_MOTOR_RIGHT_IN1) | (1 << DC_MOTOR_RIGHT_IN2);

    // Set PWM pin (OC0 -> PB3) as output
    PWM_RIGHT_DDR |= (1 << PWM_RIGHT_PIN);

    // Set Timer0 to Fast PWM mode, non-inverting
    TCCR0 = (1 << WGM00) | (1 << WGM01)   // Fast PWM Mode
           | (1 << COM01)|            // Non-Inverting Mode
		   (1<<CS01) | (1<<CS00);;   // Prescaler = 1024

    // Stop motor initially
    DC_MOTOR_RIGHT_PORT &= ~((1 << DC_MOTOR_RIGHT_IN1) | (1 << DC_MOTOR_RIGHT_IN2));
    OCR0 = 0; // 0% duty cycle
}
void DC_LEFT_INIT(void){
    // Set IN3 and IN4 as output (خليهم مثلاً PC2 و PC3)
    DC_MOTOR_LEFT_DDR |= (1 << DC_MOTOR_LEFT_IN1) | (1 << DC_MOTOR_LEFT_IN2);

    // Set PWM pin (OC2 -> PD7) as output
    PWM_LEFT_DDR |= (1 << PWM_LEFT_PIN);

    // Set Timer2 to Fast PWM mode, non-inverting
    TCCR2 = (1 << WGM20) | (1 << WGM21)   // Fast PWM Mode
           | (1 << COM21)|                 // Non-Inverting Mode
		   (1<<CS22);   // Prescaler = 1024

    // Stop motor initially
    DC_MOTOR_LEFT_PORT &= ~((1 << DC_MOTOR_LEFT_IN1) | (1 << DC_MOTOR_LEFT_IN2));
    OCR2 = 0; // 0% duty cycle


}

void DC_RIGHT(DcMotor_State state, uint8 speed)
{
    if (speed > 100) speed = 100;

    // Convert % to 8-bit PWM value (0-255)
    OCR0 = (uint8)((speed * 255) / 100);

    switch (state)
    {
        case CW:
            DC_MOTOR_RIGHT_PORT |= (1 << DC_MOTOR_RIGHT_IN1);
            DC_MOTOR_RIGHT_PORT &= ~(1 << DC_MOTOR_RIGHT_IN2);
            break;

        case A_CW:
            DC_MOTOR_RIGHT_PORT |= (1 << DC_MOTOR_RIGHT_IN2);
            DC_MOTOR_RIGHT_PORT &= ~(1 << DC_MOTOR_RIGHT_IN1);
            break;

        case STOP:
        default:
            DC_MOTOR_RIGHT_PORT &= ~((1 << DC_MOTOR_RIGHT_IN1) | (1 << DC_MOTOR_RIGHT_IN2));
            break;
    }
}
void DC_LEFT(DcMotor_State state, uint8 speed)
{
    if (speed > 100) speed = 100;

    // Convert % to 8-bit PWM value (0-255)
    OCR2 = (uint8)((speed * 255) / 100);

    switch (state)
    {
        case CW:
            DC_MOTOR_LEFT_PORT |= (1 << DC_MOTOR_LEFT_IN1);
            DC_MOTOR_LEFT_PORT &= ~(1 << DC_MOTOR_LEFT_IN2);
            break;
        case A_CW:
            DC_MOTOR_LEFT_PORT |= (1 << DC_MOTOR_LEFT_IN2);
            DC_MOTOR_LEFT_PORT &= ~(1 << DC_MOTOR_LEFT_IN1);
            break;
        case STOP:
        default:
            DC_MOTOR_LEFT_PORT &= ~((1 << DC_MOTOR_LEFT_IN1) | (1 << DC_MOTOR_LEFT_IN2));
            break;
    }
}
