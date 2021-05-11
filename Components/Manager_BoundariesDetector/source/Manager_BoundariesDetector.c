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
    IOpin_IO_init(&(settings->minX), minX_port, minX_pin);
    IOpin_IO_init(&(settings->maxX), maxX_port, maxX_pin);

    IOpin_IO_init(&(settings->minY), minY_port, minY_pin);
    IOpin_IO_init(&(settings->maxY), maxY_port, maxY_pin);

    IOpin_IO_init(&(settings->minZ), minZ_port, minZ_pin);
    IOpin_IO_init(&(settings->maxZ), maxZ_port, maxZ_pin);
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
