/*
 * ext_int0.c
 *
 *  Created on: Sep 4, 2025
 *      Author: SIGMA
 */
#include<avr/io.h>
#include"ext_int0.h"


void ext_int0_init(void){
    // زرار interrupt على PD2 (INT0)
    DDRD &= ~(1<<PD2);   // input
    PORTD |= (1<<PD2);   // pull-up

    // تهيئة INT0 على falling edge
    GICR |= (1<<INT0);
    MCUCR |= (1<<ISC01);
    MCUCR &= ~(1<<ISC00);


}

void ext_int1_init(void){
    // زرار interrupt على PD2 (INT0)
    DDRD &= ~(1<<PD3);   // input
    PORTD |= (1<<PD3);   // pull-up

    // تهيئة INT0 على falling edge
    GICR |= (1<<INT1);
    MCUCR |= (1<<ISC01);
    MCUCR &= ~(1<<ISC00);


}

void ext_int2_init(void){
    // زرار interrupt على PD2 (INT0)
    DDRD &= ~(1<<PB2);   // input
    PORTD |= (1<<PB2);   // pull-up

    // تهيئة INT0 على falling edge
    GICR |= (1<<INT2);
    MCUCR |= (1<<ISC01);
    MCUCR &= ~(1<<ISC00);


}
