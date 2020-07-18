/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *  interrupts.c
 *
 *  Created on: 04.11.2019
 *      Author: zxvcv
 * =======================================================================================================
 * COMMENTS:
 *
 * =======================================================================================================
 * EXAMPLE:
 *
 ********************************************************************************************************** */



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include "interrupts.h"
#include "ProjectObjects.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */

extern DeviceSettings printerSettings; /* manager.h */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

//transmisja danych do wyswietlacza ST7565
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
	Fifo_Err errors;

	list_pop_C(printerSettings.lcd->buffer, &errors);
	__disable_irq();
	int size = list_getSize(printerSettings.lcd->buffer, &errors);
	__enable_irq();

	if(size > 0)
		ST7565_spiwrite(printerSettings.lcd);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	for(int i=0; i< MOTORS_NUM; ++i)
		if(motorIsOn(printerSettings.motors[i]))
			motorUpdate(printerSettings.motors[i]);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	Fifo_Err errors;

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	__disable_irq();
	list_pop_C(printerSettings.bt->Buff_Bt_OUT, &errors);
	__enable_irq();

	if(list_getSize(printerSettings.bt->Buff_Bt_OUT, &errors) == 0)
	{
		printerSettings.bt->transmissionBT = false;
	}
	else
	{
		HAL_UART_Transmit_IT(printerSettings.bt->huart, (uint8_t*)list_front(printerSettings.bt->Buff_Bt_OUT, &errors), list_getDataSize(printerSettings.bt->Buff_Bt_OUT, &errors));
	}
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	Fifo_Err errors;

	if(huart == printerSettings.bt->huart){
		list_push_C(printerSettings.bt->Buff_Bt_IN, &(printerSettings.bt->recievedBT), 1, &errors);

		if(printerSettings.bt->recievedBT == '\n')
			printerSettings.bt->EOL_BT_recieved = true;
		HAL_UART_Receive_IT(printerSettings.bt->huart, &(printerSettings.bt->recievedBT), 1);
	}
}
