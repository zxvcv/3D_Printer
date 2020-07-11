/*
$Id:$

ST7565 LCD library!

Copyright (C) 2010 Limor Fried, Adafruit Industries

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

// some of this code was written by <cstone@pobox.com> originally; it is in the public domain.


*
* ******************
* 		EXAMPLE
* ******************
*
*
*
* 	void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
*	{	
*		ListStruct_Pop_C(Buff_SPI_ST7565R);
*		__disable_irq();
*		int size = ListStruct_GetSize(Buff_SPI_ST7565R);
*		__enable_irq();
*
*		if(size > 0)
*			ST7565_spiwrite();
*	}
*
*
*	ST7565_begin(0x08);
*	ST7565_clear_display();
*	char a[] = "string";
*	ST7565_drawstring(5, 1, a);
*	ST7565_display();
*
*/
#ifndef ST7565_H_
#define ST7565_H_

#include <stdint.h>
#include <stdbool.h>

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2 
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0

#include "main.h"
#include "ProjectTypes.h"
#include "FIFO_void.h"
/*----------------------------data_types----------------------------*/

typedef struct ST7565R_Settings{
	IO_Pin IOcs;
	IO_Pin IOrst;
	IO_Pin IOa0;
	SPI_HandleTypeDef* spi;
	List* buffer;
}ST7565R_Settings;

typedef struct DataToST7565{
	uint8_t val;
	bool state_a0;
}DataToST7565;

/*----------------------------END_data_types----------------------------*/


/*----------------------------externed_data----------------------------*/


/*----------------------------END_externed_data----------------------------*/


/*----------------------------function_declarations----------------------------*/

void ST7565_st7565_init(ST7565R_Settings* settings);
void ST7565_begin(ST7565R_Settings* settings, uint8_t contrast);
void ST7565_st7565_command(ST7565R_Settings* settings, uint8_t c);
void ST7565_st7565_data(ST7565R_Settings* settings, uint8_t c);
void ST7565_st7565_set_brightness(ST7565R_Settings* settings, uint8_t val);
void ST7565_clear_display(ST7565R_Settings* settings);
void ST7565_clear(ST7565R_Settings* settings);
void ST7565_display(ST7565R_Settings* settings);

void ST7565_setpixel(ST7565R_Settings* settings, uint8_t x, uint8_t y, uint8_t color);
uint8_t ST7565_getpixel(ST7565R_Settings* settings, uint8_t x, uint8_t y);
void ST7565_fillcircle(ST7565R_Settings* settings, uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
void ST7565_drawcircle(ST7565R_Settings* settings, uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
void ST7565_drawrect(ST7565R_Settings* settings, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void ST7565_fillrect(ST7565R_Settings* settings, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void ST7565_drawline(ST7565R_Settings* settings, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void ST7565_drawchar_line(ST7565R_Settings* settings, uint8_t x, uint8_t line, char c);
void ST7565_drawchar_pixel(ST7565R_Settings* settings, uint8_t x, uint8_t y, char c);
void ST7565_drawstring_line(ST7565R_Settings* settings, uint8_t x, uint8_t line, char *c);
void ST7565_drawstring_pixel(ST7565R_Settings* settings, uint8_t x, uint8_t y, char *c);
void ST7565_drawstring_P_line(ST7565R_Settings* settings, uint8_t x, uint8_t line, const char *c);
void ST7565_drawstring_P_pixel(ST7565R_Settings* settings, uint8_t x, uint8_t y, const char *c);

void ST7565_drawbitmap(ST7565R_Settings* settings, uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);

void ST7565_spiwrite(ST7565R_Settings* settings);

void ST7565_my_setpixel(ST7565R_Settings* settings, uint8_t x, uint8_t y, uint8_t color);
uint8_t ST7565_getbuffunit(ST7565R_Settings* settings, uint8_t x, uint8_t line);
void ST7565_setbuffunit(ST7565R_Settings* settings, uint8_t x, uint8_t line, uint8_t data);
/*----------------------------END_function_declarations----------------------------*/

#endif /* ST7565_H_ */
