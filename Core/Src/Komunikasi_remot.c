#include "Komunikasi_remot.h"

static SPI_HandleTypeDef *hspi;

#define HEADER_PING 0X0F
#define HEADER_POS_KAKI 0X1F
#define HEADER_STEP_ROOM 0X2F
#define HEADER_SUDUT_KAKI 0X3F
#define HEADER_GYRO 0X4F

#define SIZE 145

static uint8_t array_all[SIZE];

void komunikasi_remot_init(SPI_HandleTypeDef *spiHandler)
{
	hspi = spiHandler;
}

HAL_StatusTypeDef receive_all(handler_ping *ping, handler_pos_kaki *kaki, handler_angle_kaki *angle_kaki, handler_gyro *gyro, handler_room_step *room_step)
{
	HAL_StatusTypeDef status;
	status = HAL_SPI_Receive_DMA(hspi, array_all, SIZE);

	if (array_all[0] == HEADER_PING)
	{
		ping->ping_fl = ((uint64_t)array_all[8] << 56) | ((uint64_t)array_all[7] << 48) | ((uint64_t)array_all[6] << 40) | ((uint64_t)array_all[5] << 32) | (array_all[4] << 24) | (array_all[3] << 16) | (array_all[2] << 8) | array_all[1];
		ping->ping_fr = ((uint64_t)array_all[16] << 56) | ((uint64_t)array_all[15] << 48) | ((uint64_t)array_all[14] << 40) | ((uint64_t)array_all[13] << 32) | (array_all[12] << 24) | (array_all[11] << 16) | (array_all[10] << 8) | array_all[9];
		ping->ping_bl = ((uint64_t)array_all[24] << 56) | ((uint64_t)array_all[23] << 48) | ((uint64_t)array_all[22] << 40) | ((uint64_t)array_all[21] << 32) | (array_all[20] << 24) | (array_all[19] << 16) | (array_all[18] << 8) | array_all[17];
		ping->ping_br = ((uint64_t)array_all[32] << 56) | ((uint64_t)array_all[31] << 48) | ((uint64_t)array_all[30] << 40) | ((uint64_t)array_all[29] << 32) | (array_all[28] << 24) | (array_all[27] << 16) | (array_all[26] << 8) | array_all[25];
		ping->ping_ff = ((uint64_t)array_all[40] << 56) | ((uint64_t)array_all[39] << 48) | ((uint64_t)array_all[38] << 40) | ((uint64_t)array_all[37] << 32) | (array_all[36] << 24) | (array_all[35] << 16) | (array_all[34] << 8) | array_all[33];
		ping->ping_bb = ((uint64_t)array_all[48] << 56) | ((uint64_t)array_all[47] << 48) | ((uint64_t)array_all[46] << 40) | ((uint64_t)array_all[45] << 32) | (array_all[44] << 24) | (array_all[43] << 16) | (array_all[42] << 8) | array_all[41];
	}
	else if (array_all[0] == HEADER_POS_KAKI)
	{
		kaki->y_fl = ((uint64_t)array_all[8] << 56) | ((uint64_t)array_all[7] << 48) | ((uint64_t)array_all[6] << 40) | ((uint64_t)array_all[5] << 32) | (array_all[4] << 24) | (array_all[3] << 16) | (array_all[2] << 8) | array_all[1];
		kaki->y_fr = ((uint64_t)array_all[16] << 56) | ((uint64_t)array_all[15] << 48) | ((uint64_t)array_all[14] << 40) | ((uint64_t)array_all[13] << 32) | (array_all[12] << 24) | (array_all[11] << 16) | (array_all[10] << 8) | array_all[9];
		kaki->y_ml = ((uint64_t)array_all[24] << 56) | ((uint64_t)array_all[23] << 48) | ((uint64_t)array_all[22] << 40) | ((uint64_t)array_all[21] << 32) | (array_all[20] << 24) | (array_all[19] << 16) | (array_all[18] << 8) | array_all[17];
		kaki->y_mr = ((uint64_t)array_all[32] << 56) | ((uint64_t)array_all[31] << 48) | ((uint64_t)array_all[30] << 40) | ((uint64_t)array_all[29] << 32) | (array_all[28] << 24) | (array_all[27] << 16) | (array_all[26] << 8) | array_all[25];
		kaki->y_bl = ((uint64_t)array_all[40] << 56) | ((uint64_t)array_all[39] << 48) | ((uint64_t)array_all[38] << 40) | ((uint64_t)array_all[37] << 32) | (array_all[36] << 24) | (array_all[35] << 16) | (array_all[34] << 8) | array_all[33];
		kaki->y_br = ((uint64_t)array_all[48] << 56) | ((uint64_t)array_all[47] << 48) | ((uint64_t)array_all[46] << 40) | ((uint64_t)array_all[45] << 32) | (array_all[44] << 24) | (array_all[43] << 16) | (array_all[42] << 8) | array_all[41];

		kaki->x_fl = ((uint64_t)array_all[56] << 56) | ((uint64_t)array_all[55] << 48) | ((uint64_t)array_all[54] << 40) | ((uint64_t)array_all[53] << 32) | array_all[52] << 24 | array_all[51] << 16 | array_all[50] << 8 | array_all[49];
		kaki->x_fr = ((uint64_t)array_all[64] << 56) | ((uint64_t)array_all[63] << 48) | ((uint64_t)array_all[62] << 40) | ((uint64_t)array_all[61] << 32) | array_all[60] << 24 | array_all[59] << 16 | array_all[58] << 8 | array_all[57];
		kaki->x_ml = ((uint64_t)array_all[72] << 56) | ((uint64_t)array_all[71] << 48) | ((uint64_t)array_all[70] << 40) | ((uint64_t)array_all[69] << 32) | array_all[68] << 24 | array_all[67] << 16 | array_all[66] << 8 | array_all[65];
		kaki->x_mr = ((uint64_t)array_all[80] << 56) | ((uint64_t)array_all[79] << 48) | ((uint64_t)array_all[78] << 40) | ((uint64_t)array_all[77] << 32) | array_all[76] << 24 | array_all[75] << 16 | array_all[74] << 8 | array_all[73];
		kaki->x_bl = ((uint64_t)array_all[88] << 56) | ((uint64_t)array_all[87] << 48) | ((uint64_t)array_all[86] << 40) | ((uint64_t)array_all[85] << 32) | array_all[84] << 24 | array_all[83] << 16 | array_all[82] << 8 | array_all[81];
		kaki->x_br = ((uint64_t)array_all[96] << 56) | ((uint64_t)array_all[95] << 48) | ((uint64_t)array_all[94] << 40) | ((uint64_t)array_all[93] << 32) | array_all[92] << 24 | array_all[91] << 16 | array_all[90] << 8 | array_all[89];

		kaki->z_fl = ((uint64_t)array_all[104] << 56) | ((uint64_t)array_all[103] << 48) | ((uint64_t)array_all[102] << 40) | ((uint64_t)array_all[101] << 32) | array_all[100] << 24 | array_all[99] << 16 | array_all[98] << 8 | array_all[97];
		kaki->z_fr = ((uint64_t)array_all[112] << 56) | ((uint64_t)array_all[111] << 48) | ((uint64_t)array_all[110] << 40) | ((uint64_t)array_all[109] << 32) | array_all[108] << 24 | array_all[107] << 16 | array_all[106] << 8 | array_all[105];
		kaki->z_ml = ((uint64_t)array_all[120] << 56) | ((uint64_t)array_all[119] << 48) | ((uint64_t)array_all[118] << 40) | ((uint64_t)array_all[117] << 32) | array_all[116] << 24 | array_all[115] << 16 | array_all[114] << 8 | array_all[113];
		kaki->z_mr = ((uint64_t)array_all[128] << 56) | ((uint64_t)array_all[127] << 48) | ((uint64_t)array_all[126] << 40) | ((uint64_t)array_all[125] << 32) | array_all[124] << 24 | array_all[123] << 16 | array_all[122] << 8 | array_all[121];
		kaki->z_bl = ((uint64_t)array_all[136] << 56) | ((uint64_t)array_all[135] << 48) | ((uint64_t)array_all[134] << 40) | ((uint64_t)array_all[133] << 32) | array_all[132] << 24 | array_all[131] << 16 | array_all[130] << 8 | array_all[129];
		kaki->z_br = ((uint64_t)array_all[144] << 56) | ((uint64_t)array_all[143] << 48) | ((uint64_t)array_all[142] << 40) | ((uint64_t)array_all[141] << 32) | array_all[140] << 24 | array_all[139] << 16 | array_all[138] << 8 | array_all[137];
	}
	else if (array_all[0] == HEADER_STEP_ROOM)
	{
		room_step->room = array_all[1];
		room_step->step = array_all[2];
	}

	else if (array_all[0] == HEADER_SUDUT_KAKI)
	{
		angle_kaki->coxa_fr = (array_all[4] << 24) | (array_all[3] << 16) | (array_all[2] << 8) | array_all[1];
		angle_kaki->femur_fr = (array_all[8] << 24) | (array_all[7] << 16) | (array_all[6] << 8) | array_all[5];
		angle_kaki->tibia_fr = (array_all[12] << 24) | (array_all[11] << 16) | (array_all[10] << 8) | array_all[9];

		angle_kaki->coxa_mr = (array_all[16] << 24) | (array_all[15] << 16) | (array_all[14] << 8) | array_all[13];
		angle_kaki->femur_mr = (array_all[20] << 24) | (array_all[19] << 16) | (array_all[18] << 8) | array_all[17];
		angle_kaki->tibia_mr = (array_all[24] << 24) | (array_all[23] << 16) | (array_all[22] << 8) | array_all[21];

		angle_kaki->coxa_br = (array_all[28] << 24) | (array_all[27] << 16) | (array_all[26] << 8) | array_all[25];
		angle_kaki->femur_br = (array_all[32] << 24) | (array_all[31] << 16) | (array_all[30] << 8) | array_all[29];
		angle_kaki->tibia_br = (array_all[36] << 24) | (array_all[35] << 16) | (array_all[34] << 8) | array_all[33];

		angle_kaki->coxa_bl = (array_all[40] << 24) | (array_all[39] << 16) | (array_all[38] << 8) | array_all[37];
		angle_kaki->femur_bl = (array_all[44] << 24) | (array_all[43] << 16) | (array_all[42] << 8) | array_all[41];
		angle_kaki->tibia_bl = (array_all[48] << 24) | (array_all[47] << 16) | (array_all[46] << 8) | array_all[45];

		angle_kaki->coxa_ml = (array_all[52] << 24) | (array_all[51] << 16) | (array_all[50] << 8) | array_all[49];
		angle_kaki->femur_ml = (array_all[56] << 24) | (array_all[55] << 16) | (array_all[54] << 8) | array_all[53];
		angle_kaki->tibia_ml = (array_all[60] << 24) | (array_all[59] << 16) | (array_all[58] << 8) | array_all[57];

		angle_kaki->coxa_fl = (array_all[64] << 24) | (array_all[63] << 16) | (array_all[62] << 8) | array_all[61];
		angle_kaki->femur_fl = (array_all[68] << 24) | (array_all[67] << 16) | (array_all[66] << 8) | array_all[65];
		angle_kaki->tibia_fl = (array_all[72] << 24) | (array_all[71] << 16) | (array_all[70] << 8) | array_all[69];
	}

	else if (array_all[0] == HEADER_GYRO)
	{
		gyro->roll = ((uint64_t)array_all[8] << 56) | ((uint64_t)array_all[7] << 48) | ((uint64_t)array_all[6] << 40) | ((uint64_t)array_all[5] << 32) | (array_all[4] << 24) | (array_all[3] << 16) | (array_all[2] << 8) | array_all[1];
		gyro->pitch = ((uint64_t)array_all[16] << 56) | ((uint64_t)array_all[15] << 48) | ((uint64_t)array_all[14] << 40) | ((uint64_t)array_all[13] << 32) | (array_all[12] << 24) | (array_all[11] << 16) | (array_all[10] << 8) | array_all[9];
		gyro->yaw = ((uint64_t)array_all[24] << 56) | ((uint64_t)array_all[23] << 48) | ((uint64_t)array_all[22] << 40) | ((uint64_t)array_all[21] << 32) | (array_all[20] << 24) | (array_all[19] << 16) | (array_all[18] << 8) | array_all[17];
	}

	return status;
}

HAL_StatusTypeDef send_ping(double *array_ping)
{
	HAL_StatusTypeDef status;
	uint8_t txbuf[SIZE];

	txbuf[0] = HEADER_PING; // Identifikasi header

	for (int i = 0; i < 6; i++)
	{
		uint8_t *doubleBytes = (uint8_t *)&array_ping[i];
		for (int j = 0; j < sizeof(double); j++)
		{
			txbuf[1 + i * sizeof(double) + j] = doubleBytes[j];
		}
	}

	// Fill sisa dengan 0x00
	for (int i = 49; i < 145; i++)
	{
		txbuf[i] = 0x00;
	}

	status = HAL_SPI_Transmit(hspi, txbuf, sizeof(txbuf), 100);
	return status;
}

HAL_StatusTypeDef send_pos_kaki(double *array_pos_kaki)
{
	HAL_StatusTypeDef status;

	uint8_t txbuf[SIZE];

	txbuf[0] = HEADER_POS_KAKI; // Identifikasi header

	for (int i = 0; i < 18; i++)
	{
		uint8_t *doubleBytes = (uint8_t *)&array_pos_kaki[i];
		for (int j = 0; j < sizeof(double); j++)
		{
			txbuf[1 + i * sizeof(double) + j] = doubleBytes[j];
		}
	}

	status = HAL_SPI_Transmit(hspi, txbuf, 145, 100);
	return status;
}

HAL_StatusTypeDef send_room_step(uint8_t room, uint8_t step)
{
	HAL_StatusTypeDef status;

	uint8_t room_step_array[2];
	room_step_array[0] = room;
	room_step_array[1] = step;

	uint8_t txbuf[SIZE];

	txbuf[0] = HEADER_STEP_ROOM; // Identifikasi header

	for (int i = 0; i < 2; i++)
	{
		txbuf[i + 1] = room_step_array[i];
	}

	// Fill sisa dengan 0x00
	for (int i = 3; i < sizeof(txbuf); i++)
	{
		txbuf[i] = 0x00;
	}

	status = HAL_SPI_Transmit(hspi, txbuf, sizeof(txbuf), 100);
	return status;
}

HAL_StatusTypeDef send_angle_kaki(int *array_sudut_kaki)
{
	HAL_StatusTypeDef status;

	uint8_t txbuf[SIZE];

	txbuf[0] = HEADER_SUDUT_KAKI; // Identifikasi header

	for (int i = 0; i < 18; i++)
	{
		uint8_t *intBytes = (uint8_t *)&array_sudut_kaki[i];
		for (int j = 0; j < sizeof(int); j++)
		{
			txbuf[1 + i * sizeof(int) + j] = intBytes[j];
		}
	}

	for (int i = 73; i < sizeof(txbuf); i++)
	{
		txbuf[i] = 0x00;
	}

	status = HAL_SPI_Transmit(hspi, txbuf, 145, 100);
	return status;
}

HAL_StatusTypeDef send_gyro(double *array_gyro)
{
	HAL_StatusTypeDef status;
	uint8_t txbuf[SIZE];

	txbuf[0] = HEADER_GYRO; // Identifikasi header

	for (int i = 0; i < 3; i++)
	{
		uint8_t *doubleBytes = (uint8_t *)&array_gyro[i];
		for (int j = 0; j < sizeof(double); j++)
		{
			txbuf[1 + i * sizeof(double) + j] = doubleBytes[j];
		}
	}

	for (int i = 25; i < sizeof(txbuf); i++)
	{
		txbuf[i] = 0x00;
	}

	status = HAL_SPI_Transmit(hspi, txbuf, 145, 100);
	return status;
}
