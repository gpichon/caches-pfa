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
  int i;

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
	for (i=0; i<nb_caches; i++) {
	  struct cache *cache;
	  cache = init_cache(size, linesize, nb_ways, nb_blocks);
	  
	  add_list(caches[i], cache);	  
	}
      }      
      /* fprintf(stdout, "Nb_cache:%d, Links:%d, Size:%d, Linesize:%d, Ways:%d, Blocks:%d\n", nb_caches, nb_links, size, linesize, nb_ways, nb_blocks); */
    }
    c = fgetc(f2);
  }

  for (i=1; i<9; i++) {
    add_line(caches[0], 163+2048*i, 0);
    add_line(caches[1], 163+2048*i, 0);
  }

  for (i=1; i<9; i++) {
    add_line(caches[0], 163+2048*i, 0);
    add_line(caches[1], 163+2048*i, 0);
  }

  fprintf(stdout, "Infos cache L1_0:\n");
  print_infos(caches[0]->cache);

  fprintf(stdout, "Infos cache L1_1:\n");
  print_infos(caches[1]->cache);

  fprintf(stdout, "Infos cache L2_0:\n");
  print_infos(caches[0]->next->cache);

  for (i=0; i<nb_threads; i++) {
    delete_list(caches[i]);
  }
  free(caches);
  
  fclose(f1);
  fclose(f2);
  return EXIT_SUCCESS;
}
