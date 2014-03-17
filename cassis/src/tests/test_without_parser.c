#include "test_without_parser.h"

int test_without_parser(int argc, char** argv) {
  (void) argc;
  (void) argv;

  /* Hierarchy */
  /*    L3
	|      |
	L2     L2
	|   |  |   |
	L1  L1 L1  L1 */
  
  struct node **threads = malloc(4 * sizeof(struct node *));

  /* Creation of cache hierarchy */
  int i;
  for (i=0; i<4; i++) {
    struct node *node = init_node();
    struct cache *cache;
  /* Pour version sans automates: MESI -> &coherence_MESI */
    cache = init_cache(8192, 64, 4, 32, 1, &replacement_LFU, MESI, Inclusive, false, false);
    node->data = cache;
    threads[i] = node;
  }

  struct node *node_2_0, *node_2_1, *node_3;
  struct cache *cache_L2_0, *cache_L2_1, *cache_L3;
  cache_L2_0 = init_cache(32768, 64, 8, 64 , 2, &replacement_LFU, MESI, Inclusive, false, false);
  cache_L2_1 = init_cache(32768, 64, 8, 64 , 2, &replacement_LFU, MESI, Inclusive, false, false);
  cache_L3 = init_cache(130172, 64, 16, 128, 3, &replacement_LFU, MESI, Inclusive, false, false);
  node_2_0 = init_node(); node_2_0->data = cache_L2_0;
  node_2_1 = init_node(); node_2_1->data = cache_L2_1;
  node_3   = init_node(); node_3->data   = cache_L3;

  threads[0]->next_sibling = threads[1];
  threads[1]->next_sibling = threads[2];
  threads[2]->next_sibling = threads[3];
  threads[3]->next_sibling = threads[0];
  node_2_0->next_sibling = node_2_1;
  node_2_1->next_sibling = node_2_0;

  threads[0]->parent = node_2_0;
  threads[1]->parent = node_2_0;
  threads[2]->parent = node_2_1;
  threads[3]->parent = node_2_1;
  node_2_0->parent = node_3;
  node_2_1->parent = node_3;

  /* Classics loads */
  load_line_hierarchy(threads[0], 163+2048); /* Miss L1_0, L2_0, L3_0 */
  load_line_hierarchy(threads[1], 163+2048); /* Miss L1_1 Hit L2_0 */
  load_line_hierarchy(threads[2], 163+2048); /* Miss L1_2, L2_1 Hit L3_0 */

  assert(threads[0]->data->misses[0] == 1);
  assert(threads[1]->data->misses[0] == 1);
  assert(threads[2]->data->misses[0] == 1);

  assert(cache_L2_0->misses[0] == 1);
  assert(cache_L2_0->hits[0]   == 1);
  assert(cache_L2_1->misses[0] == 1);

  assert(cache_L3->hits[0]   == 1);
  assert(cache_L3->misses[0] == 1);

  /* Store value in cache -> Hit */
  store_line_hierarchy(threads[0], 163+2048);   /* Hit L1_0 */
  assert(threads[0]->data->hits[0] == 1);

  /* Value in L1_1 was invalidated last store, but value is still in L2_0 */
  load_line_hierarchy(threads[1], 163+2048); /* WB L1_0 Miss L1_1 Hit L2_0*/
  assert(threads[0]->data->writes_back[0] == 1);
  assert(threads[1]->data->misses[0]      == 2);
  assert(cache_L2_0->hits[0]              == 2);


  /* Invalidated caches */
  store_line_hierarchy(threads[2], 163+2048); /* Miss L1_2, L2_1 WB L2_0 Hit L3_0 */
  assert(threads[2]->data->misses[0] == 2);
  assert(cache_L2_1->misses[0]       == 2);
  assert(cache_L2_0->writes_back[0]  == 1);
  assert(cache_L3->hits[0]           == 2);

  for(i=0; i<4; i++){
    delete_cache(threads[i]->data);
    free(threads[i]);
  }

  delete_cache(cache_L2_0);
  delete_cache(cache_L2_1);
  delete_cache(cache_L3);

  free(node_2_0);
  free(node_2_1);
  free(node_3);
  free(threads);

  printf("Tests MESI OK\n");
  return EXIT_SUCCESS;
}
