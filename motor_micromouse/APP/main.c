/*
 * main.c
 *
 *  Created on: Aug 25, 2025
 *      Author: SIGMA
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../common/std_types.h"
#include "../common/common_macros.h"
#include "../HAL/dc_motor.h"
#include "../HAL/lcd.h"
#include "../HAL/ir_sensor.h"

int main(void){
	LCD_init();

	IR1_init();
	IR2_init();
	IR3_init();
	IR4_init();
	IR5_init();
	IR6_init();
	IR7_init();
	IR8_init();

	DC_RIGHT_INIT();
	DC_LEFT_INIT();

	LCD_displayString("mazaryta_team");

	while(1){
		uint8 s1 = IR1_getState();
		uint8 s2 = IR2_getState();
		uint8 s3 = IR3_getState();
		uint8 s4 = IR4_getState();
		uint8 s5 = IR5_getState();
		uint8 s6 = IR6_getState();
		uint8 s7 = IR7_getState();
		uint8 s8 = IR8_getState();



		if(s1 && s2 && s7 && s8  ){
			DC_LEFT(CW,100);
			DC_RIGHT(CW,100);
		}
		else if((s1 && s2) | s3 |s4  ){
			DC_LEFT(CW,100);
			DC_RIGHT(A_CW,0);
			_delay_ms(300);
		}
		else if((s7 && s8) | s5 |s6){
			DC_LEFT(A_CW,0);
			DC_RIGHT(CW,100);
			_delay_ms(300);
		}
		else{
			DC_LEFT(STOP,0);
			DC_RIGHT(STOP,0);
		}
	}
}
