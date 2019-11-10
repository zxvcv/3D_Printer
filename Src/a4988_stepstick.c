/*
 * motor.c
 *
 *  Created on: 10.10.2019
 *      Author: zxvcv
 */

#include "a4988_stepstick.h"
#include <stdlib.h>

MotorSettings motor1 = {.IOreset = { .PORT = MOT1_RESET_GPIO_Port, .PIN = MOT1_RESET_Pin },
						.IOsleep = { .PORT = MOT1_SLEEP_GPIO_Port, .PIN = MOT1_SLEEP_Pin },
						.IOdirection = { .PORT = MOT1_DIRECTION_GPIO_Port, .PIN = MOT1_DIRECTION_Pin },
						//.IOstep = { .PORT = MOT1_STEP_GPIO_Port, .PIN = MOT1_STEP_Pin }
						.IOstep = { .PORT = LD2_GPIO_Port, .PIN = LD2_Pin }
};

void  motorInitSettings(MotorSettings* stct)
{
	stct->stateReset = START;
	stct->stateSleep = SLEEP;
	stct->stateDirection = CLOCK;
	stct->stateStep = OFF;

	stct->changeTime = 0;
	stct->changeTimeCounter = 0;
	stct->stepLeftCounter = 0;
}

bool motorChangeState(MotorSettings* settings)
{
	bool returnVal = false;
	--settings->changeTimeCounter;
	if(settings->changeTimeCounter <= 0){
		settings->changeTimeCounter = settings->changeTime;
		--settings->stepLeftCounter;
		if(settings->stepLeftCounter <= 0){
			settings->stateStep = OFF;
			settings->stateSleep = SLEEP;
		}


		switch (settings->stateStep) {
			case HIGH:
				settings->stateStep = LOW;
				break;
			case LOW:
				settings->stateStep = HIGH;
				returnVal = true;
				break;
			default:
				break;
		}
		motorUpdatePinoutState(settings);
		return returnVal;
	}else
		return returnVal;
}

void motorUpdatePinoutState(MotorSettings* settings)
{
	if(settings->IOreset.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOreset.PORT, settings->IOreset.PIN, settings->stateReset);
	if(settings->IOsleep.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOsleep.PORT, settings->IOsleep.PIN, settings->stateSleep);
	if(settings->IOdirection.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOdirection.PORT, settings->IOdirection.PIN, settings->stateDirection);
	if(settings->IOstep.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOstep.PORT, settings->IOstep.PIN, settings->stateStep);
}
//extern UART_HandleTypeDef huart2;
//char data[50];
//uint8_t size;
void setMotorMove(MotorSettings* motor, double move, double speed){
	double stepsNum = abs(move)/STEP_LEN;
	double time = abs(move)*10/speed;
	double changeFreq = TIMER_FREQ*time/stepsNum;

	motor->stateDirection = move > 0 ? CLOCK : RCLOCK;
	motor->stepLeftCounter = stepsNum*2;	//odleglosc (* 2 poniewaz poruszenie silnikiem wymaga jednego impulsu a nie jednego zbocza)
	motor->changeTime = changeFreq/2;		//szybkosc ruchu (/ 2 poniewaz poruszenie silnikiem wymaga jednego impulsu a nie jednego zbocza)
	//size = sprintf(data, "%d %d\n", motor->stepLeftCounter, motor->changeTime);
	//size = sprintf(data, "%f %f\n", move, speed);
	//HAL_UART_Transmit(&huart2, (uint8_t*)data, size, 1000);
}

void motorSetStart(MotorSettings* settings){
	motor1.stateSleep = AWAKE;
	motor1.changeTimeCounter = motor1.changeTime;

	motor1.stateStep = LOW;
}
