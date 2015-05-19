#include "mgl_config.h"
#include "mgl_resource.h"
#include "mgl_logger.h"
#include "mgl_yaml_parse.h"
#include "mgl_json_parse.h"


struct MglConfig_S
{
  MglDict *_dictionary;
};

static MglResourceManager * __mgl_config_manager = NULL;

void mgl_config_delete(void *data);
MglBool mgl_config_load_from_file(char *filename,void *data);

void mgl_config_close();

void mgl_config_init()
{
  __mgl_config_manager = mgl_resource_manager_init(
    "mgl_config",
    100,
    sizeof(struct MglConfig_S),
    MglFalse,
    mgl_config_delete,
    mgl_config_load_from_file
  );
  atexit(mgl_config_close);
}

void mgl_config_close()
{
  mgl_resource_manager_free(&__mgl_config_manager);
}

MglConfig *mgl_config_load(MglLine filename)
{
  return (MglConfig *)mgl_resource_manager_load_resource(__mgl_config_manager,filename);
}

void mgl_config_free(MglConfig **config)
{
  mgl_resource_free_element(__mgl_config_manager,(void **)config);
}

MglBool mgl_config_load_from_file(char *filename,void *data)
{
  MglConfig *config;
  MglDict *dict;
  config = (MglConfig *)data;
  dict = mgl_json_parse(filename);
  if (!dict)
  {
    dict = mgl_yaml_parse(filename);
  }
  if (!dict)
  {
    return MglFalse;
  }
  config->_dictionary = dict;
  return MglTrue;
}

void mgl_config_delete(void *data)
{
  MglConfig *config;
  config = (MglConfig *)data;
  mgl_dict_free(&config->_dictionary);
}

MglDict *mgl_config_get_dictionary(MglConfig *config)
{
  if (!config)return NULL;
  return config->_dictionary;
}
/*eol@eof*/
