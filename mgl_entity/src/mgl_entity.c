#include "mgl_entity.h"
#include "mgl_resource.h"
#include "mgl_logger.h"

static MglResourceManager * __mgl_entity_resource_manager = NULL;
static MglBool              __mgl_entity_use_custom_data = MglFalse;

void mgl_entity_close();
MglBool mgl_entity_load_resource(char *filename,void *data);
void mgl_entity_delete(void *data);

void mgl_entity_init(
    MglUint maxEntities,
    MglBool useCustomData,
    size_t  customDataSize
                   )
{
    if (!useCustomData)customDataSize = 0;
    __mgl_entity_resource_manager = mgl_resource_manager_init(
        "mgl entity",
        maxEntities,
        sizeof(MglEntity) + customDataSize,
        MglTrue,
        mgl_entity_delete,
        mgl_entity_load_resource
    );
    atexit(mgl_entity_close);    
}

void mgl_entity_close()
{
    mgl_resource_manager_free(&__mgl_entity_resource_manager);
}

MglBool mgl_entity_load_resource(char *filename,void *data)
{
    MglEntity *ent;
    if (!data)return MglFalse;
    if (__mgl_entity_use_custom_data)
    {
        ent = (MglEntity *)data;
        ent->data = &ent[1];
    }
    return MglTrue;
}

MglEntity *mgl_entity_new()
{
    MglEntity *ent;
    ent = mgl_resource_new_element(__mgl_entity_resource_manager);
    if (!ent)return NULL;
    if (__mgl_entity_use_custom_data)
    {
        ent->data = &ent[1];
    }
    return ent;
}




/*eol@eof*/
