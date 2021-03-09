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
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

bool _check_motors_state(Motor* motors)
{
    bool flag = false;

    for(int i=0; i<MOTORS_NUM; ++i)
    {
        flag |= settings->motors[i]->flags.isOn;
    }

    return flag;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void execute_step(DeviceSettings* settings)
{
    bool motors_state = _check_motors_state(settings->motors);

    parse_communication_command(settings->buff_comm);
    execute_communication_command(settings->buff_comm, motors_state);
    send_communication_command(settings->buff_comm);

    parse_command_SDcard(settings->sd);
    execute_command_SDcard(settings->sd, motors_state);
}


/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
