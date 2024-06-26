/*
 * ball_control.h
 *
 *  Created on: Apr 15, 2023
 *      Author: chedoska
 */

#ifndef INC_BALL_CONTROL_H_
#define INC_BALL_CONTROL_H_

#include "gyroscope.h"
#include "math.h"

#define PI_CONST 3.1415926
#define GRAVITY_CONST 800//450
#define FRICTION_CONST 50
#define WALL_ENERGY_LOSS_CONST 0.65
#define COLLISION_RESTART_AREA_OFFSET 15

struct Ball_control_data
{
	float ctrlX_angle;
	float ctrlY_angle;
	float ctrlZ_angle;
	float X_screen_pos;
	float Y_screen_pos;
	float X_screen_speed;
	float Y_screen_speed;
} typedef Ball_control_data;

typedef enum WallHit{BALL_COLLISION_DETECTED, BALL_NO_COLLISION} WallHit;
typedef enum CollisionFilterStatus{BALL_COLLISIONS_ALOWED, BALL_COLLISIONS_BLOCKED} CollisionFilterStatus;

void ball_ctrl_init(Ball_control_data *ball_data, uint16_t X_start_pos, uint16_t Y_start_pos);
void ball_ctrl_restetDrift(Ball_control_data *ball_data);
void ball_update_ctrl_angles(Ball_control_data *ball_data, L3GD20 *L3GD20_data, int delta_time_ms);
void ball_update_pos(Ball_control_data *ball_data, int delta_time_ms);
WallHit ball_handle_collision(Ball_control_data *ball_data,
						   int maxX, int minX, int maxY, int minY);

#endif /* INC_BALL_CONTROL_H_ */
