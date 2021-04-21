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

Std_Err init_U24(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr = STD_OK;
    MotorData_EEPROM motor_data;

    cmd->remove = NULL;
    cmd->step = NULL;

    uint8_t val = PARAM_X;
    for(int i=0; i<MOTORS_NUM; ++i, val<<=1)
    {
        if(cmd->used_fields & val)
        {
            stdErr = get_motor_data_EEPROM(settings->eeprom, settings->motor_data_addresses[i],
                &motor_data);
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
            motor_data.step_size = data;

            stdErr = set_motor_data_EEPROM(settings->eeprom, settings->motor_data_addresses[i],
                &motor_data);
            if(stdErr != STD_OK) { return stdErr; }

            settings->motors[i]->settings.step_size = data;
        }
    }

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
