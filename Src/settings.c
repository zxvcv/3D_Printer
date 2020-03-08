#include "settings.h"

PrinterSettings printerSettings = {
		.posMode = false,
		.speed = 0.0

};

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