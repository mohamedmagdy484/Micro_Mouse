#include "IR_sensor.h"

// ==== INIT Functions ====
void IR1_init(void){ IR_DDR &= ~(1<<IR1_PIN); }
void IR3_init(void){ IR_DDR &= ~(1<<IR3_PIN); }
void IR8_init(void){ IR_DDR &= ~(1<<IR8_PIN); }

// ==== GET STATE Functions ====
uint8 IR1_getState(void){ return (IR_PIN_REG & (1<<IR1_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
uint8 IR3_getState(void){ return (IR_PIN_REG & (1<<IR3_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
uint8 IR8_getState(void){ return (IR_PIN_REG & (1<<IR8_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
