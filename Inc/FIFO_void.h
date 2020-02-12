#ifndef FIFO_STRUCT_H
#define FIFO_STRUCT_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/*
*kolejka FIFO dla ci¹gów znaków (string)
w tej bibliotece znajduj¹ siê funkcje do obs³ugi Listy FIFO na 2 sposoby:
1 - podawana wartoœæ przez wskaznik na ci¹g znaków jest kopiowana i alokowana jest dla kopii osobna pamieæ
2 - wskaŸnik podany w wywo³aniu funkcji jest zapisywany bezpoœrednio na liœcie i przy usuwaniu takeigo elementu jest te¿ zwalniana pamiêæ dla takiego wskaŸnika

Dziêki czemu nie musimy usuwac zawartoœci tych wskaŸników na koñcu programu (miejsce w pamieci (funkcja free()) jest wywo³ywana
w funkcji List_Pop()
*/



/*----------------------------data_types----------------------------*/

typedef struct List_memb {
	struct List_memb* next;
	void* data;
	uint8_t dataSize;
}List_memb;

typedef struct List {
	List_memb* begin;
	int size;
}List;


/*----------------------------function_declarations----------------------------*/

/*
funkcja tworz¹ca listê FIFO na wskaŸniku do struktury List
!wskaznik musi byc zainicjalizowany na poczatku wartoscia NULL

@param lista wskaznik na wskaznik do struktury do pracy na liœcie
@return (true) gdy inicjalizacja sie powiod³a (false) w przeciwnym wypadku
*/
bool List_Create(List** lista);

/*
funkcja dodajaca element na koniec listy FIFO
funkcja typu C(COPY) - przy inicjalizacji elementu listy tworzy kopie wartosci spod wskaznika i inicjalizuje ni¹ watroœæ na liscie

@param lista wskaznik na listê elementów na której pracujemy
@param val wskaznik na wartosc która zostanie przekopiowania i przechowywana na liscie
@return (true) gdy dodanie sie powiod³o (false) w przeciwnym wypadku
*/
bool List_Push_C(List* lista, void* val, int valSize);

/*
funkcja dodajaca element na koniec listy FIFO
funkcja typu NC(NO COPY) - przy inicjalizacji elementu listy tylko wykorzystujaca podany wskaznik
!!! trzeba podawaæ wskaŸnik wskazuj¹cy na miejsce w pamiêci zaalokowane wczeœniej przez funkcjê malloc()

@param lista wskaznik na listê elementów na której pracujemy
@param val wskaznik na wartosc który zostanie umieszczony na liscie
@return (true) gdy dodanie sie powiod³o (false) w przeciwnym wypadku
*/
bool List_Push_NC(List* lista, void* val);

/*
funkcja zwracaj¹ca wskaznik na strukture na pocz¹tku listy

@param lista wskaznik na listê elementów na której pracujemy
@return wskaznik na pierwszy element na liscie lub NULL gdy takeigo nie ma lub wystapi³ blad
*/
const void* const List_Front(List* lista);

/*
funkcja usuwaj¹ca element z pocz¹tku listy FIFO
funkcja typu C(COPY) - usuwa element jednoczeœnie zwalniaj¹c pamiec danej
common use -> je¿eli wczeœniej element zosta³ zainicjalizowany funkcj¹ List_Pusch_C

@param lista wskaznik na listê elementów na której pracujemy
@return (true) gdy usuniecie sie powiod³o (false) w przeciwnym wypadku
*/
bool List_Pop_C(List* lista);

/*
funkcja usuwaj¹ca element z pocz¹tku listy FIFO
funkcja typu NC(NO COPY) - usuwa element nie zwalniaj¹c pamieci danej
common use -> nale¿y uzywaæ je¿eli wczeœniej element zosta³ zainicjalizowany funkcj¹ List_Pusch_NC

@param lista wskaznik na listê elementów na której pracujemy
@return (true) gdy usuniecie sie powiod³o (false) w przeciwnym wypadku
*/
bool List_Pop_NC(List* lista);

/*
funkcja usuwaj¹ca wszystkie elementy z listy FIFO
funkcja typu C(COPY) - nale¿y uzywaæ je¿eli wczeœniej elementy zosta³y zainicjalizowane funkcj¹ List_Pusch_C
!!! nie wolno u¿ywaæ gdy lista zosta³a inicjalizowana jednoczeœnie funkcjami List_Pusch_C i List_Pusch_NC

@param lista wskaznik na listê elementów na której pracujemy
*/
void List_Clear_C(List* lista);

/*
funkcja usuwaj¹ca wszystkie elementy z listy FIFO
funkcja typu NC(NO COPY) - nale¿y uzywaæ je¿eli wczeœniej elementy zosta³y zainicjalizowane funkcj¹ List_Pusch_NC
!!! nie wolno u¿ywaæ gdy lista zosta³a inicjalizowana jednoczeœnie funkcjami List_Pusch_C i List_Pusch_NC

@param lista wskaznik na listê elementów na której pracujemy
*/
void List_Clear_NC(List* lista);

/*
funkcja usuwaj¹ca listê FIFO
funkcja typu C(COPY) - nale¿y uzywaæ je¿eli wczeœniej elementy zosta³y zainicjalizowane funkcj¹ List_Pusch_C
!!! nie wolno u¿ywaæ gdy lista zosta³a inicjalizowana jednoczeœnie funkcjami List_Pusch_C i List_Pusch_NC

@param lista wskaznik na listê elementów na której pracujemy
@return (true) jezeli udalo sie wykonac operacje usuwania listy (false) w przeciwnym przypadku
*/
bool List_Delete_C(List** lista);

/*
funkcja usuwaj¹ca listê FIFO
funkcja typu NC(NO COPY) - nale¿y uzywaæ je¿eli wczeœniej elementy zosta³y zainicjalizowane funkcj¹ List_Pusch_NC
!!! nie wolno u¿ywaæ gdy lista zosta³a inicjalizowana jednoczeœnie funkcjami List_Pusch_C i List_Pusch_NC

@param lista wskaznik na listê elementów na której pracujemy
@return (true) jezeli udalo sie wykonac operacje usuwania listy (false) w przeciwnym przypadku
*/
bool List_Delete_NC(List** lista);

/*
funkcja zwracaj¹ca aktualn¹ wielkoœc listy list

@param lista wskaznik na listê elementów na której pracujemy
@return aktualna wielkoœæ listy
*/
int List_GetSize(List* lista);

/*
funkcja zwracaj¹ca wielkoœæ elementu zapisanego na pocz¹tku listy (w przypadku listy C)
w przeciwnym wypadku zwraca 0. Tak¿e 0 jest zwracane gdy lista C jest pusta.

@param lista wskaznik na listê elementów na której pracujemy
@return wielkoœæ elementu na poczatku listy
*/
uint8_t List_GetDataSize(List* lista);

/*----------------------------END_function_declarations----------------------------*/

#endif //FIFO_STRUCT_H
