/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *  vectorOperations.c
 *
 *  Created on: 20.11.2019
 *      Author: zxvcv
 * =======================================================================================================
 * COMMENTS:
 *
 * =======================================================================================================
 * EXAMPLE:
 *
 ********************************************************************************************************** */



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include "vectorOperations.h"
#include <math.h>



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

vect2D_d getVelocity2D(vect2D_d move, double v){
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

vect3D_d getVelocity3D(vect3D_d move, double v){
	double alfa, beta;
	double mProjXY, vProjXY;
	vect3D_d vel;
	vect2D_d vect2D;
	vect3D_d sign;

	if(move.x == 0){
		vel.x = 0.;
		vect2D.x = move.y;
		vect2D.y = move.z;
		vect2D = getVelocity2D(vect2D, v);
		vel.y = vect2D.x;
		vel.z = vect2D.y;
	} else if(move.y == 0){
		vel.y = 0.;
		vect2D.x = move.x;
		vect2D.y = move.z;
		vect2D = getVelocity2D(vect2D, v);
		vel.x = vect2D.x;
		vel.z = vect2D.y;
	} else if(move.z == 0){
		vel.z = 0.;
		vect2D.x = move.x;
		vect2D.y = move.y;
		vect2D = getVelocity2D(vect2D, v);
		vel.x = vect2D.x;
		vel.y = vect2D.y;
	}else{
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











