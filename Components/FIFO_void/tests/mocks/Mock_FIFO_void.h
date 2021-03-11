#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "FIFO_void.h"
}

class Mock_FIFO_void {
public:
    MOCK_METHOD1(fifo_create, Std_Err(Fifo** list));
    MOCK_METHOD3(fifo_push_C, Std_Err(Fifo_C* list, void* val, int valSize));
    MOCK_METHOD2(fifo_push_NC, Std_Err(Fifo_NC* list, void* val));
    MOCK_METHOD2(fifo_front, Std_Err(Fifo* list, void** data));
    MOCK_METHOD1(fifo_pop_C, Std_Err(Fifo_C* list));
    MOCK_METHOD1(fifo_pop_NC, Std_Err(Fifo_NC* list));
    MOCK_METHOD1(fifo_clear_C, Std_Err(Fifo_C* list));
    MOCK_METHOD1(fifo_clear_NC, Std_Err(Fifo_NC* list));
    MOCK_METHOD1(fifo_delete_C, Std_Err(Fifo_C** list));
    MOCK_METHOD1(fifo_delete_NC, Std_Err(Fifo_NC** list));
    MOCK_METHOD1(fifo_getSize, uint8_t(Fifo* list));
    MOCK_METHOD1(fifo_getDataSize, uint8_t(Fifo* list));

    static Mock_FIFO_void* mock;
};
