#include "mgl_yaml_parse.h"
#include "mgl_logger.h"
#include "mgl_dict.h"
#include <yaml.h>


enum MglConfigState { KEY, VAL, SEQ };

void mgl_config_parse_tier(yaml_parser_t *parser, MglDict *chain);

MglDict *mgl_yaml_parse(char* filename)
{
  FILE *file;
  yaml_parser_t parser;
  MglDict *data;
  
  if(!yaml_parser_initialize(&parser))
  {
    return NULL;
  }
  file = fopen(filename,"r");
  if (!file)
  {
    mgl_logger_error("failed to open input file for parsing: %s",filename);
    return NULL;
  }
    
  yaml_parser_set_input_file(&parser, file);
  
  data = mgl_dict_new_hash();
  mgl_config_parse_tier(&parser, data);

  yaml_parser_delete(&parser);
  fclose(file);
  
  return data;
}

void mgl_config_parse_sequence(yaml_parser_t *parser, MglDict *chain)
{
  int done = 0;
  MglDict *next = NULL;
  yaml_event_t event;
  /* First element must be a variable, or we'll change states to SEQ */
  int state = KEY;
  do
  {
    yaml_parser_parse(parser, &event);
    switch(event.type)
    {
      case YAML_MAPPING_START_EVENT:
        next = mgl_dict_new_hash();
        mgl_dict_list_append(chain,next);
        state ^= VAL;
        mgl_config_parse_tier(parser, next);
        break;
      case YAML_SEQUENCE_END_EVENT:
      case YAML_MAPPING_END_EVENT:
      case YAML_STREAM_END_EVENT:
        done = 1;
        /* terminate the while loop, see below */
        break;
      case YAML_STREAM_START_EVENT:
      case YAML_DOCUMENT_START_EVENT:
        /*ignored*/
        break;
      default:
        mgl_logger_info( "mgl_config: unhandled YAML event %d\n", event.type);
    }
    if(parser->error != YAML_NO_ERROR)
    {
      mgl_logger_error( "mgl_config: yaml_error_type_e %d: %s %s at (line: %lu, col: %lu)\n",
                          parser->error, parser->context, parser->problem, parser->problem_mark.line,
                          parser->problem_mark.column);
                          return;
    }
    yaml_event_delete(&event);
  }while (!done);
}

void mgl_config_parse_tier(yaml_parser_t *parser, MglDict *chain)
{
  int done = 0;
  MglLine last_key;
  MglDict *next = NULL;
  yaml_event_t event;
  /* First element must be a variable, or we'll change states to SEQ */
  int state = KEY;
  mgl_line_cpy(last_key,"");
  do
  {
    yaml_parser_parse(parser, &event);
    switch(event.type)
    {
      case YAML_SCALAR_EVENT:
        if (state == KEY)
        {
          /* new key, hold on to it until we get a value as well */
          mgl_line_cpy(last_key,(char *)event.data.scalar.value);
        }
        else
        {
          /* state is VAL or SEQ */
          /* TODO data type logic should go here */
          mgl_logger_info( "mgl_config: adding key -> value (%s -> %s)\n", last_key, event.data.scalar.value);
          next = mgl_dict_new_string((char *)event.data.scalar.value);
          mgl_dict_hash_insert(chain,last_key,next);
        }
        state ^= VAL; /* Toggles KEY/VAL, avoids touching SEQ */
        break;
      case YAML_SEQUENCE_START_EVENT:
        mgl_logger_info("mgl_config: adding sequence %s...\n",last_key);
        next = mgl_dict_new_list();
        mgl_dict_hash_insert(chain,last_key,
                                 next);
        mgl_config_parse_sequence(parser, next);
        break;
      case YAML_MAPPING_START_EVENT:
        if (strlen(last_key) == 0)break;/*first level is implied hash.*/
        mgl_logger_info("mgl_config: adding hash %s...\n",last_key);
        next = mgl_dict_new_hash();
        mgl_dict_hash_insert(chain,last_key,next);
        state ^= VAL;
        mgl_config_parse_tier(parser, next);
        break;
      case YAML_MAPPING_END_EVENT:
      case YAML_STREAM_END_EVENT:
      case YAML_SEQUENCE_END_EVENT:
        done = 1;
        /* terminate the while loop, see below */
        break;
      default:
        mgl_logger_info("mgl_config: unhandled YAML event %d\n", event.type);
    }
    if(parser->error != YAML_NO_ERROR)
    {
      mgl_logger_info( "mgl_config: yaml_error_type_e %d: %s %s at (line: %lu, col: %lu)\n",
                          parser->error, parser->context, parser->problem, parser->problem_mark.line,
                          parser->problem_mark.column);
                          return;
    }
    yaml_event_delete(&event);
  }
  while (!done);
}

/*eol@eof*/
