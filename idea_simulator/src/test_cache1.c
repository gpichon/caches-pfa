#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

int main(int argc, char *argv[]) {

  struct cache *L1;
  L1 = init_cache(8192, 64, 4, 32);

  /* Should be in block 0 */
  fprintf(stdout, "Line:%d Block: %d\n", 62/ARCH, block_id(L1, 62));

  /* Should be in block 3 */
  fprintf(stdout, "Line:%d Block: %d\n", (64*35+25)/ARCH, block_id(L1, 64*35+25));


  add_line_cache(L1, 163, 0);
  fprintf(stdout, "Is in cache is true: %d\n", is_in_cache(L1, 163));
  fprintf(stdout, "Is in cache is true: %d\n", is_in_cache(L1, 158));
  fprintf(stdout, "Is in cache is false: %d\n", is_in_cache(L1, 193));


  int i;
  for (i=1; i<5; i++) {
    add_line_cache(L1, 163+2048*i, 0);
  }

  for (i=1; i<5; i++, 0) {
    fprintf(stdout, "Is in cache is true: %d\n", is_in_cache(L1, 163+2048*i));
  }
  

  add_line_cache(L1, 2230, 0);
  fprintf(stdout, "Is in cache is true: %d\n", is_in_cache(L1, 2230));
  fprintf(stdout, "Is in cache is false: %d\n", is_in_cache(L1, 158));


  delete_cache(L1);
  return EXIT_SUCCESS;
}
