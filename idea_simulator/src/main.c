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
  struct cache *L1;
  L1 = init_cache(8192, 64, 4, 32);

  char c1, c2;
  int r;
  c1 = fgetc(f);
  while (c1!=EOF) {
    c2 = fgetc(f);
    fscanf(f, "%x", &r);
    c1 = fgetc(f);

    /* fprintf(stdout, "Value: %d\n Char:%c", r, c2); */
    if (c2 == 'L') {
      add_line_cache(L1, r, 0);
    }
    if (c2 == 'S') {
      add_line_cache(L1, r, 1);
    }
  }


  /* add_line_cache(L1, 163, 0); */

  fprintf(stdout, "Infos cache L1:\n");
  print_infos(L1);

  delete_cache(L1);
  fclose(f);
  return EXIT_SUCCESS;
}
