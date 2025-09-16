#ifndef HAL_IR_SENSOR_H_
#define HAL_IR_SENSOR_H_

#include <avr/io.h>
#include "../common/std_types.h"

// ==== Configuration Macros for 8 IR Sensors on PORTC ====
#define IR_PORT     PORTC
#define IR_PIN_REG  PINC
#define IR_DDR      DDRC

#define IR1_PIN   PC0
#define IR2_PIN   PC1
#define IR3_PIN   PC2
#define IR6_PIN   PC5
#define IR7_PIN   PC6
#define IR8_PIN   PC7

// ==== Function Prototypes ====
void IR1_init(void);
void IR3_init(void);
void IR8_init(void);

uint8 IR1_getState(void);
uint8 IR3_getState(void);
uint8 IR8_getState(void);

#endif /* HAL_IR_SENSOR_H_ */
