/*
 * ModKB4x4.c
 *
 *  Created on: 12.10.2019
 *      Author: zxvcv
 */

#include "ModKB4x4.h"

KeyboardSettings keyboard = { .IOin1 = { .PORT = KEYBOARD_1I_GPIO_Port, .PIN = KEYBOARD_1I_Pin },
							.IOin2 = { .PORT = KEYBOARD_2I_GPIO_Port, .PIN = KEYBOARD_2I_Pin },
							.IOin3 = { .PORT = KEYBOARD_3I_GPIO_Port, .PIN = KEYBOARD_3I_Pin },
							.IOin4 = { .PORT = KEYBOARD_4I_GPIO_Port, .PIN = KEYBOARD_4I_Pin },
							.IOout1 = { .PORT = KEYBOARD_1O_GPIO_Port, .PIN = KEYBOARD_1O_Pin },
							.IOout2 = { .PORT = KEYBOARD_2O_GPIO_Port, .PIN = KEYBOARD_2O_Pin },
							.IOout3 = { .PORT = KEYBOARD_3O_GPIO_Port, .PIN = KEYBOARD_3O_Pin },
							.IOout4 = { .PORT = KEYBOARD_4O_GPIO_Port, .PIN = KEYBOARD_4O_Pin },
};

void ModKB4x4_initKeyboard(KeyboardSettings* keyboard)
{
	keyboard->oldState = 0xff;
	keyboard->newState = 0xff;
	keyboard->outputState = ~keyboard->newState;
	keyboard->stateChanged = false;

	HAL_GPIO_WritePin(keyboard->IOout1.PORT, keyboard->IOout1.PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(keyboard->IOout2.PORT, keyboard->IOout2.PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(keyboard->IOout3.PORT, keyboard->IOout3.PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(keyboard->IOout4.PORT, keyboard->IOout4.PIN, GPIO_PIN_SET);
}



void ModKB4x4_readButtons(KeyboardSettings* keyboard)
{
	keyboard->newState = 0x00;

	HAL_GPIO_WritePin(keyboard->IOout1.PORT, keyboard->IOout1.PIN, GPIO_PIN_RESET);
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin1.PORT, keyboard->IOin1.PIN) << 0;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin2.PORT, keyboard->IOin2.PIN) << 1;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin3.PORT, keyboard->IOin3.PIN) << 2;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin4.PORT, keyboard->IOin4.PIN) << 3;
	HAL_GPIO_WritePin(keyboard->IOout1.PORT, keyboard->IOout1.PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(keyboard->IOout2.PORT, keyboard->IOout2.PIN, GPIO_PIN_RESET);
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin1.PORT, keyboard->IOin1.PIN) << 4;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin2.PORT, keyboard->IOin2.PIN) << 5;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin3.PORT, keyboard->IOin3.PIN) << 6;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin4.PORT, keyboard->IOin4.PIN) << 7;
	HAL_GPIO_WritePin(keyboard->IOout2.PORT, keyboard->IOout2.PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(keyboard->IOout3.PORT, keyboard->IOout3.PIN, GPIO_PIN_RESET);
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin1.PORT, keyboard->IOin1.PIN) << 8;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin2.PORT, keyboard->IOin2.PIN) << 9;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin3.PORT, keyboard->IOin3.PIN) << 10;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin4.PORT, keyboard->IOin4.PIN) << 11;
	HAL_GPIO_WritePin(keyboard->IOout3.PORT, keyboard->IOout3.PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(keyboard->IOout4.PORT, keyboard->IOout4.PIN, GPIO_PIN_RESET);
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin1.PORT, keyboard->IOin1.PIN) << 12;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin2.PORT, keyboard->IOin2.PIN) << 13;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin3.PORT, keyboard->IOin3.PIN) << 14;
	keyboard->newState |= HAL_GPIO_ReadPin(keyboard->IOin4.PORT, keyboard->IOin4.PIN) << 15;
	HAL_GPIO_WritePin(keyboard->IOout4.PORT, keyboard->IOout4.PIN, GPIO_PIN_SET);
}

void ModKB4x4_checkKeyboard(KeyboardSettings* keyboard)
{
	if(keyboard->newState != keyboard->oldState)
		keyboard->waitCounter = VIBRATION_WAIT_LEN;
	else {
		if(keyboard->waitCounter > 0){
			--keyboard->waitCounter;
			if(keyboard->waitCounter <= 0){
				keyboard->outputState = ~keyboard->newState;
				keyboard->stateChanged = true;
			}
		}
	}
	keyboard->oldState = keyboard->newState;
}

uint16_t ModKB4x4_readKeyboard(KeyboardSettings* keyboard)
{
	keyboard->stateChanged = false;
	return keyboard->outputState;
}

KeyboardButtons ModKB4x4_getButton(uint16_t button)
{
	switch(button)
	{
	case 0x0000: return NONE;
	case 0x0001: return BUTT_1;
	case 0x0002: return BUTT_2;
	case 0x0004: return BUTT_3;
	case 0x0008: return BUTT_A;
	case 0x0010: return BUTT_4;
	case 0x0020: return BUTT_5;
	case 0x0040: return BUTT_6;
	case 0x0080: return BUTT_B;
	case 0x0100: return BUTT_7;
	case 0x0200: return BUTT_8;
	case 0x0400: return BUTT_9;
	case 0x0800: return BUTT_C;
	case 0x1000: return BUTT_S;
	case 0x2000: return BUTT_0;
	case 0x4000: return BUTT_H;
	case 0x8000: return BUTT_D;
	default: return ERR;
	}
}
