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
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "main.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

#define ACCURACY 1000

#define HIGH	1
#define LOW		0
#define CLOCKWISE			1
#define COUNTER_CLOCKWISE	0



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

void motorUpdatePins(MotorSettings* settings)
{
	if(settings->IOreset.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOreset.PORT, settings->IOreset.PIN,
				((settings->flags.reset) ? GPIO_PIN_RESET : GPIO_PIN_SET));

	if(settings->IOsleep.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOsleep.PORT, settings->IOsleep.PIN,
				((settings->flags.sleep) ? GPIO_PIN_RESET : GPIO_PIN_SET));

	if(settings->IOdirection.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOdirection.PORT, settings->IOdirection.PIN,
				((settings->flags.direction) ? GPIO_PIN_SET : GPIO_PIN_RESET));

	if(settings->IOstep.PORT != NULL)
		HAL_GPIO_WritePin(settings->IOstep.PORT, settings->IOstep.PIN,
				((settings->flags.stepPhase) ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

void motorInit(MotorSettings* settings)
{
	settings->flags.isOn = 0;
	settings->flags.reset = 0;
	settings->flags.sleep = 1;
	settings->flags.stepPhase = LOW;
	settings->flags.direction = CLOCKWISE;

	settings->changeTime = 0;
	settings->counters.stepLeft = 0;

	settings->data.position = 0;
	settings->data.speed = 0;

	motorUpdatePins(settings);
}

Std_Err motorUpdate(MotorSettings* settings)
{
	Std_Err retVal = STD_OK;

	if(settings->flags.isOn && !settings->flags.reset && !settings->flags.sleep)
	{
		--(settings->counters.changeTime);
		if(settings->counters.changeTime <= 0)
		{
			settings->counters.changeTime = settings->changeTime;
			--(settings->counters.stepLeft);
			if(settings->counters.stepLeft <= 0)
			{
				settings->flags.isOn = 0;
				settings->flags.sleep = 1;
			}

			switch (settings->flags.stepPhase) 
			{
				case HIGH:
					settings->flags.stepPhase = LOW;
					break;
				case LOW:
					settings->flags.stepPhase = HIGH;
					if(settings->flags.direction == CLOCKWISE)
						settings->data.position -= settings->device.stepSize;
					else
						settings->data.position += settings->device.stepSize;
					break;
				default:
					retVal = STD_REFERENCE_ERROR;
					break;
			}

			if(retVal == STD_OK)
			{
				motorUpdatePins(settings);
			}
		}
	}
	else
	{
		retVal = STD_ERROR;
	}

	return retVal;
}

Std_Err motorSetMove(MotorSettings* settings, double move, RoundingErrorData* roundingError)
{
	const int _moveInt = (int)round(move * ACCURACY); //[mm/ACCURACY]
	const int _position = settings->data.position;
	const int _positionZero = settings->device.positionZero;
	const int _positionEnd = settings->device.positionEnd;

	const int _moveSign = (signbit(move) ? -1 : 1);

	const double _speed = settings->data.speed; //[mm/s]
	const int _absMove = abs(_moveInt);
	const int _stepSize = settings->device.stepSize;

	/* speed */
	if(_speed <= 0 || _speed > settings->device.maxSpeed)
	{
		settings->counters.stepLeft = 0;
		settings->counters.changeTime = 0;
		settings->changeTime = 0;
		roundingError->moveError = 0;
		roundingError->speedError = 0;

		return STD_PARAMETER_ERROR;
	}
	double changeFreq = _speed / ((double)_stepSize / ACCURACY);
	double changeTimeD = settings->device.timerFrequency / (changeFreq * 2);

	settings->changeTime = (uint16_t)changeTimeD;
	settings->counters.changeTime = settings->changeTime;

	/* move */
	int stepsNum1 = _absMove / _stepSize;
	int stepsNum2 = stepsNum1 + 1;
	int accuracy1 = (stepsNum1 * _stepSize) - _absMove;
	int accuracy2 = (stepsNum2 * _stepSize) - _absMove;

	int newPosition1 = (stepsNum1 * _stepSize * _moveSign) + _position;
	int newPosition2 = (stepsNum2 * _stepSize * _moveSign) + _position;

	if(newPosition1 > _positionEnd || newPosition1 < _positionZero)
		accuracy1 = INT_MAX;
	if(newPosition2 > _positionEnd || newPosition2 < _positionZero)
		accuracy2 = INT_MAX;

	if(accuracy1 == INT_MAX && accuracy2 == INT_MAX)
	{
		settings->counters.stepLeft = 0;
		settings->counters.changeTime = 0;
		settings->changeTime = 0;
		roundingError->moveError = 0;
		roundingError->speedError = 0;

		return STD_PARAMETER_ERROR;
	}
	else if(abs(accuracy1) <= abs(accuracy2))
	{
		settings->counters.stepLeft = stepsNum1;
		roundingError->moveError = abs(accuracy1) * _moveSign;
	}
	else
	{
		settings->counters.stepLeft = stepsNum2;
		roundingError->moveError = abs(accuracy2) * _moveSign * ((int)-1);
	}

	settings->counters.stepLeft *= 2;
	/*TODO: check calculating speedError*/
	roundingError->speedError = _speed - 
		(((settings->device.timerFrequency * _stepSize) / ACCURACY) / (settings->changeTime * 2));


	/* direction */
	if(_moveSign > 0)
		settings->flags.direction = ((settings->device.isReversed)? CLOCKWISE : COUNTER_CLOCKWISE );
	else
		settings->flags.direction = ((settings->device.isReversed)? COUNTER_CLOCKWISE : CLOCKWISE );

	return STD_OK;
}

Std_Err motorStart(MotorSettings* settings)
{
	if(settings->counters.stepLeft <= 0 || settings->changeTime <= 0)
	{
		return STD_PARAMETER_ERROR;
	}

	if(settings->flags.reset)
	{
		return STD_ERROR;
	}

	if(settings->flags.stepPhase != LOW)
	{
		return STD_ERROR;
	}

	settings->flags.sleep = 0;
	settings->flags.isOn = 1;

	motorUpdatePins(settings);

	return STD_OK;
}

Std_Err motorStop(MotorSettings* settings)
{
	Std_Err retVal = STD_OK;

	if(settings->flags.reset)
	{
		return STD_ERROR;
	}

	settings->flags.isOn = 0;
	settings->flags.sleep = 1;

	if(settings->counters.stepLeft > 0 || settings->changeTime > 0)
	{
		return STD_INTERRUPTED_ERROR;
	}
	
	if(settings->flags.stepPhase != LOW)
	{
		settings->flags.stepPhase = LOW;

		retVal = STD_IO_ERROR;
	}

	motorUpdatePins(settings);

	return retVal;
}

bool motorIsOn(MotorSettings* settings)
{
	return settings->flags.isOn;
}

bool motorGetReset(MotorSettings* settings)
{
	return settings->flags.reset;
}

bool motorGetDirection(MotorSettings* settings)
{
	return settings->flags.direction;
}

bool motorGetSleep(MotorSettings* settings)
{
	return settings->flags.sleep;
}

double motorGetTimerFreq(MotorSettings* settings)
{
	return settings->device.timerFrequency;
}

double motorGetStepSize(MotorSettings* settings)
{
	return settings->device.stepSize;
}

MotorData* motorGetData(MotorSettings* settings)
{
	return &(settings->data);
}
