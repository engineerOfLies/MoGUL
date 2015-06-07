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

MglCallback mgl_callback(void (*function)(void *data),void *data)
{
    MglCallback cb;
    cb.function = function;
    cb.data = data;
    return cb;
}

void mgl_callback_copy(MglCallback *dst,MglCallback src)
{
    if (!dst)return;
    dst->function = src.function;
    dst->data = src.data;
}

void mgl_callback_set(MglCallback *cb,void (*function)(void *data),void *data)
{
    if (!cb)return;
    cb->function = function;
    cb->data = data;
}

MglCallback *mgl_callback_new()
{
    MglCallback *cb;
    cb = (MglCallback *)malloc(sizeof(MglCallback));
    memset(cb,0,sizeof(MglCallback));
    return cb;
}

void mgl_callback_free(MglCallback **cb)
{
    if (!cb)return;
    if (!*cb)return;
    free(*cb);
    *cb = NULL;
}

void mgl_callback_delete(MglCallback *cb)
{
    if (!cb)return;
    free(cb);
}
/*eol@eof*/