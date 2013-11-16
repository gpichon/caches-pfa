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

  char *s = malloc(3*sizeof(char));
  int size, linesize, nb_ways, nb_blocks;
  char c = 'a';

  struct list *caches = NULL;

  while (c!=EOF) {
    c = fscanf(f2, "%s", s);
    fscanf(f2, "%d", &size);
    fscanf(f2, "%d", &linesize);
    fscanf(f2, "%d", &nb_ways);
    fscanf(f2, "%d", &nb_blocks);

    if (c!=EOF) {
      struct cache *cache;
      cache = init_cache(size, linesize, nb_ways, nb_blocks);
      if (caches == NULL) {
  	caches = init_list(cache);
      }
      else {
  	add_list(caches, cache);
      }
      fprintf(stdout, "Infos cache %s Size:%d, Linesize:%d, Ways:%d, Blocks:%d\n", s, size, linesize, nb_ways, nb_blocks);
    }
    c = fgetc(f2);
  }

  int i;
  for (i=1; i<9; i++) {
    add_line(caches, 163+2048*i, 0);
  }

  for (i=1; i<9; i++) {
    add_line(caches, 163+2048*i, 0);
  }

  fprintf(stdout, "Infos cache L1:\n");
  print_infos(caches->cache);

  fprintf(stdout, "Infos cache L2:\n");
  print_infos(caches->next->cache);

  free(s);
  delete_list(caches);
  fclose(f1);
  fclose(f2);
  return EXIT_SUCCESS;
}
