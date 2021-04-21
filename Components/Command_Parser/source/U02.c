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

Std_Err step_U02(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr = STD_ERROR;

    uint8_t val = PARAM_X;
    for(int i=0; i<MOTORS_NUM; ++i, val<<=1)
    {
        if(cmd->used_fields & val)
        {
            uint8_t msgSize = sprintf(settings->msg_buff,
                "%cU02 %c %u %u %u %u %u %u\n",
                '3',
                motor_indentyficator[i],
                settings->motors[i]->flags.isOn,
                settings->motors[i]->flags.reset,
                settings->motors[i]->flags.sleep,
                settings->motors[i]->flags.stepPhase,
                settings->motors[i]->flags.direction,
                settings->motors[i]->flags.reversed);

            stdErr = add_message_to_send(settings->buff_comm,
                settings->msg_buff, msgSize);
            if(stdErr == STD_BUSY_ERROR) { stdErr = STD_OK; }
            if(stdErr != STD_OK) { return stdErr; }
        }
    }

    cmd->step = NULL;
    return stdErr;
}


Std_Err init_U02(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    cmd->remove = NULL;
    cmd->step = step_U02;

    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
