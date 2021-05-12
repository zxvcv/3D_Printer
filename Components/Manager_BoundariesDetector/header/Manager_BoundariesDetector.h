/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Manager_BoundariesDetector
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef MANAGER_BOUNDARIES_DETECTOR_H_
#define MANAGER_BOUNDARIES_DETECTOR_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Project_Config.h"
#include "Boundary_Detector.h"
#include "stm32f3xx_hal.h"
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

typedef struct BoundariesDetector_Settings_Tag{
    BoundDetector bound_MinX;
    BoundDetector bound_MaxX;

    BoundDetector bound_MinY;
    BoundDetector bound_MaxY;

    BoundDetector bound_MinZ;
    BoundDetector bound_MaxZ;
}BoundariesDetector_Settings;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_boundariesDetector_manager(BoundariesDetector_Settings* settings,
    GPIO_TypeDef* minX_port, uint16_t minX_pin, GPIO_TypeDef* maxX_port, uint16_t maxX_pin,
    GPIO_TypeDef* minY_port, uint16_t minY_pin, GPIO_TypeDef* maxY_port, uint16_t maxY_pin,
    GPIO_TypeDef* minZ_port, uint16_t minZ_pin, GPIO_TypeDef* maxZ_port, uint16_t maxZ_pin);

Std_Err check_boundaries(BoundariesDetector_Settings* settings, uint16_t interruptPin);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* MANAGER_BOUNDARIES_DETECTOR_H_ */
