/*
 * ball_controll.c
 *
 *  Created on: Apr 15, 2023
 *      Author: chedoska
 */
#include "ball_control.h"

// Inicjalizacja srtruktury do zarządzania pozycją i fizyką piłki
void ball_ctrl_init(Ball_control_data *ball_data, uint16_t X_start_pos, uint16_t Y_start_pos)
{
	ball_ctrl_restetDrift(ball_data);
	ball_data->X_screen_pos = X_start_pos;
	ball_data->Y_screen_pos = Y_start_pos;
	ball_data->X_screen_speed = 0;
	ball_data->Y_screen_speed = 0;
}

// Usuwanie dryftu powstałęgo przy całkowaniu pedkości obrotowych
void ball_ctrl_restetDrift(Ball_control_data *ball_data)
{
	ball_data->ctrlX_angle = 0;
	ball_data->ctrlY_angle = 0;
	ball_data->ctrlY_angle = 0;
}

// Aktualizacja i obliczanie aktualnego wychylenia płytki poprzez całkowanie wartości z żyroskopu
/*
 * int delta_time_ms - czas jaki upłyną od poprzedniego pomiaru wyrażony w milisekundach
 * L3GD20 *L3GD20_data - wskaźnik do struktury z danymi odczytu żyroskoou
 * Ball_control_data *ball_data - wskaźnik do struktury zarządzającej pozycją piłki.
 */
void ball_update_ctrl_angles(Ball_control_data *ball_data, L3GD20 *L3GD20_data, int delta_time_ms)
{
	float delta_time = delta_time_ms / 1000.0;
	ball_data->ctrlX_angle += L3GD20_data->X_val * delta_time;
	ball_data->ctrlY_angle += L3GD20_data->Y_val * delta_time;
	ball_data->ctrlZ_angle += L3GD20_data->Z_val * delta_time;
}

// Aktualizacja i obliczenie pozycji piłki na ekranie wykorzystując obliczone wartości wychylenia ekjranu
/*
 * Ball_control_data *ball_data - wskaźnik do struktury zarządzającej pozycją piłki.
 * int delta_time_ms - czas jaki upłyną od poprzedniego pomiaru wyrażony w milisekundach
 */
void ball_update_pos(Ball_control_data *ball_data, int delta_time_ms)
{
	float delta_time = delta_time_ms / 1000.0;
	float X_radAngle = (ball_data->ctrlX_angle * PI_CONST / 180.0);				// Wyrażenie orientacji w radianch
	float Y_radAngle = (ball_data->ctrlY_angle * PI_CONST / 180.0);				// dla osi X i Y

	ball_data->X_screen_speed += sinf(X_radAngle) * GRAVITY_CONST * delta_time;	// całkowanie przyśpieszenia by otrzymać
	ball_data->Y_screen_speed += sinf(Y_radAngle) * GRAVITY_CONST * delta_time;	// prędkośc chwilową piłki

	if(ball_data->X_screen_speed > 0)											// Uwzględnienie oporu siły tarcia
		ball_data->X_screen_speed -= FRICTION_CONST * delta_time;				// W przeciwnym kierunku do prędkosci
	else
		ball_data->X_screen_speed += FRICTION_CONST * delta_time;

	if(ball_data->Y_screen_speed > 0)
			ball_data->Y_screen_speed -= FRICTION_CONST * delta_time;
		else
			ball_data->Y_screen_speed += FRICTION_CONST * delta_time;

	ball_data->X_screen_pos += (ball_data->X_screen_speed * delta_time);		// Całkowanie prędkosći chwilowej by otrzymać
	ball_data->Y_screen_pos += (ball_data->Y_screen_speed * delta_time);		// aktualną pozycję piłki na ekranie
}

// Sprawdzenien wystąpienia kolizji pi.ki z wirtualną granicą i obliczenie reakcji
/*
 * Ball_control_data *ball_data - wskaźnik do struktury zarządzającej pozycją piłki.
 * int maxX - max. dopuszczalna wartość dla współrzędnej X
 * int maxY - max. dopuszczalna wartość dla współrzędnej Y
 * int minX - min. dopuszczalna wartość dla współrzędnej X
 * int minY - min. dopuszczalna wartość dla współrzędnej Y
 */
WallHit ball_handle_collision(Ball_control_data *ball_data,
						   int maxX, int minX, int maxY, int minY)
{
	static CollisionFilterStatus collisionFilterFlag_Y = BALL_COLLISIONS_ALOWED;
	static CollisionFilterStatus collisionFilterFlag_X = BALL_COLLISIONS_ALOWED;

	if	(ball_data->X_screen_pos > minX + COLLISION_RESTART_AREA_OFFSET &&
	 	 ball_data->X_screen_pos < maxX - COLLISION_RESTART_AREA_OFFSET)
	{
		collisionFilterFlag_X = BALL_COLLISIONS_ALOWED;
	}

	if	(ball_data->Y_screen_pos > minY + COLLISION_RESTART_AREA_OFFSET &&
		 ball_data->Y_screen_pos < maxY - COLLISION_RESTART_AREA_OFFSET)
	{
		collisionFilterFlag_Y = BALL_COLLISIONS_ALOWED;
	}

	WallHit wallHitFlag = BALL_NO_COLLISION;

	// Spradzenie kontaktu ze ścianami horyzontalnie wzdłuż osi X
	if(ball_data->X_screen_pos > maxX || ball_data->X_screen_pos < minX)
	{
		// Zmiana kierunku prędkości piłki po odbiciu
		ball_data->X_screen_speed = -ball_data->X_screen_speed * WALL_ENERGY_LOSS_CONST;
		// Cofnięcie piłki do obrębu ekranu
		ball_data->X_screen_pos = ball_data->X_screen_pos > maxX ? maxX : ball_data->X_screen_pos;
		ball_data->X_screen_pos = ball_data->X_screen_pos < minX ? minX : ball_data->X_screen_pos;

		if(collisionFilterFlag_X == BALL_COLLISIONS_ALOWED){
			wallHitFlag = BALL_COLLISION_DETECTED;
			collisionFilterFlag_X = BALL_COLLISIONS_BLOCKED;
		}
	}

	// Spradzenie kontaktu ze ścianami horyzontalnie wzdłuż osi Y
	if(ball_data->Y_screen_pos > maxY || ball_data->Y_screen_pos < minY)
	{
		// Zmiana kierunku prędkości piłki po odbiciu
		ball_data->Y_screen_speed = -ball_data->Y_screen_speed * WALL_ENERGY_LOSS_CONST;
		// Cofnięcie piłki do obrębu ekranu
		ball_data->Y_screen_pos = ball_data->Y_screen_pos > maxY ? maxY : ball_data->Y_screen_pos;
		ball_data->Y_screen_pos = ball_data->Y_screen_pos < minY ? minY : ball_data->Y_screen_pos;

		if(collisionFilterFlag_Y == BALL_COLLISIONS_ALOWED){
			wallHitFlag = BALL_COLLISION_DETECTED;
			collisionFilterFlag_Y = BALL_COLLISIONS_BLOCKED;
		}
	}
	return wallHitFlag;
}






