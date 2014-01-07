#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "add_line_hierarchy.h"

int test_without_parser(int argc, char** argv) {

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
  int i;
  for (i=0; i<4; i++) {
    struct cache *cache;
    cache = init_cache(8192, 64, 4, 32, 1, &replacement_LFU, &coherence_MESI);
    caches[i] = init_list(cache);

    if (i == 0)
      levels[0] = init_list(cache);
    else
      add_list(levels[0], cache);
  }

  struct cache *cache_L2_0, *cache_L2_1, *cache_L3;
  cache_L2_0 = init_cache(32768, 64, 8, 64 , 2, &replacement_LFU, &coherence_MESI);
  cache_L2_1 = init_cache(32768, 64, 8, 64 , 2, &replacement_LFU, &coherence_MESI);
  cache_L3 = init_cache(130172, 64, 16, 128, 3, &replacement_LFU, &coherence_MESI);
  
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


  /* Architecture */
  struct architecture *archi;
  archi = malloc(sizeof(struct architecture));
  archi->threads = caches;
  archi->levels = levels;
  archi->number_threads = 4;

  /* Classics loads */
  
  load_line_hierarchy(archi, caches[0], 163+2048); /* Miss L1_0, L2_0, L3_0 */
  load_line_hierarchy(archi, caches[1], 163+2048); /* Miss L1_1 Hit L2_0 */
  load_line_hierarchy(archi, caches[2], 163+2048); /* Miss L1_2, L2_1 Hit L3_0 */

  assert(caches[0]->cache->misses = 1);
  assert(caches[1]->cache->misses = 1);
  assert(caches[2]->cache->misses = 1);

  assert(cache_L2_0->misses = 1);
  assert(cache_L2_0->hits   = 1);
  assert(cache_L2_1->misses = 1);

  assert(cache_L3->hits   = 1);
  assert(cache_L3->misses = 1);

  /* Store value in cache -> Hit */
  store_line_hierarchy(archi, caches[0], 163+2048);   /* Hit L1_0 */
  assert(caches[1]->cache->hits = 1);

  /* Value in L1_1 was invalidated last store, but value is still in L2_0 */
  load_line_hierarchy(archi, caches[1], 163+2048); /* WB L1_0 Miss L1_1 Hit L2_0*/
  assert(caches[0]->cache->writes_back = 1);
  assert(caches[1]->cache->misses      = 2);
  assert(cache_L2_0->hits              = 2);

  /* Invalidated caches */
  store_line_hierarchy(archi, caches[2], 163+2048); /* Miss L1_2, L2_1 WB L2_0 Hit L3_0 */
  assert(caches[2]->cache->misses = 2);
  assert(cache_L2_0->misses       = 2);
  assert(cache_L2_0->writes_back  = 1);
  assert(cache_L3->hits           = 2);

  for (i=0; i<4; i++) {
    delete_list(caches[i]);
  }

  for (i=0; i<3; i++) {
    delete_list_def(levels[i]);
  }

  printf("Tests MESI OK\n");

  free(caches);
  free(levels);
  free(archi);
  return EXIT_SUCCESS;
}
