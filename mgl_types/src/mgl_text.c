#include "mgl_text.h"

void mgl_text_chomp(char *text,int length,int strl)
{
    int i;
    if (!text)return;
    for(i = 0;i < strl - length;i++)
    {
        text[i] = text[i + length];
    }
    if (i > 0)
    {
        text[i - 1] = '\0';/*null terminate in case its overwritten*/
    }
    else
    {
        text[0] = '\0';
    }
}

char * mgl_text_clean_control_characters(char *in)
{
    char *out;
    int outIndex = 0;
    int inIndex = 0;
    out = malloc(strlen(in)*2);
    if (!out)return NULL;
    for (inIndex = 0;inIndex < strlen(in);inIndex++,outIndex++)
    {
        if (in[inIndex] == '\t')
        {
            out[outIndex++] = ' ';
            out[outIndex] = ' ';
            continue;
        }
        if (in[inIndex] == '\r')
        {
            out[outIndex] = ' ';
            continue;
        }
        out[outIndex] = in[inIndex];
    }
    out[outIndex] = '\0';
    return out;
}

/*eol@eof*/