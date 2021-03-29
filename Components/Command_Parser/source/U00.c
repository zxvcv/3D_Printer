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

Std_Err step_U00(SystemCommand* cmd)
{
    Std_Err stdErr = STD_ERROR;

    uint8_t val = 0x01;
    for(int i=0; i<MOTORS_NUM; ++i, val<<=1)
    {
        if(cmd->used_fields & val)
        {
            uint8_t msgSize = sprintf(global_systemCmd_settings->msg_buff,
                "%cU00 %c %d %d\n",
                '3',
                'X', // TODO: motor indentyficator (X, Y, Z, E)
                global_systemCmd_settings->motors[0]->data.position, // TODO: motor number not only 0
                global_systemCmd_settings->motors[0]->data.position_error);  // TODO: motor number not only 0

            stdErr = fifo_push_C(global_systemCmd_settings->buff_comm->Buff_OUT,
                (char*)global_systemCmd_settings->msg_buff, msgSize);

            if(stdErr != STD_OK) { return stdErr; }
        }
    }

    return stdErr;
}


Std_Err init_U00(SystemCommand* cmd)
{
    cmd->remove = NULL;
    cmd->step = step_U00;

    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
