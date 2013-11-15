#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "add_line.h"

#define ARCH 64

/*
  Module utilisé pour la gestion d'un cache.
  Gère lecture/écriture au sein du cache.
  Gère la mémoire et les stats relatives à ce cache.
*/

struct cache {
  int size;
  int linesize; // linesize = ARCH ?
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
  int use;
};

/* Data allocations */
struct cache *init_cache(int, int, int, int);
struct block **init_block(int, int);
struct line** init_line(int);

/* Data removal */
void delete_cache(struct cache *);
void delete_blocks(struct block **, int);
void delete_lines(struct line **, int);

int block_id(struct cache *, int);
int is_in_cache(struct cache *, int);
void add_line_cache(struct cache *, int);

int id_line_to_replace(struct block *);
void add_line_block(struct block *, struct line *);

#endif
