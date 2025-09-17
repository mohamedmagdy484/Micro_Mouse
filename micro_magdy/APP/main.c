#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../HAL/dc_motor.h"
#include "../HAL/ir_sensor.h"
#include <stdbool.h>
#include"../Micro_Mouse_Services/micro_mouse.h"
extern uint8 wsl;
int main(void) {

    IR1_init();  IR3_init();
    IR8_init();
    Motors_init();
    IR_FRONT_LEFT_init();
    IR_FRONT_RIGHT_getState();
    sei();
    DDRB|=(1<<1);
    PORTB&=~(1<<1);

    // اعادة تهيئة المتاهة
    for (uint8 x = 0; x < MAZE_SIZE; x++)
        for (uint8 y = 0; y < MAZE_SIZE; y++)
            maze[x][y] = 0;

    pos_x = 0; pos_y = 0; heading = 0;
    floodfill(); // حساب اولي

    // فتح البداية دايماً (أمان)
    maze[0][0] = 0;

    // حلقة الاستكشاف
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

        _delay_ms(120);

        // شرط التوقف عند الهدف (المراكز 3,3 3,4 4,3 4,4)
        if (((pos_x == 3 || pos_x == 4) && (pos_y == 3 || pos_y == 4)) && maze[pos_x][pos_y] == 0) {
            // ابني طريق الذهاب (لو عايز)
            build_shortest_path(pos_x, pos_y);

            // فتح البداية أمان تاني
            maze[0][0] = 0;

            // Flood fill جديد يكون هدفه نقطة البداية (0,0)
            for (uint8 x = 0; x < MAZE_SIZE; x++)
                for (uint8 y = 0; y < MAZE_SIZE; y++)
                    distance[x][y] = 255;
            distance[0][0] = 0;
            uint16 front = 0, rear = 0;
            queue_x[rear] = 0;
            queue_y[rear] = 0;
            rear++;
            while (front < rear) {
                uint8 x = queue_x[front];
                uint8 y = queue_y[front];
                front++;
                for (uint8 dir = 0; dir < 4; dir++) {
                    int nx = x + dx[dir];
                    int ny = y + dy[dir];
                    if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE &&
                            maze[nx][ny] == 0 && distance[nx][ny] == 255) {
                        distance[nx][ny] = distance[x][y] + 1;
                        queue_x[rear] = nx;
                        queue_y[rear] = ny;
                        rear++;
                    }
                }
            }

            // ابني المسار من مكانك الحالي (الهدف) لنقطة البداية
            build_shortest_path_reverse(pos_x, pos_y);

            // نفذ طريق الرجوع من أقصر مسار فعلي
            replay_path();

            goal_reached = 1;
            break;
        }
    }

    // هنا ممكن تضيف عرض أو حركة أخرى بعد الرجوع (لو عايز)
    while (1) {
        // في انتظار أو أي لوجيك إضافي
    }
}
