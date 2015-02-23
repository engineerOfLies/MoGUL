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

/**
 * 3D Vector Math
 * These macros handle most of the common operations for vector math.
 */

#define mgl_vec4d_into_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y,ar[2] = vec.z,ar[3] = vec.w)
#define mgl_vec3d_into_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y,ar[2] = vec.z)
#define mgl_vec2d_into_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y)

/**
 * @brief calculates the cross product of the two input vectors
 * @param out the resultant cross product
 * @param v1 one of the components
 * @param v2 one of the components
 */
void mgl_vec3d_cross_product(MglVec3D *out, MglVec3D v1, MglVec3D v2);

/**
 * @brief Macro to get the dot product from a vector.
 *
 * @param a MglVect3D component of the dot product
 * @param b MglVect3D component of the dot product
 *
 * @return the calculated dot product
 */
#define mgl_vec4d_dot_product(a,b)      (a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w)
#define mgl_vec3d_dot_product(a,b)      (a.x*b.x+a.y*b.y+a.z*b.z)
#define mgl_vec2d_dot_product(a,b)      (a.x*b.x+a.y*b.y)

/**
 * @brief copies the data from one vector into another
 *
 * @param dst the destination vector
 * @param src the source vector
 */
#define mgl_vec2d_copy(dst,src)  (dst.x = src.x,dst.y = src.y)
#define mgl_vec3d_copy(dst,src)  (dst.x = src.x,dst.y = src.y,dst.z = src.z)
#define mgl_vec4d_copy(dst,src)  (dst.x = src.x,dst.y = src.y,dst.z = src.z,dst.w=src.w)

/**
 * @brief checks if vectors are exactly matches of each other
 *
 * @param a one vector to check
 * @param b other vector to check
 */
#define mgl_vec2d_equal(a,b) ((a.x == b.x)&&(a.y == b.y))
#define mgl_vec3d_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z))
#define mgl_vec4d_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z)&&(a.w == b.w))

/**
 * @brief Macro to subtract two vectors
 * varient ending in p takes a pointer to MglVect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result MglVect3D output
 * @param a MglVect3D input
 * @param b MglVect3D input
 */
#define mgl_vec2d_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y)
#define mgl_vec3d_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y,dst.z=a.z-b.z)
#define mgl_vec4d_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y,dst.z=a.z-b.z,dst.w=a.w-b.w)

/**
 * @brief Macro to add two vectors
 * varient ending in p takes a pointer to MglVect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result MglVect3D output
 * @param a MglVect3D input
 * @param b MglVect3D input
 */
#define mgl_vec2d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y)
#define mgl_vec3d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z)
#define mgl_vec4d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z,dst.w = a.w+b.w)

/**
 * @brief Macro to scale a vector by a scalar value
 * varient ending in p takes a pointer to MglVect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result MglVect3D output
 * @param src MglVect3D input
 * @Param factpr the scalar value to scale the vector by.
 */
#define mgl_vec2d_scale(dst,src,factor) (dst.x = src.x *factor,\
                                         dst.y = src.y *factor)
#define mgl_vec3d_scale(dst,src,factor) (dst.x = src.x *factor,\
                                         dst.y = src.y *factor,\
                                         dst.z = src.z *factor)
#define mgl_vec4d_scale(dst,src,factor) (dst.x = src.x *factor,\
                                         dst.y = src.y *factor,\
                                         dst.z = src.z *factor,\
                                         dst.w = src.w *factor)
/**
 * @brief Macro that sets vector to zero.
 * @param a MglVect[2D|3D|4D] input
 */

#define mgl_vec2d_clear(a)       (a.x=a.y=0)
#define mgl_vec3d_clear(a)       (a.x=a.y=a.z=0)
#define mgl_vec4d_clear(a)       (a.x=a.y=a.z=a.w=0)

/**
 * @brief Macro to get the negative of a vector
 *
 * @param src MglVect[2D|3D|4D] input
 * @param dst MglVect[2D|3D|4D] negated output
 */
#define mgl_vec2d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y)
#define mgl_vec3d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z)
#define mgl_vec4d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z,dst.w = -src.w)

/**
 * @brief Macro to set the components of the vector
 *
 * @param v MglVect3D output
 * @param a MglFloat x component
 * @param b MglFloat y component
 * @param c MglFloat z component (only in 3D & 4D version)
 * @param d MglFloat w component (only in 4D version)
 */
#define mgl_vec2d_set(v, a, b)  (v.x=(a), v.y=(b))
#define mgl_vec3d_set(v, a, b, c)  (v.x=(a), v.y=(b), v.z=(c))
#define mgl_vec4d_set(v, a, b, c,d)  (v.x=(a), v.y=(b), v.z=(c), v.w=(d))

MglVec2D *mgl_vec2d_new();
MglVec3D *mgl_vec3d_new();
MglVec4D *mgl_vec4d_new();

MglVec2D *mgl_vec2d_dup(MglVec2D old);
MglVec3D *mgl_vec3d_dup(MglVec3D old);
MglVec4D *mgl_vec4d_dup(MglVec4D old);

/**
 * @brief reflect a vector about a normal.  Simulates "Bounce"
 * @param out the resultant vector
 * @param normal the normal vector for the surface that is providing the bounce
 * @param in the input vector that will bounce off of the normal
 */
void mgl_vec2d_reflect(MglVec2D *out, MglVec2D normal,MglVec2D in);
void mgl_vec3d_reflect(MglVec3D *out, MglVec3D normal,MglVec3D in);
void mgl_vec4d_reflect(MglVec4D *out, MglVec4D normal,MglVec4D in);


/**
 * @brief normalizes the vector passed.  does nothing for a zero length vector.
 *
 * @param v pointer to the vector to be normalized.
 */
MglFloat mgl_vec2d_magnitude(MglVec2D V);
MglFloat mgl_vec3d_magnitude(MglVec3D V);
MglFloat mgl_vec4d_magnitude(MglVec4D V);

void mgl_vec2d_normalize (MglVec2D *V);
void mgl_vec3d_normalize (MglVec3D *V);
void mgl_vec4d_normalize (MglVec4D *V);

/**
 * @brief returns the magnitude squared, which is faster than getting the magnitude
 * which would involve taking the square root of a floating point number.
 * @param V the vector to get the magnitude for
 * @return the square of the magnitude of V
 */
MglFloat mgl_vec2d_magnitude_squared(MglVec2D V);
MglFloat mgl_vec3d_magnitude_squared(MglVec3D V);
MglFloat mgl_vec4d_magnitude_squared(MglVec4D V);

/**
 * @brief checks if the magnitude of V is less than size.  It does this without
 * doing square roots, which are costly.  It will still do floating point multiplication
 * @param V the vector to check
 * @param size the magnitude to check against
 * @return MglTrue if the magnitude of V is less than size or MglFalse otherwise
 */
MglBool  mgl_vec2d_magnitude_less_than(MglVec2D V,MglFloat size);
MglBool  mgl_vec3d_magnitude_less_than(MglVec3D V,MglFloat size);
MglBool  mgl_vec4d_magnitude_less_than(MglVec4D V,MglFloat size);

/**
 * @brief scales the vector to the specified length without changing direction
 * No op is magnitude is 0 or V is NULL
 * @param V a pointer to the vector to scale
 * @param magnitude the new length for the vector
 */
void mgl_vec2d_set_magnitude(MglVec2D * V,MglFloat magnitude);
void mgl_vec3d_set_magnitude(MglVec3D * V,MglFloat magnitude);
void mgl_vec4d_set_magnitude(MglVec4D * V,MglFloat magnitude);

/**
 * @brief checks if the distance between the two points provided is less than size.
 * @param p1 one point for the distance check
 * @param p2 another point for the distance check
 * @param size the value to check against
 * @return MglTrue if the distance between P1 and P2 is less than size, false otherwise
 */
MglBool mgl_distance_between_less_than2d(MglVec3D p1,MglVec3D p2,MglFloat size);
MglBool mgl_distance_between_less_than3d(MglVec3D p1,MglVec3D p2,MglFloat size);
MglBool mgl_distance_between_less_than4d(MglVec3D p1,MglVec3D p2,MglFloat size);

/**
 * @brief given a rotation, get the component vectors
 * @param angles the input rotation
 * @param forward output optional calculated forward vector
 * @param right output optional calculated right vector
 * @param up output optional calculated up vector
 */
void mgl_vec3d_angle_vectors(MglVec3D angles, MglVec3D *forward, MglVec3D *right, MglVec3D *up);

/**
 * @brief rotate an MglVec3D about another vector
 * @param dst output result
 * @param dir the vector to rotate about
 * @param point the point to rotate
 * @param degrees how far to rotate
 */
void mgl_vec3d_rotate_about_vector(MglVec3D *dst, MglVec3D dir, MglVec3D point, MglFloat degrees);

void mgl_vec3d_rotate_about_x(MglVec3D *vect, MglFloat angle);
void mgl_vec3d_rotate_about_y(MglVec3D *vect, MglFloat angle);
void mgl_vec3d_rotate_about_z(MglVec3D *vect, MglFloat angle);

#endif
