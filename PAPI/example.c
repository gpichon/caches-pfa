#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#define NUM_EVENTS 4

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

void matmul(const double *A, const double *B,
	    double *C, int m, int n, int p)
{
  int i, j, k;
  for (i = 0; i < m; ++i)
    for (j = 0; j < p; ++j) {
      double sum = 0;
      for (k = 0; k < n; ++k)
	sum += A[i*n + k] * B[k*p + j];
      C[i*p + j] = sum;
    }
}

int main(int argc, char **argv)
{
  const int size = 512000;
  //double a[size][size];
  //double b[size][size];
  //double c[size][size];
  double *A, *B;
  A = malloc(size*sizeof(double));
  B = malloc(size*sizeof(double));

  int event[NUM_EVENTS] = {PAPI_TOT_INS, PAPI_L1_TCM, PAPI_L2_TCM, PAPI_L3_TCM};
  long long values[NUM_EVENTS];

  /* Start counting events */
  if (PAPI_start_counters(event, NUM_EVENTS) != PAPI_OK) {
    fprintf(stderr, "PAPI_start_counters - FAILED\n");
    exit(1);
  }

  //  matmul((double *)a, (double *)b, (double *)c, size, size, size);
  s1113(A, B, 2, size);

  /* Read the counters */
  if (PAPI_read_counters(values, NUM_EVENTS) != PAPI_OK) {
    fprintf(stderr, "PAPI_read_counters - FAILED\n");
    exit(1);
  }

  printf("Total instructions: %lld\n", nb_instructions);
  printf("L1 Cache misses: %lld\n", values[1]);
  printf("L2 Cache misses: %lld\n", values[2]);
  printf("L3 Cache misses: %lld\n", values[3]);

  /* Stop counting events */
  if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK) {
    fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
    exit(1);
  }

  return 0;
}
