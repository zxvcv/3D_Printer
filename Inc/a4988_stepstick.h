/*
 * a4988_stepstick.h
 *
 *  Created on: 10.10.2019
 *      Author: zxvcv
 *
 *
 * ******************
 * 		EXAMPLE
 * ******************
 *
 *	motorInitSettings(&motor1);
 *	motorUpdatePinoutState(&motor1);
 *
 *  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 *	{
 *		if(motor1.stateStep != OFF)
 *		motorChangeState(&motor1);
 *	}
 *
 *
 *	motor1.stateDirection = CLOCK;
 * 	motor1.stateReset = START;
 * 	motor1.stateSleep = AWAKE;
 *	motor1.changeTimeCounter = motor1.changeTime;
 * 	motor1.stepLeftCounter *= 2;
 *
 * 	motor1.stateStep = LOW;
 * 	motorUpdatePinoutState(&motor1);
 *
 *
 */

#ifndef A4988_STEPSTICK_H_
#define A4988_STEPSTICK_H_

#include "main.h"
#include <stdbool.h>
#include <stdint.h>
#include "pin_struct.h"

#define MOTORS_NUM 4
#define ACCURACY 1000

typedef enum MOTOR_STEP_FAZE{
	HIGH = GPIO_PIN_SET,
	LOW = GPIO_PIN_RESET,
}MOTOR_STEP_FAZE;

typedef enum MOTOR_DIRECTION_FAZE{
	CLOCK = GPIO_PIN_SET, //zgodnie ze wskaz�wkami zegara
	RCLOCK = GPIO_PIN_RESET //przeciwnie do ruchu wskazowek zegara (reverse clock)
}MOTOR_DIRECTION_FAZE;

typedef enum MOTOR_SLEEP_FAZE{
	AWAKE = GPIO_PIN_SET,
	SLEEP = GPIO_PIN_RESET
}MOTOR_SLEEP_FAZE;

typedef enum MOTOR_RESET_FAZE{
	START = GPIO_PIN_SET,
	STOP = GPIO_PIN_RESET
}MOTOR_RESET_FAZE;

typedef struct RoundingErrorData{
	double roundingMoveError;
	double roundingSpeedError;
	bool errMove;
} RoundingErrorData;

typedef struct MotorData{
	uint8_t motorNum;

	int position; //pos * ACCURACY
	RoundingErrorData err;
	double speed;

	double maxSpeed;
	int positionZero; //posZero * ACCURACY
	int positionEnd; //posEnd * ACCURACY
} MotorData;

typedef struct MotorSettings{
	IO_Pin IOreset;
	IO_Pin IOsleep;
	IO_Pin IOdirection;
	IO_Pin IOstep;

	bool isOn;
	MOTOR_STEP_FAZE stateStep;
	bool isReversed;
	MOTOR_DIRECTION_FAZE stateDirection;
	MOTOR_SLEEP_FAZE stateSleep;
	MOTOR_RESET_FAZE stateReset;

	uint16_t changeTime;
	uint16_t changeTimeCounter;
	uint16_t stepLeftCounter;

	double timerFrequency;	//[Hz] timer frequency
	int stepSize;			//[mm] length of move with one motor step (stepSize * ACCURACY)

	uint8_t eepromDataAddress;

	MotorData data;
} MotorSettings;

extern MotorSettings motors[MOTORS_NUM];


void  motorInit(MotorSettings* settings);

bool motorUpdate(MotorSettings* settings);

RoundingErrorData motorSetMove(MotorSettings* settings, double move);

void motorStart(MotorSettings* settings);

void motorStop(MotorSettings* settings);

bool motorIsOn(MotorSettings* settings);

MOTOR_RESET_FAZE motorGetReset(MotorSettings* settings);

MOTOR_DIRECTION_FAZE motorGetDirection(MotorSettings* settings);

MOTOR_SLEEP_FAZE motorGetSleep(MotorSettings* settings);

double motorGetTimerFreq(MotorSettings* settings);

double motorGetStepSize(MotorSettings* settings);

MotorData* motorGetData(MotorSettings* settings);

#endif /* A4988_STEPSTICK_H_ */
