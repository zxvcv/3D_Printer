/*
 * 24AA01_EEPROM.h
 *
 *  Created on: 06.03.2020
 *      Author: zxvcv
 *
 *
 * ******************
 * 		EXAMPLE
 * ******************
 *
 * ...
 */

#ifndef EEPROM_24AA01_H_
#define EEPROM_24AA01_H_

#include <stdbool.h>

typedef struct EEPROMSettings{
	bool isReady;

	I2C_HandleTypeDef* i2c;
} EEPROMSettings;

extern EEPROMSettings eeprom;


void EEPROM_clear(EEPROMSettings *settings);

void EEPROM_writeData(EEPROMSettings *settings, uint16_t address, uint8_t *data, int size);

void EEPROM_readData(EEPROMSettings *settings, uint16_t address, uint8_t *dataOUT, int size);

#endif //EEPROM_24AA01_H_
