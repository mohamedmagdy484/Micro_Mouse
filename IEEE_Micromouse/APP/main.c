#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include "../HAL/dc_motor.h"
#include "../HAL/lcd.h"
#include "../HAL/ir_sensor.h"
#include <stdbool.h>
#include "../Micro_Mouse_Services/micro_mouse.h"
#include "../mcal/ext_int0.h"
#include "../HAL/encoder.h"
#include "../Micro_Mouse_Services/PID.h"

// تهيئة الكنترول (هيكل عالمي)
ControlSystem_t ctrl;

void setup_timer2_20ms()
{
	TCCR2 |= (1 << WGM21); // CTC mode
	TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler = 1024
	OCR2 = 39; // 20ms على 16MHz مع prescaler 1024
	TIMSK |= (1 << OCIE2); // Enable compare interrupt
}

ISR(TIMER2_COMP_vect)
{
	float left_pwm, right_pwm;
	Control_Update(&ctrl, set_velocity, set_angle, &left_pwm, &right_pwm);
	DC_LEFT(left_pwm >= 0 ? CW : A_CW, (uint8)fabs(left_pwm));   // اتجاه وسرعة
	DC_RIGHT(right_pwm >= 0 ? CW : A_CW, (uint8)fabs(right_pwm));
}

int main(void)
{
	// تهيئة PID للسرعة الأمامية
	PID_Init(&ctrl.forwardPID, 0.7f, 0.05f, 0.01f, 0.02f, -100.0f, 100.0f);

	// تهيئة PID للدوران
	PID_Init(&ctrl.rotationPID, 2.0f, 0.07f, 0.04f, 0.02f, -100.0f, 100.0f);

	LCD_init();
	IR1_init(); IR3_init();
	IR8_init();
	Motors_init();
	ext_int0_init();
	ext_int2_init();
	ext_int1_init();
	setup_timer2_20ms();
	sei();

	Motors_init();

	//set_velocity = 0.0f; // اختبار مباشر للحركة
	//set_angle = 0.0f;
	//set_velocity = 0.0f;



	// اعادة تهيئة المتاهة
	for (uint8 x = 0; x < MAZE_SIZE; x++)
		for (uint8 y = 0; y < MAZE_SIZE; y++)
			maze[x][y] = 0;

	pos_x = 0; pos_y = 0; heading = 0;
	floodfill();

	while (1) {
		uint8 changed = update_walls();
		if (changed) {
			floodfill();
		}

		uint8 next_dir = choose_next_direction();
		rotate_to_dir(next_dir);

		int fx = pos_x + dx[heading];
		int fy = pos_y + dy[heading];
		if (fx >= 0 && fx < MAZE_SIZE && fy >= 0 && fy < MAZE_SIZE && maze[fx][fy] == 0) {
			move_forward_one_cell();
		} else {
			update_walls();
			floodfill();
		}



		//_delay_ms(120);

		if (((pos_x == 7 || pos_x == 8) && (pos_y == 7 || pos_y == 8)) && maze[pos_x][pos_y] == 0) {
			build_shortest_path(pos_x, pos_y);
			goal_reached = 1;
			break;
		}
	}

	while (1) {
		_delay_ms(500);
	}
}
