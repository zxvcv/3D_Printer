#include "FIFO_void.h"

#include "main.h"
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()

/*----------------------------functions----------------------------*/

bool List_Create(List** lista)
{
	if ((*lista) != NULL) //lista nie jest pusta
		return false;
	
	IRQ_DISABLE;
	*lista = (List*)malloc(sizeof(List));
	IRQ_ENABLE;
	(*lista)->begin = NULL;
	(*lista)->size = 0;
	return true;
}


bool List_Push_C(List* lista, void* val, int valSize)
{
	if (lista == NULL) //lista jest niezainicjalizowana
		return false;

	if (lista->size == 0)
	{
		IRQ_DISABLE;
		lista->begin = (List_memb*)malloc(sizeof(List_memb));
		lista->begin->next = NULL;
		lista->begin->data = malloc(valSize);
		IRQ_ENABLE;
		memcpy(lista->begin->data, val, valSize);
		lista->begin->dataSize = valSize;
		lista->size += 1;
	}
	else
	{
		List_memb* prev = NULL;
		List_memb* memb = lista->begin;
		while (memb != NULL)
		{
			prev = memb;
			memb = memb->next;
		}
		IRQ_DISABLE;
		prev->next = (List_memb*)malloc(sizeof(List_memb));
		prev->next->next = NULL;
		prev->next->data = malloc(valSize);
		IRQ_ENABLE;
		memcpy(prev->next->data, val, valSize);
		prev->next->dataSize = valSize;
		lista->size += 1;
	}
	return true;
}


bool List_Push_NC(List* lista, void* val)
{
	if (lista == NULL) //lista jest niezainicjalizowana
		return false;

	if (lista->size == 0)
	{
		IRQ_DISABLE;
		lista->begin = (List_memb*)malloc(sizeof(List_memb));
		IRQ_ENABLE;
		lista->begin->next = NULL;
		lista->begin->data = val;
		lista->begin->dataSize = 0;
		lista->size += 1;
	}
	else
	{
		List_memb* prev = NULL;
		List_memb* memb = lista->begin;
		while (memb != NULL)
		{
			prev = memb;
			memb = memb->next;
		}
		IRQ_DISABLE;
		prev->next = (List_memb*)malloc(sizeof(List_memb));
		IRQ_ENABLE;
		prev->next->next = NULL;
		prev->next->data = val;
		prev->next->dataSize = 0;
		lista->size += 1;
	}
	return true;
}


const void* const List_Front(List* lista)
{
	if (lista == NULL || lista->size == 0) //lista jest niezainicjalizowana
		return NULL;

	return lista->begin->data;
}


bool List_Pop_C(List* lista)
{
	if (lista == NULL || lista->size == 0) //lista jest niezainicjalizowana
		return false;


	if (lista->size == 1)
	{
		IRQ_DISABLE;
		free(lista->begin->data);
		free(lista->begin);
		IRQ_ENABLE;
		lista->begin = NULL;
		lista->size = 0;
	}
	else
	{
		List_memb* del = lista->begin;
		lista->begin = lista->begin->next;
		IRQ_DISABLE;
		free(del->data);
		free(del);
		IRQ_ENABLE;
		lista->size -= 1;
	}
	return true;
}


bool List_Pop_NC(List* lista)
{
	if (lista == NULL || lista->size == 0) //lista jest niezainicjalizowana
	{
		return false;
	}


	if (lista->size == 1)
	{
		IRQ_DISABLE;
		free(lista->begin);
		IRQ_ENABLE;
		lista->begin = NULL;
		lista->size = 0;
	}
	else
	{
		List_memb* del = lista->begin;
		lista->begin = lista->begin->next;
		IRQ_DISABLE;
		free(del);
		IRQ_ENABLE;
		lista->size -= 1;
	}
	return true;
}


void List_Clear_C(List* lista)
{
	while (List_Pop_C(lista)) {}
}


void List_Clear_NC(List* lista)
{
	while (List_Pop_NC(lista)) {}
}


bool List_Delete_C(List** lista)
{
	if ((*lista) == NULL)
		return false;
	List_Clear_C(*lista);
	IRQ_DISABLE;
	free(*lista);
	IRQ_ENABLE;
	*lista = NULL;
	return true;
}


bool List_Delete_NC(List** lista)
{
	if ((*lista) == NULL)
		return false;
	List_Clear_NC(*lista);
	IRQ_DISABLE;
	free(*lista);
	IRQ_ENABLE;
	*lista = NULL;
	return true;
}


int List_GetSize(List* lista)
{
	return lista->size;
}


uint8_t List_GetDataSize(List* lista) 
{
	if (lista->size == 0)
		return 0;
	return lista->begin->dataSize;
}
/*----------------------------functions----------------------------*/
