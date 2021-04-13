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

bool _check_motors_are_on(Motor** motors)
{
    bool flag = false;

    for(int i=0; i<MOTORS_NUM; ++i)
    {
        flag |= motors[i]->flags.isOn;
    }

    return flag;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err execute_step(DeviceSettings* settings)
{
    Std_Err stdErr;

    settings->motors_are_on = _check_motors_are_on(settings->motors);

    stdErr = parse_communication_command(settings->buff_comm);
    if(stdErr != STD_OK) { return stdErr; }
    stdErr = execute_communication_command(settings->buff_comm);
    if(stdErr != STD_OK) { return stdErr; }
    // stdErr = send_communication_command(settings->buff_comm);
    // if(stdErr != STD_OK) { return stdErr; }

    // stdErr = parse_command_SDcard(settings->sd);
    // if(stdErr != STD_OK) { return stdErr; }
    // stdErr = execute_command_SDcard(settings->sd, settings->motors_are_on);

    return stdErr;
}


/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
