#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_Error_Codes.h"

#define TAB_SIZE 5

extern "C"
{
    #include "FIFO_void.h"
}

class FIFO_void_UT: public ::testing::Test {
public:
    FIFO_void_UT(){}
    ~FIFO_void_UT(){}

    virtual void SetUp()
    {
        Std_Err stdErr;

        ASSERT_EQ(list, nullptr);
        stdErr = fifo_create(&list);
        ASSERT_EQ(stdErr, STD_OK);
        ASSERT_NE(list, nullptr);
    }

    virtual void TearDown()
    {
        Std_Err stdErr;

        if(fifoType == COPY)
            stdErr = fifo_delete_C(&list);
        else
            stdErr = fifo_delete_NC(&list);

        ASSERT_EQ(stdErr, STD_OK);
        ASSERT_EQ(list, nullptr);
    }

    enum FifoType{
        COPY,
        NON_COPY
    }fifoType;
    Fifo* list = NULL;

    void addValuesToQueue();

    /* test values */
    int tab[TAB_SIZE] = {5, 10, 15, 20, 25};
};

/************************** TESTS **************************/

TEST_F(FIFO_void_UT, listC_basicOperations)
{
    Std_Err stdErr;
    int* data = nullptr;

    fifoType = COPY;

    EXPECT_EQ(fifo_getSize(list), 0);

    for(int i=0; i<TAB_SIZE; ++i)
    {
        stdErr = fifo_push_C(list, &(tab[i]), sizeof(tab[i]));
        EXPECT_EQ(stdErr, STD_OK);

        EXPECT_EQ(fifo_getSize(list), i+1);
        EXPECT_EQ(stdErr, STD_OK);

        stdErr = fifo_front(list, (void**)&data);
        EXPECT_EQ(stdErr, STD_OK);
        EXPECT_EQ(*data, tab[0]);

        stdErr = fifo_front(list, (void**)&data);
        EXPECT_EQ(stdErr, STD_OK);
        EXPECT_NE(data, &(tab[0]));
    }

    for(int i=0; i<TAB_SIZE; ++i)
    {
        stdErr = fifo_front(list, (void**)&data);
        EXPECT_EQ(stdErr, STD_OK);
        EXPECT_EQ(*data, tab[i]);

        EXPECT_EQ(fifo_getSize(list), TAB_SIZE-i);

        stdErr = fifo_pop_C(list);
        EXPECT_EQ(stdErr, STD_OK);

        EXPECT_EQ(fifo_getSize(list), TAB_SIZE-i-1);
    }

    EXPECT_EQ(fifo_getSize(list), 0);
}

TEST_F(FIFO_void_UT, listC_clear)
{
    Std_Err stdErr;

    fifoType = COPY;

    addValuesToQueue();

    stdErr = fifo_clear_C(list);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(fifo_getSize(list), 0);
}

TEST_F(FIFO_void_UT, listC_getDataSize)
{
    fifoType = COPY;

    addValuesToQueue();

    for(int i=0; i<TAB_SIZE; ++i)
    {
        EXPECT_EQ(fifo_getDataSize(list), sizeof(tab[i]));
    }
}

TEST_F(FIFO_void_UT, listC_uninitializedList)
{
    Std_Err stdErr;

    fifoType = COPY;

    stdErr = fifo_delete_C(&list);
    ASSERT_EQ(stdErr, STD_OK);
    ASSERT_EQ(list, nullptr);

    for(int i=0; i<TAB_SIZE; ++i)
    {
        stdErr = fifo_push_C(list, &(tab[i]), sizeof(tab[i]));
        ASSERT_EQ(stdErr, STD_ERROR);
    }
    EXPECT_EQ(fifo_getSize(list), 0);

    stdErr = fifo_create(&list);
    ASSERT_EQ(stdErr, STD_OK);
    ASSERT_NE(list, nullptr);
}

TEST_F(FIFO_void_UT, listNC_basicOperations)
{
    Std_Err stdErr;
    int* data;

    fifoType = NON_COPY;

    EXPECT_EQ(fifo_getSize(list), 0);

    for(int i=0; i<TAB_SIZE; ++i)
    {
        stdErr = fifo_push_NC(list, &(tab[i]));
        EXPECT_EQ(stdErr, STD_OK);

        EXPECT_EQ(fifo_getSize(list), i+1);

        stdErr = fifo_front(list, (void**)&data);
        EXPECT_EQ(stdErr, STD_OK);
        EXPECT_EQ(*data, tab[0]);

        stdErr = fifo_front(list, (void**)&data);
        EXPECT_EQ(stdErr, STD_OK);
        EXPECT_EQ(data, &(tab[0]));
    }

    for(int i=0; i<TAB_SIZE; ++i)
    {
        stdErr = fifo_front(list, (void**)&data);
        EXPECT_EQ(stdErr, STD_OK);
        EXPECT_EQ(*data, tab[i]);

        EXPECT_EQ(fifo_getSize(list), TAB_SIZE-i);

        stdErr = fifo_pop_NC(list);
        EXPECT_EQ(stdErr, STD_OK);

        EXPECT_EQ(fifo_getSize(list), TAB_SIZE-i-1);
    }

    EXPECT_EQ(fifo_getSize(list), 0);
}

TEST_F(FIFO_void_UT, listNC_clear)
{
    Std_Err stdErr;

    fifoType = NON_COPY;

    addValuesToQueue();

    stdErr = fifo_clear_NC(list);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(fifo_getSize(list), 0);
}

TEST_F(FIFO_void_UT, listNC_getDataSize)
{
    fifoType = NON_COPY;

    addValuesToQueue();

    for(int i=0; i<TAB_SIZE; ++i)
    {
        EXPECT_EQ(fifo_getDataSize(list), 0);
    }
}

TEST_F(FIFO_void_UT, listNC_uninitializedList)
{
    Std_Err stdErr;

    fifoType = NON_COPY;

    stdErr = fifo_delete_C(&list);
    ASSERT_EQ(stdErr, STD_OK);
    ASSERT_EQ(list, nullptr);

    for(int i=0; i<TAB_SIZE; ++i)
    {
        stdErr = fifo_push_NC(list, &(tab[i]));
        EXPECT_EQ(stdErr, STD_ERROR);
    }
    EXPECT_EQ(fifo_getSize(list), 0);

    stdErr = fifo_create(&list);
    ASSERT_EQ(stdErr, STD_OK);
    ASSERT_NE(list, nullptr);
}

/************************** PUBLIC FUNCTIONS **************************/

void FIFO_void_UT::addValuesToQueue()
{
    Std_Err stdErr;

    for(int i=0; i<TAB_SIZE; ++i)
    {
        if(fifoType == COPY)
            stdErr = fifo_push_C(list, &(tab[i]), sizeof(tab[i]));
        else
            stdErr = fifo_push_NC(list, &(tab[i]));

        EXPECT_EQ(stdErr, STD_OK);
    }
    EXPECT_EQ(fifo_getSize(list), TAB_SIZE);
}