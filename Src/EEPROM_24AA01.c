/*
 * 24AA01_EEPROM.c
 *
 *  Created on: 06.03.2020
 *      Author: ppisk
 */

#include "main.h"
#include "EEPROM_24AA01.h"

#define _24AA01_ADDRESS 0xa0

#define _24AA01_READ 0x01
#define _24AA01_WRITE 0x00

#define _24AA01_FIRST_ADDRESS 0x00
#define _24AA01_LAST_ADDRESS 0x7F
#define _24AA01_MEM_SIZE 128 //Bytes
#define _24AA01_PAGE_SIZE 8 //Bytes

extern I2C_HandleTypeDef hi2c1;

EEPROMSettings eeprom = {
		.isReady = true,
		.i2c = &hi2c1
};

void EEPROM_clear(EEPROMSettings *settings){
	uint8_t clsTab[_24AA01_PAGE_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t offset = 0;

	for(int i=0; i<_24AA01_MEM_SIZE/_24AA01_PAGE_SIZE; ++i){
		while(HAL_I2C_Mem_Write(&hi2c1, 0xa0, offset, 1, clsTab, _24AA01_PAGE_SIZE, HAL_MAX_DELAY) != HAL_OK);
		offset += _24AA01_PAGE_SIZE;
	}
}

void EEPROM_writeData(EEPROMSettings *settings, uint16_t address, uint8_t *data, int size){
	if(size <= 0) return;
	if(address + size > _24AA01_MEM_SIZE) size = _24AA01_MEM_SIZE - address;

	uint8_t offset = 0;
	uint8_t sendSize = _24AA01_PAGE_SIZE - (address % _24AA01_PAGE_SIZE);
	if(sendSize == 0) sendSize = _24AA01_PAGE_SIZE;
	if(sendSize > size) sendSize = size;

	while(HAL_I2C_Mem_Write(settings->i2c, _24AA01_ADDRESS, address, 1, data, sendSize, HAL_MAX_DELAY) != HAL_OK);
	offset += sendSize;
	size -= sendSize;

	while(size > 0){
		while(HAL_I2C_Mem_Write(settings->i2c, _24AA01_ADDRESS, address + offset, 1, data + offset, size > _24AA01_PAGE_SIZE ? _24AA01_PAGE_SIZE : size, HAL_MAX_DELAY) != HAL_OK);
		offset += _24AA01_PAGE_SIZE; size -= _24AA01_PAGE_SIZE;
	}
}

void EEPROM_readData(EEPROMSettings *settings, uint16_t address, uint8_t *data, int size){
	if(size <= 0) return;
	if(address + size > _24AA01_MEM_SIZE) size = _24AA01_MEM_SIZE - address;
	HAL_I2C_Mem_Read(settings->i2c, _24AA01_ADDRESS, address, 1, data, size, HAL_MAX_DELAY);
}
