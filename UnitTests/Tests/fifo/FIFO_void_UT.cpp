#include <gtest/gtest.h>

#define TAB_SIZE 5

extern "C"
{
    #include "../../../Inc/FIFO_void.h"
}

class Fifo_void_test : public ::testing::Test {
public:
    virtual void SetUp(){
        ASSERT_EQ(list, nullptr);
        list_create(&list, &errors);
        ASSERT_EQ(errors, QUEUE_OK);
        ASSERT_NE(list, nullptr);
    }
    virtual void TearDown(){
        if(fifoType == COPY)
            list_delete_C(&list, &errors);
        else
            list_delete_NC(&list, &errors);

        ASSERT_EQ(errors, QUEUE_OK);
        ASSERT_EQ(list, nullptr);
    }

    enum FifoType{
        COPY,
        NON_COPY
    }fifoType;
    List* list = NULL;
    Fifo_Err errors = QUEUE_OK;

    void addValuesToQueue();

    /* test values */
    int tab[TAB_SIZE] = {5, 10, 15, 20, 25};
};

/************************** TESTS **************************/

TEST_F(Fifo_void_test, listC_basicOperations_test) 
{
    fifoType = COPY;

    EXPECT_EQ(list_getSize(list, &errors), 0);
    EXPECT_EQ(errors, QUEUE_OK);

    for(int i=0; i<TAB_SIZE; ++i)
    {
        list_push_C(list, &(tab[i]), sizeof(tab[i]), &errors);
        EXPECT_EQ(errors, QUEUE_OK);
        EXPECT_EQ(list_getSize(list, &errors), i+1);
        EXPECT_EQ(errors, QUEUE_OK);
        EXPECT_EQ(*((int*)list_front(list, &errors)), tab[0]);
        EXPECT_NE(((int*)list_front(list, &errors)), &(tab[0]));
        EXPECT_EQ(errors, QUEUE_OK);
    }

    for(int i=0; i<TAB_SIZE; ++i)
    {
        EXPECT_EQ(*((int*)list_front(list, &errors)), tab[i]);
        EXPECT_EQ(errors, QUEUE_OK);
        EXPECT_EQ(list_getSize(list, &errors), TAB_SIZE-i);
        EXPECT_EQ(errors, QUEUE_OK);
        list_pop_C(list, &errors);
        EXPECT_EQ(errors, QUEUE_OK);
        EXPECT_EQ(list_getSize(list, &errors), TAB_SIZE-i-1);
        EXPECT_EQ(errors, QUEUE_OK);
    }
    
    EXPECT_EQ(list_getSize(list, &errors), 0);
    EXPECT_EQ(errors, QUEUE_OK);
}

TEST_F(Fifo_void_test, listC_clear_test)
{
    fifoType = COPY;

    addValuesToQueue();

    list_clear_C(list, &errors);
    EXPECT_EQ(errors, QUEUE_OK);

    EXPECT_EQ(list_getSize(list, &errors), 0);
    EXPECT_EQ(errors, QUEUE_OK);
}

TEST_F(Fifo_void_test, listC_getDataSize_test)
{
    fifoType = COPY;

    addValuesToQueue();

    for(int i=0; i<TAB_SIZE; ++i)
    {
        EXPECT_EQ(list_getDataSize(list, &errors), sizeof(tab[i]));
        EXPECT_EQ(errors, QUEUE_OK);
    }
}

TEST_F(Fifo_void_test, listC_uninitializedList_test)
{
    fifoType = COPY;

    list_delete_C(&list, &errors);
    ASSERT_EQ(errors, QUEUE_OK);
    ASSERT_EQ(list, nullptr);

    for(int i=0; i<TAB_SIZE; ++i)
    {
        list_push_C(list, &(tab[i]), sizeof(tab[i]), &errors);
        EXPECT_EQ(errors, QUEUE_ERROR);
    }
    EXPECT_EQ(list_getSize(list, &errors), 0);
    EXPECT_EQ(errors, QUEUE_ERROR);
}


TEST_F(Fifo_void_test, listNC_basicOperations_test) 
{
    fifoType = NON_COPY;

    EXPECT_EQ(list_getSize(list, &errors), 0);
    EXPECT_EQ(errors, QUEUE_OK);

    for(int i=0; i<TAB_SIZE; ++i)
    {
        list_push_NC(list, &(tab[i]), &errors);
        EXPECT_EQ(errors, QUEUE_OK);
        EXPECT_EQ(list_getSize(list, &errors), i+1);
        EXPECT_EQ(errors, QUEUE_OK);
        EXPECT_EQ(*((int*)list_front(list, &errors)), tab[0]);
        EXPECT_EQ(((int*)list_front(list, &errors)), &(tab[0]));
        EXPECT_EQ(errors, QUEUE_OK);
    }

    for(int i=0; i<TAB_SIZE; ++i)
    {
        EXPECT_EQ(*((int*)list_front(list, &errors)), tab[i]);
        EXPECT_EQ(errors, QUEUE_OK);
        EXPECT_EQ(list_getSize(list, &errors), TAB_SIZE-i);
        EXPECT_EQ(errors, QUEUE_OK);
        list_pop_NC(list, &errors);
        EXPECT_EQ(errors, QUEUE_OK);
        EXPECT_EQ(list_getSize(list, &errors), TAB_SIZE-i-1);
        EXPECT_EQ(errors, QUEUE_OK);
    }
    
    EXPECT_EQ(list_getSize(list, &errors), 0);
    EXPECT_EQ(errors, QUEUE_OK);
}

TEST_F(Fifo_void_test, listNC_clear_test)
{
    fifoType = NON_COPY;

    addValuesToQueue();

    list_clear_NC(list, &errors);
    EXPECT_EQ(errors, QUEUE_OK);

    EXPECT_EQ(list_getSize(list, &errors), 0);
    EXPECT_EQ(errors, QUEUE_OK);
}

TEST_F(Fifo_void_test, listNC_getDataSize_test)
{
    fifoType = NON_COPY;

    addValuesToQueue();

    for(int i=0; i<TAB_SIZE; ++i)
    {
        EXPECT_EQ(list_getDataSize(list, &errors), 0);
        EXPECT_EQ(errors, QUEUE_OK);
    }
}

TEST_F(Fifo_void_test, listNC_uninitializedList_test)
{
    fifoType = NON_COPY;

    list_delete_C(&list, &errors);
    ASSERT_EQ(errors, QUEUE_OK);
    ASSERT_EQ(list, nullptr);

    for(int i=0; i<TAB_SIZE; ++i)
    {
        list_push_NC(list, &(tab[i]), &errors);
        EXPECT_EQ(errors, QUEUE_ERROR);
    }
    EXPECT_EQ(list_getSize(list, &errors), 0);
    EXPECT_EQ(errors, QUEUE_ERROR);
}

/************************** PUBLIC FUNCTIONS **************************/

void Fifo_void_test::addValuesToQueue()
{
    for(int i=0; i<TAB_SIZE; ++i)
    {
        if(fifoType == COPY)
            list_push_C(list, &(tab[i]), sizeof(tab[i]), &errors);
        else
            list_push_NC(list, &(tab[i]), &errors);
        
        EXPECT_EQ(errors, QUEUE_OK);
    }
    EXPECT_EQ(list_getSize(list, &errors), TAB_SIZE);
    EXPECT_EQ(errors, QUEUE_OK);
}