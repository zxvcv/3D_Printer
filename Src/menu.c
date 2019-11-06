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
	C7_SETTINGS
}COMUNICAT;

char* comunicats[] = {
		"error",
		"MAIN MENU",
		"run program",
		"move head",
		"settings",
		"RUN PROGRAM",
		"MOVE HEAD",
		"SETTINGS"
};

PROGRAM_STATE actualPrgState;
uint8_t arrowLinePos;

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
			if(buttState == BUTT_B){
				ST7565_clear();
				draw_st1_main_menu();
				ST7565_display();
				actualPrgState = ST1_MAIN_MENU;
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

void draw_st1_main_menu() {
	ST7565_drawstring(10, 0, comunicats[C1_MAIN_MENU]);
	ST7565_drawstring(6, 1, comunicats[C2_run_program]);
	ST7565_drawstring(6, 2, comunicats[C3_move_head]);
	ST7565_drawstring(6, 3, comunicats[C4_settings]);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
}

void draw_st2_run_program(){
	ST7565_drawstring(10, 0, comunicats[C5_RUN_PROGRAM]);
	//ST7565_drawstring(6, 1, comunicats[C2_run_program]);
	//ST7565_drawstring(6, 2, comunicats[C3_move_head]);
	//ST7565_drawstring(6, 3, comunicats[C4_settings]);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
}

void draw_st3_move_head(){
	ST7565_drawstring(10, 0, comunicats[C6_MOVE_HEAD]);
	//ST7565_drawstring(6, 1, comunicats[C2_run_program]);
	//ST7565_drawstring(6, 2, comunicats[C3_move_head]);
	//ST7565_drawstring(6, 3, comunicats[C4_settings]);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
}

void draw_st4_settings(){
	ST7565_drawstring(10, 0, comunicats[C7_SETTINGS]);
	//ST7565_drawstring(6, 1, comunicats[C2_run_program]);
	//ST7565_drawstring(6, 2, comunicats[C3_move_head]);
	//ST7565_drawstring(6, 3, comunicats[C4_settings]);
	arrowLinePos = 1;
	draw_arrow(0, arrowLinePos * LINE_HIGH + 1, 255);
}
