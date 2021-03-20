#include "Mock_Manager_EEPROM.h"


Mock_Manager_EEPROM* Mock_Manager_EEPROM::mock;

extern "C"
{
    Std_Err get_motor_data_EEPROM(EEPROMSettings* eeprom_settigns, uint8_t address, MotorData_EEPROM* dataOUT)
    {
        return Mock_Manager_EEPROM::mock->get_motor_data_EEPROM(eeprom_settigns, address, dataOUT);
    }

    Std_Err set_motor_data_EEPROM(EEPROMSettings *eeprom_settigns, uint8_t address, MotorData_EEPROM *data)
    {
        return Mock_Manager_EEPROM::mock->set_motor_data_EEPROM(eeprom_settigns, address, data);
    }
}
