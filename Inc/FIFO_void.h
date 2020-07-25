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

typedef struct List_memb_Tag {
	struct List_memb_Tag* next;
	void* data;
	uint8_t dataSize;
}List_memb;

typedef struct List_Tag {
	List_memb* begin;
	uint8_t size;
}List;

typedef enum Fifo_Err_Tag {
	QUEUE_OK,			//everything ok, no error occur
	QUEUE_REF_ERR,		//reference to unknown object
	QUEUE_ALLOC_ERR,	//allocation error
	QUEUE_ERROR			//other error
}Fifo_Err;



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
void list_create(List** list, Fifo_Err* errors);


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
void list_push_C(List* list, void* val, int valSize, Fifo_Err* errors);


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
void list_push_NC(List* list, void* val, Fifo_Err* errors);


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
void* list_front(List* list, Fifo_Err* errors);


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
void list_pop_C(List* list, Fifo_Err* errors);


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
void list_pop_NC(List* list, Fifo_Err* errors);


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
void list_clear_C(List* list, Fifo_Err* errors);


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
void list_clear_NC(List* list, Fifo_Err* errors);


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
void list_delete_C(List** list, Fifo_Err* errors);


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
 *		removes only all pointers stored in list
 * =======================================================================================================
 * OTHERS:
 * 		None
 ********************************************************************************************************** */
void list_delete_NC(List** list, Fifo_Err* errors);


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
uint8_t list_getSize(List* list, Fifo_Err* errors);


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
uint8_t list_getDataSize(List* list, Fifo_Err* errors);


/* *********************************************************************************************************
 * 	TO DO
 *	Parameters:
 
 *
 *	Return:
 *		
 * =======================================================================================================
 * PRECONDITIONS:
 *		
 * =======================================================================================================
 * POSTCONDITIONS:
 *		
 * =======================================================================================================
 * COMMENTS:
 *		
 * =======================================================================================================
 * OTHERS:
 * 		
 ********************************************************************************************************** */
Std_Err translate_error_fifo_to_project(Fifo_Err fifoErr);

#endif /* FIFO_VOID_H */
