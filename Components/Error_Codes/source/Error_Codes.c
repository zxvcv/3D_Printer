/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Error_Codes
 *      TODO: (...)
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Error_Codes.h"
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

Err_Msg get_std_error_message(Std_Err err, const char *msg)
{
    Err_Msg error;
    error.err = err;
    error.msg = msg;
    return error;
}


Std_Err translate_error_hal_to_project(HAL_StatusTypeDef halStatus)
{
    Std_Err retVal;

    switch(halStatus)
    {
        case HAL_OK: retVal = STD_OK; break;
        case HAL_ERROR: retVal = STD_ERROR; break;
        case HAL_BUSY: retVal = STD_BUSY_ERROR; break;
        case HAL_TIMEOUT: retVal = STD_TIMEOUT_ERROR; break;
        default: retVal = STD_ERROR; break;
    }
    return retVal;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
