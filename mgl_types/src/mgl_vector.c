#include "mgl_vector.h"
#include <math.h>

MglBool mgl_equals(MglDouble a, MglDouble b)
{
  return (fabs(a-b) < MGL_EPSILON)?MglTrue:MglFalse;
}

MglVec2D mgl_vec2d(MglDouble x, MglDouble y)
{
  MglVec2D vec;
  mgl_vec2d_set(vec,x, y);
  return vec;
}

MglVec3D mgl_vec3d(MglDouble x, MglDouble y, MglDouble z)
{
  MglVec3D vec;
  mgl_vec3d_set(vec,x, y, z);
  return vec;
}

MglVec4D mgl_vec4d(MglDouble x, MglDouble y, MglDouble z, MglDouble w)
{
  MglVec4D vec;
  mgl_vec4d_set(vec,x, y, z, w);
  return vec;
}

MglFloat mgl_vec2d_magnitude (MglVec2D V)
{
  return sqrt (V.x * V.x + V.y * V.y);
}

MglFloat mgl_vec3d_magnitude (MglVec3D V)
{
  return sqrt (V.x * V.x + V.y * V.y + V.z * V.z);
}

MglFloat mgl_vec4d_magnitude (MglVec4D V)
{
  return sqrt (V.x * V.x + V.y * V.y + V.z * V.z + V.w * V.w);
}

MglFloat mgl_vec2d_magnitude_squared(MglVec2D V)
{
  return (V.x * V.x + V.y * V.y);
}

MglFloat mgl_vec3d_magnitude_squared(MglVec3D V)
{
  return (V.x * V.x + V.y * V.y + V.z * V.z);
}

MglFloat mgl_vec4d_magnitude_squared(MglVec4D V)
{
  return (V.x * V.x + V.y * V.y + V.z * V.z + V.w * V.w);
}

void mgl_vec2d_set_magnitude(MglVec2D * V,MglFloat magnitude)
{
  if (!V)return;
  mgl_vec2d_normalize(V);
  V->x *= magnitude;
  V->y *= magnitude;
}

void mgl_vec3d_set_magnitude(MglVec3D * V,MglFloat magnitude)
{
  if (!V)return;
  mgl_vec3d_normalize(V);
  V->x *= magnitude;
  V->y *= magnitude;
  V->z *= magnitude;
}

void mgl_vec4d_set_magnitude(MglVec4D * V,MglFloat magnitude)
{
  if (!V)return;
  mgl_vec4d_normalize(V);
  V->x *= magnitude;
  V->y *= magnitude;
  V->z *= magnitude;
  V->w *= magnitude;
}

MglBool  mgl_vec3d_magnitude_less_than(MglVec3D V,MglFloat size)
{
  return (mgl_vec3d_magnitude_squared(V) < (size * size));
}

MglBool mgl_distance_between_less_than3d(MglVec3D p1,MglVec3D p2,MglFloat size)
{
  return mgl_vec3d_magnitude_less_than(mgl_vec3d(p1.x - p2.x,
                                                 p1.y - p2.y,
                                                 p1.z - p2.z),
                                       size);
}

void mgl_vec3d_set_angle_by_radians(MglVec3D *out,MglFloat radians)
{
  if(!out)return;
  out->x = cos(radians);
  out->y = sin(radians);
}

void mgl_vec4d_set_angle_by_radians(MglVec4D *out,MglFloat radians)
{
  if(!out)return;
  out->x = cos(radians);
  out->y = sin(radians);
}

void mgl_vec2d_normalize (MglVec2D *V)
{
  MglFloat M;
  if (!V)return;
  M = mgl_vec2d_magnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
}

void mgl_vec3d_normalize (MglVec3D *V)
{
  MglFloat M;
  if (!V)return;
  M= mgl_vec3d_magnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
  V->z *= M;
}

void mgl_vec4d_normalize (MglVec4D *V)
{
  MglFloat M;
  if (!V)return;
  M = mgl_vec4d_magnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
  V->z *= M;
  V->w *= M;
}

MglVec2D *mgl_vec2d_dup(MglVec2D old)
{
  MglVec2D *duped = NULL;
  duped = mgl_vec2d_new();
  if (!duped)return NULL;
  mgl_vec2d_copy((*duped),old);
  return duped;
}

MglVec3D *mgl_vec3d_dup(MglVec3D old)
{
  MglVec3D *duped = NULL;
  duped = mgl_vec3d_new();
  if (!duped)return NULL;
  mgl_vec3d_copy((*duped),old);
  return duped;
}

MglVec4D *mgl_vec4d_dup(MglVec4D old)
{
  MglVec4D *duped = NULL;
  duped = mgl_vec4d_new();
  if (!duped)return NULL;
  mgl_vec4d_copy((*duped),old);
  return duped;
}

MglVec2D *mgl_vec2d_new()
{
  MglVec2D *vec = NULL;
  vec = (MglVec2D *)malloc(sizeof(MglVec2D));
  if (vec == NULL)
  {
    mgl_logger_message(
      MGL_LOG_ERROR,
      "mgl_types:failed to allocate a new vector!\n");
    return NULL;
  }
  mgl_vec2d_clear((*vec));
  return vec;
}

MglVec3D *mgl_vec3d_new()
{
  MglVec3D *vec = NULL;
  vec = (MglVec3D *)malloc(sizeof(MglVec3D));
  if (vec == NULL)
  {
    mgl_logger_message(
      MGL_LOG_ERROR,
      "mgl_types:failed to allocate a new vector!\n");
    return NULL;
  }
  mgl_vec3d_clear((*vec));
  return vec;
}

MglVec4D *mgl_vec4d_new()
{
  MglVec4D *vec = NULL;
  vec = (MglVec4D *)malloc(sizeof(MglVec4D));
  if (vec == NULL)
  {
    mgl_logger_message(
      MGL_LOG_ERROR,
      "mgl_types:failed to allocate a new vector!\n");
    return NULL;
  }
  mgl_vec4d_clear((*vec));
  return vec;
}

void mgl_vec3d_project_to_plane( MglVec3D *dst, MglVec3D p, MglVec3D normal )
{
  MglFloat d;
  MglVec3D n;
  MglFloat inv_denom;
  if (!dst)return;
  inv_denom = 1.0F / mgl_vec3d_dot_product( normal, normal );
  
  d = mgl_vec3d_dot_product( normal, p ) * inv_denom;
  
  n.x = normal.x * inv_denom;
  n.y = normal.y * inv_denom;
  n.z = normal.z * inv_denom;
  
  dst->x = p.z - d * n.x;
  dst->y = p.y - d * n.y;
  dst->z = p.x - d * n.z;
}


void mgl_vec3d_perpendicular( MglVec3D *dst, MglVec3D src )
{
  int pos;
  MglFloat minelem = 1.0F;
  MglVec3D tempvec;
  if (!dst)return;
  /*
   * * find the smallest magnitude axially aligned vector
   */
  pos=0;
  
  if ( fabs( src.x ) < minelem )
  {
    pos=0;
    minelem = fabs( src.x );
  }
  if ( fabs( src.y ) < minelem )
  {
    pos=1;
    minelem = fabs( src.y );
  }
  if ( fabs( src.y ) < minelem )
  {
    pos=2;
    minelem = fabs( src.z );
  }
  
  tempvec.x=0;
  tempvec.y=0;
  tempvec.z=0;
  
  switch(pos)
  {
    case 0:
      tempvec.x=1;
      break;
    case 1:
      tempvec.y=1;
      break;
    case 2:
      tempvec.z=1;
      break;
  }
  
  /*
   * * project the point onto the plane defined by src
   */
  mgl_vec3d_project_to_plane( dst, tempvec, src );
  
  /*
   * * normalize the result
   */
  mgl_vec3d_normalize( dst );
}

void mgl_rotation_concacenate(MglFloat in1[3][3], MglFloat in2[3][3], MglFloat out[3][3])
{
  out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
  in1[0][2] * in2[2][0];
  out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
  in1[0][2] * in2[2][1];
  out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
  in1[0][2] * in2[2][2];
  out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
  in1[1][2] * in2[2][0];
  out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
  in1[1][2] * in2[2][1];
  out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
  in1[1][2] * in2[2][2];
  out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
  in1[2][2] * in2[2][0];
  out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
  in1[2][2] * in2[2][1];
  out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
  in1[2][2] * in2[2][2];
}

void mgl_vec3d_rotate_about_vector(MglVec3D *dst, MglVec3D dir, MglVec3D point, MglFloat degrees)
{
  MglFloat m[3][3];
  MglFloat im[3][3];
  MglFloat zrot[3][3];
  MglFloat tmpmat[3][3];
  MglFloat rot[3][3];
  MglVec3D vr, vup, vf;
  
  if (!dst)return;
  
  vf.x = dir.x;
  vf.y = dir.y;
  vf.z = dir.z;
  
  mgl_vec3d_perpendicular( &vr, dir );
  mgl_vec3d_cross_product(&vup, vr, vf);

  m[0][0] = vr.x;
  m[1][0] = vr.y;
  m[2][0] = vr.z;
  
  m[0][1] = vup.x;
  m[1][1] = vup.y;
  m[2][1] = vup.z;
  
  m[0][2] = vf.x;
  m[1][2] = vf.y;
  m[2][2] = vf.z;
  
  memcpy( im, m, sizeof( im ) );
  
  im[0][1] = m[1][0];
  im[0][2] = m[2][0];
  im[1][0] = m[0][1];
  im[1][2] = m[2][1];
  im[2][0] = m[0][2];
  im[2][1] = m[1][2];
  
  memset( zrot, 0, sizeof( zrot ) );
  zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;
  
  zrot[0][0] = cos( ( degrees*MGL_DEGTORAD ) );
  zrot[0][1] = sin( ( degrees*MGL_DEGTORAD ) );
  zrot[1][0] = -sin( ( degrees*MGL_DEGTORAD ) );
  zrot[1][1] = cos( ( degrees*MGL_DEGTORAD ) );
  
  mgl_rotation_concacenate( m, zrot, tmpmat );
  mgl_rotation_concacenate( tmpmat, im, rot );
  
  dst->x = rot[0][0] * point.x + rot[0][1] * point.y + rot[0][2] * point.z;
  dst->y = rot[1][0] * point.x + rot[1][1] * point.y + rot[1][2] * point.z;
  dst->z = rot[2][0] * point.x + rot[2][1] * point.y + rot[2][2] * point.z;
}

void mgl_vec3d_rotate_about_x(MglVec3D *vect, MglFloat angle)
{
  MglVec3D temp;
  if (!vect)return;
  
  angle=angle*MGL_DEGTORAD;
  
  temp.x=vect->x;
  temp.y=(vect->y*cos(angle))-(vect->z*sin(angle));
  temp.z=(vect->y*sin(angle))+(vect->z*cos(angle));
  
  vect->x=temp.x;
  vect->y=temp.y;
  vect->z=temp.z;
}

void mgl_vec3d_rotate_about_y(MglVec3D *vect, MglFloat angle)
{
  MglVec3D temp;
  if (!vect)return;
  
  angle=angle*MGL_DEGTORAD;
  
  temp.y=vect->y;
  temp.x=(vect->x*cos(angle))+(vect->z*sin(angle));
  temp.z=(vect->x*sin(angle)*(-1))+(vect->z*cos(angle));
  
  vect->x=temp.x;
  vect->y=temp.y;
  vect->z=temp.z;
}

void mgl_vec3d_rotate_about_z(MglVec3D *vect, MglFloat angle)
{
  MglVec3D temp;
  if (!vect)return;
  
  angle=angle*MGL_DEGTORAD;
  
  temp.z=vect->z;
  temp.x=(vect->x*cos(angle))-(vect->y*sin(angle));
  temp.y=(vect->x*sin(angle))+(vect->y*cos(angle));
  
  vect->x=temp.x;
  vect->y=temp.y;
  vect->z=temp.z;
}


void mgl_vec3d_angle_vectors(MglVec3D angles, MglVec3D *forward, MglVec3D *right, MglVec3D *up)
{
  MglFloat angle;
  MglFloat sr, sp, sy, cr, cp, cy;
  
  angle = angles.x * (MGL_DEGTORAD);
  sy = sin(angle);
  cy = cos(angle);
  angle = angles.y * (MGL_DEGTORAD);
  sp = sin(angle);
  cp = cos(angle);
  angle = angles.z * (MGL_DEGTORAD);
  sr = sin(angle);
  cr = cos(angle);
  
  if(forward)
  {
    forward->x = cp*cy;
    forward->y = cp*sy;
    forward->z = -sp;
  }
  if(right)
  {
    right->x = (-1*sr*sp*cy+-1*cr*-sy);
    right->y = (-1*sr*sp*sy+-1*cr*cy);
    right->z = -1*sr*cp;
  }
  if(up)
  {
    up->x = (cr*sp*cy+-sr*-sy);
    up->y = (cr*sp*sy+-sr*cy);
    up->z = cr*cp;
  }
}


MglFloat mgl_vec2d_angle(MglVec2D v)
{
  return mgl_vector_angle(v.x,v.y);
}

MglFloat mgl_vector_angle(MglFloat x,MglFloat y)
{
  MglFloat angle = atan2(y, x) + M_PI;
  MglFloat fraction = angle * 0.5 / M_PI;
  if (fraction >= 1.0)
  {
    fraction -= 1.0;
  }
  return (fraction * 360);
}

void mgl_angle_clamp_degrees(MglFloat *a)
{
  if (!a)return;
  while (*a >= 360)*a -= 360;
  while (*a < 0)*a += 360;
}

MglFloat mgl_angle_between_degrees(MglFloat a, MglFloat b)
{
  MglFloat angle;
  angle = fabs(a - b);
  while (angle >= 360)angle -= 360;
  while (angle < 0)angle += 360;
  if (angle > 180)angle -= 180;
  return angle;
}

void mgl_vec2d_reflect(MglVec2D *out, MglVec2D normal,MglVec2D in)
{
  MglFloat f;
  f = mgl_vec2d_dot_product(in,normal);
  out->x = in.x - (2 * normal.x * f);
  out->y = in.y - (2 * normal.y * f);
}

void mgl_vec3d_reflect(MglVec3D *out, MglVec3D normal,MglVec3D in)
{
  MglFloat f;
  f = mgl_vec3d_dot_product(in,normal);
  out->x = in.x - (2 * normal.x * f);
  out->y = in.y - (2 * normal.y * f);
  out->z = in.z - (2 * normal.z * f);
}

void mgl_vec4d_reflect(MglVec4D *out, MglVec4D normal,MglVec4D in)
{
  MglFloat f;
  f = mgl_vec4d_dot_product(in,normal);
  out->x = in.x - (2 * normal.x * f);
  out->y = in.y - (2 * normal.y * f);
  out->z = in.z - (2 * normal.z * f);
  out->w = in.w - (2 * normal.w * f);
}

void mgl_vec3d_cross_product(MglVec3D *out, MglVec3D v1, MglVec3D v2)
{
  out->x = v1.y*v2.z - v1.z*v2.y;
  out->y = v1.z*v2.x - v1.x*v2.z;
  out->z = v1.x*v2.y - v1.y*v2.x;
}


/*eol@eof*/
