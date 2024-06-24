
/** Put this in the src folder **/

#include "i2c-lcd.h"
// extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

static I2C_HandleTypeDef *handler;

#define I2C_TIMEOUT 100

// 0x4e
#define SLAVE_ADDRESS_LCD 0x4e // change this according to ur setup

uint8_t buff_tx[4];

void lcd_send_cmd(char data)
{
	//  char data_u, data_l;
	//	uint8_t data_t[4];
	//	data_u = (cmd&0xf0);
	//	data_l = ((cmd<<4)&0xf0);
	//	data_t[0] = data_u|0x0C;  //en=1, rs=0
	//	data_t[1] = data_u|0x08;  //en=0, rs=0
	//	data_t[2] = data_l|0x0C;  //en=1, rs=0
	//	data_t[3] = data_l|0x08;  //en=0, rs=0
	//	HAL_I2C_Master_Transmit (&handler, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);

	buff_tx[0] = (data & 0xF0) | ENA_HIGH_RST_LOW_HB;
	buff_tx[1] = (data & 0xF0) | ENA_LOW_RST_LOW_HB;
	buff_tx[2] = ((data << 4) & 0xF0) | ENA_HIGH_RST_LOW_HB;
	buff_tx[3] = ((data << 4) & 0xF0) | ENA_LOW_RST_LOW_HB;

	HAL_I2C_Master_Transmit(handler, LCD_WRITE_ADDR, (uint8_t *)buff_tx, 4, I2C_TIMEOUT);
}

void lcd_send_data(char data)
{
	//	char data_u, data_l;
	//	uint8_t data_t[4];
	//	data_u = (data&0xf0);
	//	data_l = ((data<<4)&0xf0);
	//	data_t[0] = data_u|0x0D;  //en=1, rs=0
	//	data_t[1] = data_u|0x09;  //en=0, rs=0
	//	data_t[2] = data_l|0x0D;  //en=1, rs=0
	//	data_t[3] = data_l|0x09;  //en=0, rs=0
	//	HAL_I2C_Master_Transmit (handler, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);

	buff_tx[0] = (data & 0xF0) | ENA_HIGH_RST_LOW_LB;
	buff_tx[1] = (data & 0xF0) | ENA_LOW_RST_LOW_LB;
	buff_tx[2] = ((data << 4) & 0xF0) | ENA_HIGH_RST_LOW_LB;
	buff_tx[3] = ((data << 4) & 0xF0) | ENA_LOW_RST_LOW_LB;

	HAL_I2C_Master_Transmit(handler, LCD_WRITE_ADDR, (uint8_t *)buff_tx, 4, I2C_TIMEOUT);
}

void lcd_clear(void)
{
	lcd_send_cmd(0x80);
	for (int i = 0; i < 70; i++)
	{
		lcd_send_data(' ');
	}
}

void lcd_put_cur(int row, int col)
{
	switch (row)
	{
	case 0:
		col |= 0x80;
		break;
	case 1:
		col |= 0xC0;
		break;
	}

	lcd_send_cmd(col);
}

void lcd_init(I2C_HandleTypeDef *set_i2c)
{
	handler = set_i2c;
	//	// 4 bit initialisation
	//	HAL_Delay(50);  // wait for >40ms
	//	lcd_send_cmd (0x30);
	//	HAL_Delay(5);  // wait for >4.1ms
	//	lcd_send_cmd (0x30);
	//	HAL_Delay(1);  // wait for >100us
	//	lcd_send_cmd (0x30);
	//	HAL_Delay(10);
	//	lcd_send_cmd (0x20);  // 4bit mode
	//	HAL_Delay(10);

	//  // dislay initialisation
	//	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	//	HAL_Delay(1);
	//	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	//	HAL_Delay(1);
	//	lcd_send_cmd (0x01);  // clear display
	//	HAL_Delay(1);
	//	HAL_Delay(1);
	//	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	//	HAL_Delay(1);
	//	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)

	uint8_t cmd;

	HAL_Delay(50);
	cmd = LCD_FUNCTIONSET | LCD_4BITMODE;
	lcd_send_cmd(cmd);

	HAL_Delay(10);
	cmd = LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_FTYPE_5X8;
	lcd_send_cmd(cmd);

	HAL_Delay(1);
	cmd = LCD_DISPLAYCTRL | LCD_OFF;
	lcd_send_cmd(cmd);

	HAL_Delay(1);
	cmd = LCD_CLEAR;
	lcd_send_cmd(cmd);

	HAL_Delay(2);
	cmd = LCD_ENTRYMODE | LCD_ENTRYINCREMENT | LCD_ENTRYSHIFTDISABLE;
	lcd_send_cmd(cmd);

	HAL_Delay(1);
	cmd = LCD_DISPLAYCTRL | LCD_ON;
	lcd_send_cmd(cmd);
}

void lcd_send_string(char *str)
{
	while (*str)
		lcd_send_data(*str++);
}
