/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Manager
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Manager.h"
#include "Manager_Communication.h"
#include "A4988_stepstick.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

bool _check_motors_are_on(Motor* motors)
{
    bool flag = false;

    for(int i=0; i<MOTORS_NUM; ++i)
    {
        flag |= motors[i].flags.isOn;
    }

    return flag;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void execute_step(DeviceSettings* settings)
{
    settings->motors_are_on = _check_motors_are_on(*(settings->motors));

    parse_communication_command(settings->buff_comm);
    execute_communication_command(settings->buff_comm, settings->motors_are_on);
    send_communication_command(settings->buff_comm);

    parse_command_SDcard(settings->sd);
    execute_command_SDcard(settings->sd, settings->motors_are_on);
}


/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
