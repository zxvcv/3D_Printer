#ifndef FIFO_STRUCT_H
#define FIFO_STRUCT_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/*
*kolejka FIFO dla ci�g�w znak�w (string)
w tej bibliotece znajduj� si� funkcje do obs�ugi Listy FIFO na 2 sposoby:
1 - podawana warto�� przez wskaznik na ci�g znak�w jest kopiowana i alokowana jest dla kopii osobna pamie�
2 - wska�nik podany w wywo�aniu funkcji jest zapisywany bezpo�rednio na li�cie i przy usuwaniu takeigo elementu jest te� zwalniana pami�� dla takiego wska�nika

Dzi�ki czemu nie musimy usuwac zawarto�ci tych wska�nik�w na ko�cu programu (miejsce w pamieci (funkcja free()) jest wywo�ywana
w funkcji List_Pop()
*/



/*----------------------------data_types----------------------------*/

typedef struct DataToST7565 structData;


typedef struct List_memb_struct {
	struct List_memb_struct* next;
	structData* data;
}List_memb_struct;

typedef struct List_struct {
	List_memb_struct* begin;
	int size;
}List_struct;

/*----------------------------END_data_types----------------------------*/

#include "ST7565.h"

/*----------------------------function_declarations----------------------------*/

/*
funkcja tworz�ca list� FIFO na wska�niku do struktury List
!wskaznik musi byc zainicjalizowany na poczatku wartoscia NULL

@param lista wskaznik na wskaznik do struktury do pracy na li�cie
@return (true) gdy inicjalizacja sie powiod�a (false) w przeciwnym wypadku
*/
bool ListStruct_Create(List_struct** lista);

/*
funkcja dodajaca element na koniec listy FIFO
funkcja typu C(COPY) - przy inicjalizacji elementu listy tworzy kopie wartosci spod wskaznika i inicjalizuje ni� watro�� na liscie

@param lista wskaznik na list� element�w na kt�rej pracujemy
@param val wskaznik na wartosc kt�ra zostanie przekopiowania i przechowywana na liscie
@return (true) gdy dodanie sie powiod�o (false) w przeciwnym wypadku
*/
bool ListStruct_Push_C(List_struct* lista, const structData* val);

/*
funkcja dodajaca element na koniec listy FIFO
funkcja typu NC(NO COPY) - przy inicjalizacji elementu listy tylko wykorzystujaca podany wskaznik
!!! trzeba podawa� wska�nik wskazuj�cy na miejsce w pami�ci zaalokowane wcze�niej przez funkcj� malloc()

@param lista wskaznik na list� element�w na kt�rej pracujemy
@param val wskaznik na wartosc kt�ry zostanie umieszczony na liscie
@return (true) gdy dodanie sie powiod�o (false) w przeciwnym wypadku
*/
bool ListStruct_Push_NC(List_struct* lista, structData* val);

/*
funkcja zwracaj�ca wskaznik na strukture na pocz�tku listy

@param lista wskaznik na list� element�w na kt�rej pracujemy
@return wskaznik na pierwszy element na liscie lub NULL gdy takeigo nie ma lub wystapi� blad
*/
const structData* const ListStruct_Front(List_struct* lista);

/*
funkcja usuwaj�ca element z pocz�tku listy FIFO
funkcja typu C(COPY) - usuwa element jednocze�nie zwalniaj�c pamiec danej
common use -> je�eli wcze�niej element zosta� zainicjalizowany funkcj� List_Pusch_C

@param lista wskaznik na list� element�w na kt�rej pracujemy
@return (true) gdy usuniecie sie powiod�o (false) w przeciwnym wypadku
*/
bool ListStruct_Pop_C(List_struct* lista);

/*
funkcja usuwaj�ca element z pocz�tku listy FIFO
funkcja typu NC(NO COPY) - usuwa element nie zwalniaj�c pamieci danej
common use -> nale�y uzywa� je�eli wcze�niej element zosta� zainicjalizowany funkcj� List_Pusch_NC

@param lista wskaznik na list� element�w na kt�rej pracujemy
@return (true) gdy usuniecie sie powiod�o (false) w przeciwnym wypadku
*/
bool ListStruct_Pop_NC(List_struct* lista);

/*
funkcja usuwaj�ca wszystkie elementy z listy FIFO
funkcja typu C(COPY) - nale�y uzywa� je�eli wcze�niej elementy zosta�y zainicjalizowane funkcj� List_Pusch_C
!!! nie wolno u�ywa� gdy lista zosta�a inicjalizowana jednocze�nie funkcjami List_Pusch_C i List_Pusch_NC

@param lista wskaznik na list� element�w na kt�rej pracujemy
*/
void ListStruct_Clear_C(List_struct* lista);

/*
funkcja usuwaj�ca wszystkie elementy z listy FIFO
funkcja typu NC(NO COPY) - nale�y uzywa� je�eli wcze�niej elementy zosta�y zainicjalizowane funkcj� List_Pusch_NC
!!! nie wolno u�ywa� gdy lista zosta�a inicjalizowana jednocze�nie funkcjami List_Pusch_C i List_Pusch_NC

@param lista wskaznik na list� element�w na kt�rej pracujemy
*/
void ListStruct_Clear_NC(List_struct* lista);

/*
funkcja usuwaj�ca list� FIFO
funkcja typu C(COPY) - nale�y uzywa� je�eli wcze�niej elementy zosta�y zainicjalizowane funkcj� List_Pusch_C
!!! nie wolno u�ywa� gdy lista zosta�a inicjalizowana jednocze�nie funkcjami List_Pusch_C i List_Pusch_NC

@param lista wskaznik na list� element�w na kt�rej pracujemy
@return (true) jezeli udalo sie wykonac operacje usuwania listy (false) w przeciwnym przypadku
*/
bool ListStruct_Delete_C(List_struct** lista);

/*
funkcja usuwaj�ca list� FIFO
funkcja typu NC(NO COPY) - nale�y uzywa� je�eli wcze�niej elementy zosta�y zainicjalizowane funkcj� List_Pusch_NC
!!! nie wolno u�ywa� gdy lista zosta�a inicjalizowana jednocze�nie funkcjami List_Pusch_C i List_Pusch_NC

@param lista wskaznik na list� element�w na kt�rej pracujemy
@return (true) jezeli udalo sie wykonac operacje usuwania listy (false) w przeciwnym przypadku
*/
bool ListStruct_Delete_NC(List_struct** lista);

/*
funkcja zwracaj�ca aktualn� wielko�c listy list

@param lista wskaznik na list� element�w na kt�rej pracujemy
@return aktualna wielko�� listy
*/
int ListStruct_GetSize(List_struct* lista);

/*----------------------------END_function_declarations----------------------------*/

#endif //FIFO_STRUCT_H
