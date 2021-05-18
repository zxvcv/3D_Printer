/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Boundary_Detector
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Boundary_Detector.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

bool get_state(BoundDetector* settings)
{
    settings->state =
        (HAL_GPIO_ReadPin(settings->detector.PORT, settings->detector.PIN) == GPIO_PIN_SET) ?
        true : false;
}

Std_Err onDetection_breakProgram(BoundDetector* settings)
{
    return STD_IO_ERROR;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void init_boundaryDetector(BoundDetector* settings,
    GPIO_TypeDef* detector_port, uint16_t detector_pin)
{
    IOpin_IO_init(&(settings->detector), detector_port, detector_pin);

    settings->on_detection = onDetection_breakProgram;
    settings->delay_counter = 0;
    get_state(settings);
}


Std_Err check_boundDetector_IT(BoundDetector* settings, uint16_t interruptPin)
{
    Std_Err stdErr = STD_OK;

    if(interruptPin == settings->detector.PIN && settings->delay_counter == 0)
    {
        settings->state = !(settings->state);
        settings->delay_counter = 2;
        stdErr = settings->on_detection(settings);
    }

    return stdErr;
}


void set_onDetection_event(BoundDetector* settings, Std_Err (*event)(BoundDetector*))
{
    settings->on_detection = event;
}


void reset_onDetection_event(BoundDetector* settings)
{
    settings->on_detection = onDetection_breakProgram;
}


void subtract_vibrations_delay_counter(BoundDetector* settings)
{
    if(settings->delay_counter > 0)
    {
        settings->delay_counter -= 1;
    }
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
