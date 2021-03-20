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
 * ============================================================================================
 * COMMENTS:
 *      Remember to use same Queue type to enqueue and dequeue.
 *      If the functions do not have a compatible type there, memory leaks will occur.
 *      
 *      Don't mix type 1 and type 2 FIFO in one queue or memory leaks will occur.
 * ============================================================================================
 * EXAMPLE:
 *      None
 ************************************************************************************************/

#ifndef FIFO_VOID_H_
#define FIFO_VOID_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <stdlib.h>
#include <stdint.h>
#include "Error_Codes.h"
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

typedef struct Fifo_memb_Tag {
    struct Fifo_memb_Tag* next;
    void* data;
    uint8_t dataSize;
}Fifo_memb;

typedef struct Fifo_Tag {
    Fifo_memb* begin;
    uint8_t size;
}Fifo;

typedef Fifo    Fifo_C;
typedef Fifo    Fifo_NC;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Std_Err fifo_create(Fifo** list);

Std_Err fifo_push_C(Fifo_C* list, void* val, int valSize);

Std_Err fifo_push_NC(Fifo_NC* list, void* val);

Std_Err fifo_front(Fifo* list, void** data);

Std_Err fifo_pop_C(Fifo_C* list);

Std_Err fifo_pop_NC(Fifo_NC* list);

Std_Err fifo_clear_C(Fifo_C* list);

Std_Err fifo_clear_NC(Fifo_NC* list);

Std_Err fifo_delete_C(Fifo_C** list);

Std_Err fifo_delete_NC(Fifo_NC** list);

uint8_t fifo_getSize(Fifo* list);

uint8_t fifo_getDataSize(Fifo* list);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* FIFO_VOID_H_ */
