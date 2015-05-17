#include "mgl_json_parse.h"
#include "mgl_logger.h"

#include <jansson.h>

MglDict *mgl_json_convert(json_t *json);
MglDict *mgl_json_hash_convert(json_t *json);
MglDict *mgl_json_list_convert(json_t *json);

void mgl_json_log_error(json_error_t *er);

MglDict *mgl_json_parse(char *filename)
{
  MglDict *data;
  json_t *json;
  json_error_t jer;
  fprintf(stdout,"mgl_json_parse: being json parse\n");
  json = json_load_file(filename,0,&jer);
  if (json == NULL)
  {
    mgl_logger_info("mgl_json_parse: failed to load file %s as json\n", filename);
    mgl_json_log_error(&jer);
    return NULL;
  }
  data = mgl_json_convert(json);
  json_decref(json); 
  if (data == NULL)
  {
    mgl_logger_info("mgl_json_parse: failed to parse file %s as json\n", filename);
    mgl_json_log_error(&jer);
  }
  return data;
}

MglDict *mgl_json_convert(json_t *json)
{
  mgl_logger_info("mgl_json_convert: converting json object to: ");
  if (json_is_object(json))
  {
    mgl_logger_info("hash\n");
    return mgl_json_hash_convert(json);
  }
  if (json_is_array(json))
  {
    mgl_logger_info("list\n");
    return mgl_json_list_convert(json);
  }
  if (json_is_string(json))
  {
    mgl_logger_info("string\n");
    return mgl_dict_new_string((char *)json_string_value(json));
  }
  if (json_is_boolean(json))
  {
    mgl_logger_info("bool\n");
    return mgl_dict_new_bool(json_is_true(json));
  }
  if (json_is_integer(json))
  {
    mgl_logger_info("int\n");
    return mgl_dict_new_int(json_integer_value(json));
  }
  if (json_is_real(json))
  {
    mgl_logger_info("float\n");
    return mgl_dict_new_float(json_real_value(json));
  }
  if (json_is_null(json))
  {
    mgl_logger_info("NULL\n");
    return NULL;
  }
  mgl_logger_info("failed to convert object\n");
  return NULL;
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

void mgl_json_log_error(json_error_t *er)
{
  if (!er)return;
  mgl_logger_error("json error: %s in %s at %i:%i",er->text,er->source,er->line,er->column);
}

/*eol@eof*/
