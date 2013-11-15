#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

int main(int argc, char *argv[]) {

  /* if (argc != 2) { */
  /*   fprintf(stdout, "Please enter a trace\n"); */
  /*   assert(0); */
  /* } */

  /* FILE *f = fopen(argv[1], "r"); */

  /* char c; */
  /* int r; */
  /* c = fgetc(f); */
  /* while (c!=EOF) { */
  /*   c = fgetc(f); */
  /*   fscanf(f, "%x", &r); */
  /*   c = fgetc(f); */
  /*   fprintf(stdout, "Value: %d\n", r); */
  /* } */


  struct cache *L1;
  L1 = init_cache(8192, 64, 4, 32);

  /* Should be in block 0 */
  fprintf(stdout, "Line:%d Block: %d\n", 62/ARCH, block_id(L1, 62));

  /* Should be in block 3 */
  fprintf(stdout, "Line:%d Block: %d\n", (64*35+25)/ARCH, block_id(L1, 64*35+25));

  /* Should not be in L1 */
  fprintf(stdout, "Is in cache is false: %d\n", is_in_cache(L1, 163));

  add_line_cache(L1, 163);

  /* Should be in L1 */
  fprintf(stdout, "Is in cache is true: %d\n", is_in_cache(L1, 163));
  fprintf(stdout, "Is in cache is true: %d\n", is_in_cache(L1, 158));
  fprintf(stdout, "Is in cache is false: %d\n", is_in_cache(L1, 193));


  /* fclose(f); */
  return EXIT_SUCCESS;
}
