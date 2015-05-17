#include "mgl_resource.h"
#include <string.h>

/**
 * @purpose mgl_resource_test is meant to test the abstract resource manager system
 */


typedef struct
{
  GString *string;
  unsigned long int id;
}TestElement;

void test_delete(void *data);
MglBool test_load(char *filename,void *data);

MglResourceManager * manager = NULL;

int main(int argc,char *argv[])
{
  int i;
  if ((argc == 2) && (strcmp(argv[1],"-h")==0))
  {
    fprintf(stdout,"usage:\n");
    fprintf(stdout,"%s [FILES]\n",argv[0]);
    return 0;
  }
  fprintf(stdout,"mgl_resource_test begin\n");
  manager = mgl_resource_manager_init(
    "test manager",
    10,
    sizeof(TestElement),
    MglFalse,
    test_delete,
    test_load
  );
  
  for (i = 1;i < argc;++i)
  {
    /*grab "filenames" from command line*/
    fprintf(stdout,"adding resource: %s\n",argv[i]);
    mgl_resource_manager_load_resource(manager,argv[i]);
  }
  fprintf(stdout,"resource manage has %i elements\n",mgl_resource_manager_get_element_count(manager));
  fprintf(stdout,"freeing resource manager...\n");
  
  fprintf(stdout,"mgl_resource_test end\n");
}

void test_delete(void *data)
{
  TestElement *element;
  element = (TestElement *)data;
  if (!element)return;
  if (element->string != NULL)
  {
    g_string_free(element->string,TRUE);
  }
}

MglBool test_load(char *filename,void *data)
{
  TestElement *element;
  element = (TestElement *)data;
  if (!element)return MglFalse;
  element->id = mgl_resource_element_get_id(manager,data);
  element->string = g_string_new(filename);
  return MglTrue;
}
