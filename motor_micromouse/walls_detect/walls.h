/*
 * walls.h
 *
 *  Created on: Aug 28, 2025
 *      Author: SIGMA
 */
#include"../common/std_types.h"
#ifndef WALLS_DETECT_WALLS_H_
#define WALLS_DETECT_WALLS_H_

 volatile uint8 s1;
 volatile uint8 s2;
 volatile uint8 s3;
 volatile uint8 s4;
 volatile uint8 s5;
 volatile uint8 s6;
 volatile uint8 s7;
 volatile uint8 s8;

uint8 right_wall(void);
uint8 left_wall(void);
uint8 front_wall(void);

#endif /* WALLS_DETECT_WALLS_H_ */
