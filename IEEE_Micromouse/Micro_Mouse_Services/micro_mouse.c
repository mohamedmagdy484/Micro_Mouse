/*
 * micro_mouse.c
 *
 *  Created on: Sep 4, 2025
 *      Author: SIGMA
 */

#include "micro_mouse.h"
#include "../hal/ir_sensor.h"
#include "../hal/lcd.h"
#include "../hal/dc_motor.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdio.h>
#include "../HAL/encoder.h"
#include "control.h"

// متغيرات النظام
volatile uint8 goal_reached = 0;
uint8 maze[MAZE_SIZE][MAZE_SIZE];
uint8 distance[MAZE_SIZE][MAZE_SIZE];
uint8 queue_x[MAZE_SIZE * MAZE_SIZE];
uint8 queue_y[MAZE_SIZE * MAZE_SIZE];
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, -1, 0, 1};
sint16 pos_x = 0, pos_y = 0;
uint8 heading = 0;
uint8 path_x[MAX_PATH];
uint8 path_y[MAX_PATH];
uint8 path_dir[MAX_PATH];
uint16 path_len = 0;

// متغيرات الحركة والتحكم
volatile float set_velocity = 0.0f;
volatile float set_angle = 0.0f;
ControlSystem_t ctrl;

// تعريفات ثابتة
#define PPR 200
#define Ts 0.02f
#define WHEEL_DIAMETER_MM 60.0f
#define WHEEL_BASE_MM 100.0f
#define CELL_SIZE_MM 180.0f
#define DIST_PER_COUNT_MM ((M_PI * WHEEL_DIAMETER_MM) / (float)PPR)

// ================== Update Walls ==================
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

    if ((IR1_getState()) && lx >= 0 && lx < MAZE_SIZE && ly >= 0 && ly < MAZE_SIZE)
        if (maze[lx][ly] != 1) { maze[lx][ly] = 1; updated = 1; }
    if ((IR8_getState()) && rx >= 0 && rx < MAZE_SIZE && ry >= 0 && ry < MAZE_SIZE)
        if (maze[rx][ry] != 1) { maze[rx][ry] = 1; updated = 1; }
    if ((IR3_getState()) && fx >= 0 && fx < MAZE_SIZE && fy >= 0 && fy < MAZE_SIZE)
        if (maze[fx][fy] != 1) { maze[fx][fy] = 1; updated = 1; }

    return updated;
}

// ================== Flood Fill ==================
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

// ================== Choose Next Direction ==================
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
    return best_dir;
}

// ================== Rotate To Direction ==================
void rotate_to_dir(uint8 target_dir)
{
    uint8 diff = (target_dir + 4 - heading) % 4;
    if (diff == 0) return;

    float target_angle = 0.0f;
    if (diff == 1)      target_angle = 90.0f;
    else if (diff == 3) target_angle = -90.0f;
    else                target_angle = 180.0f;

    set_velocity = 0.0f;
    set_angle = target_angle;

    long start_count1 = encoder_count1;
    long start_count2 = encoder_count2;
    float current_angle = 0.0f;
    while (fabs(current_angle) < fabs(target_angle)) {
        long delta_left = encoder_count1 - start_count1;
        long delta_right = encoder_count2 - start_count2;
        current_angle = ((float)(delta_right - delta_left) / PPR) * (WHEEL_DIAMETER_MM / WHEEL_BASE_MM) * 360.0f;
        _delay_ms(5);
    }

    set_angle = 0.0f;
    heading = target_dir;
}

// ================== Move Forward One Cell ==================
void move_forward_one_cell(void)
{
    set_velocity = 100.0f;
    set_angle = 0.0f;

    long start_count1 = encoder_count1;
    long start_count2 = encoder_count2;
    float current_distance = 0.0f;

    while (current_distance < CELL_SIZE_MM) {
        long avg_counts = ((encoder_count1 - start_count1) + (encoder_count2 - start_count2)) / 2;
        current_distance = (float)avg_counts * DIST_PER_COUNT_MM;
        _delay_ms(5);
    }
    set_velocity = 0.0f;

    pos_x += dx[heading];
    pos_y += dy[heading];
}

// ================== Build Shortest Path ==================
void build_shortest_path(int goal_x, int goal_y) {
    path_len = 0;
    int cx = 0, cy = 0;
    int gx = goal_x, gy = goal_y;
    if (distance[gx][gy] == 255) return;

    int temp_path_x[MAX_PATH];
    int temp_path_y[MAX_PATH];
    int temp_path_len = 0;

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
        if (next_dir == 0xFF) break;
        temp_path_x[temp_path_len] = x + dx[next_dir];
        temp_path_y[temp_path_len] = y + dy[next_dir];
        path_dir[temp_path_len] = next_dir;
        temp_path_len++;
        x += dx[next_dir];
        y += dy[next_dir];
    }
    path_len = temp_path_len;
    for (int i = 0; i < path_len; i++) {
        path_x[i] = temp_path_x[i];
        path_y[i] = temp_path_y[i];
    }
}

// ================== Replay Path ==================
void replay_path(void) {
    LCD_clearScreen();
    LCD_displayString("REPLAYING...");
    _delay_ms(300);

    cli();

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

    sei();
}

// ================== ISR ==================
ISR(INT0_vect) {
    if (goal_reached) {
        replay_path();
    }
}
