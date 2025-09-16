#ifndef MICRO_MOUSE_SERVICES_MICRO_MOUSE_H_
#define MICRO_MOUSE_SERVICES_MICRO_MOUSE_H_

#include "../common/std_types.h"
#include <avr/io.h>
#include "control.h"
#include "../HAL/encoder.h"

#define MAZE_SIZE 16
#define MAX_PATH (MAZE_SIZE*MAZE_SIZE)
#define REPLAY_BUTTON_PIN PD2

extern volatile uint8 goal_reached;
extern uint8 maze[MAZE_SIZE][MAZE_SIZE];
extern uint8 distance[MAZE_SIZE][MAZE_SIZE];
extern uint8 queue_x[MAZE_SIZE * MAZE_SIZE];
extern uint8 queue_y[MAZE_SIZE * MAZE_SIZE];
extern int dx[4];
extern int dy[4];

extern sint16 pos_x, pos_y;
extern uint8 heading;

extern uint8 path_x[MAX_PATH];
extern uint8 path_y[MAX_PATH];
extern uint8 path_dir[MAX_PATH];
extern uint16 path_len;

extern volatile float set_velocity;
extern volatile float set_angle;

extern ControlSystem_t ctrl;

// Functions
uint8 update_walls(void);
void floodfill(void);
uint8 choose_next_direction(void);
void rotate_to_dir(uint8 target_dir);
void move_forward_one_cell(void);
void build_shortest_path(int goal_x, int goal_y);
void replay_path(void);

#endif /* MICRO_MOUSE_SERVICES_MICRO_MOUSE_H_ */
