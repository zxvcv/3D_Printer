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
}


Std_Err check_boundDetector_IT(BoundDetector* settings, uint16_t interruptPin)
{
    Std_Err stdErr = STD_OK;

    if(interruptPin == settings->detector.PIN)
    {
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
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
