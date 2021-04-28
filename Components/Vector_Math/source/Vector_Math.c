/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Vector_Math
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <math.h>
#include "Vector_Math.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

vect2D_d getVector2D(double p1X, double p1Y, double p2X, double p2Y)
{
    vect2D_d vect;
    vect.x = p1X - p2X;
    vect.y = p1Y - p2Y;
    return vect;
}


double getVectorLength2D(vect2D_d vect)
{
    return sqrt(pow(vect.x, 2) + pow(vect.y, 2));
}


double getVectorScalarProduct2D(vect2D_d vect1, vect2D_d vect2)
{
    return vect1.x * vect2.x + vect1.y * vect2.y;
}


double getAngleBetweenVectors2D(vect2D_d vect1, vect2D_d vect2)
{
    return acos(getVectorScalarProduct2D(vect1, vect2) /
        (getVectorLength2D(vect1) * getVectorLength2D(vect2)));
}


vect2D_d getVelocity2D(vect2D_d move, double v)
{
    double alfa;
    vect2D_d vel;
    vect2D_d sign;

    sign.x = signbit(move.x) ? -1 : 1;
    sign.y = signbit(move.y) ? -1 : 1;

    if(move.x == 0 && move.y == 0)
    {
        vel.x = 0;
        vel.y = 0;
    }
    else if(move.x == 0)
    {
        vel.x = 0;
        vel.y = v * sign.y;
    }
    else if(move.y == 0)
    {
        vel.x = v * sign.x;
        vel.y = 0;
    }
    else
    {
        alfa = atan(move.y / move.x);
        vel.x = fabs(cos(alfa) * v) * sign.x;
        vel.y = fabs(sin(alfa) * v) * sign.y;
    }

    return vel;
}


vect3D_d getVelocity3D(vect3D_d move, double v)
{
    double alfa, beta;
    double mProjXY, vProjXY;
    vect3D_d vel;
    vect2D_d vect2D;
    vect3D_d sign;

    if(move.x == 0)
    {
        vel.x = 0.;
        vect2D.x = move.y;
        vect2D.y = move.z;
        vect2D = getVelocity2D(vect2D, v);
        vel.y = vect2D.x;
        vel.z = vect2D.y;
    }
    else if(move.y == 0)
    {
        vel.y = 0.;
        vect2D.x = move.x;
        vect2D.y = move.z;
        vect2D = getVelocity2D(vect2D, v);
        vel.x = vect2D.x;
        vel.z = vect2D.y;
    }
    else if(move.z == 0)
    {
        vel.z = 0.;
        vect2D.x = move.x;
        vect2D.y = move.y;
        vect2D = getVelocity2D(vect2D, v);
        vel.x = vect2D.x;
        vel.y = vect2D.y;
    }
    else
    {
        sign.x = signbit(move.x) ? -1 : 1;
        sign.y = signbit(move.y) ? -1 : 1;
        sign.z = signbit(move.z) ? -1 : 1;

        alfa = atan(move.y / move.x);
        mProjXY = move.y / sin(alfa);
        beta = atan(mProjXY / move.z);
        vel.z = fabs(cos(beta) * v) * sign.z;
        vProjXY = sin(beta) * v;
        vel.x = fabs(cos(alfa) * vProjXY) * sign.x;
        vel.y = fabs(sin(alfa) * vProjXY) * sign.y;
    }

    return vel;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
