#include "Mock_EEPROM_24AA01.h"


Mock_EEPROM_24AA01* Mock_EEPROM_24AA01::mock;

extern "C"
{
    void EEPROM_init(EEPROMSettings *settings, I2C_HandleTypeDef* i2c)
    {
        Mock_EEPROM_24AA01::mock->EEPROM_init(settings, i2c);
    }

    Std_Err EEPROM_clear(EEPROMSettings *settings)
    {
        return Mock_EEPROM_24AA01::mock->EEPROM_clear(settings);
    }

    Std_Err EEPROM_writeData(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size)
    {
        return Mock_EEPROM_24AA01::mock->EEPROM_writeData(settings, address, data, size);
    }

    Std_Err EEPROM_readData(EEPROMSettings *settings, uint8_t address, uint8_t *dataOUT, int size)
    {
        return Mock_EEPROM_24AA01::mock->EEPROM_readData(settings, address, dataOUT, size);
    }
}

