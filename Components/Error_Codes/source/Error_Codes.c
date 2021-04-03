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

#include <stddef.h>
#include "Error_Codes.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/


/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define MSG_STD_OK ("STD_OK")
#define MSG_STD_ERROR ("STD_ERROR")
#define MSG_STD_ALLOC_ERROR ("STD_ALLOC_ERROR")
#define MSG_STD_REFERENCE_ERROR ("STD_REFERENCE_ERROR")
#define MSG_STD_PARAMETER_ERROR ("STD_PARAMETER_ERROR")
#define MSG_STD_BUSY_ERROR ("STD_BUSY_ERROR")
#define MSG_STD_TIMEOUT_ERROR ("STD_TIMEOUT_ERROR")
#define MSG_STD_INTERRUPTED_ERROR ("STD_INTERRUPTED_ERROR")
#define MSG_STD_IO_ERROR ("STD_IO_ERROR")
#define MSG_UNKNOWN ("UNKNOWN_ERROR")
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


char* get_str_error_code(Std_Err err)
{
    char* str_err_code = NULL;
    switch(err)
    {
        case STD_OK: str_err_code = MSG_STD_OK; break;
        case STD_ERROR: str_err_code = MSG_STD_ERROR; break;
        case STD_ALLOC_ERROR: str_err_code = MSG_STD_ALLOC_ERROR; break;
        case STD_REFERENCE_ERROR: str_err_code = MSG_STD_REFERENCE_ERROR; break;
        case STD_PARAMETER_ERROR: str_err_code = MSG_STD_PARAMETER_ERROR; break;
        case STD_BUSY_ERROR: str_err_code = MSG_STD_BUSY_ERROR; break;
        case STD_TIMEOUT_ERROR: str_err_code = MSG_STD_TIMEOUT_ERROR; break;
        case STD_INTERRUPTED_ERROR: str_err_code = MSG_STD_INTERRUPTED_ERROR; break;
        case STD_IO_ERROR: str_err_code = MSG_STD_IO_ERROR; break;
        default: str_err_code = MSG_UNKNOWN; break;
    }
    return str_err_code;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
