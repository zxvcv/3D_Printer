#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "EEPROM_24AA01.h"
}

class Mock_EEPROM_24AA01 {
public:
    Mock_EEPROM_24AA01() { mock = this; }
    MOCK_METHOD2(EEPROM_init, void(EEPROMSettings *settings, I2C_HandleTypeDef* i2c));
    MOCK_METHOD1(EEPROM_clear, Std_Err(EEPROMSettings *settings));
    MOCK_METHOD4(EEPROM_writeData, Std_Err(EEPROMSettings *settings, uint8_t address,
        uint8_t *data, int size));
    MOCK_METHOD4(EEPROM_readData, Std_Err(EEPROMSettings *settings, uint8_t address,
        uint8_t *dataOUT, int size));

    static Mock_EEPROM_24AA01* mock;
};
