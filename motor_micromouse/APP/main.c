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

		if(IR1_getState() == LOGIC_HIGH){
			// شمال اوي → لف يمين بقوة
			DC_LEFT(CW, 150);
			DC_RIGHT(CW, 50);
		}
		else if(IR2_getState() == LOGIC_HIGH){
			// شمال → لف يمين خفيف
			DC_LEFT(CW, 130);
			DC_RIGHT(CW, 80);
		}
		else if(IR3_getState() == LOGIC_HIGH){
			// شمال نص نص
			DC_LEFT(CW, 120);
			DC_RIGHT(CW, 100);
		}
		else if(IR4_getState() == LOGIC_HIGH || IR4_getState() == LOGIC_HIGH){
			// نص → امشي عدل
			DC_LEFT(CW, 120);
			DC_RIGHT(CW, 120);
		}
		else if(IR5_getState() == LOGIC_HIGH){
			// يمين نص نص
			DC_LEFT(CW, 100);
			DC_RIGHT(CW, 120);
		}
		else if(IR6_getState() == LOGIC_HIGH){
			// يمين
			DC_LEFT(CW, 80);
			DC_RIGHT(CW, 130);
		}
		else if(IR7_getState() == LOGIC_HIGH){
			// يمين اوي → لف شمال بقوة
			DC_LEFT(CW, 50);
			DC_RIGHT(CW, 150);
		}
		else{
			// مفيش حاجة detected → وقف
			DC_LEFT(STOP, 0);
			DC_RIGHT(STOP, 0);


	}
}
}
