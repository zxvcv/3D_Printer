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


Std_Err fifo_push_NC(Fifo_NC* list, void* val)
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

#ifdef USE_INTERRUPTS
			IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

			if(list->begin == NULL)
			{
				stdErr = STD_ALLOC_ERROR;
			}
			else
			{
				list->begin->next = NULL;
				list->begin->data = val;
				list->begin->dataSize = 0;
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

#ifdef USE_INTERRUPTS
			IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

			if(prev->next == NULL)
			{
				stdErr = STD_ALLOC_ERROR;
			}
			else
			{
				prev->next->next = NULL;
				prev->next->data = val;
				prev->next->dataSize = 0;
				list->size += 1;

				stdErr = STD_OK;
			}
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


Std_Err fifo_pop_NC(Fifo_NC* list)
{
	Std_Err stdErr = STD_ERROR;

	if (list != NULL)
	{
		if(list->size == 0)
		{
			stdErr = STD_REFERENCE_ERROR;
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

			stdErr = STD_OK;
		}
		else
		{
			Fifo_memb* del = list->begin;
			list->begin = list->begin->next;

#ifdef USE_INTERRUPTS
			IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

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


Std_Err fifo_clear_NC(Fifo_NC* list)
{
	Std_Err stdErr = STD_OK;

	if(list == NULL)
	{
		return STD_PARAMETER_ERROR;
	}

	while (fifo_getSize(list) > 0)
	{
		stdErr = fifo_pop_NC(list);

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


Std_Err fifo_delete_NC(Fifo_NC** list)
{
	Std_Err stdErr = STD_ERROR;

	if ((*list) != NULL)
	{
		stdErr = fifo_clear_NC(*list);
		
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
