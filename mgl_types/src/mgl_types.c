#include "mgl_types.h"

MglBool mgl_equals(MglDouble a, MglDouble b)
{
  return (fabs(a-b) < MGL_EPSILON)?MglTrue:MglFalse;
}

char *mgl_string_from_bool(MglBool b)
{
  if (b == MglTrue)
  {
    return "true";
  }
  else if (b == MglFalse)
  {
    return "false";
  }
  else return "ERROR";
}

MglInt mgl_bool_from_string(char * truth)
{
  if ((strcmp(truth,"TRUE") == 0) ||
    (strcmp(truth,"true") == 0) ||
    (strcmp(truth,"True") == 0) ||
    (strcmp(truth,"1") == 0))
    return MglTrue;
  if ((strcmp(truth,"FALSE") == 0) ||
    (strcmp(truth,"false") == 0) ||
    (strcmp(truth,"False") == 0) ||
    (strcmp(truth,"0") == 0))
    return MglFalse;
  return -1;
}

void mgl_swap(void *a,void *b,size_t size)
{
    void *tmp;
    if ((!a)||(!b))return;
    tmp = malloc(size);
    memcpy(tmp,a,size);
    memcpy(a,b,size);
    memcpy(b,tmp,size);
    free(tmp);
}

/*eol@eof*/