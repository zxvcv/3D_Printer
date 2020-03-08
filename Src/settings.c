#include "settings.h"

PrinterSettings printerSettings = {
		.posMode = false,
		.speed = 0.0,

		.errMotor1 = { 0.0, 0.0 },
		.errMotor2 = { 0.0, 0.0 },
		.errMotor3 = { 0.0, 0.0 },
		.errMotor4 = { 0.0, 0.0 }
};


void clearAllMotorsRoundingErrors(PrinterSettings *printerSettings){
	printerSettings->errMotor1.roundingMoveError = 0.0;
	printerSettings->errMotor1.roundingSpeedError = 0.0;
	printerSettings->errMotor2.roundingMoveError = 0.0;
	printerSettings->errMotor2.roundingSpeedError = 0.0;
	printerSettings->errMotor3.roundingMoveError = 0.0;
	printerSettings->errMotor3.roundingSpeedError = 0.0;
	printerSettings->errMotor4.roundingMoveError = 0.0;
	printerSettings->errMotor4.roundingSpeedError = 0.0;
}

void getMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns){
	MotorData_EEPROM data;
	EEPROM_readData(memSettigns, motSettings->eepromDataAddress, (uint8_t*)(&data), sizeof(MotorData_EEPROM));
	motSettings->stepSize = data.stepSize;
	motSettings->data.maxSpeed = data.maxSpeed;
	motSettings->data.positionZero = data.positionZero;
	motSettings->data.positionEnd = data.positionEnd;
}

void setMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns, MotorData_EEPROM *data){
	EEPROM_writeData(memSettigns, motSettings->eepromDataAddress, (uint8_t*)data, sizeof(MotorData_EEPROM));
}
