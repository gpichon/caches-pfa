#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "add_line_hierarchy.h"

int main(int argc, char *argv[]) {

  (void) argc;
  (void) argv;

  /* Hierarchy */
  /*    L3
     |      |
     L2     L2
   |   |  |   |
   L1  L1 L1  L1 */

  struct list **caches = NULL;
  struct list **levels = NULL;
  int nb_threads = 4;
  int nb_levels = 3;

  caches = malloc(nb_threads * sizeof(struct list *));
  levels = malloc(nb_levels * sizeof(struct list *));

  /* Creation of cache hierarchy */
  int i, j;
  for (i=0; i<4; i++) {
    struct cache *cache;
    cache = init_cache(8192, 64, 4, 32);
    caches[i] = init_list(cache);

    if (i == 0)
      levels[0] = init_list(cache);
    else
      add_list(levels[0], cache);
  }

  struct cache *cache_L2_0, *cache_L2_1, *cache_L3;
  cache_L2_0 = init_cache(32768, 64, 8, 64);	  
  cache_L2_1 = init_cache(32768, 64, 8, 64);	  
  cache_L3 = init_cache(130172, 64, 16, 128);
  
  for (i=0; i<2; i++) {
    add_list(caches[i], cache_L2_0);
    add_list(caches[i+2], cache_L2_1);
  }
  for (i=0; i<4; i++) {
    add_list(caches[i], cache_L3);
  }

  levels[1] = init_list(cache_L2_0);
  add_list(levels[1], cache_L2_1);
  levels[2] = init_list(cache_L3);


  /* Classics loads */
  
  load_line_hierarchy(levels, caches[0], 163+2048); /* Miss L1_0, L2_0, L3_0 */
  load_line_hierarchy(levels, caches[1], 163+2048); /* Miss L1_1 Hit L2_0 */
  load_line_hierarchy(levels, caches[2], 163+2048); /* Miss L1_2, L2_1 Hit L3_0 */

  /* Store value in cache -> Hit */
  store_line_hierarchy(levels, caches[0], 163+2048);   /* Hit L1_0 */

  /* Value in L1_1 was invalidated last store, but value is still in L2_0 */
  load_line_hierarchy(levels, caches[1], 163+2048); /* WB L1_0 Miss L1_1 Hit L2_0*/

  /* Invalidated caches */
  load_line_hierarchy(levels, caches[2], 163+2048); /* Miss L1_2, L2_1 Hit L3_0 */


  /* Informations about caches */
  struct list *current;
  for (i=0; i<3; i++){
    j = 0;
    current = levels[i];
    while (current != NULL){
      printf("Niveau: %d, Cache: %d\n", i+1, j++);
      print_infos(current->cache);
      current = current->next;
    }
  }

  for (i=0; i<4; i++) {
    delete_list(caches[i]);
  }

  for (i=0; i<3; i++) {
    delete_list_def(levels[i]);
  }

  free(caches);
  free(levels);
  return EXIT_SUCCESS;
}
