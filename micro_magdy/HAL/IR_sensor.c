#include "IR_sensor.h"

// ==== INIT Functions ====
void IR1_init(void){ IR_DDR &= ~(1<<IR1_PIN); }
void IR3_init(void){ IR_DDR &= ~(1<<IR3_PIN); }
void IR8_init(void){ IR_DDR &= ~(1<<IR8_PIN); }

void IR_FRONT_LEFT_init(void){ IR_DDR &= ~(1<<IR_FRONT_LEFT_PIN); }
void IR_FRONT_RIGHT_init(void){ IR_DDR &= ~(1<<IR_FRONT_RIGHT_PIN); }



// ==== GET STATE Functions ====
uint8 IR1_getState(void){ return (IR_PIN_REG & (1<<IR1_PIN)) ? LAQT : MS_LAQT; }
uint8 IR3_getState(void){ return (IR_PIN_REG & (1<<IR3_PIN)) ? LAQT : MS_LAQT; }
uint8 IR8_getState(void){ return (IR_PIN_REG & (1<<IR8_PIN)) ? LAQT : MS_LAQT; }
uint8 IR_FRONT_LEFT_getState(void){ return (IR_PIN_REG & (1<<IR_FRONT_LEFT_PIN)) ? LAQT : MS_LAQT; }
uint8 IR_FRONT_RIGHT_getState(void){ return (IR_PIN_REG & (1<<IR_FRONT_RIGHT_PIN)) ? LAQT : MS_LAQT; }
//LAQT == 0
//MS_LAQT == 1
