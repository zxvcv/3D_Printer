#ifndef _VECTOR_OPERATIONS
#define _VECTOR_OPERATIONS

typedef struct vect3D_d{
	double x;
	double y;
	double z;
}vect3D_d;

typedef struct vect2D_d{
	double x;
	double y;
}vect2D_d;


vect2D_d getVelocity2D(vect2D_d move, double v);
vect3D_d getVelocity3D(vect3D_d move, double v);

#endif //_VECTOR_OPERATIONS
