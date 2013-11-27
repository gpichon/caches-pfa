#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "block.h"

/*
  Module utilisé pour la gestion d'un cache.
  Gère lecture/écriture au sein du cache.
  Gère la mémoire et les stats relatives à ce cache.
*/


/* A cache of size size, with nb_blocks blocks.
   Each block contains nb_ways lines and a line size is linesize */

struct cache {
  int depth;
  int size;
  int linesize;
  int nb_ways;
  int nb_blocks;
  struct block **blocks;

  int misses;
  int hits;
  int writes_back;
  int broadcasts;

  int (*replacement)(struct block *); /* Replace a line in the block */
  void (*update_line)(struct line *, int); /* Updating lines in respect to replacement protocol */

  int (*flags)(struct line *, void(*)(struct line *)); /* Special flags: E, O */
  void (*flags_new_line)(int, struct line *);	       /* Create a new line: S or E with MESI */
};

/* Data allocations */
struct cache* init_cache(int size, int linesize, int nb_ways, int nb_blocks, int depth, void (*replacement)(struct cache *), void (*coherence)(struct cache *));

/* Data removal */
void delete_cache(struct cache *cache);

/* Returns in which block the entry should be store */
int block_id(struct cache *cache, int entry);

/* Return whether or not the cache contains the entry */
int is_in_cache(struct cache *cache, int entry);

/* Add a line in the cache
   If w = 1, modified line 
   Call add_line_block */
int add_line_cache(struct cache *cache, int entry, int w);

/* Prints infos about a cache */
void print_infos(struct cache *cache);

/* Returns the line which contains the entry in the cache */
struct line *line_in_cache(struct cache *cache, int line);


/* Replacement protocols */
void replacement_LFU(struct cache *cache);
void replacement_FIFO(struct cache *cache);

/* Coherency protocols */
void coherence_MESI(struct cache *cache);
void coherence_MSI(struct cache *cache);

int flags_MESI(struct line *line, void (*action) (struct line*));
void flags_new_line_MESI(int ret, struct line *line);

int flags_MSI(struct line *line, void (*action) (struct line*));
void flags_new_line_MSI(int ret, struct line *line);

#endif
