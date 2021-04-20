/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: GCode_Parser
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "_helpers.h"
#include <stdlib.h>
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

PixelDirection get_next_circle_pixel_direction(point3D_d position, point3D_d circle_center,
    point3D_i direction, double radius, point3D_i step_size, bool is_clockwise)
{
    point3D_d new_position_X, new_position_Y, new_position_XY;
    double newR_X, newR_Y, newR_XY;
    double diff_X, diff_Y, diff_XY;

    new_position_X.a  = position.a + direction.a * ((double)step_size.a / ACCURACY);
    new_position_X.b  = position.b;

    new_position_Y.a  = position.a;
    new_position_Y.b  = position.b + direction.b * ((double)step_size.b / ACCURACY);

    new_position_XY.a = position.a + direction.a * ((double)step_size.a / ACCURACY);
    new_position_XY.b = position.b + direction.b * ((double)step_size.b / ACCURACY);

    newR_X  = get_distance_between_points(circle_center, new_position_X);
    newR_Y  = get_distance_between_points(circle_center, new_position_Y);
    newR_XY = get_distance_between_points(circle_center, new_position_XY);

    diff_X  = abs(radius - newR_X);
    diff_Y  = abs(radius - newR_Y);
    diff_XY = abs(radius - newR_XY);

    if(diff_X < diff_Y && diff_X < diff_XY)
    {
        return FIRST_AXIS;
    }
    else if(diff_Y < diff_XY)
    {
        return SECOND_AXIS;
    }
    else
    {
        return BOTH_AXIS;
    }
}


point3D_d get_next_circle_line(point3D_d start, point3D_d end, point3D_d circle_center,
    double radius, point3D_i step_size, bool is_clockwise)
{
    point3D_d current_point = start;
    point3D_d previous_point;
    point3D_i direction;
    PixelDirection next_step, previous_step;

    direction = get_circle_step_direction(current_point, circle_center, is_clockwise);
    next_step = get_next_circle_pixel_direction(current_point, circle_center, direction, radius,
        step_size, is_clockwise);

    while(true)
    {
        previous_step = next_step;
        previous_point.a = current_point.a;
        previous_point.b = current_point.b;
        switch(next_step)
        {
            case FIRST_AXIS:
                current_point.a += direction.a * step_size.a;
                break;
            case SECOND_AXIS:
                current_point.b += direction.b * step_size.b;
                break;
            case BOTH_AXIS:
                current_point.a += direction.a * step_size.a;
                current_point.b += direction.b * step_size.b;
                break;
        }

        direction = get_circle_step_direction(current_point, circle_center, is_clockwise);
        next_step = get_next_circle_pixel_direction(current_point, circle_center, direction, radius,
            step_size, is_clockwise);

        if(compare_points(current_point, end, ((double)1)/ACCURACY))
        {
            return end;
        }

        if(previous_step != next_step)
        {
            return previous_point;
        }
    }
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
