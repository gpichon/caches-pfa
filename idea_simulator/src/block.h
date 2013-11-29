#ifndef BLOCK_H
#define BLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "line.h"

/*
  Module utilisé pour la gestion d'un block.
  Gère lecture/écriture au sein du block.
*/

/* A block contains nb_ways lines */
struct block {
  int nb_ways;
  struct line **lines;
};

/* Data allocation */
struct block **init_block(int nb_blocks, int nb_ways);

/* Data removal */
void delete_blocks(struct block **blocks, int nb_ways);

/* Returns the number line to replace in the set */
int id_line_to_replace_LFU(struct block *block);
int id_line_to_replace_FIFO(struct block *block);
int id_line_to_replace_LRU(struct block *block);

/* Update replacement flag */
void update_FIFO(struct block *block, int nb_ways, int entry);
void update_LFU(struct block *block, int nb_ways, int entry);
void update_LRU(struct block *block, int nb_ways, int entry);

/* Returns 1 if write back (when deleting a modified line), else 0 */
int add_line_block(struct block *block, struct line *line, int (*coherence)(struct block *));

#endif
