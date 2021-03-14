#include <gtest/gtest.h>

#include "Mock_Project_Config.h"

extern "C"
{
    #include "Vector_Math.h"
}

class Vector_Math_UT : public ::testing::Test
{
public:
    Vector_Math_UT(){}
    ~Vector_Math_UT(){}

    virtual void SetUp(){}

    virtual void TearDown(){}
};

/************************** TESTS **************************/

TEST_F(Vector_Math_UT, getVelocity2D_test)
{
    vect2D_d move = { 2., 3. };
    vect2D_d velocityOUT = {};

    velocityOUT = getVelocity2D(move, 1.);

    EXPECT_NEAR(velocityOUT.x, 0.5547, 0.0001);
    EXPECT_NEAR(velocityOUT.y, 0.83205, 0.0001);
}

TEST_F(Vector_Math_UT, getVelocity3D_test)
{
    vect3D_d move = { 2., 3., 4.};
    vect3D_d velocityOUT = {};

    velocityOUT = getVelocity3D(move, 1.);

    EXPECT_NEAR(velocityOUT.x, 0.371391, 0.0001);
    EXPECT_NEAR(velocityOUT.y, 0.557086, 0.0001);
    EXPECT_NEAR(velocityOUT.z, 0.742781, 0.0001);
}
