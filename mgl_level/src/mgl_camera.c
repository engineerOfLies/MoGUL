#include "mgl_camera.h"
#include "mgl_resource.h"
#include "mgl_logger.h"

struct MglCamera_S
{
    MglVec2D position;  /**<width and height of the camera*/
    MglVec2D size;      /**<width and height of the camera*/
    MglBool  useBounds; /**<if true, camera will be limited by the bounds field*/
    MglRect  bounds;    /**<the camera should not be allowed outside this range*/
};


static MglResourceManager * __mgl_camera_resource_manager = NULL;
void mgl_camera_close();
MglBool mgl_camera_load_resource(char *filename,void *data);
void mgl_camera_delete(void *data);

void mgl_camera_init(
    MglUint maxCameras)
{
    __mgl_camera_resource_manager = mgl_resource_manager_init(
        "mgl camera",
        maxCameras,
        sizeof(struct MglCamera_S),
        MglFalse,
        mgl_camera_delete,
        mgl_camera_load_resource
    );
    atexit(mgl_camera_close);
}

void mgl_camera_close()
{
    mgl_resource_manager_free(&__mgl_camera_resource_manager);
}

void mgl_camera_delete(void *data)
{
    /*nothing to be done*/
    return;
}

void mgl_camera_free(MglCamera **camera)
{
    if (!camera)return;
    mgl_resource_free_element(__mgl_camera_resource_manager,(void **)camera);
}


MglBool mgl_camera_load_resource(char *filename,void *data)
{
    return MglTrue;
}

MglCamera *mgl_camera_new(
    MglVec2D size)
{
    MglCamera * cam;
    cam = (MglCamera *)mgl_resource_new_element(__mgl_camera_resource_manager);
    if (!cam)return NULL;
    mgl_vec2d_copy(cam->size,size);
    return cam;
}

MglBool mgl_camera_check_view(MglCamera *cam,MglRect rect)
{
    if (!cam)
    {
        mgl_logger_error("provided NULL camera!");
        return MglFalse;
    }
    return mgl_rect_on_rect(
        mgl_rect(cam->position.x,cam->position.y,cam->size.x,cam->size.y),
        rect);
}

MglVec2D mgl_camera_get_adjusted_position(MglCamera *cam,MglVec2D position)
{
    MglVec2D out = {0,0};
    if (!cam)
    {
        mgl_logger_error("provided NULL camera!");
        return out;
    }
    mgl_vec2d_set(out,position.x - cam->position.x,position.y - cam->position.y);
    return out;
}

void mgl_camera_set_bounds(MglCamera *cam,MglRect bounds)
{
    if (!cam)return;
    mgl_camera_enable_bounds(cam, MglTrue);
    mgl_rect_copy(&cam->bounds,bounds);
}

void mgl_camera_enable_bounds(MglCamera *cam, MglBool enable)
{
    if (!cam)return;
    cam->useBounds = enable;
}

void mgl_camera_change_position(MglCamera *cam, MglVec2D position)
{
    if (!cam)
    {
        mgl_logger_error("provided NULL camera!");
        return;
    }
    mgl_vec2d_copy(cam->position,position);
    if (!cam->useBounds)return;
    
    if ((cam->position.x + cam->size.x) > (cam->bounds.x + cam->bounds.w))
    {
        cam->position.x = (cam->bounds.x + cam->bounds.w) - cam->size.x;
    }
    if ((cam->position.y + cam->size.y) > (cam->bounds.y + cam->bounds.h))
    {
        cam->position.y = (cam->bounds.y + cam->bounds.h) - cam->size.y;
    }
    if (cam->position.x < cam->bounds.x)
    {
        cam->position.x = cam->bounds.x;
    }
    if (cam->position.y < cam->bounds.y)
    {
        cam->position.y = cam->bounds.y;
    }
}

MglVec2D mgl_camera_get_position(MglCamera *cam)
{
    MglVec2D out = {0,0};
    if (!cam)
    {
        mgl_logger_error("provided NULL camera!");
        return out;
    }
    return cam->position;
}

MglVec2D mgl_camera_get_center(MglCamera *cam)
{
    MglVec2D out = {0,0};
    if (!cam)
    {
        mgl_logger_error("provided NULL camera!");
        return out;
    }
    mgl_vec2d_set(out,cam->position.x + (cam->size.x * 0.5),cam->position.y + (cam->size.y * 0.5));
    return out;
}

void mgl_camera_get_size(MglCamera *cam,MglUint *w, MglUint *h)
{
    if (!cam)
    {
        mgl_logger_error("provided NULL camera!");
        return;
    }
    if (w)
    {
        *w = cam->size.x;
    }
    if (h)
    {
        *h = cam->size.y;
    }
}

/*eol@eof*/
