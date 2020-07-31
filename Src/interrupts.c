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

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */



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
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	//Std_Err stdErr;

	fifo_pop_C(printerSettings.lcd->buffer);
	/*TODO: error handling*/

#ifdef USE_INTERRUPTS
	IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

	int size = fifo_getSize(printerSettings.lcd->buffer);

#ifdef USE_INTERRUPTS
	IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

	if(size > 0)
	{
		ST7565_spiwrite(printerSettings.lcd);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	for(int i=0; i< MOTORS_NUM; ++i)
	{
		if(motorIsOn(printerSettings.motors[i]))
		{
			motorUpdate(printerSettings.motors[i]);
			/*TODO: error handling*/
		}
	}
		
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	//Std_Err stdErr;
	//HAL_StatusTypeDef halErr;
	uint8_t* data;

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

#ifdef USE_INTERRUPTS
	IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

	fifo_pop_C(printerSettings.bt->Buff_Bt_OUT);
	/*TODO: error handling*/

#ifdef USE_INTERRUPTS
	IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

	if(0 == fifo_getSize(printerSettings.bt->Buff_Bt_OUT))
	{
		printerSettings.bt->transmissionBT = false;
	}
	else
	{
		fifo_front(printerSettings.bt->Buff_Bt_OUT, (void**)&data);
		/*TODO: error handling*/

		HAL_UART_Transmit_IT(printerSettings.bt->huart,
						data, fifo_getDataSize(printerSettings.bt->Buff_Bt_OUT));
		/*TODO: error handling*/
	}

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//Std_Err stdErr;
	//HAL_StatusTypeDef halErr;

	if(huart == printerSettings.bt->huart)
	{
		fifo_push_C(printerSettings.bt->Buff_Bt_IN, &(printerSettings.bt->recievedBT), 1);
		/*TODO: error handling*/

		if(printerSettings.bt->recievedBT == '\n')
		{
			printerSettings.bt->EOL_BT_recieved = true;
		}
			
		HAL_UART_Receive_IT(printerSettings.bt->huart, &(printerSettings.bt->recievedBT), 1);
		/*TODO: error handling*/
	}
}
