#include <stdio.h>
#include <stdlib.h>

// use the flags
//  $ gcc -fopenmp -g -DKERNEL=<adefinir>
// to compile it

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

int main (int argc, char** argv)
{
   int n = 100000;
   if (argc == 2) n = atoi (argv[1]);
   if (n <= 0) n = 10;
   double* tab = malloc (n*sizeof (double));
   KERNEL (tab, n);
   free (tab);
   return(0);
}
