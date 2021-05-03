/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Geometry_Math
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef GEOMETRY_MATH_H_
#define GEOMETRY_MATH_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <stdbool.h>
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

typedef struct Point2D_d_Tag{
    double x;
    double y;
}Point2D_d;

typedef struct Point3D_d_Tag{
    double x;
    double y;
    double z;
}Point3D_d;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Point2D_d get_point2D_form_point3D(Point3D_d point3D, bool withX, bool withY, bool withZ);

Point3D_d get_point3D_form_point2D(Point2D_d point2D, bool withX, bool withY, bool withZ);

bool compare_doubles(double d1, double d2, double accuracy);

double get_angle_in_degrees(double rad_angle);

double get_angle_in_radians(double degree_angle);

double get_distance_between_points(Point2D_d p1, Point2D_d p2);

Point2D_d get_line_equation_from_points(Point2D_d v1, Point2D_d v2);

Point2D_d get_next_circle_point(Point2D_d start_point, Point2D_d circle_center, double radius,
    double angle_step, bool is_clockwise);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* GEOMETRY_MATH_H_ */
