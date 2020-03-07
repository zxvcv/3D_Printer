#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdbool.h>
#include "a4988_stepstick.h"
#include "EEPROM_24AA01.h"

typedef struct PrinterSettings{
	//Positioning Mode:  Absolute-true, Relative-false
	bool posMode;

	//Speed Value
	double speed;
} PrinterSettings;

#define _OFFSET_STEPSIZE 0
#define _OFFSET_MAXSPEED 8
#define _OFFSET_POSITIONZERO 16
#define _OFFSET_POSITIONEND 24

typedef struct MotorData_EEPROM{
	double stepSize;

	double maxSpeed;
	double positionZero;
	double positionEnd;
}MotorData_EEPROM;

extern PrinterSettings printerSettings;



void getMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns);

void setMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns, MotorData_EEPROM *data);


#endif // SETTINGS_H_
