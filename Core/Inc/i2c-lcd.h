#include "stm32f1xx_hal.h"

#define LCD_WRITE_ADDR 0x4E
#define LCD_READ_ADDR 0x4F
#define ROW_ADDR_TOP 0x80
#define ROW_ADDR_BOTTOM 0xC0
#define ENA_HIGH_RST_LOW_HB 0x0C
#define ENA_LOW_RST_LOW_HB 0x08
#define ENA_HIGH_RST_LOW_LB 0x0D
#define ENA_LOW_RST_LOW_LB 0x09

/*---COMMANDS---*/
#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRYMODE 0x04
#define LCD_DISPLAYCTRL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAM_ADDR 0x40
#define LCD_SETDDRAM_ADDR 0x80

/*---ENTRY MODE SET---*/
#define LCD_ENTRYINCREMENT 0x02
#define LCD_ENTRYDECREMENT 0x00
#define LCD_ENTRYSHIFTENABLE 0x01
#define LCD_ENTRYSHIFTDISABLE 0x00

/*---ON/OFF SET---*/
#define LCD_ON 0x04
#define LCD_OFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/*---DISPLAY CURSOR SHIFT SET*/
#define LCD_MOVEDISPLAY 0x08
#define LCD_MOVECURSOR 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/*---FUNCTION SET---*/
#define LCD_4BITMODE 0x00
#define LCD_8BITMODE 0x10
#define LCD_1LINE 0x00
#define LCD_2LINE 0x08
#define LCD_FTYPE_5X8 0x00
#define LCD_FTYPE_5X10 0x04

void lcd_init(I2C_HandleTypeDef *set_i2c); // initialize lcd

void lcd_send_cmd(char cmd); // send command to the lcd

void lcd_send_data(char data); // send data to the lcd

void lcd_send_string(char *str); // send string to the lcd

void lcd_put_cur(int row, int col); // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear(void);
