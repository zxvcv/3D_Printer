/*
 * ModKB4x4.h
 *
 *  Created on: 12.10.2019
 *      Author: zxvcv
 *
 * ******************
 * 		EXAMPLE
 * ******************
 *
 *		ModKB4x4_initKeyboard(&keyboard);
 * 	  	uint16_t state;
 * 	  	HAL_TIM_Base_Start_IT(&htim6);
 *
 *      void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 *		{
 *			ModKB4x4_readButtons(&keyboard);
 *			ModKB4x4_checkKeyboard(&keyboard);
 *		}
 *
 *		while (1)
 * 	  	{
 *	    	if(keyboard.stateChanged){
 *		  		state = ModKB4x4_readKeyboard(&keyboard);
 *		  		KeyboardButtons butt = ModKB4x4_getButton(state);
 *		  	}
 * 	  	}
 */

#ifndef MODKB4X4_H_
#define MODKB4X4_H_

#include "main.h"
#include "pin_struct.h"
#include <stdbool.h>

#define VIBRATION_WAIT_LEN 20

typedef enum KeyboardButtons{
	BUTT_1 = 0,  BUTT_2 = 1,  BUTT_3 = 2,  BUTT_A = 3,
	BUTT_4 = 4,  BUTT_5 = 5,  BUTT_6 = 6,  BUTT_B = 7,
	BUTT_7 = 8,  BUTT_8 = 9,  BUTT_9 = 10, BUTT_C = 11,
	BUTT_S = 12, BUTT_0 = 13, BUTT_H = 14, BUTT_D = 15,
	NONE = 16, ERR = 17
}KeyboardButtons;

typedef struct KeyboardSettings{
	IO_Pin IOin1;
	IO_Pin IOin2;
	IO_Pin IOin3;
	IO_Pin IOin4;
	IO_Pin IOout1;
	IO_Pin IOout2;
	IO_Pin IOout3;
	IO_Pin IOout4;

	//[[1][2][3][A]]
	//[[4][5][6][B]]
	//[[7][8][9][C]]
	//[[*][0][#][D]]
	uint16_t oldState;
	uint16_t newState;

	int waitCounter;

	uint16_t outputState;
	bool stateChanged;
}KeyboardSettings;

extern KeyboardSettings keyboard;


void ModKB4x4_initKeyboard(KeyboardSettings* keyboard);

void ModKB4x4_readButtons(KeyboardSettings* keyboard);

void ModKB4x4_checkKeyboard(KeyboardSettings* keyboard);

uint16_t ModKB4x4_readKeyboard(KeyboardSettings* keyboard);

KeyboardButtons ModKB4x4_getButton(uint16_t button);

#endif /* MODKB4X4_H_ */
