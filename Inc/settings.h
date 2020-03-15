#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdbool.h>
#include "a4988_stepstick.h"
#include "EEPROM_24AA01.h"

typedef struct PrinterSettings{
	//Positioning Mode:  Absolute-true, Relative-false
	bool posMode;

	//errors
	//RoundingErrorData errMotor1;
	//RoundingErrorData errMotor2;
	//RoundingErrorData errMotor3;
	//RoundingErrorData errMotor4;
	bool errMove;

	//Speed Value
	double speed;
} PrinterSettings;

typedef struct MotorData_EEPROM{
	double maxSpeed;

	int stepSize;

	int positionZero;
	int positionEnd;
}MotorData_EEPROM;

#define _OFFSET_MAXSPEED 0
#define _OFFSET_STEPSIZE _OFFSET_MAXSPEED+sizeof(double)
#define _OFFSET_POSITIONZERO _OFFSET_STEPSIZE+sizeof(int)
#define _OFFSET_POSITIONEND _OFFSET_POSITIONZERO+sizeof(int)

extern PrinterSettings printerSettings;


void clearAllMotorsRoundingErrors(PrinterSettings *printerSettings);

void getMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns);

void setMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns, MotorData_EEPROM *data);


#endif // SETTINGS_H_
