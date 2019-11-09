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

typedef enum {
	ST0_INITIALIZE, ST1_MAIN_MENU, ST2_RUN_PROGRAM, ST3_MOVE_HEAD, ST4_SETTINGS
} PROGRAM_STATE;

typedef struct PrinterSettings {
	double maxSpeed;
	double maxPosX;
	double maxPosY;
	double maxPosZ;
	double minStep;

	double position[3];
	int speed[3];
} PrinterSettings;

PROGRAM_STATE actualPrgState;
uint8_t markVerticalPos;
uint8_t markHorizontalPos;
PrinterSettings printerSettings;
double pos[3];
double mov[3] = { 0.0, 0.0, 0.0 };
int spd[3];

char data[20]; //for sprintf

/* PRIVATE FUNCTIONS DECLARATIONS */
void st0_initialize(KeyboardButtons buttState);
void st1_main_menu(KeyboardButtons buttState);
void st2_run_program(KeyboardButtons buttState);
void st3_move_head(KeyboardButtons buttState);
void st4_settings(KeyboardButtons buttState);

void draw_st1_main_menu();
void draw_st2_run_program();
void draw_st3_move_head();
void draw_st4_settings();

void draw_mark_st3(uint8_t markX, uint8_t markY, uint8_t color);

void draw_arrow(uint8_t x, uint8_t y, uint8_t color);
void draw_coordinate_diagram(uint8_t x, uint8_t y, uint8_t color);
void draw_edit_mark(bool on);
void restore_showed_values();
double updateDoubleValue(uint8_t xDraw, uint8_t yDraw, uint8_t charWidth, bool sign, char* format, double oldValue);
int updateIntegerValue(uint8_t xDraw, uint8_t yDraw, uint8_t charWidth, bool sign, char* format, int oldValue);



/* FUNCTIONS DEFINITIONS */
void menu_init() {
	actualPrgState = ST0_INITIALIZE;
	printerSettings.maxSpeed = 53.35;
	printerSettings.maxPosX = 231.25;
	printerSettings.maxPosY = 348.80;
	printerSettings.maxPosZ = 87.05;
	printerSettings.minStep = 0.05;

	printerSettings.position[0] = 50.0;
	printerSettings.position[1] = 100.0;
	printerSettings.position[2] = 75.0;
	printerSettings.speed[0] = 100;
	printerSettings.speed[1] = 100;
	printerSettings.speed[2] = 10;

}

void menu_run(KeyboardButtons buttState) {
	switch (actualPrgState) {
		case ST0_INITIALIZE: st0_initialize(buttState); break;
		case ST1_MAIN_MENU: st1_main_menu(buttState); break;
		case ST2_RUN_PROGRAM: st2_run_program(buttState); break;
		case ST3_MOVE_HEAD: st3_move_head(buttState); break;
		case ST4_SETTINGS: st4_settings(buttState); break;
		default: break;
	}
}



void st0_initialize(KeyboardButtons buttState){
	ST7565_clear();
	draw_st1_main_menu();
	ST7565_display();
	actualPrgState = ST1_MAIN_MENU;
}

void st1_main_menu(KeyboardButtons buttState){
	switch (buttState) {
		case BUTT_2:
			if (markVerticalPos > 1) {
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 0);
				--markVerticalPos;
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 255);
				ST7565_display();
			}
			break;
		case BUTT_8:
			if (markVerticalPos < 3) {
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 0);
				++markVerticalPos;
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 255);
				ST7565_display();
			}
			break;
		case BUTT_5:
			ST7565_clear();
			if (markVerticalPos == 1) {
				draw_st2_run_program();
				markVerticalPos = 1;
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 255);
				actualPrgState = ST2_RUN_PROGRAM;
			} else if (markVerticalPos == 2) {
				draw_st3_move_head();
				markVerticalPos = 3;
				markHorizontalPos = 0;
				draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
				actualPrgState = ST3_MOVE_HEAD;
			} else if (markVerticalPos == 3) {
				draw_st4_settings();
				markVerticalPos = 1;
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 255);
				actualPrgState = ST4_SETTINGS;
			}
			ST7565_display();
			break;
		default:
			break;
	}
}

void st2_run_program(KeyboardButtons buttState){
	if (buttState == BUTT_B) {
		ST7565_clear();
		draw_st1_main_menu();
		ST7565_display();
		actualPrgState = ST1_MAIN_MENU;
	}
}

void st3_move_head(KeyboardButtons buttState){
	switch (buttState) {
		case BUTT_2:
			if (markVerticalPos > 3) {
				draw_mark_st3(markHorizontalPos, markVerticalPos, 0);
				--markVerticalPos;
				draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
				ST7565_display();
			}
			break;
		case BUTT_8:
			if (markVerticalPos < 6) {
				draw_mark_st3(markHorizontalPos, markVerticalPos, 0);
				++markVerticalPos;
				draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
				ST7565_display();
			}
			break;
		case BUTT_6:
			if (markHorizontalPos < 3) {
				draw_mark_st3(markHorizontalPos, markVerticalPos, 0);
				++markHorizontalPos;
				draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
				ST7565_display();
			}
			break;
		case BUTT_4:
			if (markHorizontalPos > 0) {
				draw_mark_st3(markHorizontalPos, markVerticalPos, 0);
				--markHorizontalPos;
				draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
				ST7565_display();
			}
			break;
		case BUTT_5:
			if (markVerticalPos != 3 && markHorizontalPos == 1) {
				draw_mark_st3(markHorizontalPos, markVerticalPos, 0);
				ST7565_display();
				pos[markVerticalPos - 4] = updateDoubleValue(14, LINE_HIGH * markVerticalPos + markVerticalPos - 1, 6, false, "%6.2f%c", pos[markVerticalPos - 4]);
				draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
				ST7565_display();
			} else if (markHorizontalPos == 2) {
				draw_mark_st3(markHorizontalPos, markVerticalPos, 0);
				ST7565_display();
				mov[markVerticalPos - 4] = updateDoubleValue(52, LINE_HIGH * markVerticalPos + markVerticalPos - 1, 7, true, "%+7.2f%c", mov[markVerticalPos - 4]);
				draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
				ST7565_display();
			} else if (markHorizontalPos == 3) {
				draw_mark_st3(markHorizontalPos, markVerticalPos, 0);
				ST7565_display();
				spd[markVerticalPos - 4] = updateIntegerValue(96, LINE_HIGH * markVerticalPos + markVerticalPos - 1, 5, false, "%5d%c", spd[markVerticalPos - 4]);
				draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
				ST7565_display();
			}
			break;
		case BUTT_B:
			ST7565_clear();
			restore_showed_values();
			draw_st1_main_menu();
			ST7565_display();
			actualPrgState = ST1_MAIN_MENU;
			break;
		case BUTT_D:
			ST7565_clear();
			restore_showed_values();
			draw_st3_move_head();
			draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
			ST7565_display();
			break;
		case BUTT_A:
			switch (markVerticalPos) {
				case 3:
					switch (markHorizontalPos) {
						case 1:
							//wyslij komende ruchu do wszystkich osi na wyznaczona pozycje
							//ruch g³owic¹ zmienia polozenie (poni¿szy kod zmieniajacy wartoœci printerSettings.position sa tu tylko na czas testów)
							printerSettings.position[0] = pos[0];
							printerSettings.position[1] = pos[1];
							printerSettings.position[2] = pos[2];
							//poczekaj do koñca ruchu (brak mozliwoœci zmian wartoœci (ekran i klawiatura zablokowana))

							restore_showed_values();
							ST7565_clear();
							draw_st3_move_head();
							draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
							ST7565_display();
							break;
						case 2:
							//wyslij komende ruchu do wszystkich osi o podana wartosc mov
							//ruch g³owic¹ zmienia polozenie (poni¿szy kod zmieniajacy wartoœci printerSettings.position sa tu tylko na czas testów)
							printerSettings.position[0] = pos[0] + mov[0];
							printerSettings.position[1] = pos[1] + mov[1];
							printerSettings.position[2] = pos[2] + mov[2];
							//poczekaj do koñca ruchu (brak mozliwoœci zmian wartoœci (ekran i klawiatura zablokowana))

							restore_showed_values();
							ST7565_clear();
							draw_st3_move_head();
							draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
							ST7565_display();
							break;
						case 3:
							printerSettings.speed[0] = spd[0];
							printerSettings.speed[1] = spd[1];
							printerSettings.speed[2] = spd[2];
							restore_showed_values();
							ST7565_clear();
							draw_st3_move_head();
							draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
							ST7565_display();
							break;
						default:
							break;
					}
					break;
				case 4:
					//wyslij komende ruchu do odpowiedniej osi na wyznaczona pozycje
					//ruch g³owic¹ zmienia polozenie (poni¿szy kod zmieniajacy wartoœci printerSettings.position sa tu tylko na czas testów)
					printerSettings.position[markHorizontalPos - 1] = pos[markHorizontalPos - 1];
					//poczekaj do koñca ruchu (brak mozliwoœci zmian wartoœci (ekran i klawiatura zablokowana))

					mov[markHorizontalPos - 1] = 0.0;
					ST7565_clear();
					draw_st3_move_head();
					draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
					ST7565_display();
					break;
				case 5:
					//wyslij komende ruchu do wszystkich osi o podana wartosc mov
					//ruch g³owic¹ zmienia polozenie (poni¿szy kod zmieniajacy wartoœci printerSettings.position sa tu tylko na czas testów)
					printerSettings.position[markHorizontalPos - 1] = pos[markHorizontalPos - 1] + mov[markHorizontalPos - 1];
					//poczekaj do koñca ruchu (brak mozliwoœci zmian wartoœci (ekran i klawiatura zablokowana))

					pos[markHorizontalPos - 1] = printerSettings.position[markHorizontalPos - 1];
					mov[markHorizontalPos - 1] = 0.0;
					ST7565_clear();
					draw_st3_move_head();
					draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
					ST7565_display();
					break;
				case 6:
					printerSettings.speed[markHorizontalPos - 1] = spd[markHorizontalPos - 1];
					ST7565_clear();
					draw_st3_move_head();
					draw_mark_st3(markHorizontalPos, markVerticalPos, 255);
					ST7565_display();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void st4_settings(KeyboardButtons buttState){
	switch (buttState) {
		case BUTT_2:
			if (markVerticalPos > 1) {
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 0);
				--markVerticalPos;
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 255);
				ST7565_display();
			}
			break;
		case BUTT_8:
			if (markVerticalPos < 5) {
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 0);
				++markVerticalPos;
				draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 255);
				ST7565_display();
			}
			break;
		case BUTT_5:
			switch (markVerticalPos) {
				case 1:
					printerSettings.maxSpeed = updateDoubleValue(60,
					LINE_HIGH * markVerticalPos, 6, false, "%6.2f%c", printerSettings.maxSpeed);
					break;
				case 2:
					printerSettings.maxPosX = updateDoubleValue(60,
					LINE_HIGH * markVerticalPos, 6, false, "%6.2f%c", printerSettings.maxPosX);
					break;
				case 3:
					printerSettings.maxPosY = updateDoubleValue(60,
					LINE_HIGH * markVerticalPos, 6, false, "%6.2f%c", printerSettings.maxPosY);
					break;
				case 4:
					printerSettings.maxPosZ = updateDoubleValue(60,
					LINE_HIGH * markVerticalPos, 6, false, "%6.2f%c", printerSettings.maxPosZ);
					break;
				case 5:
					printerSettings.minStep = updateDoubleValue(60,
					LINE_HIGH * markVerticalPos, 6, false, "%6.2f%c", printerSettings.minStep);
					break;
				default:
					break;
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
}



void draw_st1_main_menu() {
	ST7565_drawstring_line(10, 0, "MAIN MENU");
	ST7565_drawstring_line(6, 1, "run program");
	ST7565_drawstring_line(6, 2, "move head");
	ST7565_drawstring_line(6, 3, "settings");
	markVerticalPos = 1;
	draw_arrow(0, markVerticalPos * LINE_HIGH + 1, 255);
}

void draw_st2_run_program() {
	ST7565_drawstring_line(10, 0, "RUN PROGRAM");
}

void draw_st3_move_head() {
	for (int i = 0; i < 3; ++i) {
		pos[i] = printerSettings.position[i];
		spd[i] = printerSettings.speed[i];
	}

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
}

void draw_st4_settings() {
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
}



void draw_mark_st3(uint8_t markX, uint8_t markY, uint8_t color) {
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;

	if (markX > 0 && markY > 3) {
		switch (markX) {
			case 1: x1 = 14; x2 = 48; break;
			case 2: x1 = 52; x2 = 92; break;
			case 3: x1 = 96; x2 = 124; break;
			default: break;
		}
		y1 = y2 = markY * LINE_HIGH + markY + 6;
		ST7565_drawline(x1, y1, x2, y2, color);
	} else {
		switch (markX) {
			case 0: x1 = 0; break;
			case 1: x1 = 14; break;
			case 2: x1 = 52; break;
			case 3: x1 = 96; break;
			default: break;
		}
		y1 = markY * LINE_HIGH + markY;
		draw_arrow(x1, y1, color);
	}
}



void draw_arrow(uint8_t x, uint8_t y, uint8_t color) {
	int yy = 0;
	for (int xx = 2; xx < 3; ++xx)
		ST7565_setpixel(x + xx, y + yy, color);
	++yy;
	for (int xx = 0; xx < 4; ++xx)
		ST7565_setpixel(x + xx, y + yy, color);
	++yy;
	for (int xx = 0; xx < 5; ++xx)
		ST7565_setpixel(x + xx, y + yy, color);
	++yy;
	for (int xx = 0; xx < 4; ++xx)
		ST7565_setpixel(x + xx, y + yy, color);
	++yy;
	for (int xx = 2; xx < 3; ++xx)
		ST7565_setpixel(x + xx, y + yy, color);
}

void draw_coordinate_diagram(uint8_t x, uint8_t y, uint8_t color) {
	ST7565_drawline(x, y, x, y + 15, 255);
	ST7565_drawline(x, y + 15, x + 20, y + 15, 255);
	ST7565_drawline(x, y + 15, x + 13, y + 6, 255);

	ST7565_drawchar_pixel(x + 22, y + 9, 'x');
	ST7565_drawchar_pixel(x + 16, y + 2, 'y');
	ST7565_drawchar_pixel(x + 2, y, 'z');
}

void draw_edit_mark(bool on) {
	uint8_t color;
	if (on == true)
		color = 255;
	else
		color = 0;

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

void restore_showed_values() {
	mov[0] = 0.0;
	mov[1] = 0.0;
	mov[2] = 0.0;
	pos[0] = printerSettings.position[0];
	pos[1] = printerSettings.position[1];
	pos[2] = printerSettings.position[2];
	spd[0] = printerSettings.speed[0];
	spd[1] = printerSettings.speed[1];
	spd[2] = printerSettings.speed[2];
}

double updateDoubleValue(uint8_t xDraw, uint8_t yDraw, uint8_t charWidth, bool sign, char* format, double oldValue) {
	KeyboardButtons buttState = NONE;
	int exit = charWidth;
	double val = 0.0;
	double sig = 1;
	draw_edit_mark(true);
	ST7565_display();
	while (1) {
		if (keyboard.stateChanged) {
			buttState = ModKB4x4_getButton(ModKB4x4_readKeyboard(&keyboard));

			if (buttState == BUTT_B) {
				sprintf(data, format, oldValue, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
				draw_edit_mark(false);
				ST7565_display();
				return oldValue;
			} else if (sign && exit == charWidth) {
				if (buttState >= BUTT_1 && buttState <= BUTT_9)
					sig = 1;
				else if (buttState == BUTT_0)
					sig = -1;
				else
					continue;
				--exit;
				sprintf(data, format, sig * val, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
			} else if (buttState >= BUTT_0 && buttState <= BUTT_9) {
				val += buttState * 0.01 * pow(10, exit - 2);
				sprintf(data, format, sig * val, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
				if (--exit <= 1) {
					draw_edit_mark(false);
					ST7565_display();
					return sig * val;
				}
			}
		}
	}
}

int updateIntegerValue(uint8_t xDraw, uint8_t yDraw, uint8_t charWidth, bool sign, char* format, int oldValue) {
	KeyboardButtons buttState = NONE;
	int exit = charWidth;
	int val = 0;
	int sig = 1;
	draw_edit_mark(true);
	ST7565_display();
	while (1) {
		if (keyboard.stateChanged) {
			buttState = ModKB4x4_getButton(ModKB4x4_readKeyboard(&keyboard));

			if (buttState == BUTT_B) {
				sprintf(data, format, oldValue, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
				draw_edit_mark(false);
				ST7565_display();
				return oldValue;
			} else if (sign && exit == charWidth) {
				if (buttState >= BUTT_1 && buttState <= BUTT_9)
					sig = 1;
				else if (buttState == BUTT_0)
					sig = -1;
				else
					continue;
				--exit;
				sprintf(data, format, sig * val, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
			} else if (buttState >= BUTT_0 && buttState <= BUTT_9) {
				val += buttState * pow(10, exit - 1);
				sprintf(data, format, sig * val, '\0');
				ST7565_drawstring_pixel(xDraw, yDraw, data);
				ST7565_display();
				if (--exit <= 0) {
					draw_edit_mark(false);
					ST7565_display();
					return sig * val;
				}
			}
		}
	}
}
