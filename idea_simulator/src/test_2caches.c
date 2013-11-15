#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

int main(int argc, char *argv[]) {

  struct cache *L1;
  L1 = init_cache(8192, 64, 4, 32);

  struct cache *L2;
  L2 = init_cache(65536, 64, 8, 128);

  int i;
  for (i=1; i<9; i++) {
    add_line_2caches(L1, L2, 163+2048*i, 0);
  }

  for (i=1; i<9; i++) {
    add_line_2caches(L1, L2, 163+2048*i, 0);
  }

  /* for (i=1; i<5; i++, 0) { */
  /*   fprintf(stdout, "Is in cache is true: %d\n", is_in_cache(L1, 163+2048*i)); */
  /* } */

  print_infos();

  delete_cache(L1);
  delete_cache(L2);
  return EXIT_SUCCESS;
}
