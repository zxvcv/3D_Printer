/*
 * motor.c
 *
 *  Created on: 10.10.2019
 *      Author: zxvcv
 */

#include "a4988_stepstick.h"
#include <stdlib.h>
#include <math.h>

MotorSettings motor1 = {.IOreset = { .PORT = MOT1_RESET_GPIO_Port, .PIN = MOT1_RESET_Pin },
						.IOsleep = { .PORT = MOT1_SLEEP_GPIO_Port, .PIN = MOT1_SLEEP_Pin },
						.IOdirection = { .PORT = MOT1_DIRECTION_GPIO_Port, .PIN = MOT1_DIRECTION_Pin },
						.IOstep = { .PORT = MOT1_STEP_GPIO_Port, .PIN = MOT1_STEP_Pin },
						//.IOstep = { .PORT = LD2_GPIO_Port, .PIN = LD2_Pin },
						.timerFrequency = 1000,
						.stepSize = 0.203
};

/* PRIVATE FUNCTIONS DECLARATIONS */
void motorUpdatePins(MotorSettings* settings);


/* FUNCTIONS DEFINITIONS */
void  motorInit(MotorSettings* settings) {
	settings->stateReset = START;
	settings->stateSleep = SLEEP;
	settings->stateDirection = CLOCK;
	settings->stateStep = HIGH; //test zmienilem na HIGH z LOW

	settings->isOn = false;

	settings->changeTime = 0;
	settings->stepLeftCounter = 0;

	settings->data.positionZero = 0;
	settings->data.positionEnd = 20;
	settings->data.maxSpeed = 200;

	settings->data.position = 0;
	settings->data.speed = 0;

	motorUpdatePins(settings);
}

bool motorUpdate(MotorSettings* settings) {
	bool returnVal = false;
	--settings->changeTimeCounter;
	if(settings->changeTimeCounter <= 0){
		settings->changeTimeCounter = settings->changeTime;
		--settings->stepLeftCounter;
		if(settings->stepLeftCounter <= 0){
			settings->stateStep = HIGH;
			settings->isOn = false;
			settings->stateSleep = SLEEP;
			returnVal = true;

			if(settings->stateDirection == RCLOCK)
				settings->data.position += settings->stepSize;
			else
				settings->data.position -= settings->stepSize;
		}


		switch (settings->stateStep) {
			case HIGH:
				settings->stateStep = LOW;
				break;
			case LOW:
				settings->stateStep = HIGH;
				if(settings->stateDirection == RCLOCK)
					settings->data.position += settings->stepSize;
				else
					settings->data.position -= settings->stepSize;
				returnVal = true;
				break;
			default:
				break;
		}
		motorUpdatePins(settings);
		return returnVal;
	}else
		return returnVal;
}

void motorUpdatePins(MotorSettings* settings) {
	if(settings->IOreset.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOreset.PORT, settings->IOreset.PIN, settings->stateReset);
	if(settings->IOsleep.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOsleep.PORT, settings->IOsleep.PIN, settings->stateSleep);
	if(settings->IOdirection.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOdirection.PORT, settings->IOdirection.PIN, settings->stateDirection);
	if(settings->IOstep.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOstep.PORT, settings->IOstep.PIN, settings->stateStep);
}

RoundingErrorData motorSetMove(MotorSettings* settings, double move){
	double speed = settings->data.speed; //w [mm/s]
	double absMove = fabs(move);
	double stepsNum = absMove / settings->stepSize;
	//double time = absMove / speed;
	double changeFreq = speed / settings->stepSize;

	double changeTimeD = settings->timerFrequency / (changeFreq * 2);
	double stepLeftCounterD = stepsNum;

	settings->stateDirection = move > 0 ? CLOCK : RCLOCK;
	settings->changeTime = changeTimeD;
	settings->stepLeftCounter = stepLeftCounterD;
	settings->stepLeftCounter *= 2;
	settings->changeTimeCounter = settings->changeTime;

	RoundingErrorData roundingError;
	roundingError.roundingMoveError = absMove - (settings->stepLeftCounter / 2 * settings->stepSize);
	roundingError.roundingSpeedError = speed - ((settings->timerFrequency * settings->stepSize) / (settings->changeTime * 2));
	//char data2[30];
	//uint8_t sizeData2 = sprintf(data2, "%d | %d | %f | %f\n", settings->changeTime, settings->stepLeftCounter, roundingError.roundingMoveError, roundingError.roundingSpeedError);
	//extern UART_HandleTypeDef huart2;
	//HAL_UART_Transmit(&huart2, (uint8_t*)data2, sizeData2, 1000);
	return roundingError;
}

void motorStart(MotorSettings* settings){
	if(settings->stepLeftCounter <= 0 || settings->changeTime <= 0)
		return;

	settings->stateSleep = AWAKE;

	settings->stateStep = LOW;
	motorUpdatePins(settings);
	settings->isOn = true;
}

void motorStop(MotorSettings* settings){
	settings->isOn = false;
	settings->stateSleep = SLEEP;
	motorUpdatePins(settings);
}

bool motorIsOn(MotorSettings* settings){
	return settings->isOn;
}

MOTOR_RESET_FAZE motorGetReset(MotorSettings* settings){
	return settings->stateReset;
}

MOTOR_DIRECTION_FAZE motorGetDirection(MotorSettings* settings){
	return settings->stateDirection;
}

MOTOR_SLEEP_FAZE motorGetSleep(MotorSettings* settings){
	return settings->stateSleep;
}

double motorGetTimerFreq(MotorSettings* settings){
	return settings->timerFrequency;
}

double motorGetStepSize(MotorSettings* settings){
	return settings->stepSize;
}

MotorData* motorGetData(MotorSettings* settings){
	return &(settings->data);
}
