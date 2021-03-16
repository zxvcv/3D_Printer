#include "Mock_Vector_Math.h"


Mock_Vector_Math* Mock_Vector_Math::mock;

extern "C"
{
    vect2D_d getVelocity2D(vect2D_d move, double v)
    {
        return Mock_Vector_Math::mock->getVelocity2D(move, v);
    }

    vect3D_d getVelocity3D(vect3D_d move, double v)
    {
        return Mock_Vector_Math::mock->getVelocity3D(move, v);
    }
}