#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#define NUM_EVENTS 3

int nb_instructions=0;
void s1113(double *A, double *B, int size1, int size2){
  int nl, i;
  for(nl = 0; nl < size1; nl++){
    for(i = 0; i < size2; i++){
      A[i] = A[size2/2] + B[i];
      nb_instructions++;
    }
  }
}

static int EventSet = PAPI_NULL;
static char *p3_native_name[] = {"PAPI_TOT_INS", "OFFCORE_RESPONSE_1:WB", NULL};
static char **native_name = p3_native_name;

int main(int argc, char **argv){
  int i, retval, native;
  const PAPI_hw_info_t *hwinfo;
  long long values[8];

  if ((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT)
    printf("Erreur: PAPI_library_init\n");

  if ((retval = PAPI_create_eventset(&EventSet)) != PAPI_OK)
    printf("Erreur: PAPI_create_eventset\n");

  if ((hwinfo = PAPI_get_hardware_info( )) == NULL)
    printf("Erreur: PAPI_get_hardware_info\n");

  printf( "Architecture %s, %d\n", hwinfo->model_string, hwinfo->model );

  for (i=0; native_name[i] != NULL; i++) {
    retval = PAPI_event_name_to_code(native_name[i], &native);
    if (retval != PAPI_OK)
      printf("Erreur: PAPI_event_name_to_code\n");
    printf("Adding %s\n", native_name[i]);
    if ((retval = PAPI_add_event(EventSet, native)) != PAPI_OK)
      printf("Erreur: PAPI_add_event\n");
  }

  if ((retval = PAPI_start(EventSet)) != PAPI_OK)
    printf("Erreur: PAPI_start\n");

  /* do_both(1000); */

  if ((retval = PAPI_stop(EventSet, values)) != PAPI_OK)
    printf("Erreur: PAPI_stop\n");

  for (i=0; native_name[i] != NULL; i++) {
    fprintf(stderr, "%-40s: ", native_name[i]);
    fprintf(stderr, "values\n", values[i]);
  }

  retval = PAPI_cleanup_eventset(EventSet);
  if (retval != PAPI_OK)
    printf("Erreur: PAPI_cleanup\n");
  retval = PAPI_destroy_eventset(&EventSet);
  if (retval != PAPI_OK)
    printf("Erreur: PAPI_destroy_eventset\n");

  exit(0);
}
