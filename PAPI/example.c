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
  int size = 50*32000;
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

/*
PAPI_L1_DCM    Yes   Level 1 data cache misses
PAPI_L1_ICM    Yes   Level 1 instruction cache misses
PAPI_L2_DCM    Yes   Level 2 data cache misses
PAPI_L2_ICM    Yes   Level 2 instruction cache misses
PAPI_L1_TCM    Yes   Level 1 cache misses
PAPI_L2_TCM    Yes   Level 2 cache misses
PAPI_L3_TCM    Yes   Level 3 cache misses
PAPI_L1_LDM    Yes   Level 1 load misses
PAPI_L1_STM    Yes   Level 1 store misses
PAPI_L2_STM    Yes   Level 2 store misses
PAPI_LD_INS    Yes   Load instructions
PAPI_SR_INS    Yes   Store instructions
PAPI_L2_DCH    Yes   Level 2 data cache hits
PAPI_L2_DCA    Yes   Level 2 data cache accesses
PAPI_L3_DCA    Yes   Level 3 data cache accesses
PAPI_L2_DCR    Yes   Level 2 data cache reads
PAPI_L3_DCR    Yes   Level 3 data cache reads
PAPI_L2_DCW    Yes   Level 2 data cache writes
PAPI_L3_DCW    Yes   Level 3 data cache writes
PAPI_L2_ICH    Yes   Level 2 instruction cache hits
PAPI_L2_ICA    Yes   Level 2 instruction cache accesses
PAPI_L3_ICA    Yes   Level 3 instruction cache accesses
PAPI_L2_ICR    Yes   Level 2 instruction cache reads
PAPI_L3_ICR    Yes   Level 3 instruction cache reads
PAPI_L2_TCA    Yes   Level 2 total cache accesses
PAPI_L3_TCA    Yes   Level 3 total cache accesses
PAPI_L2_TCR    Yes   Level 2 total cache reads
PAPI_L3_TCR    Yes   Level 3 total cache reads
PAPI_L2_TCW    Yes   Level 2 total cache writes
PAPI_L3_TCW    Yes   Level 3 total cache writes
*/
