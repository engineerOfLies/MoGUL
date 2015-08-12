#include "mgl_json_parse.h"
#include "mgl_logger.h"

#include <jansson.h>

MglDict *mgl_json_convert(json_t *json);
MglDict *mgl_json_hash_convert(json_t *json);
MglDict *mgl_json_list_convert(json_t *json);

json_t *mgl_json_from_dict(MglDict *dict);

void mgl_json_log_error(json_error_t *er);

MglDict *mgl_json_parse_string(char *string)
{
    MglDict *data;
    json_t *json;
    json_error_t jer;
    mgl_logger_debug("begin json parse");
    json = json_loads(string,0,&jer);
    if (json == NULL)
    {
        mgl_logger_debug("mgl_json_parse: failed to load file %s as json\n", string);
        mgl_json_log_error(&jer);
        return NULL;
    }
    data = mgl_json_convert(json);
    json_decref(json); 
    if (data == NULL)
    {
        mgl_logger_debug("mgl_json_parse: failed to parse file %s as json\n", string);
        mgl_json_log_error(&jer);
    }
    return data;
}

MglDict *mgl_json_parse(char *filename)
{
  MglDict *data;
  json_t *json;
  json_error_t jer;
  mgl_logger_debug("begin json parse");
  json = json_load_file(filename,0,&jer);
  if (json == NULL)
  {
    mgl_logger_debug("mgl_json_parse: failed to load file %s as json\n", filename);
    mgl_json_log_error(&jer);
    return NULL;
  }
  data = mgl_json_convert(json);
  json_decref(json); 
  if (data == NULL)
  {
    mgl_logger_debug("mgl_json_parse: failed to parse file %s as json\n", filename);
    mgl_json_log_error(&jer);
  }
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
  mgl_logger_debug("failed to convert object\n");
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


json_t *mgl_json_from_hash(MglDict *dict)
{
    json_t *json;
    MglUint count,i;
    MglLine keystring;
    MglDict *value;
    if (!dict)return NULL;
    json = json_object();
    if (!json)return NULL;
    count = mgl_dict_get_hash_count(dict);
    for (i = 0;i < count;i++)
    {
        value = mgl_dict_get_hash_nth(keystring,dict, i);
        if (!value)continue;
        json_object_set(json,keystring,mgl_json_from_dict(value));
    }
    return json;
}

json_t *mgl_json_from_list(MglDict *dict)
{
    json_t *json;
    MglUint count,i;
    if (!dict)return NULL;
    json = json_array();
    if (!json)return NULL;
    count = mgl_dict_get_list_count(dict);
    for (i = 0; i < count;i++)
    {
        json_array_append(json,mgl_json_from_dict(mgl_dict_get_list_nth(dict,i)));
    }
    return json;
}

json_t *mgl_json_from_dict(MglDict *dict)
{
    if (!dict)return NULL;
    switch (dict->keyType)
    {
        case MGL_DICT_HASH:
            return mgl_json_from_hash(dict);
            break;
        case MGL_DICT_LIST:
            return mgl_json_from_list(dict);
            break;
        case MGL_DICT_INT:
        case MGL_DICT_UINT:
        case MGL_DICT_FLOAT:
        case MGL_DICT_STRING:
        case MGL_DICT_VOID:
        case MGL_DICT_CUSTOM0:
            return json_string(dict->keyValue);
            break;
    }
    return NULL;
}

char *mgl_json_convert_dict_to_packed_string(MglDict *dict)
{
    json_t *json;
    char *out;
    json = mgl_json_from_dict(dict);
    if (!json)return NULL;
    out = json_dumps(json, JSON_ENCODE_ANY);
    json_decref(json);
    return out;
}

char *mgl_json_convert_dict_to_string(MglDict *dict)
{
    json_t *json;
    char *out;
    json = mgl_json_from_dict(dict);
    if (!json)return NULL;
    out = json_dumps(json, JSON_INDENT(4));
    json_decref(json);
    return out;
}

/*eol@eof*/
