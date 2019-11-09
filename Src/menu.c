/*
 * menu.c
 *
 *  Created on: 05.11.2019
 *      Author: zxvcv
 */

#include "menu.h"
#include "ST7565.h"
#include <math.h>

#define LINE_HIGH 8


typedef enum{
	ST0_INITIALIZE,
	ST1_MAIN_MENU,
	ST2_RUN_PROGRAM,
	ST3_MOVE_HEAD,
	ST4_SETTINGS
}PROGRAM_STATE;

typedef struct PrinterSettings{
	double maxSpeed;
	double maxPosX;
	double maxPosY;
	double maxPosZ;
	double minStep;

	double position[3];
	double speed[3];
}PrinterSettings;

PROGRAM_STATE actualPrgState;
uint8_t arrowLinePos;
uint8_t markVerticalPos;
PrinterSettings printerSettings;
double pos[3] = {100.00, 100.00, 100.0};
double mov[3] = {0.0, 0.0, 0.0};
int spd[3] = {88888, 77777, 11111};

char data[20]; //for sprintf


/* PRIVATE FUNCTIONS */
void draw_st1_main_menu();
void draw_st2_run_program();
void draw_st3_move_head();
void draw_st4_settings();
void draw_arrow(uint8_t x, uint8_t y, uint8_t color);
void draw_coordinate_diagram(uint8_t x, uint8_t y, uint8_t color);
void draw_edit_mark(bool on);
double updateDoubleValue(uint8_t xDraw, uint8_t yDraw, uint8_t charWidth, bool sign, char* format, double oldValue);
int updateIntegerValue(uint8_t xDraw, uint8_t yDraw, uint8_t charWidth, bool sign, char* format, int oldValue);


/* FUNCTIONS DEFINITIONS */
void menu_init(){
	actualPrgState = ST0_INITIALIZE;
	printerSettings.maxSpeed = 53.35;
	printerSettings.maxPosX = 231.25;
	printerSettings.maxPosY = 348.80;
	printerSettings.maxPosZ = 87.05;
	printerSettings.minStep = 0.05;
}

void menu_run(KeyboardButtons buttState){
	switch(actualPrgState){
		case ST0_INITIALIZE:
			ST7565_clear();
			draw_st1_main_menu();
			ST7565_display();
			actualPrgState = ST1_MAIN_MENU;
			break;
		case ST1_MAIN_MENU:
			switch (buttState) {
				case BUTT_2:
					if(arrowLinePos > 1){
						draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 0);
						--arrowLinePos;
						draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
						ST7565_display();
					}
					break;
				case BUTT_8:
					if(arrowLinePos < 3){
						draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 0);
						++arrowLinePos;
						draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
						ST7565_display();
					}
					break;
				case BUTT_5:
					ST7565_clear();
					if(arrowLinePos == 1){
						draw_st2_run_program();
						actualPrgState = ST2_RUN_PROGRAM;
					}else if(arrowLinePos == 2){
						draw_st3_move_head();
						actualPrgState = ST3_MOVE_HEAD;
					}else if(arrowLinePos == 3){
						draw_st4_settings();
						actualPrgState = ST4_SETTINGS;
					}
					ST7565_display();
					break;
				default:
					break;
			}
			break;
		case ST2_RUN_PROGRAM:
			if(buttState == BUTT_B){
				ST7565_clear();
				draw_st1_main_menu();
				ST7565_display();
				actualPrgState = ST1_MAIN_MENU;
			}
			break;
		case ST3_MOVE_HEAD:
			switch (buttState) {
				case BUTT_2:
					if(arrowLinePos > 3 && markVerticalPos == 0){
						draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
						--arrowLinePos;
						draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
						ST7565_display();
					}
					break;
				case BUTT_8:
					if(arrowLinePos < 6 && markVerticalPos == 0){
						draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
						++arrowLinePos;
						draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
						ST7565_display();
					}
					break;
				case BUTT_6:
					if(markVerticalPos < 3 && arrowLinePos >= 3 && arrowLinePos <= 6){
						++markVerticalPos;
						if(arrowLinePos == 3){
							if(markVerticalPos == 1){
								draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
								draw_arrow(14, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
								ST7565_display();
							}
							if(markVerticalPos == 2){
								draw_arrow(14, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
								draw_arrow(52, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
								ST7565_display();
							}
							if(markVerticalPos == 3){
								draw_arrow(52, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
								draw_arrow(96, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
								ST7565_display();
							}

						}
						else{
							int y = arrowLinePos * LINE_HIGH + arrowLinePos + 6;
							if(markVerticalPos == 1){
								draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
								ST7565_drawline(14, y, 48, y, 255);
								ST7565_display();
							}
							if(markVerticalPos == 2){
								ST7565_drawline(14, y, 48, y, 0);
								ST7565_drawline(52, y, 92, y, 255);
								ST7565_display();
							}
							if(markVerticalPos == 3){
								ST7565_drawline(52, y, 92, y, 0);
								ST7565_drawline(96, y, 124, y, 255);
								ST7565_display();
							}
						}

					}
					break;
				case BUTT_4:
					if(markVerticalPos > 0){
						--markVerticalPos;

						if(arrowLinePos == 3){
							if(markVerticalPos == 0){
								draw_arrow(14, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
								draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
								ST7565_display();
							}
							if(markVerticalPos == 1){
								draw_arrow(52, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
								draw_arrow(14, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
								ST7565_display();
							}
							if(markVerticalPos == 2){
								draw_arrow(96, arrowLinePos * LINE_HIGH + arrowLinePos, 0);
								draw_arrow(52, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
								ST7565_display();
							}

						}
						else{
							int y = arrowLinePos * LINE_HIGH + arrowLinePos + 6;
							if(markVerticalPos == 0){
								draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
								ST7565_drawline(14, y, 48, y, 0);
								ST7565_display();
							}
							if(markVerticalPos == 1){
								ST7565_drawline(52, y, 92, y, 0);
								ST7565_drawline(14, y, 48, y, 255);
								ST7565_display();
							}
							if(markVerticalPos == 2){
								ST7565_drawline(96, y, 124, y, 0);
								ST7565_drawline(52, y, 92, y, 255);
								ST7565_display();
							}
						}
					}
					break;//poprawione do tad
				case BUTT_5:
					if(arrowLinePos != 3){
						if(markVerticalPos == 1){
							ST7565_drawline(14, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 14+6*6-2, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 0);
							ST7565_display();
							pos[arrowLinePos - 4] = updateDoubleValue(14, LINE_HIGH * arrowLinePos + arrowLinePos -1, 6, false, "%6.2f%c", pos[arrowLinePos - 4]);
							ST7565_drawline(14, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 14+6*6-2, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 255);
							ST7565_display();
						}else if(markVerticalPos == 2){
							ST7565_drawline(52, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 52+7*6-2, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 0);
							ST7565_display();
							mov[arrowLinePos - 4] = updateDoubleValue(52, LINE_HIGH * arrowLinePos + arrowLinePos - 1, 7, true, "%+7.2f%c", mov[arrowLinePos - 4]);
							ST7565_drawline(52, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 52+7*6-2, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 255);
							ST7565_display();
						}else if(markVerticalPos == 3){
							ST7565_drawline(96, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 124, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 0);
							ST7565_display();
							spd[arrowLinePos - 4] = updateIntegerValue(96, LINE_HIGH * arrowLinePos + arrowLinePos - 1, 5, false, "%5d%c", spd[arrowLinePos - 4]);
							ST7565_drawline(96, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 124, LINE_HIGH * arrowLinePos + arrowLinePos + LINE_HIGH - 2, 255);
							ST7565_display();
						}
					}
					break;
				case BUTT_B:
					ST7565_clear();
					draw_st1_main_menu();
					ST7565_display();
					actualPrgState = ST1_MAIN_MENU;
					break;
				default:
					break;
			}
			break;
		case ST4_SETTINGS:
			switch (buttState) {
				case BUTT_2:
					if (arrowLinePos > 1) {
						draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 0);
						--arrowLinePos;
						draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
						ST7565_display();
					}
					break;
				case BUTT_8:
					if (arrowLinePos < 5) {
						draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 0);
						++arrowLinePos;
						draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
						ST7565_display();
					}
					break;
				case BUTT_5:
					switch (arrowLinePos) {
						case 1: printerSettings.maxSpeed = updateDoubleValue(60, LINE_HIGH * arrowLinePos, 6, false, "%6.2f%c", printerSettings.maxSpeed); break;
						case 2: printerSettings.maxPosX = updateDoubleValue(60, LINE_HIGH * arrowLinePos, 6, false, "%6.2f%c", printerSettings.maxPosX); break;
						case 3: printerSettings.maxPosY = updateDoubleValue(60, LINE_HIGH * arrowLinePos, 6, false, "%6.2f%c", printerSettings.maxPosY); break;
						case 4: printerSettings.maxPosZ = updateDoubleValue(60, LINE_HIGH * arrowLinePos, 6, false, "%6.2f%c", printerSettings.maxPosZ); break;
						case 5: printerSettings.minStep = updateDoubleValue(60, LINE_HIGH * arrowLinePos, 6, false, "%6.2f%c", printerSettings.minStep); break;
						default: break;
					}
					break;
				case BUTT_B:
					ST7565_clear();
					draw_st1_main_menu();
					ST7565_display();
					actualPrgState = ST1_MAIN_MENU;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

}



void draw_st1_main_menu() {
	ST7565_drawstring_line(10, 0, "MAIN MENU");
	ST7565_drawstring_line(6, 1, "run program");
	ST7565_drawstring_line(6, 2, "move head");
	ST7565_drawstring_line(6, 3, "settings");
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
}

void draw_st2_run_program(){
	ST7565_drawstring_line(10, 0, "RUN PROGRAM");
	//ST7565_drawstring(6, 1, comunicats[C2_run_program]);
	//ST7565_drawstring(6, 2, comunicats[C3_move_head]);
	//ST7565_drawstring(6, 3, comunicats[C4_settings]);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
}

void draw_st3_move_head(){
	ST7565_drawstring_line(10, 0, "MOVE HEAD");
	ST7565_drawstring_line(20, 3, "pos");
	ST7565_drawstring_line(58, 3, "mov");
	ST7565_drawstring_line(102, 3, "spd");
	ST7565_drawchar_pixel(6, LINE_HIGH * 4 + 3, 'X');
	ST7565_drawchar_pixel(6, LINE_HIGH * 5 + 4, 'Y');
	ST7565_drawchar_pixel(6, LINE_HIGH * 6 + 5, 'Z');

	sprintf(data, "%6.2f%c", pos[0], '\0');
	ST7565_drawstring_pixel(14, LINE_HIGH * 4 + 3, data);
	sprintf(data, "%6.2f%c", pos[1], '\0');
	ST7565_drawstring_pixel(14, LINE_HIGH * 5 + 4, data);
	sprintf(data, "%6.2f%c", pos[2], '\0');
	ST7565_drawstring_pixel(14, LINE_HIGH * 6 + 5, data);
	sprintf(data, "%+7.2f%c", mov[0], '\0');
	ST7565_drawstring_pixel(52, LINE_HIGH * 4 + 3, data);
	sprintf(data, "%+7.2f%c", mov[1], '\0');
	ST7565_drawstring_pixel(52, LINE_HIGH * 5 + 4, data);
	sprintf(data, "%+7.2f%c", mov[2], '\0');
	ST7565_drawstring_pixel(52, LINE_HIGH * 6 + 5, data);
	sprintf(data, "%5d%c", spd[0], '\0');
	ST7565_drawstring_pixel(96, LINE_HIGH * 4 + 3, data);
	sprintf(data, "%5d%c", spd[1], '\0');
	ST7565_drawstring_pixel(96, LINE_HIGH * 5 + 4, data);
	sprintf(data, "%5d%c", spd[2], '\0');
	ST7565_drawstring_pixel(96, LINE_HIGH * 6 + 5, data);


	//ST7565_drawstring_line(100, 0, "[cm]");
	ST7565_drawline(0, LINE_HIGH * 4 + 1, 127, LINE_HIGH * 4 + 1, 255);
	ST7565_drawline(12, LINE_HIGH * 3 + 2, 12, LINE_HIGH * 7 + 4, 255);
	ST7565_drawline(50, LINE_HIGH * 3 + 2, 50, LINE_HIGH * 7 + 4, 255);
	ST7565_drawline(94, LINE_HIGH * 3 + 2, 94, LINE_HIGH * 7 + 4, 255);
	draw_coordinate_diagram(100, 0, 255);
	arrowLinePos = 3;
	draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos, 255);
	markVerticalPos = 0;
}


void draw_st4_settings(){
	ST7565_drawstring_line(10, 0, "SETTINGS");

	ST7565_drawstring_line(6, 1, "maxSpd:");
	sprintf(data, "%6.2f%c", printerSettings.maxSpeed, '\0');
	ST7565_drawstring_line(60, 1, data);

	ST7565_drawstring_line(6, 2, "XmaxPos:");
	sprintf(data, "%6.2f%c", printerSettings.maxPosX, '\0');
	ST7565_drawstring_line(60, 2, data);

	ST7565_drawstring_line(6, 3, "YmaxPos:");
	sprintf(data, "%6.2f%c", printerSettings.maxPosY, '\0');
	ST7565_drawstring_line(60, 3, data);

	ST7565_drawstring_line(6, 4, "ZmaxPos:");
	sprintf(data, "%6.2f%c", printerSettings.maxPosZ, '\0');
	ST7565_drawstring_line(60, 4, data);

	ST7565_drawstring_line(6, 5, "minStep:");
	sprintf(data, "%6.2f%c", printerSettings.minStep, '\0');
	ST7565_drawstring_line(60, 5, data);

	draw_coordinate_diagram(100, 5, 255);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);

}


void draw_arrow(uint8_t x, uint8_t y, uint8_t color){
	int yy=0;
	for(int xx=2; xx<3; ++xx)
		ST7565_setpixel(x+xx, y+yy, color);
	++yy;
	for(int xx=0; xx<4; ++xx)
		ST7565_setpixel(x+xx, y+yy, color);
	++yy;
	for(int xx=0; xx<5; ++xx)
		ST7565_setpixel(x+xx, y+yy, color);
	++yy;
	for(int xx=0; xx<4; ++xx)
		ST7565_setpixel(x+xx, y+yy, color);
	++yy;
	for(int xx=2; xx<3; ++xx)
		ST7565_setpixel(x+xx, y+yy, color);
}


void draw_coordinate_diagram(uint8_t x, uint8_t y, uint8_t color){
	ST7565_drawline(x, y, x, y+15, 255);
	ST7565_drawline(x, y+15, x+20, y+15, 255);
	ST7565_drawline(x, y+15, x+13, y+6, 255);

	ST7565_drawchar_pixel(x+22, y+9, 'x');
	ST7565_drawchar_pixel(x+16, y+2, 'y');
	ST7565_drawchar_pixel(x+2, y, 'z');
}


void draw_edit_mark(bool on){
	uint8_t color;
	if(on == true)
		color=255;
	else
		color=0;

	ST7565_setpixel(0, 0, color);
	ST7565_setpixel(1, 0, color);
	ST7565_setpixel(2, 0, color);
	ST7565_setpixel(0, 1, color);
	ST7565_setpixel(0, 2, color);
	ST7565_setpixel(1, 2, color);
	ST7565_setpixel(0, 3, color);
	ST7565_setpixel(0, 4, color);
	ST7565_setpixel(1, 4, color);
	ST7565_setpixel(2, 4, color);
}

extern UART_HandleTypeDef huart2;
double updateDoubleValue(uint8_t xDraw, uint8_t yDraw, uint8_t charWidth, bool sign, char* format, double oldValue){
	KeyboardButtons buttState = NONE;
	int exit = charWidth;
	double val = 0.0;
	double sig = 1;
	draw_edit_mark(true);
	ST7565_display();
	while(1){
		if(keyboard.stateChanged){
			buttState = ModKB4x4_getButton(ModKB4x4_readKeyboard(&keyboard));

			char data[50];
			uint8_t size;
			size = sprintf(data, "%d\n", buttState);
			HAL_UART_Transmit(&huart2, (uint8_t*)data, size, 1000);


			if(buttState == BUTT_B){
				sprintf(data, format, oldValue, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
				draw_edit_mark(false);
				ST7565_display();
				return oldValue;
			} else if(sign && exit == charWidth){
				if(buttState >= BUTT_1 && buttState <= BUTT_9)
					sig = 1;
				else if(buttState == BUTT_0)
					sig = -1;
				else
					continue;
				--exit;
				sprintf(data, format, sig * val, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
			}else if(buttState >= BUTT_0 && buttState <= BUTT_9){
				val += buttState * 0.01 * pow(10, exit-2);
				sprintf(data, format, sig * val, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
				if(--exit <= 1){
					draw_edit_mark(false);
					ST7565_display();
					return sig * val;
				}
			}
		}
	}
}


int updateIntegerValue(uint8_t xDraw, uint8_t yDraw, uint8_t charWidth, bool sign, char* format, int oldValue){
	KeyboardButtons buttState = NONE;
	int exit = charWidth;
	int val = 0;
	int sig = 1;
	draw_edit_mark(true);
	ST7565_display();
	while(1){
		if(keyboard.stateChanged){
			buttState = ModKB4x4_getButton(ModKB4x4_readKeyboard(&keyboard));

			if(buttState == BUTT_B){
				sprintf(data, format, oldValue, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
				draw_edit_mark(false);
				ST7565_display();
				return oldValue;
			} else if(sign && exit == charWidth){
				if(buttState >= BUTT_1 && buttState <= BUTT_9)
					sig = 1;
				else if(buttState == BUTT_0)
					sig = -1;
				else
					continue;
				--exit;
				sprintf(data, format, sig * val, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
			}else if(buttState >= BUTT_0 && buttState <= BUTT_9){
				val += buttState * pow(10, exit-1);
				sprintf(data, format, sig * val, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
				if(--exit <= 0){
					draw_edit_mark(false);
					ST7565_display();
					return sig * val;
				}
			}
		}
	}
}
