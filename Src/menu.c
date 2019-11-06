/*
 * menu.c
 *
 *  Created on: 05.11.2019
 *      Author: zxvcv
 */

#include "menu.h"
#include "ST7565.h"

#define LINE_HIGH 8


typedef enum{
	ST0_INITIALIZE,
	ST1_MAIN_MENU,
	ST2_RUN_PROGRAM,
	ST3_MOVE_HEAD,
	ST4_SETTINGS
}PROGRAM_STATE;

typedef enum{
	C0_error = 0,
	C1_MAIN_MENU,
	C2_run_program,
	C3_move_head,
	C4_settings,
	C5_RUN_PROGRAM,
	C6_MOVE_HEAD,
	C7_SETTINGS,
	C8_pos,
	C9_mov
}COMUNICAT;

char* comunicats[] = {
		"error",
		"MAIN MENU",
		"run program",
		"move head",
		"settings",
		"RUN PROGRAM",
		"MOVE HEAD",
		"SETTINGS",
		"pos",
		"mov"
};

PROGRAM_STATE actualPrgState;
uint8_t arrowLinePos;
uint8_t markVerticalPos;

void draw_arrow(uint8_t x, uint8_t y, uint8_t color);
void draw_st1_main_menu();
void draw_st2_run_program();
void draw_st3_move_head();
void draw_st4_settings();

void menu_init(){
	actualPrgState = ST0_INITIALIZE;
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
					if(arrowLinePos > 1 && markVerticalPos == 0){
						draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos + 2, 0);
						--arrowLinePos;
						draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos + 2, 255);
						ST7565_display();
					}
					break;
				case BUTT_8:
					if(arrowLinePos < 6 && markVerticalPos == 0){
						draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos + 2, 0);
						++arrowLinePos;
						draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos + 2, 255);
						ST7565_display();
					}
					break;
				case BUTT_6:
					if(markVerticalPos < 2 && arrowLinePos >= 2 && arrowLinePos <= 4){
						++markVerticalPos;
						int y = LINE_HIGH * (arrowLinePos + 1) + arrowLinePos;
						if(markVerticalPos == 1){
							ST7565_drawline(14, y, 48, y, 255);
							ST7565_display();
						}
						if(markVerticalPos == 2){
							ST7565_drawline(14, y, 48, y, 0);
							ST7565_drawline(52, y, 92, y, 255);
							ST7565_display();
						}
					}
					break;
				case BUTT_4:
					if(markVerticalPos > 0){
						--markVerticalPos;
						int y = LINE_HIGH * (arrowLinePos + 1) + arrowLinePos;
						if(markVerticalPos == 0){
							ST7565_drawline(14, y, 48, y, 0);
							ST7565_display();
						}
						if(markVerticalPos == 1){
							ST7565_drawline(52, y, 92, y, 0);
							ST7565_drawline(14, y, 48, y, 255);
							ST7565_display();
						}
					}
					break;
				case BUTT_5:

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
			if(buttState == BUTT_B){
				ST7565_clear();
				draw_st1_main_menu();
				ST7565_display();
				actualPrgState = ST1_MAIN_MENU;
			}
			break;
		default:
			break;
	}

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
	ST7565_drawline(x, y, x, y+25, 255);
	ST7565_drawline(x, y+25, x+25, y+25, 255);
	ST7565_drawline(x, y+25, x+15, y+10, 255);

	ST7565_drawchar_pixel(x+21, y+17, 'x');
	ST7565_drawchar_pixel(x+17, y+6, 'y');
	ST7565_drawchar_pixel(x+2, y, 'z');
}

void draw_st1_main_menu() {
	ST7565_drawstring_line(10, 0, comunicats[C1_MAIN_MENU]);
	ST7565_drawstring_line(6, 1, comunicats[C2_run_program]);
	ST7565_drawstring_line(6, 2, comunicats[C3_move_head]);
	ST7565_drawstring_line(6, 3, comunicats[C4_settings]);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
}

void draw_st2_run_program(){
	ST7565_drawstring_line(10, 0, comunicats[C5_RUN_PROGRAM]);
	//ST7565_drawstring(6, 1, comunicats[C2_run_program]);
	//ST7565_drawstring(6, 2, comunicats[C3_move_head]);
	//ST7565_drawstring(6, 3, comunicats[C4_settings]);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
}

double pos[3] = {200.25, 50.01, 5.2};
double mov[3] = {815.32, 0.0, 352.05};
char data[20];

void draw_st3_move_head(){
	ST7565_drawstring_line(10, 0, comunicats[C6_MOVE_HEAD]);
	ST7565_drawstring_line(15, 1, comunicats[C8_pos]);
	ST7565_drawstring_line(57, 1, comunicats[C9_mov]);
	ST7565_drawchar_pixel(6, LINE_HIGH * 2 + 3, 'X');
	ST7565_drawchar_pixel(6, LINE_HIGH * 3 + 4, 'Y');
	ST7565_drawchar_pixel(6, LINE_HIGH * 4 + 5, 'Z');

	sprintf(data, "%6.2f%c", pos[0], '\0');
	ST7565_drawstring_pixel(14, LINE_HIGH * 2 + 3, data);
	sprintf(data, "%6.2f%c", pos[1], '\0');
	ST7565_drawstring_pixel(14, LINE_HIGH * 3 + 4, data);
	sprintf(data, "%6.2f%c", pos[2], '\0');
	ST7565_drawstring_pixel(14, LINE_HIGH * 4 + 5, data);
	sprintf(data, "%+7.2f%c", mov[0], '\0');
	ST7565_drawstring_pixel(52, LINE_HIGH * 2 + 3, data);
	sprintf(data, "%+7.2f%c", mov[1], '\0');
	ST7565_drawstring_pixel(52, LINE_HIGH * 3 + 4, data);
	sprintf(data, "%+7.2f%c", mov[2], '\0');
	ST7565_drawstring_pixel(52, LINE_HIGH * 4 + 5, data);

	ST7565_drawstring_pixel(6, LINE_HIGH * 5 + 6, "apply pos");
	ST7565_drawstring_pixel(6, LINE_HIGH * 6 + 7, "apply mov");

	ST7565_drawstring_pixel(100, LINE_HIGH * 4 + 3, "[mm]");
	ST7565_drawline(0, LINE_HIGH * 2 + 1, 93, LINE_HIGH * 2 + 1, 255);
	ST7565_drawline(12, LINE_HIGH * 1 + 2, 12, LINE_HIGH * 5 + 4, 255);
	ST7565_drawline(50, LINE_HIGH * 1 + 2, 50, LINE_HIGH * 5 + 4, 255);
	ST7565_drawline(94, LINE_HIGH * 1 + 2, 94, LINE_HIGH * 5 + 4, 255);
	draw_coordinate_diagram(100, 5, 255);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + arrowLinePos + 2, 255);
	markVerticalPos = 0;
}

void draw_st4_settings(){
	ST7565_drawstring_line(10, 0, comunicats[C7_SETTINGS]);
	///ST7565_drawstring(6, 1, comunicats[C2_run_program]);
	///ST7565_drawstring(6, 2, comunicats[C3_move_head]);
	///ST7565_drawstring(6, 3, comunicats[C4_settings]);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);

}
