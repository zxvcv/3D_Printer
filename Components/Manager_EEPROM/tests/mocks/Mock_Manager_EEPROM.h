#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Manager_EEPROM.h"
}

class Mock_Manager_EEPROM
{
public:
    Mock_Manager_EEPROM() { mock = this; }
    MOCK_METHOD3(get_motor_data_EEPROM, Std_Err(EEPROMSettings* eeprom_settigns, uint8_t address,
        MotorData_EEPROM* dataOUT));
    MOCK_METHOD3(set_motor_data_EEPROM, Std_Err(EEPROMSettings *eeprom_settigns, uint8_t address,
        MotorData_EEPROM *data));

    static Mock_Manager_EEPROM* mock;
};
