#include "read_traces.h"

int current_thread = 0;

int next_value(int nb_threads, int *fd, struct value *value) {

  if (nb_threads == 0)
    exit(1);  

  current_thread ++;
  current_thread = current_thread % nb_threads;

  printf("Thread:%d\n", current_thread);

  char c1, c2;
  char *str = malloc(12 * sizeof(char));

  read(fd[current_thread], &c1, 1);
  
  if (c1 == '\0') {
    printf("End file: %d\n", current_thread);
    if (nb_threads == 0)
      return 0;

    int tmp = fd[nb_threads - 1];
    fd[nb_threads - 1] = fd[current_thread];
    fd[current_thread] = tmp;
    next_value(nb_threads - 1, fd, value);
  }

  read(fd[current_thread], str, 7);
  read(fd[current_thread], &c2, 1);

    
  printf("Type: %c, Value: %s\n", c1, str);
  
  free(str);
  next_value(nb_threads, fd, value);
}
