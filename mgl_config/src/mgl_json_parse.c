#include "mgl_json_parse.h"
#include "mgl_logger.h"

#include <jansson.h>

MglDict *mgl_json_convert(json_t *json);
MglDict *mgl_json_hash_convert(json_t *json);
MglDict *mgl_json_list_convert(json_t *json);

MglDict *mgl_json_parse(char *filename)
{
  MglDict *data;
  json_t *json;
  json_error_t jer;
  json = json_load_file(filename,0,&jer);
  if (json == NULL)
  {
    return NULL;
  }
  data = mgl_json_convert(json);
  json_decref(json); 
  return data;
}

MglDict *mgl_json_convert(json_t *json)
{
  if (json_is_object(json))
  {
    return mgl_json_hash_convert(json);
  }
  if (json_is_array(json))
  {
    return mgl_json_list_convert(json);
  }
  if (json_is_string(json))
  {
    return mgl_dict_new_string((char *)json_string_value(json));
  }
  if (json_is_boolean(json))
  {
    return mgl_dict_new_bool(json_is_true(json));
  }
  if (json_is_integer(json))
  {
    return mgl_dict_new_int(json_integer_value(json));
  }
  if (json_is_real(json))
  {
    return mgl_dict_new_float(json_real_value(json));
  }
  if (json_is_null(json))
  {
    return NULL;
  }
  return mgl_dict_new_string((char *)json_string_value(json));
}

MglDict *mgl_json_list_convert(json_t *json)
{
  int index;
  json_t *value;
  MglDict *data = mgl_dict_new_list();
  json_array_foreach(json, index, value)
  {
    mgl_dict_list_append(data,mgl_json_convert(value));
  }
  return data;
}

MglDict *mgl_json_hash_convert(json_t *json)
{
  MglDict *data = mgl_dict_new_hash();
  char *key;
  json_t *value;
  json_object_foreach(json, key, value)
  {
    mgl_dict_hash_insert(data,key,mgl_json_convert(value));
  }
  return data;
}

/*eol@eof*/
