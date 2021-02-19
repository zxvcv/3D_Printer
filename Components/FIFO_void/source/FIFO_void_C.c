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
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err fifo_push_C(Fifo_C* list, void* val, int valSize)
{
    Std_Err stdErr = STD_ERROR;

    if (list != NULL)
    {
        if (list->size == 0)
        {
#ifdef USE_INTERRUPTS
            IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

            list->begin = (Fifo_memb*)malloc(sizeof(Fifo_memb));
            if(list->begin == NULL)
            {
#ifdef USE_INTERRUPTS
                IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
                stdErr = STD_ALLOC_ERROR;
            }
            else
            {
                list->begin->next = NULL;
                list->begin->data = malloc(valSize);

#ifdef USE_INTERRUPTS
                IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

                memcpy(list->begin->data, val, valSize);
                list->begin->dataSize = valSize;
                list->size += 1;

                stdErr = STD_OK;
            }
        }
        else
        {
            Fifo_memb* prev = NULL;
            Fifo_memb* memb = list->begin;
            while (memb != NULL)
            {
                prev = memb;
                memb = memb->next;
            }

#ifdef USE_INTERRUPTS
            IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

            prev->next = (Fifo_memb*)malloc(sizeof(Fifo_memb));
            if(list->begin == NULL)
            {
#ifdef USE_INTERRUPTS
                IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
                stdErr = STD_ALLOC_ERROR;
            }
            else
            {
                prev->next->next = NULL;
                prev->next->data = malloc(valSize);
#ifdef USE_INTERRUPTS
                IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
                memcpy(prev->next->data, val, valSize);
                prev->next->dataSize = valSize;
                list->size += 1;

                stdErr = STD_OK;
            }
        }
    }

    return stdErr;
}


Std_Err fifo_pop_C(Fifo_C* list)
{
    Std_Err stdErr = STD_ERROR;

    if (list != NULL)
    {
        if(list->size == 0)
        {
            stdErr = STD_OK;
        }
        else if(list->size == 1)
        {
#ifdef USE_INTERRUPTS
            IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

            free(list->begin->data);
            free(list->begin);

#ifdef USE_INTERRUPTS
            IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

            list->begin = NULL;
            list->size = 0;

            stdErr = STD_OK;
        }
        else
        {
            Fifo_memb* del = list->begin;
            list->begin = list->begin->next;

#ifdef USE_INTERRUPTS
            IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

            free(del->data);
            free(del);

#ifdef USE_INTERRUPTS
            IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

            list->size -= 1;

            stdErr = STD_OK;
        }
    }

    return stdErr;
}


Std_Err fifo_clear_C(Fifo_C* list)
{
    Std_Err stdErr = STD_OK;

    if(list == NULL)
    {
        return STD_PARAMETER_ERROR;
    }

    while (fifo_getSize(list) > 0)
    {
        stdErr = fifo_pop_C(list);

        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    return stdErr;
}


Std_Err fifo_delete_C(Fifo_C** list)
{
    Std_Err stdErr = STD_ERROR;

    if ((*list) != NULL)
    {
        stdErr = fifo_clear_C(*list);

        if(stdErr == STD_OK)
        {
#ifdef USE_INTERRUPTS
            IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

            free(*list);

#ifdef USE_INTERRUPTS
                IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

            *list = NULL;
        }
    }

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
