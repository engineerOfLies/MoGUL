#include "mgl_camera.h"
#include "mgl_resource.h"

struct MglCamera_S
{
    MglVec2D Position;  /**<width and height of the camera*/
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



/*eol@eof*/
