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
#include"../walls_detect/walls.h"
//-----------------------------------
#include <stdio.h>
#include "../algo/solver.h"




int main(void){
	LCD_init();
    generateInitialWalls();

	IR1_init();
	IR2_init();
	IR3_init();
	IR4_init();\
	IR5_init();
	IR6_init();
	IR7_init();
	IR8_init();

	DC_RIGHT_INIT();
	DC_LEFT_INIT();

	LCD_displayString("mazaryta_team");

	while(1){
		 s1 = IR1_getState();
		 s2 = IR2_getState();
		 s3 = IR3_getState();
		 s4 = IR4_getState();
		 s5 = IR5_getState();
		 s6 = IR6_getState();
		 s7 = IR7_getState();
		 s8 = IR8_getState();

	   Action nextMove = solver();




	}
    return 0;

}
