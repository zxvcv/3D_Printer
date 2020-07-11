/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *
 *  Created on: 10.10.2019
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

#include "a4988_stepstick.h"
#include "main.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

#define ACCURACY 1000



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *											PUBLIC OBJECTS
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

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

/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

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

RoundingErrorData motorSetMove(MotorSettings* settings, double move){
	RoundingErrorData roundingError;
	int moveInt = (int)round(move * 1000);

	if(settings->data.position + moveInt < settings->data.positionZero || settings->data.position + moveInt > settings->data.positionEnd){
		settings->stepLeftCounter = 0;
		roundingError.roundingMoveError = 0;
		roundingError.roundingSpeedError = 0;
		roundingError.errMove = true;
		return roundingError;
	}

	double speed = settings->data.speed; //w [mm/s]
	int absMove = abs(moveInt);
	//int absMove2 = absMove + settings->stepSize;

	int stepsNum = absMove / settings->stepSize;
	double changeFreq = speed / ((double)settings->stepSize / ACCURACY);


	double changeTimeD = settings->timerFrequency / (changeFreq * 2);

	settings->stateDirection = move > 0 ? CLOCK : RCLOCK;
	settings->changeTime = (uint16_t)changeTimeD;
	settings->stepLeftCounter = stepsNum;
	settings->stepLeftCounter *= 2;
	settings->changeTimeCounter = settings->changeTime;

	roundingError.roundingMoveError = absMove - (settings->stepLeftCounter / 2 * settings->stepSize);
	if(settings->stateDirection == RCLOCK) roundingError.roundingMoveError *= -1;
	roundingError.roundingSpeedError = speed - (((settings->timerFrequency * settings->stepSize) / ACCURACY) / (settings->changeTime * 2));
	roundingError.errMove = false;

	if(settings->isReversed) settings->stateDirection = settings->stateDirection == CLOCK ? RCLOCK : CLOCK;
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
