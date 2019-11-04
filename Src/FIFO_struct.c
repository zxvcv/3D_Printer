#include <FIFO_struct.h>

/*----------------------------functions----------------------------*/

bool ListStruct_Create(List_struct** lista)
{
	if ((*lista) != NULL) //lista nie jest pusta
		return false;
	
	__disable_irq();
	*lista = (List_struct*)malloc(sizeof(List_struct));
	__enable_irq();
	(*lista)->begin = NULL;
	(*lista)->size = 0;
	return true;
}


bool ListStruct_Push_C(List_struct* lista, const structData* val)
{
	if (lista == NULL) //lista jest niezainicjalizowana
		return false;

	//int i = 0;
	//while (val[i] != '\0') { ++i; } //obliczanie d³ugoœci ci¹gu znaków

	if (lista->size == 0)
	{
		__disable_irq();
		lista->begin = (List_memb_struct*)malloc(sizeof(List_memb_struct));
		lista->begin->next = NULL;
		lista->begin->data = (structData*)malloc(sizeof(structData));
		__enable_irq();
		*lista->begin->data = *val;
		lista->size += 1;
	}
	else
	{
		List_memb_struct* prev = NULL;
		List_memb_struct* memb = lista->begin;
		while (memb != NULL)
		{
			prev = memb;
			memb = memb->next;
		}
		__disable_irq();
		prev->next = (List_memb_struct*)malloc(sizeof(List_memb_struct));
		prev->next->next = NULL;
		prev->next->data = (structData*)malloc(sizeof(structData));
		__enable_irq();
		*lista->begin->data = *val;
		lista->size += 1;
	}
	return true;
}


bool ListStruct_Push_NC(List_struct* lista, structData* val)
{
	if (lista == NULL) //lista jest niezainicjalizowana
		return false;

	if (lista->size == 0)
	{
		__disable_irq();
		lista->begin = (List_memb_struct*)malloc(sizeof(List_memb_struct));
		__enable_irq();
		lista->begin->next = NULL;
		lista->begin->data = val;
		lista->size += 1;
	}
	else
	{
		List_memb_struct* prev = NULL;
		List_memb_struct* memb = lista->begin;
		while (memb != NULL)
		{
			prev = memb;
			memb = memb->next;
		}
		__disable_irq();
		prev->next = (List_memb_struct*)malloc(sizeof(List_memb_struct));
		__enable_irq();
		prev->next->next = NULL;
		prev->next->data = val;
		lista->size += 1;
	}
	return true;
}


const structData* const ListStruct_Front(List_struct* lista)
{
	if (lista == NULL || lista->size == 0) //lista jest niezainicjalizowana
		return NULL;

	return lista->begin->data;
}


bool ListStruct_Pop_C(List_struct* lista)
{
	if (lista == NULL || lista->size == 0) //lista jest niezainicjalizowana
	{
		return false;
	}


	if (lista->size == 1)
	{
		__disable_irq();
		free(lista->begin->data);
		free(lista->begin);
		__enable_irq();
		lista->begin = NULL;
		lista->size = 0;
	}
	else
	{
		List_memb_struct* del = lista->begin;
		lista->begin = lista->begin->next;
		__disable_irq();
		free(del->data);
		free(del);
		__enable_irq();
		lista->size -= 1;
	}
	return true;
}


bool ListStruct_Pop_NC(List_struct* lista)
{
	if (lista == NULL || lista->size == 0) //lista jest niezainicjalizowana
	{
		return false;
	}


	if (lista->size == 1)
	{
		__disable_irq();
		free(lista->begin);
		__enable_irq();
		lista->begin = NULL;
		lista->size = 0;
	}
	else
	{
		List_memb_struct* del = lista->begin;
		lista->begin = lista->begin->next;
		__disable_irq();
		free(del);
		__enable_irq();
		lista->size -= 1;
	}
	return true;
}


void ListStruct_Clear_C(List_struct* lista)
{
	while (ListStruct_Pop_C(lista)) {}
}


void ListStruct_Clear_NC(List_struct* lista)
{
	while (ListStruct_Pop_NC(lista)) {}
}


bool ListStruct_Delete_C(List_struct** lista)
{
	if ((*lista) == NULL)
		return false;
	ListStruct_Clear_C(*lista);
	__disable_irq();
	free(*lista);
	__enable_irq();
	*lista = NULL;
	return true;
}


bool ListStruct_Delete_NC(List_struct** lista)
{
	if ((*lista) == NULL)
		return false;
	ListStruct_Clear_NC(*lista);
	__disable_irq();
	free(*lista);
	__enable_irq();
	*lista = NULL;
	return true;
}


int ListStruct_GetSize(List_struct* lista)
{
	return lista->size;
}

/*----------------------------functions----------------------------*/
