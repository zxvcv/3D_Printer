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
						.IOstep = { .PORT = LD2_GPIO_Port, .PIN = LD2_Pin },
						.timerFrequency = 1000,
						.stepSize = 0.5
};


/* PRIVATE FUNCTIONS DECLARATIONS */
void motorUpdatePins(MotorSettings* settings);


/* FUNCTIONS DEFINITIONS */
void  motorInit(MotorSettings* settings) {
	settings->stateReset = START;
	settings->stateSleep = SLEEP;
	settings->stateDirection = CLOCK;
	settings->stateStep = HIGH;

	settings->isOn = false;

	settings->changeTime = 0;
	settings->stepLeftCounter = 0;

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

void motorSetMove(MotorSettings* settings, double move, double speed){ //bledne obliczenia (naprawic)
	double stepsNum = abs(move) / settings->stepSize;
	double time = abs(move) * 10 / speed;
	double changeFreq = settings->timerFrequency * time / stepsNum;

	settings->stateDirection = move > 0 ? CLOCK : RCLOCK;
	settings->stepLeftCounter = stepsNum * 2;	//odleglosc (* 2 poniewaz poruszenie silnikiem wymaga jednego impulsu a nie jednego zbocza)
	settings->changeTime = changeFreq / 2;		//szybkosc ruchu (/ 2 poniewaz poruszenie silnikiem wymaga jednego impulsu a nie jednego zbocza)
	settings->changeTimeCounter = settings->changeTime;
}

void motorStart(MotorSettings* settings){
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

bool motorIsOn(MotorSettings* settigns){
	return settigns->isOn;
}

MOTOR_RESET_FAZE motorGetReset(MotorSettings* settigns){
	return settigns->stateReset;
}

MOTOR_DIRECTION_FAZE motorGetDirection(MotorSettings* settigns){
	return settigns->stateDirection;
}

MOTOR_SLEEP_FAZE motorGetSleep(MotorSettings* settigns){
	return settigns->stateSleep;
}

double motorGetTimerFreq(MotorSettings* settigns){
	return settigns->timerFrequency;
}

double motorGetStepSize(MotorSettings* settigns){
	return settigns->stepSize;
}
