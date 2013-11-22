#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "read_traces.h"

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Passez au moins une trace en entrée\n");
  }

  int nb_threads = argc-1;
  printf("Nb threads: %d\n", nb_threads);

  int *fd = malloc(nb_threads * sizeof(int));

  int i;
  for (i=0; i<nb_threads; i++) {
    fd[i] = open(argv[i+1], O_RDONLY);
  }

  struct value *value = malloc(sizeof(struct value));

  while(next_value(nb_threads, fd, value));

  for (i=0; i<nb_threads; i++) {
    close(fd[i]);
  }

  free(value);
  free(fd);
  return EXIT_SUCCESS;
}
