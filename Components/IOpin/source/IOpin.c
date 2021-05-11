/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: IOpin
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "IOpin.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define VIBRATIONS_DELAY_STEPS 2
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void IOpin_init(IO_Pin* settings, GPIO_TypeDef* port, uint16_t pin)
{
    settings->PORT = port;
    settings->PIN = pin;
}


void IOpin_IO_init(IO_Pin_IT* settings, GPIO_TypeDef* port, uint16_t pin)
{
    settings->PORT = port;
    settings->PIN = pin;
    settings->state = RELEASED;
    settings->event_received = false;
    settings->vibrations_delay_counter = 0;
}


void IOpin_set_state(IO_Pin_IT* settings, bool state)
{
    settings->state = (state ? PRESSED : RELEASED);
}


void IOpin_set_event_received(IO_Pin_IT* settings, bool state)
{
    settings->event_received = state;
}


void IOpin_set_vibrations_delay_counter(IO_Pin_IT* settings)
{
    settings->vibrations_delay_counter = VIBRATIONS_DELAY_STEPS;
}


bool IOpin_subtract_vibrations_delay_counter(IO_Pin_IT* settings)
{
    if(settings->vibrations_delay_counter > 0)
    {
        settings->vibrations_delay_counter -= 1;
        return false;
    }

    return true;
}


void IOpin_check_pin_IT(IO_Pin_IT* settings)
{
    if(settings->vibrations_delay_counter == 0)
    {
        IOpin_set_state(settings, (settings->state == PRESSED ? false : true));
        IOpin_set_event_received(settings, true);
        IOpin_set_vibrations_delay_counter(settings);
    }
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
