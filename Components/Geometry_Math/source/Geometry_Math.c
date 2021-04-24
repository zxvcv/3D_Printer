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
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <math.h>
#include "Geometry_Math.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

bool compare_doubles(double d1, double d2, double accuracy)
{
    if(d1 + accuracy > d2 && d1 - accuracy < d2)
    {
        return true;
    }
    else
    {
        return false;
    }
}


double get_angle_in_degrees(double rad_angle)
{
    double angle = (rad_angle * 360) / (2 * M_PI);
    return angle;
}


double get_angle_in_radians(double degree_angle)
{
    double angle = (degree_angle * 2 * M_PI) / 360;
    return angle;
}


double get_distance_between_points(Point2D_d p1, Point2D_d p2)
{
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}


Point2D_d get_line_equation_from_points(Point2D_d v1, Point2D_d v2)
{
    Point2D_d parameters;

    parameters.x = (v1.y - v2.y) / (v1.x - v2.x);
    parameters.y = v1.y - parameters.x * v1.x;

    return parameters;
}


Point2D_d get_next_circle_point(Point2D_d start_point, Point2D_d circle_center, double radius,
    double angle_step, bool is_clockwise)
{
    Point2D_d lineL = get_line_equation_from_points(start_point, circle_center);

    double alphaL = get_angle_in_degrees(atan(lineL.x));
    double alphaK = is_clockwise ? alphaL - angle_step : alphaL + angle_step;
    // printf("ALPHAS[%f %f]\n", alphaL, alphaK);
    Point2D_d lineK;
    lineK.x = tan(get_angle_in_radians(alphaK));
    lineK.y = circle_center.y - lineK.x * circle_center.x;
    // printf("LINEK[%f %f]\n", lineK.x, lineK.y);

    if(lineK.x > ACCURACY || lineK.x < -ACCURACY)
    {
        // oznacza natrafienie na asymptote funkcji tg() - 90st i 270st
        Point2D_d new_point;
        new_point.x = circle_center.x;
        new_point.y = circle_center.y + (start_point.y < circle_center.y ? -radius : radius);
        return new_point;
    }

    Point2D_d next_point1, next_point2;
    double A = 1 + pow(lineK.x, 2);
    double B = 2 * (lineK.x * lineK.y - circle_center.x - lineK.x * circle_center.y);
    double C = pow(circle_center.x, 2) + pow(lineK.y - circle_center.y, 2) - pow(radius, 2);
    double delta = pow(B, 2) - 4 * A * C;

    next_point1.x = (-B - sqrt(delta)) / (2 * A);
    next_point1.y = lineK.x * next_point1.x + lineK.y;

    next_point2.x = (-B + sqrt(delta)) / (2 * A);
    next_point2.y = lineK.x * next_point2.x + lineK.y;

    double len1 = get_distance_between_points(next_point1, start_point);
    double len2 = get_distance_between_points(next_point2, start_point);

    return ((len1 < len2) ? next_point1 : next_point2);
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
