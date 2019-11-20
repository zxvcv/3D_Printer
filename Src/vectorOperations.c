/*
 * vectorOperations.c
 *
 *  Created on: 20.11.2019
 *      Author: zxvcv
 */

#include "vectorOperations.h"
#include <math.h>

vect2D_d getVelocity2D(vect2D_d move, double v){
	double alfa;
	vect2D_d vel = {1.,1.};

	if(move.x == 0 || move.y == 0){
		vel.x = move.x * v;
		vel.y = move.y * v;
	}else{
		alfa = atan(move.y / move.x);
		vel.x = cos(alfa) * v;
		vel.y = sin(alfa) * v;
	}
	return vel;
}

vect3D_d getVelocity3D(vect3D_d move, double v){
	double alfa, beta;
	double mProjXY, vProjXY;
	vect3D_d vel;
	vect2D_d vect2D;

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
		alfa = atan(move.y / move.x);
		mProjXY = move.y / sin(alfa);
		beta = atan(mProjXY / move.z);
		vel.z = cos(beta) * v;
		vProjXY = sin(beta) * v;
		vel.x = cos(alfa) * vProjXY;
		vel.y = sin(alfa) * vProjXY;
	}
	return vel;
}

