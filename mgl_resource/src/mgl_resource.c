#include "mgl_resource.h"
#include "mgl_logger.h"
#include <assert.h>

/*TODO make loading a resource a threaded option.  Ie: for non-critical resources
  it can be loaded in a separate thread
  re-evaluate how the memory is packed.
  provide an ID-based hash access to the data.*/

struct MglResourceManager_S
{
  MglBool      _initialized;/**<true after setup, if false, don't touch*/
  MglLine      name;        /**<name of the resource manager, used in debugging*/
  MglUint      _data_count; /**<how many resources are currently alive*/
  MglUint      _data_max;   /**<hard limit on the number of resources that can be managed*/
  MglUint      _data_size;  /**<size of the data resource being managed*/
  MglBool      _data_unique;/**<if true, duplicates are not allowed, if false, subsequent requests for the same resource will be given a reference to an existing element*/
  unsigned long int _data_id_pool;/**<increments with every allocated resource.*/
  GHashTable * _data_hash;   /**<hash from unique id to resource*/
  char       * _data_list;   /**<character buffer of data*/
  void (*data_delete)(void *data);/**<function pointer to the delete function for the resource*/
  MglBool (*data_load)(char *filename,void *data);/**<function pointer to the loading function*/
};

/*All resources managed by this system must contain this header structure.*/
typedef struct
{
  MglUint refCount;
  MglLine filename;
  MglUint index;
  unsigned long int id;/**<unique identifier.  In case of a memory re-use it can be
  compared with expected for validity*/
  MglUint timeFree; /**<time when free was called on resource.  Oldest get reclaimed first*/
  MglUint underflowprot;
}MglResourceHeader;

/*local variables*/

/*local function prototypes*/
static void mgl_resource_delete_element(MglResourceManager *manager,MglResourceHeader *element);
static MglResourceHeader * mgl_resource_find_element_by_filename(MglResourceManager *manager,char *filename);
static void *mgl_resource_get_data_by_header(MglResourceHeader *resource);
static MglResourceHeader *mgl_resource_get_header_by_data(void *data);
static MglResourceHeader * mgl_resource_get_next_element(MglResourceManager *manager,MglResourceHeader *element);
static MglBool mgl_resource_validate_header_range(MglResourceManager *manager,MglResourceHeader *element);
static MglResourceManager *mgl_resource_manager_new();

static MglBool mgl_resource_initialize_hash(MglResourceManager *manager);
static MglBool mgl_resource_hash_element(MglResourceManager *manager,MglResourceHeader *element);
static MglBool mgl_resource_hash_clear_element(MglResourceManager *manager,MglResourceHeader *element);


MglResourceHeader *mgl_resource_get_header_by_index(MglResourceManager *manager,MglUint i)
{
  if (!manager)return NULL;
  if (i >= manager->_data_max)return NULL;
  return (MglResourceHeader *)&manager->_data_list[(i * manager->_data_size)];
}

void *mgl_resource_manager_load_resource(MglResourceManager *manager,char *filename)
{
  MglResourceHeader * element = NULL;
  if ((!filename)||(strlen(filename) <= 0))return NULL;
  if (!manager)
  {
    mgl_logger_error(
      "mgl_resource:passed in NULL resource manager for loading resource file: %s\n",
      filename);
    return NULL;
  }
  if (manager->data_load == NULL)
  {
    return NULL;
  }
  if (manager->_data_unique == MglFalse)
  {
    element = mgl_resource_find_element_by_filename(manager,filename);
    if (element != NULL)
    {
      if (element->refCount == 0)
      {
        manager->_data_count++;
      }
      element->refCount++;
      return mgl_resource_get_data_by_header(element);
    }
  }
  element = mgl_resource_get_header_by_data(mgl_resource_new_element(manager));
  if (element == NULL)
  {
    return NULL;
  }
  
  if (manager->data_load != NULL)
  {
    if (!manager->data_load(filename,mgl_resource_get_data_by_header(element)))
    {
      mgl_resource_delete_element(manager,element);
      return NULL;
    }
  }
  mgl_line_cpy(element->filename,filename);
  return mgl_resource_get_data_by_header(element);
}

void mgl_resource_manager_free(MglResourceManager **manager_pp)
{
  int i;
  MglResourceManager *manager = NULL;
  MglResourceHeader *element = NULL;
  if (!manager_pp)return;
  if (!*manager_pp)return;
  manager = *manager_pp;
  if (manager->data_delete != NULL)
  {
    for (i = 0 ; i < manager->_data_max;i++)
    {
      element = mgl_resource_get_header_by_index(manager,i);
      manager->data_delete(mgl_resource_get_data_by_header(element));
    }
  }
  free(manager->_data_list);
  free(manager);
  *manager_pp = NULL;
}

static MglResourceManager *mgl_resource_manager_new()
{
  MglResourceManager *manager = NULL;
  manager = (MglResourceManager *)malloc(sizeof(MglResourceManager));
  if (manager == NULL)
  {
    mgl_logger_error(
      "mgl_resource: unable to allocate resource manager!\n");
    return NULL;
  }
  memset(manager,0,sizeof(MglResourceManager));
  return manager;
}

MglResourceManager * mgl_resource_manager_init(
    MglLine managerName,
    MglUint max,
    MglUint dataSize,
    MglBool dataUnique,
    void    (*data_delete)(void *data),
    MglBool (*data_load)(char *filename,void *data)
  )
{
  int i;
  MglResourceHeader * element = NULL;
  MglResourceManager *manager = NULL;
  manager = mgl_resource_manager_new();
  if (manager == NULL)
  {
    return NULL;
  }
  strncpy(manager->name,managerName,MGLLINELEN);
  manager->_data_count = 0;
  manager->_data_id_pool = 0;
  manager->_data_max = max;
  manager->_data_unique = dataUnique;
  manager->_data_size = dataSize + sizeof(MglResourceHeader);
  manager->_data_list = malloc(manager->_data_size * max);
  manager->data_delete = data_delete;
  manager->data_load = data_load;
  if (manager->_data_list == NULL)
  {
    mgl_logger_error(
      "mgl_resource:unable to allocate resource list\n"
    );
    free(manager);
    return NULL;    
  }
  memset(manager->_data_list,0,manager->_data_size * max);
  for (i = 0 ; i < manager->_data_max;i++)
  {
    element = mgl_resource_get_header_by_index(manager,i);
    element->index = i;
    element->refCount = 0;
    element->timeFree = 0;
  }
  mgl_resource_initialize_hash(manager);
  manager->_initialized = MglTrue;
  return manager;
}

MglUint mgl_resource_manager_get_element_count(MglResourceManager *manager)
{
  if (!manager)return 0;
  return manager->_data_count;
}

void * mgl_resource_get_next_data(MglResourceManager *manager,void *data)
{
  MglResourceHeader *header = NULL;
  if (data != NULL)
  {
    header = (MglResourceHeader *)data;
    header--;
  }
  return mgl_resource_get_data_by_header(mgl_resource_get_next_element(manager,header));
}

MglResourceHeader * mgl_resource_get_next_element(MglResourceManager *manager,MglResourceHeader *element)
{
  int index = 0;
  if (manager == NULL)
  {
    mgl_logger_warn(
      "mgl_resource:passed a NULL manager\n");
    return NULL;
  }
  if (manager->_initialized == MglFalse)
  {
    mgl_logger_warn(
      "mgl_resource:manager %s uninitialized\n",
      manager->name);
    return NULL;
  }
  if (element == NULL)
  {
    index = 0;
  }
  else
  {
    index = element->index + 1;
  }
  for ( ; index < manager->_data_max;index++)
  {
    element = (MglResourceHeader *)&manager->_data_list[(index * manager->_data_size)];
    if (element->refCount > 0)
    {
      return element;
    }
  }
  return NULL;
}

MglResourceHeader * mgl_resource_find_element_by_filename(MglResourceManager *manager,char *filename)
{
  MglResourceHeader *element = NULL;
  int i = 0;
  if (manager == NULL)
  {
    mgl_logger_warn(
      "mgl_resource:passed a NULL manager\n");
    return NULL;
  }
  if (manager->_initialized == MglFalse)
  {
    mgl_logger_warn(
      "mgl_resource:manager %s uninitialized\n",
      manager->name);
    return NULL;
  }
  for (i = 0 ; i < manager->_data_max;i++)
  {
    element = mgl_resource_get_header_by_index(manager,i);
    if (strncmp(filename,element->filename,MGLLINELEN) == 0)
    {
      return element;
    }
  }
  return NULL;
}

void * mgl_resource_new_element(MglResourceManager *manager)
{
  int i = 0;
  MglResourceHeader *element = NULL;
  MglResourceHeader *oldest = NULL;
  MglUint            oldestTime = SDL_GetTicks() + 1;
  if (manager == NULL)
  {
    mgl_logger_warn(
      "mgl_resource:passed a NULL manager\n");
    return NULL;
  }
  if (manager->_initialized == MglFalse)
  {
    mgl_logger_warn(
      "mgl_resource:manager %s uninitialized\n",
      manager->name);
    return NULL;
  }

  for (i = 0 ; i < manager->_data_max;i++)
  {
    element = mgl_resource_get_header_by_index(manager,i);
    if (element->refCount == 0)
    {
      if (element->timeFree <= oldestTime)
      {
        oldest = element;
        oldestTime = element->timeFree;
        if (element->timeFree == 0)
        {
          /*cant get older than never used*/
          break;
        }
      }
    }
  }
  if (oldest != NULL)
  {
    element = oldest;
    mgl_resource_delete_element(manager,element);
    element->id = manager->_data_id_pool++;
    element->refCount = 1;
    manager->_data_count++;
    mgl_resource_hash_element(manager,element);
    return mgl_resource_get_data_by_header(element);
  }

  mgl_logger_error(
    "mgl_resource: manager %s has no room left for new elements!\n",
    manager->name);
  return NULL;
}

void mgl_resource_free_element(MglResourceManager *manager,void **data)
{
  MglResourceHeader *element;
  if (!manager)return;
  if (!data)return;
  if (!*data)return;
  element = mgl_resource_get_header_by_data(*data);
  if (manager->_data_unique)
  {
    mgl_resource_delete_element(manager,element);
    element->timeFree = 0;
    manager->_data_count--;
    element->refCount = 0;
  }
  else
  {
    element->refCount--;
    if (element->refCount == 0)
    {
      manager->_data_count--;
    }
    element->timeFree = SDL_GetTicks();
  }
  *data = NULL;
}

void mgl_resource_manager_clear(MglResourceManager *manager)
{
  MglResourceHeader *element = NULL;
  int i = 0;
  if (manager == NULL)
  {
    mgl_logger_warn(
      "mgl_resource:passed a NULL manager\n");
    return;
  }
  if (manager->_initialized == MglFalse)
  {
    mgl_logger_warn(
      "mgl_resource:manager %s uninitialized\n",
      manager->name);
    return;
  }
  if (manager->data_delete == NULL)
  {
    mgl_logger_warn(
      "mgl_resource:manager %s had no delete function\n",
      manager->name);
    return;
  }
  for (i = 0 ; i < manager->_data_max;i++)
  {
    element = mgl_resource_get_header_by_index(manager,i);
    manager->data_delete(&element[1]);
  }
  manager->_data_count = 0;
}

void mgl_resource_manager_clean(MglResourceManager *manager)
{
  MglResourceHeader *element = NULL;
  int i = 0;
  if (manager == NULL)
  {
    mgl_logger_warn(
      "mgl_resource:passed a NULL manager\n");
    return;
  }
  if (manager->_initialized == MglFalse)
  {
    mgl_logger_warn(
      "mgl_resource:manager %s uninitialized\n",
      manager->name);
    return;
  }
  if (manager->data_delete == NULL)
  {
    mgl_logger_warn(
      "mgl_resource:manager %s had no delete function\n",
      manager->name);
    return;
  }
  for (i = 0 ; i < manager->_data_max;i++)
  {
    element = mgl_resource_get_header_by_index(manager,i);
    if (element->refCount == 0)
    {
      manager->data_delete(&element[1]);
    }
  }
}

void mgl_resource_delete_element(MglResourceManager *manager,MglResourceHeader *element)
{
  MglUint index;
  if (!manager)return;
  if (!element)return;
  if (manager->data_delete != NULL)
  {
    manager->data_delete(&element[1]);
  }
  index = element->index;
  mgl_resource_hash_clear_element(manager,element);
  memset(element,0,manager->_data_size);
  element->index = index;
}

MglResourceHeader *mgl_resource_get_header_by_data(void *data)
{
  MglResourceHeader *header;
  if (!data)return NULL;
  header = data;
  header--;
  return header;
}

void *mgl_resource_get_data_by_header(MglResourceHeader *resource)
{
  if (!resource)return NULL;
  return (void *)((char *)resource + sizeof(MglResourceHeader));
}

MglBool mgl_resource_element_id_valid(MglResourceManager *manager,void *element,MglUint id)
{
  if (mgl_resource_element_get_id(manager,element) == id)return MglTrue;
  return MglFalse;
}

unsigned long int mgl_resource_element_get_id(MglResourceManager *manager,void *element)
{
  MglResourceHeader *header;
  if (!manager)
  {
    mgl_logger_info(
      "mgl_resource:passed a NULL manager\n");

    return -1;
  }
  if (!element)
  {
    mgl_logger_info(
      "mgl_resource:passed a NULL element\n");
    return -1;
  }
  header = mgl_resource_get_header_by_data(element);
  /*range verification*/
  if (!mgl_resource_validate_header_range(manager,header))
  {
    return -1;
  }
  return header->id;
}

MglUint mgl_resource_element_get_refcount(MglResourceManager *manager,void *element)
{
  MglResourceHeader *header;
  if (!manager)
  {
    mgl_logger_info(
        "mgl_resource:passed a NULL manager\n");
    return 0;
  }
  if (!element)
  {
    mgl_logger_info(
        "mgl_resource:passed a NULL element\n");
    return 0;
  }
  header = mgl_resource_get_header_by_data(element);
  /*range verification*/
  if (!mgl_resource_validate_header_range(manager,header))
  {
    return 0;
  }
  return header->refCount;
}

void mgl_resource_element_get_filename(MglLine filename, MglResourceManager *manager,void *element)
{
  MglResourceHeader *header;
  if (!manager)
  {
    mgl_logger_info(
        "mgl_resource:passed a NULL manager\n");
    return ;
  }
  if (!element)
  {
    mgl_logger_info(
        "mgl_resource:passed a NULL element\n");
    return ;
  }
  header = mgl_resource_get_header_by_data(element);
  /*range verification*/
  if (!mgl_resource_validate_header_range(manager,header))
  {
    return;
  }
  mgl_line_cpy(filename,header->filename);
}

MglInt mgl_resource_element_get_index(MglResourceManager *manager,void *element)
{
  MglResourceHeader *header;
  if (!manager)
  {
    mgl_logger_info(
      "mgl_resource:passed a NULL manager\n");

    return -1;
  }
  if (!element)
  {
    mgl_logger_info(
      "mgl_resource:passed a NULL element\n");
    return -1;
  }
  header = mgl_resource_get_header_by_data(element);
  /*range verification*/
  if (!mgl_resource_validate_header_range(manager,header))
  {
    return -1;
  }
  return header->index;
}

MglBool mgl_resource_validate_header_range(MglResourceManager *manager,MglResourceHeader *element)
{
  if (!manager)return MglFalse;
  if (!element)return MglFalse;
  if ((void *)element < (void *)manager->_data_list)
  {
    mgl_logger_error(
      "mgl_resource: underseeking in data list.\n");
    return MglFalse;
  }
  if ((void *)element > (void *)&manager->_data_list[manager->_data_max * manager->_data_size])
  {
    mgl_logger_error(
      "mgl_resource: overseeking in data list.\n");
    return MglFalse;
  }
  return MglTrue;
}

static MglBool mgl_resource_hash_element(MglResourceManager *manager,MglResourceHeader *element)
{
  void *data = NULL;
  if (!manager)return MglFalse;
  if (!element)return MglFalse;
  data = (void *)(element+1);
  g_hash_table_insert (manager->_data_hash,
                       (gpointer)element->id,
                       (gpointer)data);
  return MglTrue;
}

static MglBool mgl_resource_hash_clear_element(MglResourceManager *manager,MglResourceHeader *element)
{
  if (!manager)return MglFalse;
  if (!element)return MglFalse;
  g_hash_table_insert (manager->_data_hash,
                       (gpointer)element->id,
                       (gpointer)NULL);
  return MglTrue;
}

static MglBool mgl_resource_initialize_hash(MglResourceManager *manager)
{
  if (!manager)return MglFalse;
  if (manager->_data_hash != NULL)
  {
    mgl_logger_warn(
      "mgl_resource: hash already initialized.\n");
    return MglFalse;
  }
  manager->_data_hash = g_hash_table_new(g_direct_hash, g_direct_equal);
  return MglTrue;
}

void * mgl_resource_get_data_by_id(MglResourceManager *manager,unsigned long int id)
{
  if (!manager)return NULL;
  if (!manager->_data_hash)
  {
    mgl_logger_warn(
      "mgl_resource: hash not setup.\n");
    return NULL;
  }
  return g_hash_table_lookup (manager->_data_hash,
                              (gconstpointer)id);
}
/*eol@eof*/

