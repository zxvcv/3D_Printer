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
    Std_Err (*on_detection)(struct BoundDetector_Tag*);

    IO_Pin_IT detector;
}BoundDetector;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_boundaryDetector(BoundDetector* settings,
    GPIO_TypeDef* detector_port, uint16_t detector_pin);

Std_Err check_boundDetector_IT(BoundDetector* settings, uint16_t interruptPin);

void set_onDetection_event(BoundDetector* settings, Std_Err (*event)(BoundDetector*));

void reset_onDetection_event(BoundDetector* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* BOUNDARY_DETECTOR_H_ */
