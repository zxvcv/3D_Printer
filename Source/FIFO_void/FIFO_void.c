
#include "FIFO_void.h"


/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */


Fifo_Err list_create(List** list)
{
	Fifo_Err retVal = QUEUE_ERROR;

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
			retVal = QUEUE_ALLOC_ERR;
		}
		else
		{
			(*list)->begin = NULL;
			(*list)->size = 0;
			retVal = QUEUE_OK;
		}
	}

	return retVal;
}


Fifo_Err list_push_C(List* list, void* val, int valSize)
{
	Fifo_Err retVal = QUEUE_ERROR;

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
				retVal = QUEUE_ALLOC_ERR;
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

				retVal = QUEUE_OK;
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
				retVal = QUEUE_ALLOC_ERR;
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

				retVal = QUEUE_OK;
			}
		}
	}

	return retVal;
}


Fifo_Err list_push_NC(List* list, void* val)
{
	Fifo_Err retVal = QUEUE_ERROR;

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
				retVal = QUEUE_ALLOC_ERR;
			}
			else
			{
				list->begin->next = NULL;
				list->begin->data = val;
				list->begin->dataSize = 0;
				list->size += 1;

				retVal = QUEUE_OK;
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
				retVal = QUEUE_ALLOC_ERR;
			}
			else
			{
				list->begin->next = NULL;
				list->begin->data = val;
				list->begin->dataSize = 0;
				list->size += 1;

				retVal = QUEUE_OK;
			}
		}
	}

	return retVal;
}


Fifo_Err list_front(List* list, void** val)
{
	Fifo_Err retVal = QUEUE_ERROR;

	if (list != NULL)
	{
		if(list->size == 0)
		{
			*val = NULL;
			retVal = QUEUE_REF_ERR;
		}
		else
		{
			*val = list->begin->data;
			retVal = QUEUE_OK;
		}
	}

	return retVal;
}


Fifo_Err list_pop_C(List* list)
{
	Fifo_Err retVal = QUEUE_ERROR;

	if (list != NULL)
	{
		if(list->size == 0)
		{
			retVal = QUEUE_REF_ERR;
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

			retVal = QUEUE_OK;
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

			retVal = QUEUE_OK;
		}
	}

	return retVal;
}


Fifo_Err list_pop_NC(List* list)
{
	Fifo_Err retVal = QUEUE_ERROR;

	if (list != NULL)
	{
		if(list->size == 0)
		{
			retVal = QUEUE_REF_ERR;
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

			retVal = QUEUE_OK;
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

			retVal = QUEUE_OK;
		}
	}

	return retVal;
}


Fifo_Err list_clear_C(List* list)
{
	Fifo_Err retVal = QUEUE_OK;
	Fifo_Err errCheck;

	while (1)
	{
		errCheck = list_pop_C(list);

		if( errCheck != QUEUE_OK)
		{
			retVal = errCheck;
			break;
		}
	}

	return retVal;
}


Fifo_Err list_clear_NC(List* list)
{
	Fifo_Err retVal = QUEUE_OK;
	Fifo_Err errCheck;

	while (1)
	{
		errCheck = list_pop_NC(list);

		if( errCheck != QUEUE_OK)
		{
			retVal = errCheck;
			break;
		}
	}

	return retVal;
}


Fifo_Err list_delete_C(List** list)
{
	Fifo_Err retVal = QUEUE_ERROR;
	Fifo_Err errCheck;

	if ((*list) != NULL)
	{
		errCheck = list_clear_C(*list);
		if(errCheck != QUEUE_OK)
		{
			retVal = errCheck;
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

			retVal = QUEUE_OK;
		}
	}

	return retVal;
}


Fifo_Err list_delete_NC(List** list)
{
	Fifo_Err retVal = QUEUE_ERROR;
	Fifo_Err errCheck;

	if ((*list) != NULL)
	{
		errCheck = list_clear_NC(*list);
		if(errCheck != QUEUE_OK)
		{
			retVal = errCheck;
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

			retVal = QUEUE_OK;
		}
	}

	return retVal;
}


uint8_t list_getSize(List* list)
{
	uint8_t retVal;

	if (list == NULL)
		retVal = 0;
	else
		retVal = list->size;

	return retVal;
}


uint8_t list_getDataSize(List* list)
{
	uint8_t retVal;

	if (list == NULL || list->size == 0)
		retVal = 0;
	else
		retVal = list->begin->dataSize;

	return retVal;
}
