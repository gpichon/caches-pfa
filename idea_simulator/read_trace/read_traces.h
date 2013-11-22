#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Type: L/S */
struct value
{
  int type;
  int data;
};

int next_value(int nb_threads, int *fd, struct value *value);
