/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Command_Parser
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "_commands.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

Std_Err init_U23(SystemCommand* cmd)
{
    Std_Err stdErr;
    MotorData_EEPROM motor_data;

    cmd->remove = NULL;
    cmd->step = NULL;

    uint8_t val = 0x01;
    for(int i=0; i<MOTORS_NUM; ++i, val<<=1)
    {
        if(cmd->used_fields & val)
        {
            stdErr = get_motor_data_EEPROM(global_systemCmd_settings->eeprom_settings,
                global_systemCmd_settings->motor_data_addresses[i], &motor_data);
            if(stdErr != STD_OK) { return stdErr; }

            int data;
            switch(i)
            {
                case MOTOR_X: data = (int)cmd->data.x; break;
                case MOTOR_Y: data = (int)cmd->data.y; break;
                case MOTOR_Z: data = (int)cmd->data.z; break;
                case MOTOR_E: data = (int)cmd->data.e; break;
                default: return STD_ERROR;
            }
            motor_data.max_speed = data;

            stdErr = set_motor_data_EEPROM(global_systemCmd_settings->eeprom_settings,
                global_systemCmd_settings->motor_data_addresses[i], &motor_data);
            if(stdErr != STD_OK) { return stdErr; }

            global_systemCmd_settings->motors[i].settings.max_speed = data;
        }
    }

    return STD_OK;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
