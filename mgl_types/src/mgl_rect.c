#include "mgl_rect.h"

void mgl_rect_set(
  MglRect * r,
  MglInt    x,
  MglInt    y,
  MglUint   w,
  MglUint   h
)
{
  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;
}


MglRect mgl_rect(
  MglInt  x,
  MglInt  y,
  MglUint w,
  MglUint h
)
{
  MglRect r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  return r;
}

MglRectFloat mgl_rectf(
  MglDouble  x,
  MglDouble  y,
  MglDouble  w,
  MglDouble  h
)
{
  MglRectFloat r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  return r;
}

void mgl_rectf_set(
  MglRectFloat * d,
  MglFloat       x,
  MglFloat       y,
  MglFloat       w,
  MglFloat       h
)
{
  if (!d)return;
  d->x = x;
  d->y = y;
  d->w = w;
  d->h = h;
}

void mgl_rectf_copy(
  MglRectFloat * d,
  MglRectFloat   s
)
{
  if (!d)return;
  d->x = s.x;
  d->y = s.y;
  d->w = s.w;
  d->h = s.h;
}

void mgl_rect_copy(
  MglRect *d,
  MglRect s
)
{
  if (!d)return;
  d->x = s.x;
  d->y = s.y;
  d->w = s.w;
  d->h = s.h;
}

MglBool mgl_rect_in_rect(MglRect a, MglRect b)
{
  if (((b.x >= a.x)&&(b.x + b.w <= a.x + a.w)
    && (b.y >= a.y)&&(b.y + b.h <= a.y + a.h)))
    return MglTrue;
  return MglFalse;
}

MglBool mgl_rect_on_rect(MglRect a, MglRect b)
{
  if ((a.x > (b.x + b.w)) || (a.y > (b.y + b.h)) || (b.x > (a.x + a.w)) || (b.y > (a.y + a.h)))
    return MglFalse;
  return MglTrue;
}

/*eol@eof*/