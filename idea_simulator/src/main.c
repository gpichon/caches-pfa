#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stdout, "Please enter a trace\n");
    assert(0);
  }

  FILE *f = fopen(argv[1], "r");

  char c;
  int r;
  c = fgetc(f);
  while (c!=EOF) {
    c = fgetc(f);
    fscanf(f, "%x", &r);
    c = fgetc(f);
    fprintf(stdout, "Value: %d\n", r);
  }

  struct cache *L1;
  L1 = init_cache(4096, 16, 4, 16);

  delete_cache(L1);
  fclose(f);
  return EXIT_SUCCESS;
}
