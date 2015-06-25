#include "mgl_input.h"
#include "mgl_logger.h"
#include "mgl_config.h"
#include "mgl_resource.h"
#include <SDL.h>
#include <glib/glist.h>
#include <glib.h>

struct MglInput_S
{
    MglLine  name;  /**<name of the input*/
    MglUint  deviceType; /**<device type for the input, ie: keyboard, joystick, mouse etc*/
    MglUint  deviceId; /**<device index for the input, ie: keyboard 0, joystick 2, etc*/
    MglUint  type;  /**<input type: key, button, axis*/
    MglUint  id;      /**<which key or axis or button*/
    MglFloat value;    /**<current state of the input*/
    MglFloat oldvalue; /**<last state of the input*/
    MglUint  timePress; /**<this time index*/
    MglUint  lastPress; /**<time index of the last time this input was pressed*/
};

static MglResourceManager * __mgl_input_resource_manager = NULL;
MglUint    _this_frame_keyboard_mod = 0;
MglUint    _last_frame_keyboard_mod = 0;
static const Uint8    * _this_frame_keyboard = NULL;
static Uint8    * _last_frame_keyboard = NULL;
static MglInt     _mgl_input_keyboard_numkeys = 0;


void mgl_input_close();
MglBool mgl_input_load_resource(char *filename,void *data);
void mgl_input_delete(void *data);

MglInputType mgl_input_parse_type(char *type);
MglInputType mgl_input_parse_device_type(char *type);
void mgl_input_clear_keyboard();
void mgl_input_update_keyboard();
void mgl_input_keyboard_close();


void mgl_input_init(
    MglUint maxInputs
)
{
    __mgl_input_resource_manager = mgl_resource_manager_init(
        "mgl input",
        maxInputs,
        sizeof(struct MglInput_S),
        MglFalse,
        mgl_input_delete,
        mgl_input_load_resource
    );
    _this_frame_keyboard = SDL_GetKeyboardState(&_mgl_input_keyboard_numkeys);
    _last_frame_keyboard = (Uint8 *)malloc(sizeof(Uint8)*_mgl_input_keyboard_numkeys);
    memcpy(
        _last_frame_keyboard,
        _this_frame_keyboard,
        sizeof(Uint8)*_mgl_input_keyboard_numkeys
    );
    if(_last_frame_keyboard == NULL)
    {
        mgl_logger_error("mgl_input: unable to initialize keyboard structure!\n");
    }
    atexit(mgl_input_close);
}

void mgl_input_close()
{
    mgl_resource_manager_free(&__mgl_input_resource_manager);
    mgl_input_keyboard_close();
}

MglBool mgl_input_load_resource(char *filename,void *data)
{
    MglInput *input;
    char **strings;
    
    if ((!filename)||(!data))
    {
        return MglFalse;
    }
    input = (MglInput *)data;
    
    strings = g_strsplit_set (filename,
                              "|",
                              0);
    mgl_line_cpy(input->name,strings[0]);
    input->deviceType = atoi(strings[1]);
    input->deviceId = atoi(strings[2]);
    input->type = atoi(strings[3]);
    input->id = atoi(strings[4]);
    g_strfreev (strings);
    
    return MglTrue;
}

MglInput *mgl_input_new(
    MglLine  name,
    MglUint  deviceType,
    MglUint  deviceId,
    MglUint  type,
    MglUint  id)
{
    char *filenamePacked;
    MglInput *input;
    
    filenamePacked = g_strdup_printf (
        "%s|%i|%i|%i|%i",
        name,
        deviceType,
        deviceId,
        type,
        id);
    input = mgl_resource_manager_load_resource(__mgl_input_resource_manager,name);
    free(filenamePacked);
    return input;
}

void mgl_input_delete(void *data)
{
    return;
}

MglInput *mgl_input_create_from_dict(MglDict *data)
{
    MglLine  name = "<NULL>";
    MglLine  deviceType;
    MglUint  deviceId;
    MglLine  type;
    MglUint  id;
    if (!data)
    {
        mgl_logger_warn("no data provided to create input");
        return NULL;
    }
    mgl_dict_get_hash_value_as_line(name, data, "name");
    mgl_dict_get_hash_value_as_line(deviceType, data, "deviceType");
    mgl_dict_get_hash_value_as_uint(&deviceId, data, "deviceId");
    mgl_dict_get_hash_value_as_line(type, data, "type");
    mgl_dict_get_hash_value_as_uint(&id, data, "id");
    return mgl_input_new(
        name,
        mgl_input_parse_device_type(deviceType),
        deviceId,
        mgl_input_parse_type(type),
        id);
}

MglInputType mgl_input_parse_type(char *type)
{
    if(strcmp(type,"key")==0)
    {
        return MglInputKey;
    }
    if(strcmp(type,"mod")==0)
    {
        return MglInputMod;
    }
    if(strcmp(type,"button")==0)
    {
        return MglInputButton;
    }
    if(strcmp(type,"+axis")==0)
    {
        return MglInputAxisPositive;
    }
    if(strcmp(type,"-axis")==0)
    {
        return MglInputAxisNegative;
    }
    mgl_logger_warn("unknown input type: %s",type);
    return MglInputNull;
}

MglInputType mgl_input_parse_device_type(char *type)
{
    if(strcmp(type,"keyboard")==0)
    {
        return MglInputKeyboard;
    }
    if(strcmp(type,"mouse")==0)
    {
        return MglInputMouse;
    }
    if(strcmp(type,"joystick")==0)
    {
        return MglInputJoystick;
    }
    mgl_logger_warn("unknown device type: %s",type);
    return MglInputKeyboard;
}


MglInt mgl_input_parse(MglInputType type, char *input)
{
    int i;
    if(input == NULL)return -1;
    switch(type)
    {
      case MglInputKey:
        if(input[0] == 'F')
        {
            sscanf(input,"F%i",&i);
            i--;
            return SDLK_F1 + i;
        }
        if((input[0] >= 'a')&&(input[0] <= 'z'))
        {
            return input[0]-'a' + SDLK_a;
        }
        if((input[0] >= '0')&&(input[0] <= '9'))
        {
            return input[0]-'0' + SDLK_0;
        }
        if(input[0] == '/')return SDLK_SLASH;
        if(input[0] == '\\')return SDLK_BACKSLASH;
        if(input[0] == '[')return SDLK_LEFTBRACKET;
        if(input[0] == ']')return SDLK_RIGHTBRACKET;
        if(input[0] == '-')return SDLK_MINUS;
        if(input[0] == '=')return SDLK_EQUALS;
        if(input[0] == '`')return SDLK_CARET;
        if(input[0] == ',')return SDLK_COMMA;
        if(input[0] == ';')return SDLK_SEMICOLON;
        if(input[0] == ':')return SDLK_COLON;
        if(input[0] == '.')return SDLK_PERIOD;
        if(strcmp(input,"TAB")==0)return SDLK_TAB;
        if(strcmp(input,"ESC")==0)return SDLK_ESCAPE;
        if(strcmp(input,"ENTER")==0)return SDLK_RETURN;
        if(strcmp(input,"SPACE")==0)return SDLK_SPACE;
        if(strcmp(input,"UPARROW")==0)return SDLK_UP;
        if(strcmp(input,"DOWNARROW")==0)return SDLK_DOWN;
        if(strcmp(input,"LEFTARROW")==0)return SDLK_LEFT;
        if(strcmp(input,"RIGHTARROW")==0)return SDLK_RIGHT;
        if(strcmp(input,"BACKSPACE")==0)return SDLK_BACKSPACE;
        break;
      case MglInputMod:
        if(strcmp(input,"LShift")==0)
        {
            return KMOD_LSHIFT;
        }
        if(strcmp(input,"RShift")==0)
        {
            return KMOD_RSHIFT;
        }
        if(strcmp(input,"LCtrl")==0)
        {
            return KMOD_LCTRL;
        }
        if(strcmp(input,"RCtrl")==0)
        {
            return KMOD_RCTRL;
        }
        if(strcmp(input,"LAlt")==0)
        {
            return KMOD_LALT;
        }
        if(strcmp(input,"RAlt")==0)
        {
            return KMOD_RALT;
        }
        break;
      case MglInputButton:
          sscanf(input,"%i",&i);
        return i;
      case MglInputAxisPositive:
        sscanf(input,"%i",&i);
        return i;
      case MglInputAxisNegative:
        sscanf(input,"%i",&i);
        return i;
      default:
        return -1;
    }
    return -1;
}

void mgl_input_update()
{
    MglInput *input;
    SDL_PumpEvents();
    mgl_input_update_keyboard();
    input = (MglInput *)mgl_resource_get_next_data(__mgl_input_resource_manager,NULL);
    for(;input != NULL;input = (MglInput *)mgl_resource_get_next_data(__mgl_input_resource_manager,(void *)input))
    {
        input->oldvalue = input->value;
        switch(input->deviceType)
        {
          case MglInputKeyboard:
            switch(input->type)
            {
              case MglInputKey:
                input->value = _this_frame_keyboard[input->id];
                break;
              case MglInputMod:
                if(_this_frame_keyboard_mod & input->id)
                {
                    input->value = 1;
                }
                else input->value = 0;
                break;
              default:
                break;
            }
            break;
          case MglInputMouse:
            break;
          case MglInputJoystick:
            break;
        }
        if(input->value > 0)
        {
            input->lastPress = input->timePress;
            input->timePress = SDL_GetTicks();
        }
    }
}

void mgl_input_update_keyboard()
{
    int i;
    if((_last_frame_keyboard == NULL) ||
        (_this_frame_keyboard == NULL))return;
    for (i = 0; i < _mgl_input_keyboard_numkeys;i++)
    {
        _last_frame_keyboard[i] = _this_frame_keyboard[i];
    }
    _last_frame_keyboard_mod = _this_frame_keyboard_mod;
    _this_frame_keyboard_mod = SDL_GetModState();
}

void mgl_input_clear_keyboard()
{
    if(_last_frame_keyboard == NULL)return;
    memset(_last_frame_keyboard,0,sizeof(Uint8)*_mgl_input_keyboard_numkeys);
    _last_frame_keyboard_mod = 0;
    _this_frame_keyboard_mod = 0;
}

void mgl_input_keyboard_close()
{
    if (_last_frame_keyboard != NULL)free(_last_frame_keyboard);
}

MglBool mgl_input_is_key_pressed(MglInt key)
{
    if((_last_frame_keyboard == NULL) ||
        (_this_frame_keyboard == NULL))
    {
        mgl_logger_error("mgl_input: uninitialized.");
        return 0;
    }
    if ((key < 0) || (key >= _mgl_input_keyboard_numkeys))
    {
        mgl_logger_error("mgl_input: key out of range\n");
        return 0;
    }
    if ((_last_frame_keyboard[key] == 0) &&
        (_this_frame_keyboard[key] == 1))
        return MglTrue;
    return MglFalse;
}

MglBool mgl_input_is_key_released(MglInt key)
{
    if((_last_frame_keyboard == NULL) ||
        (_this_frame_keyboard == NULL))
    {
        mgl_logger_error("mgl_input: uninitialized.\n");
        return 0;
    }
    if ((key < 0) || (key >= _mgl_input_keyboard_numkeys))
    {
        mgl_logger_error("mgl_input: key out of range\n");
        return 0;
    }
    if ((_last_frame_keyboard[key] == 1) &&
        (_this_frame_keyboard[key] == 0))
        return MglTrue;
    return MglFalse;
}

MglBool mgl_input_is_key_held(MglInt key)
{
    if((_last_frame_keyboard == NULL) ||
        (_this_frame_keyboard == NULL))
    {
        mgl_logger_error("mgl_input: uninitialized.");
        return 0;
    }
    if ((key < 0) || (key >= _mgl_input_keyboard_numkeys))
    {
        mgl_logger_error("mgl_input: key out of range\n");
        return 0;
    }
    if ((_last_frame_keyboard[key] == 1) &&
        (_this_frame_keyboard[key] == 1))
    {
        return MglTrue;
    }
    return MglFalse;
}

MglBool mgl_input_is_mod_held(MglUint mod)
{
    if((_last_frame_keyboard == NULL) ||
        (_this_frame_keyboard == NULL))
    {
        mgl_logger_error("mgl_input: uninitialized.");
        return 0;
    }
    if ((_this_frame_keyboard_mod & mod) &&
        (_last_frame_keyboard_mod & mod))
    {
        return MglTrue;
    }
    return MglFalse;
}

MglBool mgl_input_is_mod_down(MglUint mod)
{
    if((_last_frame_keyboard == NULL) ||
        (_this_frame_keyboard == NULL))
    {
        mgl_logger_error("mgl_input: uninitialized.");
        return 0;
    }
    if (_this_frame_keyboard_mod & mod)
    {
        return MglTrue;
    }
    return MglFalse;
}

MglBool mgl_input_is_mod_pressed(MglUint mod)
{
    if((_last_frame_keyboard == NULL) ||
        (_this_frame_keyboard == NULL))
    {
        mgl_logger_error("mgl_input: uninitialized.");
        return 0;
    }
    if ((_this_frame_keyboard_mod & mod) &&
        (!(_last_frame_keyboard_mod & mod)))
    {
        return MglTrue;
    }
    return MglFalse;
}

MglBool mgl_input_is_mod_released(MglUint mod)
{
    if((_last_frame_keyboard == NULL) ||
        (_this_frame_keyboard == NULL))
    {
        mgl_logger_error("mgl_input: uninitialized.");
        return 0;
    }
    if ((_last_frame_keyboard_mod & mod) &&
        (!(_this_frame_keyboard_mod & mod)))
    {
        return MglTrue;
    }
    return MglFalse;
}


/*eol@eof*/
