#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "block.h"

#define ARCH 64

/*
  Module utilisé pour la gestion d'un cache.
  Gère lecture/écriture au sein du cache.
  Gère la mémoire et les stats relatives à ce cache.
*/

/* A cache of  size size, with nb_blocks blocks.
   Each block contains nb_ways lines and a line size is linesize */
struct cache {
  int size;
  int linesize; // linesize = ARCH ?
  int nb_ways;
  int nb_blocks;
  struct block **blocks;

  int misses;
  int hits;
  int writes_back;
};

/* Data allocations */
struct cache *init_cache(int size, int linesize, int nb_ways, int nb_blocks);

/* Data removal */
void delete_cache(struct cache *cache);

/* Returns in which block the entry should be store */
int block_id(struct cache *cache, int entry);

/* Return whether or not the cache contains the entry */
int is_in_cache(struct cache *cache, int entry);

/* Add a line in the cache
   If w = 1, modified line */
int add_line_cache(struct cache *cache, int entry, int w);

/* Prints infos about a cache */
void print_infos(struct cache *cache);

/* Returns the line which contains the entry in the cache */
struct line *line_in_cache(struct cache *cache, int line);

#endif
