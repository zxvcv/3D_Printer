/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Init_Manager
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef INIT_MANAGER_H_
#define INIT_MANAGER_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Error_Codes.h"
#include "Project_Objects.h"
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

/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Std_Err init_manager(DeviceSettings* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* INIT_MANAGER_H_ */