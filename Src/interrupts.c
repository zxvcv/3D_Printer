/*
 * interrupts.c
 *
 *  Created on: 04.11.2019
 *      Author: zxvcv
 */

#include "interrupts.h"
#include "FIFO_void.h"
#include "ST7565.h"
#include "a4988_stepstick.h"

extern UART_HandleTypeDef huart1;
extern List* Buff_Bt_IN;
extern List* Buff_Bt_OUT;
extern uint8_t recievedBT;
extern bool EOL_BT_recieved;
extern bool transmissionBT;

//transmisja danych do wyswietlacza ST7565
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
	List_Pop_C(Buff_SPI_ST7565R);
	__disable_irq();
	int size = List_GetSize(Buff_SPI_ST7565R);
	__enable_irq();

	if(size > 0)
		ST7565_spiwrite();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(motorIsOn(&motor1))
		motorUpdate(&motor1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	__disable_irq();
	List_Pop_C(Buff_Bt_OUT);
	__enable_irq();

	if(List_GetSize(Buff_Bt_OUT) == 0)
	{
		transmissionBT = false;
	}
	else
	{
		HAL_UART_Transmit_IT(&huart1, (uint8_t*)List_Front(Buff_Bt_OUT), List_GetDataSize(Buff_Bt_OUT));
	}
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	List_Push_C(Buff_Bt_IN, &recievedBT, 1);

	if(recievedBT == '\n')
		EOL_BT_recieved = true;
	HAL_UART_Receive_IT(&huart1, &recievedBT, 1);
}
