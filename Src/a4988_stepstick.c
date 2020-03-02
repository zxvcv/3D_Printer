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
						.stepSize = 0.203,
						.data.motorNum = 1
};

MotorSettings motor2 = {.IOreset = { .PORT = MOT2_RESET_GPIO_Port, .PIN = MOT2_RESET_Pin },
						.IOsleep = { .PORT = MOT2_SLEEP_GPIO_Port, .PIN = MOT2_SLEEP_Pin },
						.IOdirection = { .PORT = MOT2_DIRECTION_GPIO_Port, .PIN = MOT2_DIRECTION_Pin },
						.IOstep = { .PORT = MOT2_STEP_GPIO_Port, .PIN = MOT2_STEP_Pin },
						.timerFrequency = 1000,
						.stepSize = 0.203,
						.data.motorNum = 2
};

MotorSettings motor3 = {.IOreset = { .PORT = MOT3_RESET_GPIO_Port, .PIN = MOT3_RESET_Pin },
						.IOsleep = { .PORT = MOT3_SLEEP_GPIO_Port, .PIN = MOT3_SLEEP_Pin },
						.IOdirection = { .PORT = MOT3_DIRECTION_GPIO_Port, .PIN = MOT3_DIRECTION_Pin },
						.IOstep = { .PORT = MOT3_STEP_GPIO_Port, .PIN = MOT3_STEP_Pin },
						.timerFrequency = 1000,
						.stepSize = 0.203,
						.data.motorNum = 3
};

MotorSettings motor4 = {.IOreset = { .PORT = MOT4_RESET_GPIO_Port, .PIN = MOT4_RESET_Pin },
						.IOsleep = { .PORT = MOT4_SLEEP_GPIO_Port, .PIN = MOT4_SLEEP_Pin },
						.IOdirection = { .PORT = MOT4_DIRECTION_GPIO_Port, .PIN = MOT4_DIRECTION_Pin },
						.IOstep = { .PORT = MOT4_STEP_GPIO_Port, .PIN = MOT4_STEP_Pin },
						.timerFrequency = 1000,
						.stepSize = 0.203,
						.data.motorNum = 4
};



/* PRIVATE FUNCTIONS DECLARATIONS */
void motorUpdatePins(MotorSettings* settings);


/* FUNCTIONS DEFINITIONS */
void  motorInit(MotorSettings* settings) {
	settings->stateReset = START;
	settings->stateSleep = SLEEP;
	settings->stateDirection = CLOCK;
	settings->stateStep = LOW;

	settings->isOn = false;

	settings->changeTime = 0;
	settings->stepLeftCounter = 0;

	settings->data.positionZero = 0;
	settings->data.positionEnd = 20;
	settings->data.maxSpeed = 50;

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
			settings->isOn = false;
			settings->stateSleep = SLEEP;
		}


		switch (settings->stateStep) {
			case HIGH:
				settings->stateStep = LOW;
				break;
			case LOW:
				settings->stateStep = HIGH;
				if(settings->stateDirection == RCLOCK)
					settings->data.position -= settings->stepSize;
				else
					settings->data.position += settings->stepSize;
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
	RoundingErrorData roundingError;

	if(settings->data.position + move < settings->data.positionZero ||
	   settings->data.position + move > settings->data.positionEnd ||
	   settings->data.speed == 0){
		settings->stepLeftCounter = 0;
		roundingError.roundingMoveError = 0;
		roundingError.roundingSpeedError = 0;
		return roundingError;
	}

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

	roundingError.roundingMoveError = absMove - (settings->stepLeftCounter / 2 * settings->stepSize);
	roundingError.roundingSpeedError = speed - ((settings->timerFrequency * settings->stepSize) / (settings->changeTime * 2));

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
