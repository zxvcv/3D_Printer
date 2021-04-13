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

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err step_U01(SystemCommand* cmd)
{
    Std_Err stdErr = STD_ERROR;

    uint8_t val = 0x01;
    for(int i=0; i<MOTORS_NUM; ++i, val<<=1)
    {
        if(cmd->used_fields & val)
        {
            uint8_t msgSize = sprintf(global_systemCmd_settings.msg_buff,
                "%cU01 %c %f %d %f %d %d\n",
                '3',
                motor_indentyficator[i],
                global_systemCmd_settings.motors[i]->settings.timer_frequency,
                global_systemCmd_settings.motors[i]->settings.step_size,
                global_systemCmd_settings.motors[i]->settings.max_speed,
                global_systemCmd_settings.motors[i]->settings.position_zero,
                global_systemCmd_settings.motors[i]->settings.position_end);

            stdErr = add_message_to_send(global_systemCmd_settings.buff_comm,
                global_systemCmd_settings.msg_buff, msgSize);
            if(stdErr == STD_BUSY_ERROR) { stdErr = STD_OK; }
            if(stdErr != STD_OK) { return stdErr; }
        }
    }

    cmd->step = NULL;
    return stdErr;
}


Std_Err init_U01(SystemCommand* cmd)
{
    cmd->remove = NULL;
    cmd->step = step_U01;

    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
