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
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef BOUNDARY_DETECTOR_H_
#define BOUNDARY_DETECTOR_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Project_Config.h"
#include "Error_Codes.h"
#include "IOpin.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct BoundDetector_Tag{
    Std_Err (*on_detection)(struct BoundDetector_Tag*, void*);
    void* event_data; // data for on_detection call

    IO_Pin_IT detector;
    bool state;
    uint8_t delay_counter;
}BoundDetector;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_boundaryDetector(BoundDetector* settings,
    GPIO_TypeDef* detector_port, uint16_t detector_pin);

Std_Err check_boundDetector_IT(BoundDetector* settings, uint16_t interruptPin);

void set_onDetection_event(BoundDetector* settings, Std_Err (*event)(BoundDetector*, void*),
    void* invocation_data);

void reset_onDetection_event(BoundDetector* settings);

void subtract_vibrations_delay_counter(BoundDetector* settigns);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* BOUNDARY_DETECTOR_H_ */
