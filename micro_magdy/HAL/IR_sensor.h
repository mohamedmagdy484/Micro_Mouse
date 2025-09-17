#ifndef HAL_IR_SENSOR_H_
#define HAL_IR_SENSOR_H_

#include <avr/io.h>
#include "../common/std_types.h"

// ==== Configuration Macros for 8 IR Sensors on PORTC ====
#define IR_PORT     PORTC
#define IR_PIN_REG  PINC
#define IR_DDR      DDRC

#define IR1_PIN   PC0
#define IR3_PIN   PC2
#define IR8_PIN   PC7
#define IR_FRONT_LEFT_PIN   PC4
#define IR_FRONT_RIGHT_PIN  PC3
// ==== Function Prototypes ====
void IR1_init(void);
void IR3_init(void);
void IR8_init(void);


void IR_FRONT_LEFT_init(void);
void IR_FRONT_RIGHT_init(void);

uint8 IR_FRONT_LEFT_getState(void);
uint8 IR_FRONT_RIGHT_getState(void);

uint8 IR1_getState(void);
uint8 IR3_getState(void);
uint8 IR8_getState(void);

#endif /* HAL_IR_SENSOR_H_ */
