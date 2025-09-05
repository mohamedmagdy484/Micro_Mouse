#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../HAL/dc_motor.h"
#include "../HAL/lcd.h"
#include "../HAL/ir_sensor.h"
#include <stdbool.h>
#include"../Micro_Mouse_Services/micro_mouse.h"
#include"../mcal/ext_int0.h"






int main(void) {

	LCD_init();
	IR1_init(); IR2_init(); IR3_init();
	IR6_init(); IR7_init(); IR8_init();
	DC_RIGHT_INIT();
	DC_LEFT_INIT();
	ext_int0_init();
	sei();


	LCD_displayString("  EL-MAZARYTA");
	_delay_ms(600);
	LCD_clearScreen();

	// اعادة تهيئة المتاهة
	for (uint8 x = 0; x < MAZE_SIZE; x++)
		for (uint8 y = 0; y < MAZE_SIZE; y++)
			maze[x][y] = 0;

	pos_x = 0; pos_y = 0; heading = 0;
	floodfill(); // حساب اولي (قد لايكون دقيق قبل اكتشاف جدران)

	// حلقة الاستكشاف (sensing + planning via floodfill)
	while (1) {
		uint8 changed = update_walls();
		if (changed) {
			floodfill(); // إذا تغيرت الجدران — أعِد حساب المسافات
		}

		// حدد الاتجاه التالي باستخدام الفلودفل
		uint8 next_dir = choose_next_direction();

		// لو next_dir = current heading فهذا يعني امشي قدّام
		rotate_to_dir(next_dir);
		// قبل المشي تأكد إن الخلية الامامية ليست جدار (استجابة فورية للسنسور)
		// نحسب الخلية الامامية
		int fx = pos_x + dx[heading];
		int fy = pos_y + dy[heading];
		if (fx >= 0 && fx < MAZE_SIZE && fy >= 0 && fy < MAZE_SIZE && maze[fx][fy] == 0) {
			move_forward_one_cell();
		} else {
			// لو أمامنا حيطه (ربما اكتشفته للتو) حدث الجدران وأعد الـ floodfill ثم استمر
			update_walls();
			floodfill();
		}

		// عرض Debug على LCD
		char buf[17];
		char *hd[4] = {"X+","Y-","X-","Y+"};
		snprintf(buf, 17, "X:%2d Y:%2d %s", pos_x, pos_y, hd[heading]);
		LCD_displayStringRowColumn(1,0,buf);

		_delay_ms(120);

		// شرط التوقف عند الهدف (المراكز 7,7 7,8 8,7 8,8)
		if (((pos_x == 7 || pos_x == 8) && (pos_y == 7 || pos_y == 8)) && maze[pos_x][pos_y] == 0) {
			// بعد الوصول — ابني المسار القصير بحسب floodfill
			build_shortest_path(pos_x, pos_y);
			LCD_displayStringRowColumn(0,0,"      GOAL      ");
			goal_reached = 1;
			break; // اوقف الاستكشاف وابقى منتظر الزر
		}
	}

	// بعد الوصول — ننتظر ضغطة زر للريلاي عبر الـ INT0 (ISR يتعامل مع الأمر)
	while (1) {
		// ممكن تعرض حالة انتظار أو أي لوجيك آخر
		LCD_displayStringRowColumn(0,0,"PRESS REPLAY");
		_delay_ms(500);
	}

}
