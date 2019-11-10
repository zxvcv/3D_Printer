/*
 * interrupts.c
 *
 *  Created on: 04.11.2019
 *      Author: zxvcv
 */

#include "interrupts.h"
#include "FIFO_struct.h"
#include "ST7565.h"
#include "ModKB4x4.h"
#include "a4988_stepstick.h"
#include "menu.h"

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
	ListStruct_Pop_C(Buff_SPI_ST7565R);
	__disable_irq();
	int size = ListStruct_GetSize(Buff_SPI_ST7565R);
	__enable_irq();

	if(size > 0)
		ST7565_spiwrite();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(motor1.stateStep != OFF){
		//os Y
		if(motorChangeState(&motor1)){
			if(motor1.stateDirection == CLOCK)
				printerSettings.position[1] += STEP_LEN;
			else
				printerSettings.position[1] -= STEP_LEN;
		}

	}


	ModKB4x4_readButtons(&keyboard);
	ModKB4x4_checkKeyboard(&keyboard);
}
