#include "settings.h"

PrinterSettings printerSettings = {
		.posMode = false,
		.speed = 0.0,

		.errMove = false
};


void clearAllMotorsRoundingErrors(PrinterSettings *printerSettings){
	motors[0].data.err.roundingMoveError = 0.0;
	motors[0].data.err.roundingSpeedError = 0.0;
	motors[0].data.err.errMove = false;
	motors[1].data.err.roundingMoveError = 0.0;
	motors[1].data.err.roundingSpeedError = 0.0;
	motors[1].data.err.errMove = false;
	motors[2].data.err.roundingMoveError = 0.0;
	motors[2].data.err.roundingSpeedError = 0.0;
	motors[2].data.err.errMove = false;
	motors[3].data.err.roundingMoveError = 0.0;
	motors[3].data.err.roundingSpeedError = 0.0;
	motors[3].data.err.errMove = false;

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
