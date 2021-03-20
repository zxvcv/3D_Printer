#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Vector_Math.h"
}

class Mock_Vector_Math {
public:
    Mock_Vector_Math() { mock = this; }
    MOCK_METHOD2(getVelocity2D, vect2D_d(vect2D_d move, double v));
    MOCK_METHOD2(getVelocity3D, vect3D_d(vect3D_d move, double v));

    static Mock_Vector_Math* mock;
};
