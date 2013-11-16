#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "add_line_hierarchy.h"

int main(int argc, char *argv[]) {

  if (argc != 3) {
    fprintf(stdout, "Please enter a trace and an hierarchy\n");
    assert(0);
  }

  FILE *f1 = fopen(argv[1], "r");
  FILE *f2 = fopen(argv[2], "r");

  struct list **caches = NULL;
  int nb_threads= 0;

  int nb_caches, nb_links, size, linesize, nb_ways, nb_blocks;
  char c = 'a';
  int i, j;

  while (c!=EOF) {
    c = fscanf(f2, "%d", &nb_caches);
    fscanf(f2, "%d", &nb_links);
    fscanf(f2, "%d", &size);
    fscanf(f2, "%d", &linesize);
    fscanf(f2, "%d", &nb_ways);
    fscanf(f2, "%d", &nb_blocks);

    if (c!=EOF) {
      if (caches == NULL) {
	caches = malloc(nb_caches * sizeof(struct list *));
	for (i=0; i<nb_caches; i++) {
	  struct cache *cache;
	  cache = init_cache(size, linesize, nb_ways, nb_blocks);	  
	  nb_threads = nb_caches;
	  caches[i] = init_list(cache);
	}
      }
      
      else {
	int where = 0;
	for (i=0; i<nb_caches; i++) {
	  struct cache *cache;
	  cache = init_cache(size, linesize, nb_ways, nb_blocks);
	  for(j=0; j<nb_links; j++) {
	    add_list(caches[where], cache);
	    where++;
	  }
	}
      }      
      /* fprintf(stdout, "Nb_cache:%d, Links:%d, Size:%d, Linesize:%d, Ways:%d, Blocks:%d\n", nb_caches, nb_links, size, linesize, nb_ways, nb_blocks); */
    }
    c = fgetc(f2);
  }

  for (i=1; i<5; i++) {
    add_line(caches[0], 163+2048*i, 0);
    add_line(caches[1], 163+2048*i, 0);
  }

  for (i=1; i<3; i++) {
    add_line(caches[0], 163+2048*i, 0);
    add_line(caches[1], 163+2048*i, 0);
  }

  for (i=1; i<19; i++) {
    add_line(caches[0], 163+2048*i, 0);
    add_line(caches[1], 163+2048*i, 0);
  }

  add_line(caches[1], 163+2048*2, 0);

  fprintf(stdout, "L1_0:\n");
  print_infos(caches[0]->cache);

  fprintf(stdout, "L1_1:\n");
  print_infos(caches[1]->cache);

  fprintf(stdout, "L2_0:\n");
  print_infos(caches[0]->next->cache);

  fprintf(stdout, "L3:\n");
  print_infos(caches[0]->next->next->cache);

  delete_cache(caches[0]->next->next->cache);
  delete_cache(caches[0]->next->cache);
  delete_cache(caches[2]->next->cache);
  delete_cache(caches[0]->cache);
  delete_cache(caches[1]->cache);
  delete_cache(caches[2]->cache);
  delete_cache(caches[3]->cache);

  free(caches[0]->next->next);
  free(caches[1]->next->next);
  free(caches[2]->next->next);
  free(caches[3]->next->next);
  free(caches[0]->next);
  free(caches[1]->next);
  free(caches[2]->next);
  free(caches[3]->next);
  free(caches[0]);
  free(caches[1]);
  free(caches[2]);
  free(caches[3]);

  free(caches);
  
  fclose(f1);
  fclose(f2);
  return EXIT_SUCCESS;
}
