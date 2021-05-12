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
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Manager_BoundariesDetector.h"
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

void init_boundariesDetector_manager(BoundariesDetector_Settings* settings,
    GPIO_TypeDef* minX_port, uint16_t minX_pin, GPIO_TypeDef* maxX_port, uint16_t maxX_pin,
    GPIO_TypeDef* minY_port, uint16_t minY_pin, GPIO_TypeDef* maxY_port, uint16_t maxY_pin,
    GPIO_TypeDef* minZ_port, uint16_t minZ_pin, GPIO_TypeDef* maxZ_port, uint16_t maxZ_pin)
{
    init_boundaryDetector(&(settings->bound_MinX), minX_port, minX_pin);
    init_boundaryDetector(&(settings->bound_MaxX), maxX_port, maxX_pin);

    init_boundaryDetector(&(settings->bound_MinY), minY_port, minY_pin);
    init_boundaryDetector(&(settings->bound_MaxY), maxY_port, maxY_pin);

    init_boundaryDetector(&(settings->bound_MinZ), minZ_port, minZ_pin);
    init_boundaryDetector(&(settings->bound_MaxZ), maxZ_port, maxZ_pin);
}


Std_Err check_boundaries(BoundariesDetector_Settings* settings, uint16_t interruptPin)
{
    Std_Err stdErr = STD_OK;

    stdErr = check_boundDetector_IT(&(settings->bound_MinX), interruptPin);
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = check_boundDetector_IT(&(settings->bound_MaxX), interruptPin);
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = check_boundDetector_IT(&(settings->bound_MinY), interruptPin);
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = check_boundDetector_IT(&(settings->bound_MaxY), interruptPin);
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = check_boundDetector_IT(&(settings->bound_MinZ), interruptPin);
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = check_boundDetector_IT(&(settings->bound_MaxZ), interruptPin);

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
