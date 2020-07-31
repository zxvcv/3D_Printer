/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 * 2 types FIFO for void*:
 * 		type 1 (C)  - passed value is copied and allocated in new memory space
 * 		type 2 (NC) - passed pointer is assigned and stored in queue (no allocation is made)
 * =======================================================================================================
 * COMMENTS:
 * 		Remember to use same Queue type to enqueue and dequeue.
 * 		If the functions do not have a compatible type there, memory leaks will occur.
 *
 * 		Don't mix type 1 and type 2 FIFO in one queue or memory leaks will occur.
 * =======================================================================================================
 * EXAMPLE:
 * 		None
 ********************************************************************************************************** */

#ifndef FIFO_VOID_H
#define FIFO_VOID_H



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include <stdlib.h>
#include <stdint.h>
#include "ProjectTypes.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */

typedef struct Fifo_memb_Tag {
	struct Fifo_memb_Tag* next;
	void* data;
	uint8_t dataSize;
}Fifo_memb;

typedef struct Fifo_Tag {
	Fifo_memb* begin;
	uint8_t size;
}Fifo;

typedef Fifo 	Fifo_C;
typedef Fifo 	Fifo_NC;



/* #######################################################################################################
 *										PUBLIC DECLARATIONS
 * ####################################################################################################### */

/* *********************************************************************************************************
 * Function for creating FIFO object
 *	Parameters:
 *		IN  @list	-	pointer where empty list should be created
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		None
 * =======================================================================================================
 * PRECONDITIONS:
 *		None
 * =======================================================================================================
 * POSTCONDITIONS:
 *		List_Delete_C or List_Delete_NC should be called for free FIFO object, depending on
 *		the queue type (NC or C)
 * =======================================================================================================
 * COMMENTS:
 * 		None
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_create(Fifo** list);


/* *********************************************************************************************************
 * Function for pushing elements at the end of the list
 *	Parameters:
 *		IN  @list	-	pointer with created FIFO object
 *		IN  @val	-	pointer to the value
 *		IN  @valSize	-	value returned by sizeof() called on value
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		None
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		use only with FIFO type 1 (C)
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_push_C(Fifo_C* list, void* val, int valSize);


/* *********************************************************************************************************
 * Function for pushing elements at the end of the list
 *	Parameters:
 *		IN  @list	-	pointer with created FIFO object
 *		IN  @val	-	pointer to the value
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		None
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		use only with FIFO type 2 (NC)
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_push_NC(Fifo_NC* list, void* val);


/* *********************************************************************************************************
 * Function for returning pointer for first element on the list
 *	Parameters:
 *		IN  @list	-	pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		@val	-	pointer to the value
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		can be used with FIFO type 1 and FIFO type 2
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_front(Fifo* list, void** data);


/* *********************************************************************************************************
 * Function for deleting element from top of the queue
 *	Parameters:
 *		IN  @list	-	pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		None
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		use only with FIFO type 1 (C)
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_pop_C(Fifo_C* list);


/* *********************************************************************************************************
 * Function for deleting element from top of the queue
 *	Parameters:
 *		IN  @list	-	pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		None
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		use only with FIFO type 2 (NC)
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_pop_NC(Fifo_NC* list);


/* *********************************************************************************************************
 * Function for deleting all elements from top of the queue
 *	Parameters:
 *		IN  @list	-	pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		@Fifo_Err	-	see the Fifo_Err type definition for more
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		use only with FIFO type 1 (C)
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_clear_C(Fifo_C* list);


/* *********************************************************************************************************
 * Function for deleting all elements from top of the queue
 *	Parameters:
 *		IN  @list	-	pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		None
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		use only with FIFO type 2 (NC)
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_clear_NC(Fifo_NC* list);


/* *********************************************************************************************************
 * Function for deleting FIFO object
 *	Parameters:
 *		IN  @list	-	pointer to pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		None
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		use only with FIFO type 1 (C)
 *
 *		removes also all values inside queue
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_delete_C(Fifo_C** list);


/* *********************************************************************************************************
 * Function for deleting FIFO object
 *	Parameters:
 *		IN  @list	-
	if(errCheck != QUEUE_OK)
		*errors = errCheck;	pointer to pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		None
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		use only with FIFO type 1 (C)
 *
 *		removes only all pointers stored in list
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
Std_Err fifo_delete_NC(Fifo_NC** list);


/* *********************************************************************************************************
 * Function for getting list size
 *	Parameters:
 *		IN  @list	-	pointer to pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		@uint8_t	-	number of elements in queue or 0
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		can be used with FIFO type 1 and FIFO type 2
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
uint8_t fifo_getSize(Fifo* list);


/* *********************************************************************************************************
 * Function for getting size of the next element
 *	Parameters:
 *		IN  @list	-	pointer to pointer with created FIFO object
 *		OUT @Fifo_Err	-	output error state
 *
 *	Return:
 *		@uint8_t	-	number of bytes needed to store the value or 0
 * =======================================================================================================
 * PRECONDITIONS:
 *		under parameter "list" exists created FIFO object (List_Create invoked on &list)
 * =======================================================================================================
 * POSTCONDITIONS:
 *		None
 * =======================================================================================================
 * COMMENTS:
 *		can be used with FIFO type 1 and FIFO type 2, but FIFO type 2 will be always return value 0
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
uint8_t fifo_getDataSize(Fifo* list);



#endif /* FIFO_VOID_H */
