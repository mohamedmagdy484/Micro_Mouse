/*
 * micro_mouse.c
 *
 *  Created on: Sep 4, 2025
 *      Author: SIGMA
 */
#include"micro_mouse.h"
#include"../hal/ir_sensor.h"
#include"../hal/lcd.h"
#include"../hal/dc_motor.h"
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
//Variables description

volatile uint8 goal_reached = 0;// dah flag 3lshan y3ml check ano wsl ll goal wla l2a
// خرائط ومصفوفات
uint8 maze[MAZE_SIZE][MAZE_SIZE];// de array ll maze ely hya fe el7ala bta3tna 16 * 16
uint8 distance[MAZE_SIZE][MAZE_SIZE];// de array 3lshan el flood fill hya el bt5zn el msafat m3 kol 5atwa el mouse bymsheha w tbd2a t3ml update
/*
 * ely gay de arrays el mafrod anha bt5zn el amakn ely hya lsa mra7tsh leha
 */
uint8 queue_x[MAZE_SIZE * MAZE_SIZE];
uint8 queue_y[MAZE_SIZE * MAZE_SIZE];
/*                                                    ^
 * dy arrays 3lshan t5lena nmshe f arb3 atgahat <--   |  -->
 *                                                    +
 */
int dx[4] = {1, 0, -1, 0};   // EAST, NORTH, WEST, SOUTH
int dy[4] = {0, -1, 0, 1};

sint16 pos_x = 0, pos_y = 0;// dy el coordinates ely el mouse w2f feha 7alyn

uint8 heading = 0; // 0=E,1=N,2=W,3=S dah 3obara 3n el makan ely el mouse bass 3leh w hyroo7 leeh  x+ x- y+ y-

// المسار النهائي (يُبنى فقط بعد الوصول للـ GOAL)
uint8 path_x[MAX_PATH];// el tareek el nha2y ll maze x
uint8 path_y[MAX_PATH];// el tareek el nha2y ll maze y
uint8 path_dir[MAX_PATH];  // 0=forward,1=right,2=left,3=back // dy bt5zn el 7rakat ely el motor msheha bs
uint16 path_len = 0;// 3dd el 5tawat ely f x & y




/*=======================================================================================
 * -------------------------------- Update Walls ----------------------------------------
 *=======================================================================================
 */

/*
 * update_walls:
 *  - function di bt3ml check lel IR sensors (left, right, front).
 *  - lw fe wall detected bt-update el maze[][] w t7ot feha 1.
 *  - bt-check el bounds 3shan mat7ot4 wall barra el maze.
 *
 * return:
 *  - 1 lw fe update 7sl (wall gded).
 *  - 0 lw mafi4 ay taghyeer.
 */

uint8 update_walls(void) {
    uint8 updated = 0;

    int left_dir = (heading + 3) % 4;
    int right_dir = (heading + 1) % 4;
    int lx = pos_x + dx[left_dir];
    int ly = pos_y + dy[left_dir];
    int rx = pos_x + dx[right_dir];
    int ry = pos_y + dy[right_dir];
    int fx = pos_x + dx[heading];
    int fy = pos_y + dy[heading];

    // اليسار (IR1, IR2)
    if ((IR1_getState() || IR2_getState()) && lx >= 0 && lx < MAZE_SIZE && ly >= 0 && ly < MAZE_SIZE) {
        if (maze[lx][ly] != 1) { maze[lx][ly] = 1; updated = 1; }
    }
    // اليمين (IR7, IR8)
    if ((IR7_getState() || IR8_getState()) && rx >= 0 && rx < MAZE_SIZE && ry >= 0 && ry < MAZE_SIZE) {
        if (maze[rx][ry] != 1) { maze[rx][ry] = 1; updated = 1; }
    }
    // امام (IR3, IR6)
    if ((IR3_getState() || IR6_getState()) && fx >= 0 && fx < MAZE_SIZE && fy >= 0 && fy < MAZE_SIZE) {
        if (maze[fx][fy] != 1) { maze[fx][fy] = 1; updated = 1; }
    }

    return updated;
}





/*=======================================================================================
 * -------------------------------- Flood Fill ------------------------------------------
 *=======================================================================================
 */




void floodfill(void) {
    for (uint8 x = 0; x < MAZE_SIZE; x++)
        for (uint8 y = 0; y < MAZE_SIZE; y++)
            distance[x][y] = 255;

    uint8 center_x[4] = {7,7,8,8};
    uint8 center_y[4] = {7,8,7,8};
    uint16 front = 0, rear = 0;

    for (uint8 i = 0; i < 4; i++) {
        distance[center_x[i]][center_y[i]] = 0;
        queue_x[rear] = center_x[i];
        queue_y[rear] = center_y[i];
        rear++;
    }

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
}



/*=======================================================================================
 * -------------------------------- Choose Next Direction -------------------------------
 *=======================================================================================
 */


uint8 choose_next_direction(void) {
    uint8 best_dir = heading;
    uint8 min_dist = 255;
    for (uint8 dir = 0; dir < 4; dir++) {
        int nx = pos_x + dx[dir];
        int ny = pos_y + dy[dir];
        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE) {
            if (maze[nx][ny] == 0 && distance[nx][ny] < min_dist) {
                min_dist = distance[nx][ny];
                best_dir = dir;
            }
        }
    }

    // إذا لم نجد أي اتجاه صالح
    if (min_dist == 255) {
        LCD_clearScreen();
        LCD_displayString(" SYSTEM ERROR ");
        while(1) {
            // توقف الروبوت نهائيًا
        }
    }

    return best_dir;
}


/*=======================================================================================
 * --------------------------------Rotate to direction-----------------------------------
 *=======================================================================================
 */

void rotate_to_dir(uint8 target_dir) {
    // حساب الفرق (0 = already aligned, 1 = +90deg (right), 3 = -90deg (left), 2 = 180deg)
    uint8 diff = (target_dir + 4 - heading) % 4;
    if (diff == 0) return;

    if (diff == 1) {
        // دور يمين
        DC_RIGHT(CW, 0);
        DC_LEFT(CW, 100);
        LCD_displayStringRowColumn(0,0," YEMEN ");

        _delay_ms(1100); // زمن الدوران: عدّل حسب تجاربك
        heading = (heading + 1) % 4;
    } else if (diff == 3) {
        // دور شمال (left)
        DC_RIGHT(CW, 100);
        DC_LEFT(CW, 0);
        LCD_displayStringRowColumn(0,0," SHMAL ");

        _delay_ms(1100);
        heading = (heading + 3) % 4;
    } else { // diff == 2
        // دوران 180
        DC_RIGHT(CW, 100);
        DC_LEFT(A_CW, 100);
        LCD_displayStringRowColumn(0,0," BLF ");

        _delay_ms(2000);
        heading = (heading + 2) % 4;
    }
    // اوقف الموتور بعد دوران
    DC_RIGHT(CW, 0);
    DC_LEFT(CW, 0);
    _delay_ms(200);
}
/*=======================================================================================
 * -------------------------------- Move 1 Cell Forward ---------------------------------
 *=======================================================================================
 */

void move_forward_one_cell(void) {
    // محركان للأمام لزمن محدد (عدّل _delay_ms إذا احتجت)
    DC_RIGHT(CW, 100);
    DC_LEFT(CW, 100);
    _delay_ms(2000); // زمن عبور خلية واحدة (اضبطه عملياً)
    DC_RIGHT(CW, 0);
    DC_LEFT(CW, 0);
    _delay_ms(200);

    // حدّث الاحداثيات
    pos_x += dx[heading];
    pos_y += dy[heading];
}
/*=======================================================================================
 * -------------------------------- Short Path ------------------------------------------
 *=======================================================================================
 */
void build_shortest_path(int goal_x, int goal_y) {
    path_len = 0;
    int cx = 0, cy = 0; // البداية
    int gx = goal_x, gy = goal_y;

    // Safety
    if (distance[gx][gy] == 255) return;

    int temp_path_x[MAX_PATH];
    int temp_path_y[MAX_PATH];
    int temp_path_len = 0;

    // نبدأ من البداية cx,cy = 0,0
    int x = cx, y = cy;
    while (!(x == gx && y == gy)) {
        uint8 next_dir = 0xFF;
        for (uint8 dir = 0; dir < 4; dir++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE) {
                if (distance[nx][ny] < distance[x][y]) {
                    next_dir = dir;
                    break;
                }
            }
        }
        if (next_dir == 0xFF) break; // لا يوجد طريق

        // خزّن النقطة التالية في المسار
        temp_path_x[temp_path_len] = x + dx[next_dir];
        temp_path_y[temp_path_len] = y + dy[next_dir];
        path_dir[temp_path_len] = next_dir;
        temp_path_len++;

        // انتقل للنقطة التالية
        x += dx[next_dir];
        y += dy[next_dir];
    }

    // انسخ المؤقت للمصفوفة النهائية
    path_len = temp_path_len;
    for (int i = 0; i < path_len; i++) {
        path_x[i] = temp_path_x[i];
        path_y[i] = temp_path_y[i];
    }
}
/*=======================================================================================
 * -------------------------------- Replay Path -----------------------------------------
 *=======================================================================================
 */
void replay_path(void) {
    LCD_clearScreen();
    LCD_displayString("REPLAYING...");
    _delay_ms(300);

    cli(); // أغلق الإنترابت

    // أعد وضع البداية
    pos_x = 0;
    pos_y = 0;
    heading = 0;

    for (uint16_t i = 0; i < path_len; i++) {
        uint8 dir = path_dir[i];
        rotate_to_dir(dir);
        move_forward_one_cell();

        char buf[17];
        snprintf(buf, 17, "X:%2d Y:%2d D:%d", pos_x, pos_y, dir);
        LCD_displayStringRowColumn(1, 0, buf);
        _delay_ms(80);
    }

    DC_RIGHT(CW,0);
    DC_LEFT(CW,0);

    LCD_displayStringRowColumn(0,0," PATH DONE ");
    _delay_ms(500);

    sei(); // إعادة تفعيل الإنترابت
}
ISR(INT0_vect) {
    if (goal_reached) {
        replay_path();
    }
}
