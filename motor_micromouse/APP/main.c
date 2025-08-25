/*
 * main.c
 *
 *  Created on: Aug 25, 2025
 *      Author: SIGMA
 */

#include<avr/io.h>
//---------------------
#include"../common/std_types.h"
#include"../common/common_macros.h"
//----------------------
#include"../HAL/dc_motor.h"
#include"../hal/lcd.h"
#include"../HAL/ir_sensor.h"




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
		DC_RIGHT(CW,100);
		DC_LEFT(CW,100);

	
}
}

