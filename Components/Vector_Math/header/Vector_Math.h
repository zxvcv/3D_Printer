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
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef VECTOR_MATH_H_
#define VECTOR_MATH_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct vect3D_d{
    double x;
    double y;
    double z;
}vect3D_d;

typedef struct vect2D_d{
    double x;
    double y;
}vect2D_d;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

vect2D_d getVector2D(double p1X, double p1Y, double p2X, double p2Y);

double getVectorLength2D(vect2D_d vect);

double getVectorScalarProduct2D(vect2D_d vect1, vect2D_d vect2);

double getAngleBetweenVectors2D(vect2D_d vect1, vect2D_d vect2);

vect2D_d getVelocity2D(vect2D_d move, double v);

vect3D_d getVelocity3D(vect3D_d move, double v);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* VECTOR_MATH_H_ */
