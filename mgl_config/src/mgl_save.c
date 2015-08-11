#include "mgl_save.h"
#include "mgl_logger.h"
#include "mgl_json_parse.h"

#define MGLSAVEMAJOR 1
#define MGLSAVEMINOR 0
#define MGLSAVEKEY   1439072147
#define MGLSAVEOFFSET 100

typedef struct
{
    MglUint key;    /**<unique key meant to verify file type*/
    MglUint major;  /**<major version of the file type*/
    MglUint minor;  /**<minor version of the file type*/
    size_t  size;   /**<size of the header*/
}MglSaveHeader;

void mgl_save_dict_as_json(MglDict *dict, char *filepath)
{
    char *json;
    FILE *file;
    if (!dict)
    {
        mgl_logger_warn("could not save: no data provided");
        return;
    }
    file = fopen(filepath,"w");
    if (!file)
    {
        mgl_logger_warn("failed to open file %s for writing",filepath);
        return;
    }
    json = mgl_json_convert_dict_to_string(dict);
    if (!json)
    {
        mgl_logger_warn("failed to convert dict to json");
        fclose(file);
        return;
    }
    fprintf(file,"%s",json);
    fclose(file);
    free(json);
}

MglSaveHeader *mgl_save_new_header()
{
    MglSaveHeader *header;
    header = (MglSaveHeader *)malloc(sizeof(MglSaveHeader));
    if (!header)return NULL;
    memset(header,0,sizeof(MglSaveHeader));
    header->major = MGLSAVEMAJOR;
    header->minor = MGLSAVEMINOR;
    header->size = sizeof(MglSaveHeader);
    header->key = MGLSAVEKEY;
    return header;
}

void mgl_save_dict_as_binary_config(MglDict *dict, char *filepath)
{
    char *json;
    FILE *file;
    int i,count;
    MglUI64 out;
    MglSaveHeader *header;
    if (!dict)
    {
        mgl_logger_warn("could not save: no data provided");
        return;
    }
    header = mgl_save_new_header();
    if (!header)return;
    json = mgl_json_convert_dict_to_packed_string(dict);
    if (!json)
    {
        mgl_logger_warn("failed to convert dict to json");
        free(header);
        return;
    }
    file = fopen(filepath,"wb");
    if (!file)
    {
        mgl_logger_warn("failed to open file %s for writing",filepath);
        free(header);
        free(json);
        return;
    }
    count = strlen(json);
    header->size = count;
    fwrite(header,sizeof(MglSaveHeader),1,file);
    for (i = 0; i < count;i++)
    {
        out = json[i] + MGLSAVEOFFSET;
        fwrite(&out,sizeof(MglUI64),1,file);
    }
    fclose(file);
    free(header);
    free(json);
}

char * mgl_save_load_binary_data_v1_0(FILE *file, MglSaveHeader *header)
{
    char * data;
    int i;
    MglUI64 in;
    if (!file)return NULL;
    if (!header)return NULL;
    data = malloc(sizeof(char) * header->size);
    if (!data)
    {
        mgl_logger_warn("failed to allocate file data");
        return NULL;
    }
    memset(data,0,sizeof(sizeof(char) * header->size));
    for (i = 0; i < header->size;i++)
    {
        fread(&in,sizeof(MglUI64),1,file);
        data[i] = (char)in - MGLSAVEOFFSET;
    }
    return data;
}

char *mgl_save_binary_load(char *filepath)
{
    char * data = NULL;
    FILE *file;
    MglSaveHeader header;
    file = fopen(filepath,"rb");
    if (!file)
    {
        mgl_logger_warn("failed to open file %s for reading",filepath);
        return NULL;
    }
    fread(&header,sizeof(MglSaveHeader),1,file);
    if (header.key != MGLSAVEKEY)
    {
        /*NOT an MGL binary file*/
        mgl_logger_debug("file %s is not an MGL Save File",filepath);
        fclose(file);
        return NULL;
    }
    if ((header.major == 1) && (header.minor == 0))
    {
        data = mgl_save_load_binary_data_v1_0(file,&header);
    }
    else
    {
        mgl_logger_warn("unsupported file version : %i.%i",header.major,header.minor);
    }
    fclose(file);
    return data;
}

/*eol@eof*/
