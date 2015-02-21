#include "mgl_types.h"

MglBool mgl_equals(MglDouble a, MglDouble b)
{
  return (fabs(a-b) < MGL_EPSILON)?MglTrue:MglFalse;
}


/*eol@eof*/