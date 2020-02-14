/*
 * LEDdisplay_operations.c
 *
 *  Created on: 14.02.2020
 *      Author: ppisk
 */

#include "LEDdisplay_operations.h"

void drawInterface(){
	ST7565_drawstring_P_line(5, 0, "Instruction: ");
	ST7565_drawchar_line(90, 0, '/');
	ST7565_drawstring_P_line(5, 2, "Pos X: ");
	ST7565_drawstring_P_line(5, 3, "Pos Y: ");
	ST7565_drawstring_P_line(5, 4, "Pos Z: ");
	ST7565_drawstring_P_line(5, 6, "Tmp  head: ");
	ST7565_drawstring_P_line(5, 7, "Tmp board: ");
	ST7565_display();
}

void updateValues(InterfaceValues *val){
	char buff[20];
	itoa(val->instruction, buff, 10);
	ST7565_drawstring_line(70, 0, buff);
	itoa(val->numOfInstructions, buff, 10);
	ST7565_drawstring_line(92, 0, buff);
	/*
	ftoa(val->posX, buff, 2);
	ST7565_drawstring_line(50, 2, buff);
	ftoa(val->posY, buff, 2);
	ST7565_drawstring_line(50, 3, buff);
	ftoa(val->posZ, buff, 2);
	ST7565_drawstring_line(50, 4, buff);
	ftoa(val->tmpH, buff, 2);
	ST7565_drawstring_line(80, 6, buff);
	ftoa(val->tmpB, buff, 2);
	ST7565_drawstring_line(80, 7, buff);
	*/
	ST7565_display();
}
