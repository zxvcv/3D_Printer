#include <gtest/gtest.h>

#define TAB_SIZE_2D 8
#define TAB_SIZE_3D 27
#define ACCURACY 0.00001

extern "C"
{
    #include "vectorOperations.h"
}

class VectorOperations_test : public ::testing::Test {
public:
    virtual void SetUp(){}
    virtual void TearDown(){}

    vect2D_d testData2D[TAB_SIZE_2D][3] ={
    /*     move                 1D_velocity     expecterd_return    */
        { {  0.0 ,  0.0 },      {  1.0 ,  0.0 },    {  0.0 ,  0.0 } },
        { {  0.0 ,  1.0 },      {  1.0 ,  0.0 },    {  0.0 ,  1.0 } },
        { {  0.0 , -1.0 },      {  1.0 ,  0.0 },    {  0.0 , -1.0 } },
        { {  1.0 ,  0.0 },      {  1.0 ,  0.0 },    {  1.0 ,  0.0 } },
        { { -1.0 ,  0.0 },      {  1.0 ,  0.0 },    { -1.0 ,  0.0 } },
        { {  3.0 ,  4.0 },      {  1.0 ,  0.0 },    {  0.6 ,  0.8 } },
        { { -3.0 ,  4.0 },      {  1.0 ,  0.0 },    { -0.6 ,  0.8 } },
        { {  3.0 , -4.0 },      {  1.0 ,  0.0 },    {  0.6 , -0.8 } },
    };

    vect3D_d testData3D[TAB_SIZE_3D][3] ={
    /*     move                       1D_velocity            expecterd_return    */
        { {  0.0 ,  0.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 ,  0.0 ,  0.0 } },
        { {  0.0 ,  0.0 ,  1.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 ,  0.0 ,  1.0 } },
        { {  0.0 ,  0.0 , -1.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 ,  0.0 , -1.0 } },
        { {  0.0 ,  1.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 ,  1.0 ,  0.0 } },
        { {  0.0 , -1.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 , -1.0 ,  0.0 } },
        { {  0.0 ,  3.0 ,  4.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 ,  0.6 ,  0.8 } },
        { {  0.0 , -3.0 ,  4.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 , -0.6 ,  0.8 } },
        { {  0.0 ,  3.0 , -4.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 ,  0.6 , -0.8 } },
        { {  0.0 , -3.0 , -4.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.0 , -0.6 , -0.8 } },
        { {  1.0 ,  0.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    {  1.0 ,  0.0 ,  0.0 } },
        { { -1.0 ,  0.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    { -1.0 ,  0.0 ,  0.0 } },
        { {  3.0 ,  0.0 ,  4.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.6 ,  0.0 ,  0.8 } },
        { { -3.0 ,  0.0 ,  4.0 },      {  1.0 ,  0.0 ,  0.0 },    { -0.6 ,  0.0 ,  0.8 } },
        { {  3.0 ,  0.0 , -4.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.6 ,  0.0 , -0.8 } },
        { { -3.0 ,  0.0 , -4.0 },      {  1.0 ,  0.0 ,  0.0 },    { -0.6 ,  0.0 , -0.8 } },
        { {  3.0 ,  4.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.6 ,  0.8 ,  0.0 } },
        { { -3.0 ,  4.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    { -0.6 ,  0.8 ,  0.0 } },
        { {  3.0 , -4.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.6 , -0.8 ,  0.0 } },
        { { -3.0 , -4.0 ,  0.0 },      {  1.0 ,  0.0 ,  0.0 },    { -0.6 , -0.8 ,  0.0 } },
        { {  3.0 ,  4.0 ,  5.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.424264 ,  0.565685 ,  0.707107 } },
        { { -3.0 ,  4.0 ,  5.0 },      {  1.0 ,  0.0 ,  0.0 },    { -0.424264 ,  0.565685 ,  0.707107 } },
        { {  3.0 , -4.0 ,  5.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.424264 , -0.565685 ,  0.707107 } },
        { {  3.0 ,  4.0 , -5.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.424264 ,  0.565685 , -0.707107 } },
        { { -3.0 , -4.0 ,  5.0 },      {  1.0 ,  0.0 ,  0.0 },    { -0.424264 , -0.565685 ,  0.707107 } },
        { { -3.0 ,  4.0 , -5.0 },      {  1.0 ,  0.0 ,  0.0 },    { -0.424264 ,  0.565685 , -0.707107 } },
        { {  3.0 , -4.0 , -5.0 },      {  1.0 ,  0.0 ,  0.0 },    {  0.424264 , -0.565685 , -0.707107 } },
        { { -3.0 , -4.0 , -5.0 },      {  1.0 ,  0.0 ,  0.0 },    { -0.424264 , -0.565685 , -0.707107 } },
    };

    bool compareDouble(double compared, double expected);
};

/************************** TESTS **************************/

TEST_F(VectorOperations_test, vectorOperations_2D_test) 
{
    vect2D_d ret;

    for(int i=0; i<TAB_SIZE_2D; ++i)
    {
        ret = getVelocity2D(testData2D[i][0], testData2D[i][1].x);

        EXPECT_TRUE(compareDouble(ret.x, testData2D[i][2].x)) 
            << "i=" << i << " actual: " << ret.x << " expected: " << testData2D[i][2].x;

        EXPECT_TRUE(compareDouble(ret.y, testData2D[i][2].y)) 
            << "i=" << i << " actual: " << ret.y << " expected: " << testData2D[i][2].y;
    }
}

TEST_F(VectorOperations_test, vectorOperations_3D_test) 
{
    vect3D_d ret;

    for(int i=0; i<TAB_SIZE_3D; ++i)
    {
        ret = getVelocity3D(testData3D[i][0], testData3D[i][1].x);

        EXPECT_TRUE(compareDouble(ret.x, testData3D[i][2].x))
            << "i=" << i << " actual: " << ret.x << " expected: " << testData3D[i][2].x;

        EXPECT_TRUE(compareDouble(ret.y, testData3D[i][2].y))
            << "i=" << i << " actual: " << ret.y << " expected: " << testData3D[i][2].y;

        EXPECT_TRUE(compareDouble(ret.z, testData3D[i][2].z))
            << "i=" << i << " actual: " << ret.z << " expected: " << testData3D[i][2].z;
    }
}

/************************** PUBLIC FUNCTIONS **************************/

bool VectorOperations_test::compareDouble(double compared, double expected)
{
    if(expected + ACCURACY > compared && expected - ACCURACY < compared)
    {
        return true;
    }
    else
    {
        return false;
    }
}
