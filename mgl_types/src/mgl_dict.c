#include "mgl_text.h"
#include "mgl_types.h"
#include "mgl_rect.h"
#include "mgl_dict.h"

#include <glib/gstring.h>
#include <glib/glist.h>
#include <glib/ghash.h>
#include <glib.h>

/*local prototypes*/
void mgl_dict_print_link(MglDict *link,MglUint depth,MglBool listStart);

/*function definitions*/

void mgl_g_string_free(char *string)
{
  if (!string)return;
  g_free(string);
}

void mgl_dict_scalar_free(MglDict *scalar)
{
  if (!scalar)return;
  if (scalar->keyType != MGL_DICT_LIST)return;
  if (scalar->keyValue == NULL)return;
  free(scalar->keyValue);
  scalar->keyValue = NULL;
}

void mgl_dict_destroy(MglDict *link)
{
  mgl_dict_free(&link);
}

void mgl_dict_hash_free(MglDict *hash)
{
  if (!hash)return;
  if (hash->keyType != MGL_DICT_HASH)return;
  g_hash_table_destroy(hash->keyValue);
  free(hash);
}

void mgl_dict_list_free(MglDict *list)
{
  GList *it = NULL;
  if (!list)return;
  if (list->keyType != MGL_DICT_LIST)return;
  for (it = list->keyValue;it != NULL;it = it->next)
  {
    mgl_dict_destroy(it->data);
  }
  free(list);
}

void mgl_dict_list_clear(MglDict *list)
{
  GList *it = NULL;
  if (!list)return;
  if (list->keyType != MGL_DICT_LIST)return;
  for (it = list->keyValue;it != NULL;it = it->next)
  {
    mgl_dict_destroy(it->data);
  }
  g_list_free(list->keyValue);
  list->keyValue = NULL;
  list->itemCount = 0;
}

void mgl_dict_free(MglDict **link)
{
  if (!link)return;
  if (!*link)return;
  if ((*link)->keyValue != NULL)
  {
    if ((*link)->keyFree != NULL)
    {
      (*link)->keyFree((*link)->keyValue);
    }
    else
    {
      free((*link)->keyValue);
    }
  }
  free(*link);
  *link = NULL;
}

MglDict *mgl_dict_clone(MglDict *src)
{
  if (!src)return NULL;
  if (!src->keyClone)return NULL;
  return src->keyClone(src);
}

MglDict *mgl_dict_clone_string(MglDict *src)
{
  MglLine str;
  if (!mgl_dict_get_line(str,src))return NULL;
  return mgl_dict_new_string(str);
}

MglDict *mgl_dict_clone_list(MglDict *src)
{
  MglUint count,i;
  MglDict *item, *list = NULL;
  if (!src)return NULL;
  list = mgl_dict_new_list();
  if (!list)return NULL;
  count = mgl_dict_get_list_count(src);
  for (i = 0;i < count;i++)
  {
    item = mgl_dict_get_list_nth(src, i);
    if (!item)continue;
    mgl_dict_list_append(list,mgl_dict_clone(item));
  }
  return list;
}

MglDict *mgl_dict_clone_hash(MglDict *src)
{
  MglUint count,i;
  MglDict *hash;
  MglLine keystring;
  MglDict * value;
  if (!src)return NULL;
  hash = mgl_dict_new_hash();
  if (!hash)return NULL;
  count = mgl_dict_get_hash_count(src);
  for (i = 0;i < count;i++)
  {
    value = mgl_dict_get_hash_nth(keystring,src, i);
    if (!value)continue;
    mgl_dict_hash_insert(hash,keystring,mgl_dict_clone(value));
  }
  return hash;
}

MglDict *mgl_dict_new()
{
  MglDict *link = NULL;
  link = (MglDict *)malloc(sizeof(MglDict));
  if (link == NULL)
  {
    return NULL;
  }
  memset(link,0,sizeof(MglDict));
  return link;
}



MglDict *mgl_dict_new_int_scaler(MglInt value)
{
  MglInt *cast;
  MglDict *link;
  link = mgl_dict_new();
  if (!link)return NULL;
  link->keyType = MGL_DICT_INT;
  link->keyFree = (MglDictFree)mgl_dict_scalar_free;
  link->keyValue = malloc(sizeof(MglInt));
  if (link->keyValue == NULL)
  {
    free (link);
    return NULL;
  }
  cast = (MglInt *)link->keyValue;
  *cast = value;
  return link;
}

MglDict *mgl_dict_new_uint_scaler(MglUint value)
{
  MglUint *cast;
  MglDict *link;
  link = mgl_dict_new();
  if (!link)return NULL;
  link->keyType = MGL_DICT_UINT;
  link->keyFree = (MglDictFree)mgl_dict_scalar_free;
  link->keyValue = malloc(sizeof(MglUint));
  if (link->keyValue == NULL)
  {
    free (link);
    return NULL;
  }
  cast = (MglUint *)link->keyValue;
  *cast = value;
  return link;
}

MglDict *mgl_dict_new_float_scaler(MglFloat value)
{
  MglFloat *cast;
  MglDict *link;
  link = mgl_dict_new();
  if (!link)return NULL;
  link->keyType = MGL_DICT_FLOAT;
  link->keyFree = (MglDictFree)mgl_dict_scalar_free;
  link->keyValue = malloc(sizeof(MglFloat));
  if (link->keyValue == NULL)
  {
    free (link);
    return NULL;
  }
  cast = (MglFloat *)link->keyValue;
  *cast = value;
  return link;
}

MglDict *mgl_dict_new_bool(MglBool n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%s",mgl_string_from_bool(n));
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_int(MglInt n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%i",n);
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_uint(MglInt n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%u",n);
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_float(MglFloat n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%f",n);
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_vec2d(MglVec2D n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%f,%f",n.x,n.y);
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_vec3d(MglVec3D n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%f,%f,%f",n.x,n.y,n.z);
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_vec4d(MglVec4D n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%f,%f,%f,%f",n.x,n.y,n.z,n.w);
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_rect(MglRect n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%i,%i,%i,%i",n.x,n.y,n.w,n.h);
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_rectf(MglRectFloat n)
{
  MglLine text;
  snprintf(text,MGLLINELEN,"%f,%f,%f,%f",n.x,n.y,n.w,n.h);
  return mgl_dict_new_string(text);
}

MglDict *mgl_dict_new_string(char *text)
{
  MglDict *link;
  link = mgl_dict_new();
  if (!link)return NULL;
  link->keyType = MGL_DICT_STRING;
  link->itemCount = strlen(text);
  link->keyFree = (MglDictFree)mgl_g_string_free;
  link->keyClone = mgl_dict_clone_string;
  link->keyValue = g_strdup(text);
  return link;
}

MglDict *mgl_dict_new_list()
{
  MglDict *link;
  link = mgl_dict_new();
  if (!link)return NULL;
  link->keyType = MGL_DICT_LIST;
  link->itemCount = 0;
  link->keyFree = (MglDictFree)mgl_dict_list_free;
  link->keyClone = mgl_dict_clone_list;
  link->keyValue = NULL;
  return link;
}

MglDict *mgl_dict_new_hash()
{
  MglDict *link;
  link = mgl_dict_new();
  if (!link)return NULL;
  link->keyType = MGL_DICT_HASH;
  link->itemCount = 0;
  link->keyFree = (MglDictFree)mgl_dict_hash_free;
  link->keyClone = mgl_dict_clone_hash;
  link->keyValue =
    g_hash_table_new_full(g_str_hash,
                          g_str_equal,
                          (GDestroyNotify)mgl_g_string_free,
                          (GDestroyNotify)mgl_dict_destroy);
  return link;
}

MglBool mgl_dict_get_line(MglLine output,MglDict *key)
{
  if (!key)return MglFalse;
  if (key->keyType != MGL_DICT_STRING)return MglFalse;
  if (key->keyValue == NULL)return MglFalse;
  mgl_line_cpy(output,key->keyValue);
  return MglTrue;
}

void mgl_dict_hash_remove(MglDict *hash,char *key)
{
  GHashTable*hashtable = NULL;
  if (!hash)return;
  if (hash->keyType != MGL_DICT_HASH)return;
  if (hash->keyValue == NULL)return;
  hashtable = (GHashTable*)hash->keyValue;
  g_hash_table_remove(hashtable,key);
}

void mgl_dict_hash_insert(MglDict *hash,char *key,MglDict *value)
{
  GHashTable*hashtable = NULL;
  if (!hash)return;
  if (hash->keyType != MGL_DICT_HASH)return;
  if (hash->keyValue == NULL)return;
  hashtable = (GHashTable*)hash->keyValue;
  if (g_hash_table_lookup(hashtable,key) != NULL)
  {
    g_hash_table_replace(hashtable,g_strdup(key),value);
  }
  else
  {
    g_hash_table_insert(hashtable,g_strdup(key),value);
    hash->itemCount++;
  }
}

void mgl_dict_list_append(MglDict *list,MglDict *item)
{
  if (!list)return;
  if (list->keyType != MGL_DICT_LIST)return;
  list->keyValue = g_list_append(list->keyValue,item);
  list->itemCount++;
}

MglDict *mgl_dict_get_hash_value(MglDict *hash,MglLine key)
{
  GHashTable*hashtable = NULL;
  if (!hash)
  {    
    return NULL;
  }
  if (hash->keyType != MGL_DICT_HASH)
  {    
    return NULL;
  }
  if (hash->keyValue == NULL)
  {
    return NULL;
  }
  hashtable = (GHashTable*)hash->keyValue;
  return g_hash_table_lookup(hashtable,key);
}

MglUint mgl_dict_get_hash_count(MglDict *list)
{
  if (!list)return 0;
  if (list->keyType != MGL_DICT_HASH)return 0;
  if (list->keyValue == NULL)return 0;
  return list->itemCount;
}

MglUint mgl_dict_get_list_count(MglDict *list)
{
  if (!list)return 0;
  if (list->keyType != MGL_DICT_LIST)return 0;
  if (list->keyValue == NULL)return 0;
  return list->itemCount;
}

MglDict *mgl_dict_get_hash_nth(MglLine key, MglDict *hash, MglUint n)
{
  GHashTable *hashtable = NULL;
  GList *keys = NULL, *values = NULL;
  if (!hash)return NULL;
  if (hash->keyType != MGL_DICT_HASH)return NULL;
  if (hash->keyValue == NULL)return NULL;
  hashtable = (GHashTable*)hash->keyValue;
  keys = g_hash_table_get_keys(hashtable);
  values = g_hash_table_get_values(hashtable);
  if ((!keys)||(!values))return NULL;
  mgl_line_cpy(key,g_list_nth_data(keys,n));
  g_list_free(values);
  g_list_free(keys);
  return g_list_nth_data(values,n);
}

MglDict *mgl_dict_get_list_nth(MglDict *list, MglUint n)
{
  if (!list)return NULL;
  if (list->keyType != MGL_DICT_LIST)return NULL;
  if (list->keyValue == NULL)return NULL;
  return g_list_nth_data((GList*)list->keyValue,n);
}

void mgl_dict_list_remove_nth(MglDict *list, MglUint n)
{
  GList *link;
  if (!list)return;
  if (list->keyType != MGL_DICT_LIST)return;
  if (list->keyValue == NULL)return;
  link = g_list_nth((GList*)list->keyValue,n);
  if (link == NULL)return;
  list->keyValue = g_list_remove_link(list->keyValue,link);
  mgl_dict_free((MglDict**)&link->data);
  g_list_free(link);
}

void mgl_dict_list_move_nth_top(MglDict *list, MglUint n)
{
  GList *link;
  if (!list)return;
  if (list->keyType != MGL_DICT_LIST)return;
  if (list->keyValue == NULL)return;
  link = g_list_nth((GList*)list->keyValue,n);
  if (link == NULL)return;
  list->keyValue = g_list_remove_link(list->keyValue,link);
  list->keyValue = g_list_concat(link,list->keyValue);
  g_list_free(link);
}

void mgl_dict_list_move_nth_bottom(MglDict *list, MglUint n)
{
  GList *link;
  if (!list)return;
  if (list->keyType != MGL_DICT_LIST)return;
  if (list->keyValue == NULL)return;
  link = g_list_nth((GList*)list->keyValue,n);
  if (link == NULL)return;
  list->keyValue = g_list_remove_link(list->keyValue,link);
  list->keyValue = g_list_concat(list->keyValue,link);
  g_list_free(link);
}

MglBool mgl_dict_get_hash_value_as_uint(MglUint *output, MglDict *hash, MglLine key)
{
  MglUint temp = 0;
  MglLine keyValue;
  MglDict *chain;
  if ((!hash) || (!output) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)return MglFalse;
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(keyValue,chain->keyValue);
  if (sscanf(keyValue,"%ui",&temp) != 1)return MglFalse;
  *output = temp;
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_int(MglInt *output, MglDict *hash, MglLine key)
{
  MglInt temp = 0;
  MglLine keyValue;
  MglDict *chain;
  if ((!hash) || (!output) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)return MglFalse;
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(keyValue,chain->keyValue);
  if (sscanf(keyValue,"%i",&temp) != 1)return MglFalse;
  *output = temp;
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_float(MglFloat *output, MglDict *hash, MglLine key)
{
  MglFloat temp = 0;
  MglLine keyValue;
  MglDict *chain;
  if ((!hash) || (!output) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)return MglFalse;
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(keyValue,chain->keyValue);
  if (sscanf(keyValue,"%f",&temp) != 1)return MglFalse;
  *output = temp;
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_bool(MglBool *output, MglDict *hash, MglLine key)
{
  MglDict *chain;
  MglInt boo;
  if ((!hash) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)
  {
    return MglFalse;
  }
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  boo = mgl_bool_from_string(chain->keyValue);
  if (boo == -1)return MglFalse;/*tag was not boolean*/
  *output = boo;
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_line(MglLine output, MglDict *hash, MglLine key)
{
  MglDict *chain;
  if ((!hash) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)
  {
    return MglFalse;
  }
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(output,chain->keyValue);
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_vec4d(MglVec4D *output, MglDict *hash, MglLine key)
{
  MglVec4D temp = {0,0,0,0};
  MglLine keyValue;
  MglDict *chain;
  if ((!hash) || (!output) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)return MglFalse;
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(keyValue,chain->keyValue);
  if (sscanf(keyValue,"%lf,%lf,%lf,%lf",&temp.x,&temp.y,&temp.z,&temp.w) != 4)return MglFalse;
  mgl_vec4d_copy((*output),temp);
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_vec3d(MglVec3D *output, MglDict *hash, MglLine key)
{
  MglVec3D temp = {0,0,0};
  MglLine keyValue;
  MglDict *chain;
  if ((!hash) || (!output) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)return MglFalse;
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(keyValue,chain->keyValue);
  if (sscanf(keyValue,"%lf,%lf,%lf",&temp.x,&temp.y,&temp.z) != 3)return MglFalse;
  mgl_vec3d_copy((*output),temp);
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_vec2d(MglVec2D *output, MglDict *hash, MglLine key)
{
  MglVec2D temp = {0,0};
  MglLine keyValue;
  MglDict *chain;
  if ((!hash) || (!output) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)return MglFalse;
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(keyValue,chain->keyValue);
  if (sscanf(keyValue,"%lf,%lf",&temp.x,&temp.y) != 2)return MglFalse;
  mgl_vec2d_copy((*output),temp);
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_rect(MglRect *output, MglDict *hash, MglLine key)
{
  MglRect temp = {0,0,0,0};
  MglLine keyValue;
  MglDict *chain;
  if ((!hash) || (!output) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)return MglFalse;
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(keyValue,chain->keyValue);
  if (sscanf(keyValue,"%i,%i,%i,%i",&temp.x,&temp.y,&temp.w,&temp.h) != 4)return MglFalse;
  mgl_rect_copy(output,temp);
  return MglTrue;
}

MglBool mgl_dict_get_hash_value_as_rectfloat(MglRectFloat *output, MglDict *hash, MglLine key)
{
  MglRectFloat temp = {0,0,0,0};
  MglLine keyValue;
  MglDict *chain;
  if ((!hash) || (!output) || (strlen(key) == 0))return MglFalse;
  chain = mgl_dict_get_hash_value(hash,key);
  if (!chain)return MglFalse;
  if (chain->keyType != MGL_DICT_STRING)return MglFalse;
  mgl_line_cpy(keyValue,chain->keyValue);
  if (sscanf(keyValue,"%lf,%lf,%lf,%lf",&temp.x,&temp.y,&temp.w,&temp.h) != 4)return MglFalse;
  mgl_rectf_copy(output,temp);
  return MglTrue;
}

void mgl_dict_print_string(MglDict *link,MglUint depth)
{
  int i;
  if ((!link)||(!link->keyValue))return;
  for (i = 0; i < depth;i++){printf("  ");}
  printf("%s\n",(char *)link->keyValue);
}

void mgl_dict_print_hash(MglDict *link,MglUint depth,MglBool listStart)
{
  int i;
  GList *keys = NULL;
  GList *values = NULL;
  if (!link)return;
  if (link->keyType != MGL_DICT_HASH)return;
  keys = g_hash_table_get_keys((GHashTable*)link->keyValue);
  values = g_hash_table_get_values((GHashTable*)link->keyValue);
  if (!listStart)printf("\n");
  for (;(keys != NULL) && (values != NULL); keys = keys->next,values = values->next)
  {
    for (i = 0; i < depth;i++){printf("  ");}
    printf("%s :",(char *)keys->data);
    mgl_dict_print_link((MglDict *)values->data,depth + 1,MglFalse);
    printf("\n");
  }
  g_list_free(values);
  g_list_free(keys);
}

void mgl_dict_print_list(MglDict *link,MglUint depth,MglBool listStart)
{
  int i;
  GList *it;
  if (!link)return;
  if (link->keyType != MGL_DICT_LIST)return;
  printf("\n");
  for (it = (GList *)link->keyValue;it != NULL;it = it->next)
  {
    for (i = 0; i < (depth - 1);i++){printf("  ");}
    printf("- ");
    mgl_dict_print_link((MglDict *)it->data,depth + 1,MglTrue);
    printf("\n");
  }
}

void mgl_dict_print_link(MglDict *link,MglUint depth,MglBool listStart)
{
  if (!link)return;
  switch(link->keyType)
  {
    case MGL_DICT_STRING:
      mgl_dict_print_string(link,depth);
      break;
    case MGL_DICT_LIST:
      mgl_dict_print_list(link,depth,listStart);
      break;
    case MGL_DICT_HASH:
      mgl_dict_print_hash(link,depth,listStart);
      break;
    default:
      printf("unhandled type: %i\n",(int)link->keyType);
  }
}

void mgl_dict_print(const MglDict *chain)
{
  mgl_dict_print_link((MglDict *)chain,0,MglFalse);
}

/*eol@eof*/
