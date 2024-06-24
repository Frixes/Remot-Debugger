#ifndef KOMUNIKASI_REMOT_H
#define KOMUNIKASI_REMOT_H

#include "stm32f1xx_hal.h"

typedef struct
{

	uint64_t ping_fl;
	uint64_t ping_fr;
	uint64_t ping_bl;
	uint64_t ping_br;
	uint64_t ping_ff;
	uint64_t ping_bb;

} handler_ping;

typedef struct
{

	uint64_t x_fl;
	uint64_t x_fr;
	uint64_t x_ml;
	uint64_t x_mr;
	uint64_t x_bl;
	uint64_t x_br;

	uint64_t y_fl;
	uint64_t y_fr;
	uint64_t y_ml;
	uint64_t y_mr;
	uint64_t y_bl;
	uint64_t y_br;

	uint64_t z_fl;
	uint64_t z_fr;
	uint64_t z_ml;
	uint64_t z_mr;
	uint64_t z_bl;
	uint64_t z_br;

} handler_pos_kaki;

typedef struct
{
	uint8_t room;
	uint8_t step;

} handler_room_step;

typedef struct
{

	uint32_t coxa_fr;
	uint32_t femur_fr;
	uint32_t tibia_fr;

	uint32_t coxa_mr;
	uint32_t femur_mr;
	uint32_t tibia_mr;

	uint32_t coxa_br;
	uint32_t femur_br;
	uint32_t tibia_br;

	uint32_t coxa_bl;
	uint32_t femur_bl;
	uint32_t tibia_bl;

	uint32_t coxa_ml;
	uint32_t femur_ml;
	uint32_t tibia_ml;

	uint32_t coxa_fl;
	uint32_t femur_fl;
	uint32_t tibia_fl;

} handler_angle_kaki;

typedef struct
{

	uint64_t roll;
	uint64_t pitch;
	uint64_t yaw;

} handler_gyro;

void komunikasi_remot_init(SPI_HandleTypeDef *spiHandler);
HAL_StatusTypeDef receive_all(handler_ping *ping, handler_pos_kaki *kaki, handler_angle_kaki *angle_kaki, handler_gyro *gyro, handler_room_step *room_step);

// Double
HAL_StatusTypeDef send_ping(double *array_ping);
HAL_StatusTypeDef send_pos_kaki(double *array_pos_kaki);
HAL_StatusTypeDef send_gyro(double *array_gyro);

// uint8_t
HAL_StatusTypeDef send_room_step(uint8_t room, uint8_t step);

// Int
HAL_StatusTypeDef send_angle_kaki(int *array_servo_error);

#endif