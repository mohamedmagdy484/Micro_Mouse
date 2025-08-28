/*
 * wall.c
 *
 *  Created on: Aug 28, 2025
 *      Author: SIGMA
 */
#include"walls.h"


	uint8 right_wall(void){
			if(s7 == 1 || s8 == 1 ){
				return 1;
			}
			else {
				return 0;
			}
		}
		uint8 left_wall(void){
			if(s1 ==1 || s2 ==1){
				return 1;
			}
			else{
				return 0;
			}
		}
		uint8 front_wall(void){

			if(s4 == 1 || s5 == 1){
				return 1;
			}
			else{
				return 0;
			}
		}
