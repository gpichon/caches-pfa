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

int main(int argc, char **argv){
  int size = 32000;
  double *A, *B;
  A = malloc(size*sizeof(double));
  B = malloc(size*sizeof(double));

  int event[NUM_EVENTS] = {PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L3_TCM};
  long long values[NUM_EVENTS];

  /* Start counting events */
  if (PAPI_start_counters(event, NUM_EVENTS) != PAPI_OK) {
    fprintf(stderr, "PAPI_start_counters - FAILED\n");
    exit(1);
  }

  s1113(A, B, 2, size);

  /* Read the counters */
  if (PAPI_read_counters(values, NUM_EVENTS) != PAPI_OK) {
    fprintf(stderr, "PAPI_read_counters - FAILED\n");
    exit(1);
  }

  printf("L1 Cache misses: %lld\n", values[0]);
  printf("L2 Cache misses: %lld\n", values[1]);
  printf("L3 Cache misses: %lld\n", values[2]);

  /* Stop counting events */
  if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK) {
    fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
    exit(1);
  }

  return 0;
}
