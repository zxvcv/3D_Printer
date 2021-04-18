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

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

bool compare_points(point3D_d p1, point3D_d p2, double accuracy)
{
    if(p1.a + accuracy > p2.a && p1.a - accuracy < p2.a &&
       p1.b + accuracy > p2.b && p1.b - accuracy < p2.b)
    {
        return true;
    }
    else
    {
        return false;
    }
}


double get_distance_between_points(point3D_d p1, point3D_d p2)
{
    return sqrt(pow(p2.a - p1.a, 2) + pow(p2.b - p1.b, 2));
}


int get_relative_point_direction(double point, double reference)
{
    if(point > reference)
    {
        return 1;
    }
    else if(point < reference)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


point3D_i get_circle_step_direction(point3D_d position, point3D_d circle_center, bool is_clockwise)
{
    point3D_i dir;

    int direction_x = get_relative_point_direction(position.a, circle_center.a);
    int direction_y = get_relative_point_direction(position.b, circle_center.b);

    if(direction_x < 0) { dir.b = 1; }
    else if(direction_x > 0) { dir.b = -1; }
    else if(direction_y < 0) { dir.b = 1; }
    else(direction_y > 0) { dir.b = -1; }

    if(direction_y < 0) { dir.a = -1; }
    else if(direction_y > 0) { dir.a = 1; }
    else if(direction_x < 0) { dir.a = 1; }
    else(direction_x > 0) { dir.a = -1; }

    if(!is_clockwise)
    {
        dir.a *= -1;
        dir.b *= -1;
    }

    return dir;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
