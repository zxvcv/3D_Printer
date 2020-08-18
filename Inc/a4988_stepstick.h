/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *  a4988_stepstick.h
 *
 *  Created on: 10.10.2019
 *      Author: zxvcv
 * =======================================================================================================
 * COMMENTS:
 *
 * =======================================================================================================
 * EXAMPLE:
 * 		motorInitSettings(&motor1);
 *		motorUpdatePinoutState(&motor1);
 *
 *  	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 *		{
 *			if(motor1.stateStep != OFF)
 *			motorChangeState(&motor1);
 *		}
 *
 *
 *		motor1.stateDirection = CLOCK;
 * 		motor1.stateReset = START;
 * 		motor1.stateSleep = AWAKE;
 *		motor1.changeTimeCounter = motor1.changeTime;
 * 		motor1.stepLeftCounter *= 2;
 *
 * 		motor1.stateStep = LOW;
 * 		motorUpdatePinoutState(&motor1);
 ********************************************************************************************************** */

#ifndef A4988_STEPSTICK_H_
#define A4988_STEPSTICK_H_



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "ProjectTypes.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */

typedef struct RoundingErrorData_Tag{
	int moveError;
	double speedError;
} RoundingErrorData;

typedef struct MotorData_Tag{
	int position; //pos * ACCURACY
	double speed;
	RoundingErrorData err;
} MotorData;

typedef struct MotorSettings_Tag{
	IO_Pin IOreset;
	IO_Pin IOsleep;
	IO_Pin IOdirection;
	IO_Pin IOstep;

	struct{
		unsigned int isOn		:1;		/*(1-on			0-off				)*/
		unsigned int reset		:1;		/*(1-yes		0-no				)*/
		unsigned int sleep		:1;		/*(1-yes		0-no				)*/
		unsigned int stepPhase	:1;		/*(1-high		0-low				)*/
		unsigned int direction	:1;		/*(1-clockwise,	0-counter clockwise	)*/
	}flags;

	struct{
		uint16_t changeTime;
		uint16_t stepLeft;
	}counters;

	uint16_t changeTime;

	struct{
		uint8_t motorNum;

		uint8_t eepromDataAddress;

		double timerFrequency;	//[Hz] timer frequency
		int stepSize;			//[mm] length of move with one motor step (stepSize * ACCURACY)
		bool isReversed;

		double maxSpeed;
		int positionZero; //posZero * ACCURACY
		int positionEnd; //posEnd * ACCURACY
	}device;

	MotorData data;
} MotorSettings;



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DECLARATIONS
 * ####################################################################################################### */

void motorUpdatePins(MotorSettings* settings);

void motorInit(MotorSettings* settings);

Std_Err motorUpdate(MotorSettings* settings);

Std_Err motorSetMove(MotorSettings* settings, double move, RoundingErrorData* roundingError);

Std_Err motorStart(MotorSettings* settings);

Std_Err motorStop(MotorSettings* settings);

bool motorIsOn(MotorSettings* settings);

bool motorGetReset(MotorSettings* settings);

bool motorGetDirection(MotorSettings* settings);

bool motorGetSleep(MotorSettings* settings);

double motorGetTimerFreq(MotorSettings* settings);

double motorGetStepSize(MotorSettings* settings);

MotorData* motorGetData(MotorSettings* settings);



#endif /* A4988_STEPSTICK_H_ */
