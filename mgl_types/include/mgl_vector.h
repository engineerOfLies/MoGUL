#ifndef __MGL_VECTOR_H__
#define __MGL_VECTOR_H__

#include "mgl_types.h"

/*
 * The Vector Types
 * Not to be confused with the vector lists from STL
 */
typedef struct
{
  MglDouble x;
  MglDouble y;
}MglVec2D;

typedef struct
{
  MglDouble x;
  MglDouble y;
  MglDouble z;
}MglVec3D;

typedef struct
{
  MglDouble x;
  MglDouble y;
  MglDouble z;
  MglDouble w;
}MglVec4D;

#define mgl_vec_in_rect(v, r)  ((v.x >= r.x)&&(v.x < (r.x + r.w))&&(v.y >= r.y)&&(v.y < (r.y + r.h)))

/**
 * @brief create and return an MglVec2D
 */
MglVec2D mgl_vec2d(MglDouble x, MglDouble y);

/**
 * @brief create and return an MglVec3D
 */
MglVec3D mgl_vec3d(MglDouble x, MglDouble y, MglDouble z);

/**
 * @brief create and return an MglVec4D
 */
MglVec4D mgl_vec4d(MglDouble x, MglDouble y, MglDouble z, MglDouble w);

/**
 * @brief sets the outvector to a unit vector pointing at the angle specified
 * @param out output.  Vector will be set to a unit vector rotated by radians
 * @param radians specify the angle of the vector to be set.
 */
void mgl_vec3d_set_angle_by_radians(MglVec3D *out,MglFloat radians);

void mgl_vec2d_set_angle_by_radians(MglVec2D *out,MglFloat radians);

/**
 * @brief returns the angle of a vector in 2D space.
 * @param x the x component of the vector
 * @param y the y component of the vector
 * @return the angle of the vector in radians.
 */
MglFloat mgl_vector_angle(MglFloat x,MglFloat y);

/**
 * @brief returns the angle of a vector in 2D space.
 * @param v the vector
 * @return the angle of the vector in radians.
 */
MglFloat mgl_vec2d_angle(MglVec2D v);

/**
 * @brief returns the angle between the two angles.
 * Takes into account wraping around 0 and 360
 * units are degrees
 * @Param a one of the angles in question
 * @Param b one of the angles in question
 * @return the angle, in degrees, between the two angles
 */
MglFloat mgl_angle_between_degrees(MglFloat a, MglFloat b);

void mgl_angle_clamp_degrees(MglFloat *a);


#endif
