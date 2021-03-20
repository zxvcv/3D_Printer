#include "Mock_FIFO_void.h"


Mock_FIFO_void* Mock_FIFO_void::mock;

extern "C"
{
    Std_Err fifo_create(Fifo** list)
    {
        return Mock_FIFO_void::mock->fifo_create(list);
    }

    Std_Err fifo_push_C(Fifo_C* list, void* val, int valSize)
    {
        return Mock_FIFO_void::mock->fifo_push_C(list, val, valSize);
    }

    Std_Err fifo_push_NC(Fifo_NC* list, void* val)
    {
        return Mock_FIFO_void::mock->fifo_push_NC(list, val);
    }

    Std_Err fifo_front(Fifo* list, void** data)
    {
        return Mock_FIFO_void::mock->fifo_front(list, data);
    }

    Std_Err fifo_pop_C(Fifo_C* list)
    {
        return Mock_FIFO_void::mock->fifo_pop_C(list);
    }

    Std_Err fifo_pop_NC(Fifo_NC* list)
    {
        return Mock_FIFO_void::mock->fifo_pop_NC(list);
    }

    Std_Err fifo_clear_C(Fifo_C* list)
    {
        return Mock_FIFO_void::mock->fifo_clear_C(list);
    }

    Std_Err fifo_clear_NC(Fifo_NC* list)
    {
        return Mock_FIFO_void::mock->fifo_clear_NC(list);
    }

    Std_Err fifo_delete_C(Fifo_C** list)
    {
        return Mock_FIFO_void::mock->fifo_delete_C(list);
    }

    Std_Err fifo_delete_NC(Fifo_NC** list)
    {
        return Mock_FIFO_void::mock->fifo_delete_NC(list);
    }

    uint8_t fifo_getSize(Fifo* list)
    {
        return Mock_FIFO_void::mock->fifo_getSize(list);
    }

    uint8_t fifo_getDataSize(Fifo* list)
    {
        return Mock_FIFO_void::mock->fifo_getDataSize(list);
    }
}