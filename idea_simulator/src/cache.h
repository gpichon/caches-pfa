#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
  Module utilisé pour la gestion d'un cache.
  Gère lecture/écriture au sein du cache.
  Gère la mémoire et les stats relatives à ce cache.
*/

struct cache {
  int size;
  int linesize;
  int nb_ways;
  int nb_blocks;
  struct block **blocks;
};

struct block {
  int nb_ways;
  struct line **lines;
};

struct line {
  int label;
  int first_case;
  int valid;
};

struct cache *init_cache(int, int, int, int);
struct block **init_block(int, int);
struct line** init_line(int);

void delete_cache(struct cache *);
void delete_blocks(struct block **, int);
void delete_lines(struct line **, int);

int get_size(struct cache *);
