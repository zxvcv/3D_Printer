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



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include "FIFO_void.h"
#include <string.h>



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

void list_create(List** list, Fifo_Err* errors)
{
	*errors = QUEUE_ERROR;

	if ((*list) == NULL)
	{
#ifdef USE_INTERRUPTS
		IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

		*list = (List*)malloc(sizeof(List));

#ifdef USE_INTERRUPTS
		IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

		if(*list == NULL)
		{
			*errors = QUEUE_ALLOC_ERR;
		}
		else
		{
			(*list)->begin = NULL;
			(*list)->size = 0;
			*errors = QUEUE_OK;
		}
	}
}


void list_push_C(List* list, void* val, int valSize, Fifo_Err* errors)
{
	*errors = QUEUE_ERROR;

	if (list != NULL)
	{
		if (list->size == 0)
		{
#ifdef USE_INTERRUPTS
			IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

			list->begin = (List_memb*)malloc(sizeof(List_memb));
			if(list->begin == NULL)
			{
#ifdef USE_INTERRUPTS
				IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
				*errors = QUEUE_ALLOC_ERR;
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

				*errors = QUEUE_OK;
			}
		}
		else
		{
			List_memb* prev = NULL;
			List_memb* memb = list->begin;
			while (memb != NULL)
			{
				prev = memb;
				memb = memb->next;
			}

#ifdef USE_INTERRUPTS
			IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

			prev->next = (List_memb*)malloc(sizeof(List_memb));
			if(list->begin == NULL)
			{
#ifdef USE_INTERRUPTS
				IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
				*errors = QUEUE_ALLOC_ERR;
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

				*errors = QUEUE_OK;
			}
		}
	}
}


void list_push_NC(List* list, void* val, Fifo_Err* errors)
{
	*errors = QUEUE_ERROR;

	if (list != NULL)
	{
		if (list->size == 0)
		{
#ifdef USE_INTERRUPTS
			IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

			list->begin = (List_memb*)malloc(sizeof(List_memb));

#ifdef USE_INTERRUPTS
			IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

			if(list->begin == NULL)
			{
				*errors = QUEUE_ALLOC_ERR;
			}
			else
			{
				list->begin->next = NULL;
				list->begin->data = val;
				list->begin->dataSize = 0;
				list->size += 1;

				*errors = QUEUE_OK;
			}
		}
		else
		{
			List_memb* prev = NULL;
			List_memb* memb = list->begin;
			while (memb != NULL)
			{
				prev = memb;
				memb = memb->next;
			}

#ifdef USE_INTERRUPTS
			IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

			prev->next = (List_memb*)malloc(sizeof(List_memb));

#ifdef USE_INTERRUPTS
			IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

			if(prev->next == NULL)
			{
				*errors = QUEUE_ALLOC_ERR;
			}
			else
			{
				prev->next->next = NULL;
				prev->next->data = val;
				prev->next->dataSize = 0;
				list->size += 1;

				*errors = QUEUE_OK;
			}
		}
	}
}


void* list_front(List* list, Fifo_Err* errors)
{
	void* retVal = NULL;
	*errors = QUEUE_ERROR;

	if (list != NULL)
	{
		if(list->size == 0)
		{
			retVal = NULL;
			*errors = QUEUE_REF_ERR;
		}
		else
		{
			retVal = list->begin->data;
			*errors = QUEUE_OK;
		}
	}

	return retVal;
}


void list_pop_C(List* list, Fifo_Err* errors)
{
	*errors = QUEUE_ERROR;

	if (list != NULL)
	{
		if(list->size == 0)
		{
			*errors = QUEUE_REF_ERR;
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

			*errors = QUEUE_OK;
		}
		else
		{
			List_memb* del = list->begin;
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

			*errors = QUEUE_OK;
		}
	}
}


void list_pop_NC(List* list, Fifo_Err* errors)
{
	*errors = QUEUE_ERROR;

	if (list != NULL)
	{
		if(list->size == 0)
		{
			*errors = QUEUE_REF_ERR;
		}
		else if(list->size == 1)
		{
#ifdef USE_INTERRUPTS
			IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

			free(list->begin);

#ifdef USE_INTERRUPTS
			IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

			list->begin = NULL;
			list->size = 0;

			*errors = QUEUE_OK;
		}
		else
		{
			List_memb* del = list->begin;
			list->begin = list->begin->next;

#ifdef USE_INTERRUPTS
			IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

			free(del);

#ifdef USE_INTERRUPTS
			IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

			list->size -= 1;

			*errors = QUEUE_OK;
		}
	}
}


void list_clear_C(List* list, Fifo_Err* errors)
{
	*errors = QUEUE_OK;
	Fifo_Err errCheck;

	while (list_getSize(list, &errCheck) > 0)
	{
		if(errCheck != QUEUE_OK)
		{
			*errors = errCheck;
			break;
		}

		list_pop_C(list, &errCheck);

		if(errCheck != QUEUE_OK)
		{
			*errors = errCheck;
			break;
		}
	}

	if(errCheck != QUEUE_OK)
		*errors = errCheck;

}


void list_clear_NC(List* list, Fifo_Err* errors)
{
	*errors = QUEUE_OK;
	Fifo_Err errCheck;

	while (list_getSize(list, &errCheck) > 0)
	{
		if(errCheck != QUEUE_OK)
		{
			*errors = errCheck;
			break;
		}

		list_pop_NC(list, &errCheck);

		if(errCheck != QUEUE_OK)
		{
			*errors = errCheck;
			break;
		}
	}

	if(errCheck != QUEUE_OK)
		*errors = errCheck;

}


void list_delete_C(List** list, Fifo_Err* errors)
{
	*errors = QUEUE_OK;
	Fifo_Err errCheck;

	if ((*list) != NULL)
	{
		list_clear_C(*list, &errCheck);
		if(errCheck != QUEUE_OK)
		{
			*errors = errCheck;
		}
		else
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
}


void list_delete_NC(List** list, Fifo_Err* errors)
{
	*errors = QUEUE_OK;
	Fifo_Err errCheck;

	if ((*list) != NULL)
	{
		list_clear_NC(*list, &errCheck);
		if(errCheck != QUEUE_OK)
		{
			*errors = errCheck;
		}
		else
		{
#ifdef USE_INTERRUPTS
			IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

			free(*list);

#ifdef USE_INTERRUPTS
				IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

			*list = NULL;

			*errors = QUEUE_OK;
		}
	}
}


uint8_t list_getSize(List* list, Fifo_Err* errors)
{
	uint8_t retVal;

	if (list == NULL)
	{
		retVal = 0;
		*errors = QUEUE_ERROR;
	}
	else
	{
		retVal = list->size;
		*errors = QUEUE_OK;
	}

	return retVal;
}


uint8_t list_getDataSize(List* list, Fifo_Err* errors)
{
	uint8_t retVal;

	if (list == NULL)
	{
		retVal = 0;
		*errors = QUEUE_ERROR;
	}
	else if(list->size <= 0)
	{
		retVal = 0;
		*errors = QUEUE_REF_ERR;
	}
	else
	{
		retVal = list->begin->dataSize;
		*errors = QUEUE_OK;
	}

	return retVal;
}


Std_Err translate_error_fifo_to_project(Fifo_Err fifoErr)
{
	Std_Err retVal;

	switch(fifoErr){
	case QUEUE_OK: retVal = STD_OK; break;
	case QUEUE_ERROR: retVal = STD_ERROR; break;
	case QUEUE_REF_ERR: retVal = STD_REFERENCE_ERROR; break;
	case QUEUE_ALLOC_ERR: retVal = STD_ALLOC_ERROR; break;
	default: retVal = STD_ERROR; break;
	}
	return retVal;
}
