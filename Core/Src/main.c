/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Komunikasi_remot.h"
#include "ee.h"
#include "i2c-lcd.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MODE_EEPROM 0
#define MODE_REALTIME 1

// #define ADD_EEPROM 0x0801F804
#define ADD_EEPROM_2 0x0801F400
#define ADD_EEPROM_3 0x0801FC00

// Define Port dan Pin Button
#define PORT_BUTTON_PLUS GPIOA
#define PIN_BUTTON_PLUS GPIO_PIN_1
#define PORT_BUTTON_MODE GPIOA
#define PIN_BUTTON_MODE GPIO_PIN_0
#define PORT_BUTTON_CHANGE_CATEGORY GPIOA
#define PIN_BUTTON_CHANGE_CATEGORY GPIO_PIN_2
#define PORT_BUTTON_WRITE_EE GPIOA
#define PIN_BUTTON_WRITE_EE GPIO_PIN_3

HAL_StatusTypeDef status;

// Inisiasi Page
int kategori = 1, page_sensor = 1, page_pos_kaki = 1, page_angle_kaki = 1, mode = MODE_REALTIME;

int i = 0;

handler_angle_kaki angle_kaki;
handler_pos_kaki pos_kaki;
handler_ping ping;
handler_room_step room_step;
handler_gyro gyro;

// uint32_t angle_saved_array[18];

uint8_t angle_saved_array_8bit[72];
uint8_t jarak_gyro_saved_8bit[72];
uint32_t jarak_gyro_array[18];

int count=0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void writeToEEPROM()
{

	uint32_t angle_kaki_array[18];

	angle_kaki_array[0] = angle_kaki.coxa_fr;
	angle_kaki_array[1] = angle_kaki.femur_fr;
	angle_kaki_array[2] = angle_kaki.tibia_fr;

	angle_kaki_array[3] = angle_kaki.coxa_mr;
	angle_kaki_array[4] = angle_kaki.femur_mr;
	angle_kaki_array[5] = angle_kaki.tibia_mr;

	angle_kaki_array[6] = angle_kaki.coxa_br;
	angle_kaki_array[7] = angle_kaki.femur_br;
	angle_kaki_array[8] = angle_kaki.tibia_br;

	angle_kaki_array[9] = angle_kaki.coxa_bl;
	angle_kaki_array[10] = angle_kaki.femur_bl;
	angle_kaki_array[11] = angle_kaki.tibia_bl;

	angle_kaki_array[12] = angle_kaki.coxa_ml;
	angle_kaki_array[13] = angle_kaki.femur_ml;
	angle_kaki_array[14] = angle_kaki.tibia_ml;

	angle_kaki_array[15] = angle_kaki.coxa_fl;
	angle_kaki_array[16] = angle_kaki.femur_fl;
	angle_kaki_array[17] = angle_kaki.tibia_fl;

	jarak_gyro_array[0] = (uint64_t)ping.ping_bb;
	jarak_gyro_array[1] = ((uint64_t)ping.ping_bb >> 32) & 0xffffffff;
	jarak_gyro_array[2] = ping.ping_bl & 0xffffffff;
	jarak_gyro_array[3] = (ping.ping_bl >> 32) & 0xffffffff;
	jarak_gyro_array[4] = ping.ping_br & 0xffffffff;
	jarak_gyro_array[5] = (ping.ping_br >> 32) & 0xffffffff;
	jarak_gyro_array[6] = ping.ping_ff & 0xffffffff;
	jarak_gyro_array[7] = (ping.ping_ff >> 32) & 0xffffffff;
	jarak_gyro_array[8] = ping.ping_fl & 0xffffffff;
	jarak_gyro_array[9] = (ping.ping_fl >> 32) & 0xffffffff;
	jarak_gyro_array[10] = ping.ping_fr & 0xffffffff;
	jarak_gyro_array[11] = (ping.ping_fr >> 32) & 0xffffffff;

	jarak_gyro_array[12] = gyro.pitch & 0xffffffff;
	jarak_gyro_array[13] = (gyro.pitch >> 32) & 0xffffffff;
	jarak_gyro_array[14] = gyro.roll & 0xffffffff;
	jarak_gyro_array[15] = (gyro.roll >> 32) & 0xffffffff;
	jarak_gyro_array[16] = gyro.yaw & 0xffffffff;
	jarak_gyro_array[17] = (gyro.yaw >> 32) & 0xffffffff;

	Flash_Write_Data(ADD_EEPROM_2, (uint32_t *)jarak_gyro_array, 18);

	Flash_Write_Data(ADD_EEPROM_3, (uint32_t *)angle_kaki_array, 18);
}

void readFromEE()
{
	//	Flash_Read_Data(ADD_EEPROM_3, angle_saved_array, 19);
	read_flash_memory(ADD_EEPROM_3, angle_saved_array_8bit, 72);
	read_flash_memory(ADD_EEPROM_2, jarak_gyro_saved_8bit, 72);
}

void displayDataOnLCD()
{

	if (HAL_GPIO_ReadPin(PORT_BUTTON_PLUS, PIN_BUTTON_PLUS) == GPIO_PIN_RESET)
	{ // tombol page +
		switch (kategori)
		{
		case 1:
			page_sensor++;
			if (page_sensor > 2)
			{
				page_sensor = 1;
			}
			break;

		case 2:
			page_pos_kaki++;
			if (page_pos_kaki > 6)
			{
				page_pos_kaki = 1;
			}
			break;

		case 4:
			page_angle_kaki++;
			if (page_angle_kaki > 6)
			{
				page_angle_kaki = 1;
			}
			break;
		}

		HAL_Delay(250);
		lcd_clear();
	}

	else if (HAL_GPIO_ReadPin(PORT_BUTTON_MODE, PIN_BUTTON_MODE) == GPIO_PIN_RESET)
	{ // tombol page -
		lcd_clear();
		if (mode == MODE_REALTIME)
		{
			mode = MODE_EEPROM;
			lcd_put_cur(0, 0);
			lcd_send_string("EEPROM");
		}
		else
		{
			mode = MODE_REALTIME;
			lcd_put_cur(0, 0);
			lcd_send_string("REALTIME");
		}
		HAL_Delay(675);
		lcd_clear();
	}

	else if (HAL_GPIO_ReadPin(PORT_BUTTON_CHANGE_CATEGORY, PIN_BUTTON_CHANGE_CATEGORY) == GPIO_PIN_RESET)
	{
		kategori++;
		if (kategori > 5)
		{
			kategori = 1;
		}
		HAL_Delay(250);
		lcd_clear();
	}

	else if (HAL_GPIO_ReadPin(PORT_BUTTON_WRITE_EE, PIN_BUTTON_WRITE_EE) == GPIO_PIN_RESET)
	{
		writeToEEPROM();
		lcd_clear();
		lcd_put_cur(0, 0);
		lcd_send_string("Data Saved");
		HAL_Delay(500);
		lcd_clear();
		readFromEE();
	}

	switch (kategori)
	{
	case 1:
		switch (page_sensor)
		{
		case 1:
			if (mode == MODE_EEPROM)
			{
				// MEMORY
				lcd_put_cur(0, 0);
				lcd_send_string("FL:");
				char ping_fl_str[15];
				uint64_t fl_saved = ((uint64_t)jarak_gyro_saved_8bit[39] << 56) | ((uint64_t)jarak_gyro_saved_8bit[38] << 48) | ((uint64_t)jarak_gyro_saved_8bit[37] << 40) | ((uint64_t)jarak_gyro_saved_8bit[36] << 32) | (jarak_gyro_saved_8bit[35] << 24) | (jarak_gyro_saved_8bit[34] << 16) | (jarak_gyro_saved_8bit[33] << 8) | jarak_gyro_saved_8bit[32];
				sprintf(ping_fl_str, "%.1f", *((double *)&fl_saved));
				lcd_send_string(ping_fl_str);
				lcd_put_cur(1, 0);
				lcd_send_string("BL:");
				char ping_bl_str[15];
				uint64_t bl_saved = ((uint64_t)jarak_gyro_saved_8bit[15] << 56) | ((uint64_t)jarak_gyro_saved_8bit[14] << 48) | ((uint64_t)jarak_gyro_saved_8bit[13] << 40) | ((uint64_t)jarak_gyro_saved_8bit[12] << 32) | (jarak_gyro_saved_8bit[11] << 24) | (jarak_gyro_saved_8bit[10] << 16) | (jarak_gyro_saved_8bit[9] << 8) | jarak_gyro_saved_8bit[8];
				sprintf(ping_bl_str, "%.1f", *((double *)&bl_saved));
				lcd_send_string(ping_bl_str);
				lcd_put_cur(0, 8);
				lcd_send_string("FR:");
				char ping_fr_str[15];
				uint64_t fr_saved = ((uint64_t)jarak_gyro_saved_8bit[47] << 56) | ((uint64_t)jarak_gyro_saved_8bit[46] << 48) | ((uint64_t)jarak_gyro_saved_8bit[45] << 40) | ((uint64_t)jarak_gyro_saved_8bit[44] << 32) | (jarak_gyro_saved_8bit[43] << 24) | (jarak_gyro_saved_8bit[42] << 16) | (jarak_gyro_saved_8bit[41] << 8) | jarak_gyro_saved_8bit[40];
				sprintf(ping_fr_str, "%.1f", *((double *)&fr_saved));
				lcd_send_string(ping_fr_str);
				lcd_put_cur(1, 8);
				lcd_send_string("BR:");
				char ping_br_str[15];
				uint64_t br_saved = ((uint64_t)jarak_gyro_saved_8bit[23] << 56) | ((uint64_t)jarak_gyro_saved_8bit[22] << 48) | ((uint64_t)jarak_gyro_saved_8bit[21] << 40) | ((uint64_t)jarak_gyro_saved_8bit[20] << 32) | (jarak_gyro_saved_8bit[19] << 24) | (jarak_gyro_saved_8bit[18] << 16) | (jarak_gyro_saved_8bit[17] << 8) | jarak_gyro_saved_8bit[16];
				sprintf(ping_br_str, "%.1f", *((double *)&br_saved));
				lcd_send_string(ping_br_str);
			}
			else
			{
				lcd_put_cur(0, 0);
				lcd_send_string("FL:");
				char ping_fl_str[15];
				sprintf(ping_fl_str, "%.1f", *((double *)&ping.ping_fl));
				lcd_send_string(ping_fl_str);
				lcd_put_cur(1, 0);
				lcd_send_string("BL:");
				char ping_bl_str[15];
				sprintf(ping_bl_str, "%.1f", *((double *)&ping.ping_bl));
				lcd_send_string(ping_bl_str);
				lcd_put_cur(0, 8);
				lcd_send_string("FR:");
				char ping_fr_str[15];
				sprintf(ping_fr_str, "%.1f", *((double *)&ping.ping_fr));
				lcd_send_string(ping_fr_str);
				lcd_put_cur(1, 8);
				lcd_send_string("BR:");
				char ping_br_str[15];
				sprintf(ping_br_str, "%.1f", *((double *)&ping.ping_br));
				lcd_send_string(ping_br_str);
				count++;
			}
			break;

		case 2:
			if (mode == MODE_EEPROM)
			{
				lcd_put_cur(0, 0);
				lcd_send_string("FF:");
				char ping_ff_str[15];
				uint64_t ff_saved = ((uint64_t)jarak_gyro_saved_8bit[31] << 56) | ((uint64_t)jarak_gyro_saved_8bit[30] << 48) | ((uint64_t)jarak_gyro_saved_8bit[29] << 40) | ((uint64_t)jarak_gyro_saved_8bit[28] << 32) | (jarak_gyro_saved_8bit[27] << 24) | (jarak_gyro_saved_8bit[26] << 16) | (jarak_gyro_saved_8bit[25] << 8) | jarak_gyro_saved_8bit[24];
				sprintf(ping_ff_str, "%.1f", *((double *)&ff_saved));
				lcd_send_string(ping_ff_str);
				lcd_put_cur(1, 0);
				lcd_send_string("BB:");
				char ping_bb_str[15];
				uint64_t bb_saved = ((uint64_t)jarak_gyro_saved_8bit[7] << 56) | ((uint64_t)jarak_gyro_saved_8bit[6] << 48) | ((uint64_t)jarak_gyro_saved_8bit[5] << 40) | ((uint64_t)jarak_gyro_saved_8bit[4] << 32) | (jarak_gyro_saved_8bit[3] << 24) | (jarak_gyro_saved_8bit[2] << 16) | (jarak_gyro_saved_8bit[1] << 8) | jarak_gyro_saved_8bit[0];
				sprintf(ping_bb_str, "%.1f", *((double *)&bb_saved));
				lcd_send_string(ping_bb_str);
			}
			else
			{
				lcd_put_cur(0, 0);
				lcd_send_string("FF:");
				char ping_ff_str[15];
				sprintf(ping_ff_str, "%.1f", *((double *)&ping.ping_ff));
				lcd_send_string(ping_ff_str);
				lcd_put_cur(1, 0);
				lcd_send_string("BB:");
				char ping_bb_str[15];
				sprintf(ping_bb_str, "%.1f", *((double *)&ping.ping_bb));
				lcd_send_string(ping_bb_str);
			}

			break;
		}
		break;

	case 2:
		switch (page_pos_kaki)
		{
		case 1:
			lcd_put_cur(0, 0);
			lcd_send_string("FR");
			lcd_put_cur(0, 8);
			lcd_send_string("X:");
			char kaki_fr_x_str[20];
			sprintf(kaki_fr_x_str, "%.1f", *((double *)&pos_kaki.x_fr));
			lcd_send_string(kaki_fr_x_str);
			lcd_put_cur(1, 0);
			lcd_send_string("Y:");
			char kaki_fr_y_str[20];
			sprintf(kaki_fr_y_str, "%.1f", *((double *)&pos_kaki.y_fr));
			lcd_send_string(kaki_fr_y_str);
			lcd_put_cur(1, 8);
			lcd_send_string("Z:");
			char kaki_fr_z_str[20];
			sprintf(kaki_fr_z_str, "%.1f", *((double *)&pos_kaki.z_fr));
			lcd_send_string(kaki_fr_z_str);
			break;
		case 2:
			lcd_put_cur(0, 0);
			lcd_send_string("MR");
			lcd_put_cur(0, 8);
			lcd_send_string("X:");
			char kaki_mr_x_str[20];
			sprintf(kaki_mr_x_str, "%.1f", *((double *)&pos_kaki.x_mr));
			lcd_send_string(kaki_mr_x_str);
			lcd_put_cur(1, 0);
			lcd_send_string("Y:");
			char kaki_mr_y_str[20];
			sprintf(kaki_mr_y_str, "%.1f", *((double *)&pos_kaki.y_mr));
			lcd_send_string(kaki_mr_y_str);
			lcd_put_cur(1, 8);
			lcd_send_string("Z:");
			char kaki_mr_z_str[20];
			sprintf(kaki_mr_z_str, "%.1f", *((double *)&pos_kaki.z_mr));
			lcd_send_string(kaki_mr_z_str);
			break;
		case 3:
			lcd_put_cur(0, 0);
			lcd_send_string("BR");
			lcd_put_cur(0, 8);
			lcd_send_string("X:");
			char kaki_br_x_str[20];
			sprintf(kaki_br_x_str, "%.1f", *((double *)&pos_kaki.x_br));
			lcd_send_string(kaki_br_x_str);
			lcd_put_cur(1, 0);
			lcd_send_string("Y:");
			char kaki_br_y_str[20];
			sprintf(kaki_br_y_str, "%.1f", *((double *)&pos_kaki.y_br));
			lcd_send_string(kaki_br_y_str);
			lcd_put_cur(1, 8);
			lcd_send_string("Z:");
			char kaki_br_z_str[20];
			sprintf(kaki_br_z_str, "%.1f", *((double *)&pos_kaki.z_br));
			lcd_send_string(kaki_br_z_str);
			break;
		case 4:
			lcd_put_cur(0, 0);
			lcd_send_string("BL");
			lcd_put_cur(0, 8);
			lcd_send_string("X:");
			char kaki_bl_x_str[20];
			sprintf(kaki_bl_x_str, "%.1f", *((double *)&pos_kaki.x_bl));
			lcd_send_string(kaki_bl_x_str);
			lcd_put_cur(1, 0);
			lcd_send_string("Y:");
			char kaki_bl_y_str[20];
			sprintf(kaki_bl_y_str, "%.1f", *((double *)&pos_kaki.y_bl));
			lcd_send_string(kaki_bl_y_str);
			lcd_put_cur(1, 8);
			lcd_send_string("Z:");
			char kaki_bl_z_str[20];
			sprintf(kaki_bl_z_str, "%.1f", *((double *)&pos_kaki.z_bl));
			lcd_send_string(kaki_bl_z_str);
			break;
		case 5:
			lcd_put_cur(0, 0);
			lcd_send_string("ML");
			lcd_put_cur(0, 8);
			lcd_send_string("X:");
			char kaki_ml_x_str[20];
			sprintf(kaki_ml_x_str, "%.1f", *((double *)&pos_kaki.x_ml));
			lcd_send_string(kaki_ml_x_str);
			lcd_put_cur(1, 0);
			lcd_send_string("Y:");
			char kaki_ml_y_str[20];
			sprintf(kaki_ml_y_str, "%.1f", *((double *)&pos_kaki.y_ml));
			lcd_send_string(kaki_ml_y_str);
			lcd_put_cur(1, 8);
			lcd_send_string("Z:");
			char kaki_ml_z_str[20];
			sprintf(kaki_ml_z_str, "%.1f", *((double *)&pos_kaki.z_ml));
			lcd_send_string(kaki_ml_z_str);
			break;
		case 6:
			lcd_put_cur(0, 0);
			lcd_send_string("FL");
			lcd_put_cur(0, 8);
			lcd_send_string("X:");
			char kaki_fl_x_str[20];
			sprintf(kaki_fl_x_str, "%.1f", *((double *)&pos_kaki.x_fl));
			lcd_send_string(kaki_fl_x_str);
			lcd_put_cur(1, 0);
			lcd_send_string("Y:");
			char kaki_fl_y_str[20];
			sprintf(kaki_fl_y_str, "%.1f", *((double *)&pos_kaki.y_fl));
			lcd_send_string(kaki_fl_y_str);
			lcd_put_cur(1, 8);
			lcd_send_string("Z:");
			char kaki_fl_z_str[20];
			sprintf(kaki_fl_z_str, "%.1f", *((double *)&pos_kaki.z_fl));
			lcd_send_string(kaki_fl_z_str);
			break;
		}
		break;

	case 3:
		lcd_put_cur(0, 0);
		lcd_send_string("Room:");
		char ruang_str[20];
		sprintf(ruang_str, "%u", *((uint8_t *)&room_step.room));
		lcd_send_string(ruang_str);

		lcd_put_cur(1, 0);
		lcd_send_string("Step:");
		char flag_str[20];
		sprintf(flag_str, "%u", *((uint8_t *)&room_step.step));
		lcd_send_string(flag_str);
		break;

	case 4:
		switch (page_angle_kaki)
		{
		case 1:
			lcd_put_cur(0, 0);
			lcd_send_string("FR");
			lcd_put_cur(0, 8);
			lcd_send_string("C:");
			char kaki_fr_c_str[20];
			uint32_t fr_c = angle_saved_array_8bit[3] << 24 | (angle_saved_array_8bit[2] << 16) | (angle_saved_array_8bit[1] << 8) | angle_saved_array_8bit[0];
			sprintf(kaki_fr_c_str, "%d", *((int *)&fr_c));
			lcd_send_string(kaki_fr_c_str);
			lcd_put_cur(1, 0);
			lcd_send_string("F:");
			char kaki_fr_f_str[20];
			uint32_t fr_f = angle_saved_array_8bit[7] << 24 | (angle_saved_array_8bit[6] << 16) | (angle_saved_array_8bit[5] << 8) | angle_saved_array_8bit[4];
			sprintf(kaki_fr_f_str, "%d", *((int *)&fr_f));
			lcd_send_string(kaki_fr_f_str);
			lcd_put_cur(1, 8);
			lcd_send_string("T:");
			char kaki_fr_t_str[20];
			uint32_t fr_t = angle_saved_array_8bit[11] << 24 | (angle_saved_array_8bit[10] << 16) | (angle_saved_array_8bit[9] << 8) | angle_saved_array_8bit[8];
			sprintf(kaki_fr_t_str, "%d", *((int *)&fr_t));
			lcd_send_string(kaki_fr_t_str);
			break;
		case 2:
			lcd_put_cur(0, 0);
			lcd_send_string("MR");
			lcd_put_cur(0, 8);
			lcd_send_string("C:");
			char kaki_mr_c_str[20];
			uint32_t mr_c = angle_saved_array_8bit[15] << 24 | (angle_saved_array_8bit[14] << 16) | (angle_saved_array_8bit[13] << 8) | angle_saved_array_8bit[12];
			sprintf(kaki_mr_c_str, "%d", *((int *)&mr_c));
			lcd_send_string(kaki_mr_c_str);
			lcd_put_cur(1, 0);
			lcd_send_string("F:");
			char kaki_mr_f_str[20];
			uint32_t mr_f = angle_saved_array_8bit[19] << 24 | (angle_saved_array_8bit[18] << 16) | (angle_saved_array_8bit[17] << 8) | angle_saved_array_8bit[16];
			sprintf(kaki_mr_f_str, "%d", *((int *)&mr_f));
			lcd_send_string(kaki_mr_f_str);
			lcd_put_cur(1, 8);
			lcd_send_string("T:");
			char kaki_mr_t_str[20];
			uint32_t mr_t = angle_saved_array_8bit[23] << 24 | (angle_saved_array_8bit[22] << 16) | (angle_saved_array_8bit[21] << 8) | angle_saved_array_8bit[20];
			sprintf(kaki_mr_t_str, "%d", *((int *)&mr_t));
			lcd_send_string(kaki_mr_t_str);
			break;
		case 3:
			lcd_put_cur(0, 0);
			lcd_send_string("BR");
			lcd_put_cur(0, 8);
			lcd_send_string("C:");
			char kaki_br_c_str[20];
			uint32_t br_c = angle_saved_array_8bit[27] << 24 | (angle_saved_array_8bit[26] << 16) | (angle_saved_array_8bit[25] << 8) | angle_saved_array_8bit[24];
			sprintf(kaki_br_c_str, "%d", *((int *)&br_c));
			lcd_send_string(kaki_br_c_str);
			lcd_put_cur(1, 0);
			lcd_send_string("F:");
			char kaki_br_f_str[20];
			uint32_t br_f = angle_saved_array_8bit[31] << 24 | (angle_saved_array_8bit[30] << 16) | (angle_saved_array_8bit[29] << 8) | angle_saved_array_8bit[28];
			sprintf(kaki_br_f_str, "%d", *((int *)&br_f));
			lcd_send_string(kaki_br_f_str);
			lcd_put_cur(1, 8);
			lcd_send_string("T:");
			char kaki_br_t_str[20];
			uint32_t br_t = angle_saved_array_8bit[35] << 24 | (angle_saved_array_8bit[34] << 16) | (angle_saved_array_8bit[33] << 8) | angle_saved_array_8bit[32];
			sprintf(kaki_br_t_str, "%d", *((int *)&br_t));
			lcd_send_string(kaki_br_t_str);
			break;
		case 4:
			lcd_put_cur(0, 0);
			lcd_send_string("BL");
			lcd_put_cur(0, 8);
			lcd_send_string("C:");
			char kaki_bl_c_str[20];
			uint32_t bl_c = angle_saved_array_8bit[39] << 24 | (angle_saved_array_8bit[38] << 16) | (angle_saved_array_8bit[37] << 8) | angle_saved_array_8bit[36];
			sprintf(kaki_bl_c_str, "%d", *((int *)&bl_c));
			lcd_send_string(kaki_bl_c_str);
			lcd_put_cur(1, 0);
			lcd_send_string("F:");
			char kaki_bl_f_str[20];
			uint32_t bl_f = angle_saved_array_8bit[43] << 24 | (angle_saved_array_8bit[42] << 16) | (angle_saved_array_8bit[41] << 8) | angle_saved_array_8bit[40];
			sprintf(kaki_bl_f_str, "%d", *((int *)&bl_f));
			lcd_send_string(kaki_bl_f_str);
			lcd_put_cur(1, 8);
			lcd_send_string("T:");
			char kaki_bl_t_str[20];
			uint32_t bl_t = angle_saved_array_8bit[47] << 24 | (angle_saved_array_8bit[46] << 16) | (angle_saved_array_8bit[45] << 8) | angle_saved_array_8bit[44];
			sprintf(kaki_bl_t_str, "%d", *((int *)&bl_t));
			lcd_send_string(kaki_bl_t_str);
			break;
		case 5:
			lcd_put_cur(0, 0);
			lcd_send_string("ML");
			lcd_put_cur(0, 8);
			lcd_send_string("C:");
			char kaki_ml_c_str[20];
			uint32_t ml_c = angle_saved_array_8bit[51] << 24 | (angle_saved_array_8bit[50] << 16) | (angle_saved_array_8bit[49] << 8) | angle_saved_array_8bit[48];
			sprintf(kaki_ml_c_str, "%d", *((int *)&ml_c));
			lcd_send_string(kaki_ml_c_str);
			lcd_put_cur(1, 0);
			lcd_send_string("F:");
			char kaki_ml_f_str[20];
			uint32_t ml_f = angle_saved_array_8bit[55] << 24 | (angle_saved_array_8bit[54] << 16) | (angle_saved_array_8bit[53] << 8) | angle_saved_array_8bit[52];
			sprintf(kaki_ml_f_str, "%d", *((int *)&ml_f));
			lcd_send_string(kaki_ml_f_str);
			lcd_put_cur(1, 8);
			lcd_send_string("T:");
			char kaki_ml_t_str[20];
			uint32_t ml_t = angle_saved_array_8bit[59] << 24 | (angle_saved_array_8bit[58] << 16) | (angle_saved_array_8bit[57] << 8) | angle_saved_array_8bit[56];
			sprintf(kaki_ml_t_str, "%d", *((int *)&ml_t));
			lcd_send_string(kaki_ml_t_str);
			break;
		case 6:
			lcd_put_cur(0, 0);
			lcd_send_string("FL");
			lcd_put_cur(0, 8);
			lcd_send_string("C:");
			char kaki_fl_c_str[20];
			uint32_t fl_c = angle_saved_array_8bit[63] << 24 | (angle_saved_array_8bit[62] << 16) | (angle_saved_array_8bit[61] << 8) | angle_saved_array_8bit[60];
			sprintf(kaki_fl_c_str, "%d", *((int *)&fl_c));
			lcd_send_string(kaki_fl_c_str);
			lcd_put_cur(1, 0);
			lcd_send_string("F:");
			char kaki_fl_f_str[20];
			uint32_t fl_f = angle_saved_array_8bit[67] << 24 | (angle_saved_array_8bit[66] << 16) | (angle_saved_array_8bit[65] << 8) | angle_saved_array_8bit[64];
			sprintf(kaki_fl_f_str, "%d", *((int *)&fl_f));
			lcd_send_string(kaki_fl_f_str);
			lcd_put_cur(1, 8);
			lcd_send_string("T:");
			char kaki_fl_t_str[20];
			uint32_t fl_t = angle_saved_array_8bit[71] << 24 | (angle_saved_array_8bit[70] << 16) | (angle_saved_array_8bit[69] << 8) | angle_saved_array_8bit[68];
			sprintf(kaki_fl_t_str, "%d", *((int *)&fl_t));
			lcd_send_string(kaki_fl_t_str);
			break;
		}
		break;

	case 5:
		if (mode == MODE_REALTIME)
		{
			lcd_put_cur(0, 0);
			lcd_send_string("GYRO");
			lcd_put_cur(0, 8);
			lcd_send_string("R:");
			char gyro_r[20];
			sprintf(gyro_r, "%.1f", *((double *)&gyro.roll));
			lcd_send_string(gyro_r);
			lcd_put_cur(1, 0);
			lcd_send_string("P:");
			char gyro_p[20];
			sprintf(gyro_p, "%.1f", *((double *)&gyro.pitch));
			lcd_send_string(gyro_p);
			lcd_put_cur(1, 8);
			lcd_send_string("Y:");
			char gyro_y[20];
			sprintf(gyro_y, "%.1f", *((double *)&gyro.yaw));
			lcd_send_string(gyro_y);
		}
		else
		{
			lcd_put_cur(0, 0);
			lcd_send_string("GYRO");
			lcd_put_cur(0, 8);
			lcd_send_string("R:");
			char gyro_r[20];
			uint64_t roll_saved = ((uint64_t)jarak_gyro_saved_8bit[55] << 56) | ((uint64_t)jarak_gyro_saved_8bit[54] << 48) | ((uint64_t)jarak_gyro_saved_8bit[53] << 40) | ((uint64_t)jarak_gyro_saved_8bit[52] << 32) | (jarak_gyro_saved_8bit[51] << 24) | (jarak_gyro_saved_8bit[50] << 16) | (jarak_gyro_saved_8bit[49] << 8) | jarak_gyro_saved_8bit[48];
			sprintf(gyro_r, "%.1f", *((double *)&roll_saved));
			lcd_send_string(gyro_r);
			lcd_put_cur(1, 0);
			lcd_send_string("P:");
			char gyro_p[20];
			uint64_t pitch_saved = ((uint64_t)jarak_gyro_saved_8bit[63] << 56) | ((uint64_t)jarak_gyro_saved_8bit[62] << 48) | ((uint64_t)jarak_gyro_saved_8bit[61] << 40) | ((uint64_t)jarak_gyro_saved_8bit[60] << 32) | (jarak_gyro_saved_8bit[59] << 24) | (jarak_gyro_saved_8bit[58] << 16) | (jarak_gyro_saved_8bit[57] << 8) | jarak_gyro_saved_8bit[56];
			sprintf(gyro_p, "%.1f", *((double *)&pitch_saved));
			lcd_send_string(gyro_p);
			lcd_put_cur(1, 8);
			lcd_send_string("Y:");
			char gyro_y[20];
			uint64_t yaw_saved = ((uint64_t)jarak_gyro_saved_8bit[71] << 56) | ((uint64_t)jarak_gyro_saved_8bit[70] << 48) | ((uint64_t)jarak_gyro_saved_8bit[69] << 40) | ((uint64_t)jarak_gyro_saved_8bit[68] << 32) | (jarak_gyro_saved_8bit[67] << 24) | (jarak_gyro_saved_8bit[66] << 16) | (jarak_gyro_saved_8bit[65] << 8) | jarak_gyro_saved_8bit[64];
			sprintf(gyro_y, "%.1f", *((double *)&yaw_saved));
			lcd_send_string(gyro_y);
		}
		break;
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t data[5];


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi1)
{
	receive_all(&ping, &pos_kaki, &angle_kaki, &gyro, &room_step);
}

HAL_StatusTypeDef status;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	
	lcd_init(&hi2c1);

	komunikasi_remot_init(&hspi1);

	//readFromEE();
		
	receive_all(&ping, &pos_kaki, &angle_kaki, &gyro, &room_step);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//receive_all(&ping, &pos_kaki, &angle_kaki, &gyro, &room_step);
		displayDataOnLCD();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
