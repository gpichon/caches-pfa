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

  char c1, c2;
  int r;
  c1 = fgetc(f1);
  while (c1!=EOF) {
    c2 = fgetc(f1);
    fscanf(f1, "%x", &r);
    c1 = fgetc(f1);

    if (c2 == 'L') {
      add_line_cache(caches->cache, r, 0);
    }
    if (c2 == 'S') {
      add_line_cache(caches->cache, r, 1);
    }
  }

  fprintf(stdout, "Infos cache L1:\n");
  print_infos(caches->cache);

  free(s);
  delete_list(caches);
  fclose(f1);
  fclose(f2);
  return EXIT_SUCCESS;
}
