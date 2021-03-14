#include <gtest/gtest.h>
#include <stdint.h>

#include "Mock_Project_Config.h"
#include "Mock_HAL_Drivers.h"
#include "Mock_Error_Codes.h"

using ::testing::_;
using ::testing::Assign;
using ::testing::Return;
using ::testing::SetArrayArgument;

#define _24AA01_MEM_SIZE        ((uint16_t)128) //Bytes
#define _24AA01_PAGE_SIZE       ((uint16_t)8)   //Bytes

extern "C"
{
    #include "EEPROM_24AA01.h"
}


class EEPROM_24AA01_UT : public ::testing::Test
{
public:
    EEPROM_24AA01_UT()
    {
         mock_HAL_Drivers = new Mock_HAL_Drivers();
    }
    ~EEPROM_24AA01_UT()
    {
        delete  mock_HAL_Drivers;
    }

    virtual void SetUp()
    {
        memory = new uint8_t[128];
        hi2c = (I2C_HandleTypeDef*)malloc(sizeof(I2C_HandleTypeDef));
        eeprom = (EEPROMSettings*)malloc(sizeof(EEPROMSettings));

        EEPROM_init(eeprom, hi2c);
    }

    virtual void TearDown()
    {
        free(eeprom);
        free(hi2c);

        delete[] memory;
    }


    uint8_t* memory;
    EEPROMSettings* eeprom;
    I2C_HandleTypeDef* hi2c;

    static Mock_HAL_Drivers* mock_HAL_Drivers;
};

Mock_HAL_Drivers* EEPROM_24AA01_UT::mock_HAL_Drivers;



/************************** TESTS **************************/

TEST_F(EEPROM_24AA01_UT, EEPROM_clear_basic)
{
    Std_Err stdErr;

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        memory[i] = i + 1;
    }

    uint8_t offset = 0;
    for(int i=0; i<_24AA01_MEM_SIZE/_24AA01_PAGE_SIZE; ++i)
    {
        EXPECT_CALL(*mock_HAL_Drivers, HAL_I2C_Mem_Write(_, _, offset, 1, _, _24AA01_PAGE_SIZE, _))
            .Times(1).WillRepeatedly(DoAll(
                Assign(memory + offset + 0, (uint8_t)0),
                Assign(memory + offset + 1, (uint8_t)0),
                Assign(memory + offset + 2, (uint8_t)0),
                Assign(memory + offset + 3, (uint8_t)0),
                Assign(memory + offset + 4, (uint8_t)0),
                Assign(memory + offset + 5, (uint8_t)0),
                Assign(memory + offset + 6, (uint8_t)0),
                Assign(memory + offset + 7, (uint8_t)0),
                Return(HAL_OK)
                ));

        offset += _24AA01_PAGE_SIZE;
    }

    stdErr = EEPROM_clear(eeprom);
    EXPECT_EQ(stdErr, STD_OK);

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        EXPECT_EQ(memory[i], 0);
    }
}

TEST_F(EEPROM_24AA01_UT, EEPROM_clear_basic_err)
{
    Std_Err stdErr;

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        memory[i] = i + 1;
    }
    eeprom->isReady = false;

    stdErr = EEPROM_clear(eeprom);
    EXPECT_EQ(stdErr, STD_BUSY_ERROR);

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        EXPECT_EQ(memory[i], i + 1);
    }
}

TEST_F(EEPROM_24AA01_UT, EEPROM_write_all)
{
    Std_Err stdErr;
    uint8_t test[128];

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        test[i] = i + 50;
        memory[i] = i + 1;
    }

    uint8_t offset = 0;
    for(int i=0; i<_24AA01_MEM_SIZE/_24AA01_PAGE_SIZE; ++i)
    {
        EXPECT_CALL(*mock_HAL_Drivers, HAL_I2C_Mem_Write(_, _, offset, 1, test + offset, _24AA01_PAGE_SIZE, _))
            .Times(1).WillRepeatedly(DoAll(
                Assign(memory + offset + 0, test[offset + 0]),
                Assign(memory + offset + 1, test[offset + 1]),
                Assign(memory + offset + 2, test[offset + 2]),
                Assign(memory + offset + 3, test[offset + 3]),
                Assign(memory + offset + 4, test[offset + 4]),
                Assign(memory + offset + 5, test[offset + 5]),
                Assign(memory + offset + 6, test[offset + 6]),
                Assign(memory + offset + 7, test[offset + 7]),
                Return(HAL_OK)
                ));

        offset += _24AA01_PAGE_SIZE;
    }

    stdErr = EEPROM_writeData(eeprom, 0x00, test, 128);
    EXPECT_EQ(stdErr, STD_OK);

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        EXPECT_EQ(memory[i], i + 50);
    }
}

TEST_F(EEPROM_24AA01_UT, EEPROM_write_16bits_in_place)
{
    Std_Err stdErr;
    uint8_t test[2];

    test[0] = 210; test[1] = 220;

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        memory[i] = i + 1;
    }

    uint8_t offset = 8;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_I2C_Mem_Write(_, _, offset, 1, test, 2, _))
        .Times(1).WillRepeatedly(DoAll(
            Assign(memory + offset + 0, test[0]),
            Assign(memory + offset + 1, test[1]),
            Return(HAL_OK)
            ));

    stdErr = EEPROM_writeData(eeprom, offset, test, 2);
    EXPECT_EQ(stdErr, STD_OK);

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        if(i == offset)
            EXPECT_EQ(memory[i], test[0]);
        else if(i == (offset + 1))
            EXPECT_EQ(memory[i], test[1]);
        else
            EXPECT_EQ(memory[i], i + 1);
    }
}

TEST_F(EEPROM_24AA01_UT, EEPROM_write_16bits_out_place)
{
    Std_Err stdErr;
    uint8_t test[2];

    test[0] = 0; test[1] = 0;

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        memory[i] = 255;
    }

    uint8_t offset = 15;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_I2C_Mem_Write(_, _, offset, 1, test, 1, _))
        .Times(1).WillRepeatedly(DoAll(
            Assign(memory + offset + 0, test[0]),
            Return(HAL_OK)
            ));

    EXPECT_CALL(*mock_HAL_Drivers, HAL_I2C_Mem_Write(_, _, offset + 1, 1, test + 1, 1, _))
        .Times(1).WillRepeatedly(DoAll(
            Assign(memory + offset + 1, test[1]),
            Return(HAL_OK)
            ));

    stdErr = EEPROM_writeData(eeprom, offset, test, 2);
    EXPECT_EQ(stdErr, STD_OK);

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        if(i == offset)
            EXPECT_EQ(memory[i], test[0]);
        else if(i == (offset + 1))
            EXPECT_EQ(memory[i], test[1]);
        else
            EXPECT_EQ(memory[i], 255);
    }
}
/*
I DONT KNOW WHY THIS TEST DONT WANNA WORK ??? (SEGMENTATION FAULT)
TEST_F(EEPROM_24AA01_UT, EEPROM_24AA01__read16bits__test)
{
    Std_Err err;
    uint8_t test[2];

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        memory[i] = i + 1;
        test[i] = 0;
    }

    uint8_t offset = 8;

    EXPECT_CALL(*mock, HAL_I2C_Mem_Read(_, _, offset, 1, test, 2, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(memory + offset + 0, memory + offset + 2),
            Return(HAL_OK)
        ));

    err = EEPROM_readData(eeprom, offset, test, 2);
    EXPECT_EQ(err, STD_OK);

    for(int i=0; i<_24AA01_MEM_SIZE; ++i)
    {
        EXPECT_EQ(memory[i], i + 1);
    }
    EXPECT_EQ(test[0], 9);
    EXPECT_EQ(test[1], 10);
}
*/
