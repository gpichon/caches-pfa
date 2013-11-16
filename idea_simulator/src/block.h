#ifndef BLOCK_H
#define BLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "line.h"

/*
  Module utilis� pour la gestion d'un block.
  G�re lecture/�criture au sein du block.
*/

struct block {
  int nb_ways;
  struct line **lines;
};

/* Data allocation */
struct block **init_block(int, int);

/* Data removal */
void delete_blocks(struct block **, int);

/* Returns the number line to replace in the set */
int id_line_to_replace(struct block *);

/* Returns 1 if write back, else 0 */
int add_line_block(struct block *, struct line *);

#endif
