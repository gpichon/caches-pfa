#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#define NUM_EVENTS 8


/* Tous les threads accedent aux donnees du premier */
void broadcast (double* tab, int n)
{
   int i;
   for (i=0; i<n; i++) 
     tab[i]=1;

#pragma omp parallel for
   for (i=0; i<n; i++) {
	tab[i]=tab[i]*2;
   }
   return;
}
/* Pas de probleme dans cette boucle, tous les threads accedent a des donnees differentes */
void par(double *tab,int n)
{
  int i;
#pragma omp parallel for
  for (i=0; i<n; i++)
    tab[i]=1;

#pragma omp parallel for
  for (i=0; i<n; i++) {
    tab[i]=tab[i]*2;
  }
  return;
}
/* Les threads se passent des donnees entre eux */
void pipeline(double *tab,int n)
{
	int i;
  #pragma omp parallel for
  for (i=0; i<n/2; i++) {
    tab[i]=1;
  }
  #pragma omp parallel for
  for (i=0; i<n/2; i++) {
    int k=i+n/10;
    if (k>=n/2) k=0;
    tab[i+n/2] = tab[k] ; 
  }
}
/* False sharing */
void falsesharing(double *tab,int n) {
int i;
#pragma omp parallel for schedule(static, 1)
  for (i=0; i<n; i++) {
    tab[i] = tab[i] + 2;
  }
}

/* simple test */
void s1113(double *A, double *B, int size){
  int i;
  for(i = 0; i < size; i++){
    A[i] = A[size/2] + B[i];
  }
}

void print(long long *values){
   printf("PAPI_L1_DCM: %lld\n", values[0]);
   printf("PAPI_L1_TCM: %lld\n", values[1]);
   printf("PAPI_L1_DCH: %lld\n", values[2]);
   printf("PAPI_L1_ICH: %lld\n", values[3]);
   printf("PAPI_L2_DCM: %lld\n", values[4]);
   printf("PAPI_L2_TCM: %lld\n", values[5]);
   printf("PAPI_L2_ICH: %lld\n", values[6]);
   printf("PAPI_L2_TCH: %lld\n", values[7]);
}


int main(int argc, char **argv){
  int i,k, size = 128000;
  int moy = 100;
  double *A, *B;
  A = malloc(size*sizeof(double));
  B = malloc(size*sizeof(double));

  int event[NUM_EVENTS] = {PAPI_L2_DCM, PAPI_L1_TCM, PAPI_L1_DCH, PAPI_L1_ICH, PAPI_L2_DCM, PAPI_L2_TCM, PAPI_L2_ICH, PAPI_L2_TCH};
  long long values_1[NUM_EVENTS];
  long long values_2[NUM_EVENTS];
  long long values_3[NUM_EVENTS];
  long long values_4[NUM_EVENTS];
  long long values_5[NUM_EVENTS];
  long long j = 0;

  for(i = 0; i<NUM_EVENTS; i++){
    values_1[i] = 0;
    values_2[i] = 0;
    values_3[i] = 0;
    values_4[i] = 0;
    values_5[i] = 0;
  }


  printf("Tests with %d values.\n", size);
  printf("=====================\n");

  for(k = 0; k<moy; k++){
    for(i = 0; i<NUM_EVENTS;i++){
      /* Start counting events */
      if (PAPI_start_counters(event+i, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_start_counters - FAILED\n");
	exit(1);
      }   
      s1113(A, B, size);
      /* Read the counters */
      if (PAPI_read_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_read_counters - FAILED\n");
	exit(1);
      }
      values_1[i] += j;
      /* Stop counting events */
      if (PAPI_stop_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
	exit(1);
      }
    }
   
    for(i = 0; i<NUM_EVENTS;i++){
      /* Start counting events */
      if (PAPI_start_counters(event+i, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_start_counters - FAILED\n");
	exit(1);
      }   
      falsesharing(A, size);
      /* Read the counters */
      if (PAPI_read_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_read_counters - FAILED\n");
	exit(1);
      }
      values_2[i] += j;
      /* Stop counting events */
      if (PAPI_stop_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
	exit(1);
      }
    }

    for(i = 0; i<NUM_EVENTS;i++){
      /* Start counting events */
      if (PAPI_start_counters(event+i, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_start_counters - FAILED\n");
	exit(1);
      }   
      pipeline(B, size);
      /* Read the counters */
      if (PAPI_read_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_read_counters - FAILED\n");
	exit(1);
      }
      values_3[i] += j;
      /* Stop counting events */
      if (PAPI_stop_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
	exit(1);
      }
    }

    for(i = 0; i<NUM_EVENTS;i++){
      /* Start counting events */
      if (PAPI_start_counters(event+i, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_start_counters - FAILED\n");
	exit(1);
      }   
      par(A, size);
      /* Read the counters */
      if (PAPI_read_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_read_counters - FAILED\n");
	exit(1);
      }
      values_4[i] += j;
      /* Stop counting events */
      if (PAPI_stop_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
	exit(1);
      }
    }

    for(i = 0; i<NUM_EVENTS;i++){
      /* Start counting events */
      if (PAPI_start_counters(event+i, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_start_counters - FAILED\n");
	exit(1);
      }   
      broadcast(B,size);
      /* Read the counters */
      if (PAPI_read_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_read_counters - FAILED\n");
	exit(1);
      }
      values_5[i] += j;
      /* Stop counting events */
      if (PAPI_stop_counters(&j, 1) != PAPI_OK) {
	fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
	exit(1);
      }
    }
  }

  for(i = 0; i<NUM_EVENTS; i++){
    values_1[i] /= moy;
    values_2[i] /= moy;
    values_3[i] /= moy;
    values_4[i] /= moy;
    values_5[i] /= moy;
  }
  printf("-----------\n");
  printf("Test s1133:\n");
  printf("-----------\n");
  print(values_1);
  printf("------------------\n");
  printf("Test falsesharing:\n");
  printf("------------------\n");
  print(values_2);
  printf("--------------\n");
  printf("Test pipeline:\n");
  printf("--------------\n");
  print(values_3);
  printf("---------\n");
  printf("Test par:\n");
  printf("---------\n");
  print(values_4);
  printf("---------------\n");
  printf("Test broadcast:\n");
  printf("---------------\n");
  print(values_5);

  return 0;
}
