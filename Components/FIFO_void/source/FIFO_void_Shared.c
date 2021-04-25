/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: FIFO_void
 *      2 types FIFO for void*:
 *          type 1 (C)  - passed value is copied and allocated in new memory space
 *          type 2 (NC) - passed pointer is assigned and stored in queue (no allocation is made)
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "FIFO_void.h"
#include <string.h>
#include "Project_Config.h"
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

Std_Err fifo_create(Fifo** list)
{
    Std_Err stdErr = STD_ERROR;

    if ((*list) == NULL)
    {
        #ifdef USE_INTERRUPTS
        IRQ_DISABLE;
        #endif /* USE_INTERRUPTS */

        *list = (Fifo*)malloc(sizeof(Fifo));

        #ifdef USE_INTERRUPTS
        IRQ_ENABLE;
        #endif /* USE_INTERRUPTS */

        if(*list == NULL)
        {
            stdErr = STD_ALLOC_ERROR;
        }
        else
        {
            (*list)->begin = NULL;
            (*list)->size = 0;
            stdErr = STD_OK;
        }
    }

    return stdErr;
}


Std_Err fifo_front(Fifo* list, void** data)
{
    Std_Err stdErr = STD_ERROR;

    if (list != NULL)
    {
        if(list->size == 0)
        {
            (*data) = NULL;
            stdErr = STD_REFERENCE_ERROR;
        }
        else
        {
            (*data) = list->begin->data;
            stdErr = STD_OK;
        }
    }

    return stdErr;
}


uint8_t fifo_getSize(Fifo* list)
{
    uint8_t retVal;

    if (list == NULL)
    {
        retVal = 0;
    }
    else
    {
        retVal = list->size;
    }

    return retVal;
}


uint8_t fifo_getDataSize(Fifo* list)
{
    uint8_t retVal;

    if (list == NULL)
    {
        retVal = 0;
    }
    else if(list->size <= 0)
    {
        retVal = 0;
    }
    else
    {
        retVal = list->begin->dataSize;
    }

    return retVal;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
