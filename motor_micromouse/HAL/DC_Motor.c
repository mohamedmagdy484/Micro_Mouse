
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
    DC_MOTOR_RIGHT_DDR |= (1 << DC_MOTOR_RIGHT_IN1) | (1 << DC_MOTOR_RIGHT_IN2);

    PWM_RIGHT_DDR |= (1 << PWM_RIGHT_PIN);

    TCCR0 = (1 << WGM00) | (1 << WGM01)   
           | (1 << COM01)|           
		   (1<<CS01) | (1<<CS00);;   


    DC_MOTOR_RIGHT_PORT &= ~((1 << DC_MOTOR_RIGHT_IN1) | (1 << DC_MOTOR_RIGHT_IN2));
    OCR0 = 0; 
}
void DC_LEFT_INIT(void){
  
    DC_MOTOR_LEFT_DDR |= (1 << DC_MOTOR_LEFT_IN1) | (1 << DC_MOTOR_LEFT_IN2);

  
    PWM_LEFT_DDR |= (1 << PWM_LEFT_PIN);

 
    TCCR2 = (1 << WGM20) | (1 << WGM21)  
           | (1 << COM21)|               
		   (1<<CS22);   

  
    DC_MOTOR_LEFT_PORT &= ~((1 << DC_MOTOR_LEFT_IN1) | (1 << DC_MOTOR_LEFT_IN2));
    OCR2 = 0; 


}

void DC_RIGHT(DcMotor_State state, uint8 speed)
{
    if (speed > 100) speed = 100;

 
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

