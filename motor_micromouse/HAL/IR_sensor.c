#include "IR_sensor.h"

// ==== INIT Functions ====
void IR1_init(void){ IR_DDR &= ~(1<<IR1_PIN); }
void IR2_init(void){ IR_DDR &= ~(1<<IR2_PIN); }
void IR3_init(void){ IR_DDR &= ~(1<<IR3_PIN); }
void IR6_init(void){ IR_DDR &= ~(1<<IR6_PIN); }
void IR7_init(void){ IR_DDR &= ~(1<<IR7_PIN); }
void IR8_init(void){ IR_DDR &= ~(1<<IR8_PIN); }

// ==== GET STATE Functions ====
uint8 IR1_getState(void){ return (IR_PIN_REG & (1<<IR1_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
uint8 IR2_getState(void){ return (IR_PIN_REG & (1<<IR2_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
uint8 IR3_getState(void){ return (IR_PIN_REG & (1<<IR3_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
uint8 IR6_getState(void){ return (IR_PIN_REG & (1<<IR6_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
uint8 IR7_getState(void){ return (IR_PIN_REG & (1<<IR7_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
uint8 IR8_getState(void){ return (IR_PIN_REG & (1<<IR8_PIN)) ? LOGIC_HIGH : LOGIC_LOW; }
