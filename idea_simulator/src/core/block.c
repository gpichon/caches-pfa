/**
 * \file block.c
 * \brief Manage a block : initialization, removal, replacement policy.
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */

#include "block.h"

/* Data allocations */
struct block **init_block(int nb_blocks, int nb_ways) {
  struct block **blocks = malloc(nb_blocks * sizeof(struct block *));
  int i;
  for (i=0; i<nb_blocks; i++) {
    struct block *block = malloc(sizeof(struct block));
    block->nb_ways = nb_ways;
    struct line ** lines = init_line(nb_ways);
    block->lines = lines;  
    blocks[i] = block;
  }
  return blocks;
}

/* Data removal */
void delete_blocks(struct block **blocks, int nb_blocks) {
  int i;
  for (i=0; i<nb_blocks; i++) {
    delete_lines(blocks[i]->lines, blocks[i]->nb_ways);
    free(blocks[i]);
  }
  free(blocks);
}

int id_line_to_replace_LFU(struct block *block) {
  int nb_ways = block->nb_ways;
  int use = 1000;
  int id = 0;
  int line_use;

  int i;
  for (i=0; i<nb_ways; i++) {
    line_use = block->lines[i]->use;
    if (line_use < use) {
      use = line_use;
      id = i;
    }
  }
  return id;
}

int id_line_to_replace_FIFO(struct block *block) {
  int nb_ways = block->nb_ways;
  int use = 0;
  int id = 0;
  int line_use;

  int i;
  for (i=0; i<nb_ways; i++) {
    line_use = block->lines[i]->use;
    if (line_use > use) {
      use = line_use;
      id = i;
    }
    if (line_use == 0) {
      return i;
    }
  }
  return id;
}

int id_line_to_replace_LRU(struct block *block) {
  int nb_ways = block->nb_ways;
  int use = 1000;
  int id = 0;
  int line_use;

  int i;
  for (i=0; i<nb_ways; i++) {
    line_use = block->lines[i]->use;
    if (line_use < use) {
      use = line_use;
      id = i;
    }
    if (line_use == 0) {
      return i;
    }
  }
  return id;
}

struct line *add_line_block(struct block *block, struct line *line, int (*replacement)(struct block *)) {
  int id_line = replacement(block);
  struct line *del_line = block->lines[id_line];
  block->lines[id_line] = line;

  if (del_line == NULL)
    return NULL;
  return del_line;
}


void update_FIFO(struct block *block, int nb_ways, int entry) {
  (void) entry;
  int i;
  struct line *line;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(block->lines[i])) {
      line->use++;;
    }
  }
}

void update_LFU(struct block *block, int nb_ways, int entry) {
  int i;
  struct line *line;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(line) && (line->first_case == entry / ARCH * ARCH)) {
      line->use++;
    }
  }
}  

void update_LRU(struct block *block, int nb_ways, int entry) {
  (void) entry;
  int i;
  int max = 0;
  struct line *line;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(block->lines[i])) {
      if (line->use > max) {
	max = line->use;
      }
    }
  }
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(line) && (line->first_case == entry / ARCH * ARCH)) {
      line->use++;
    }
  } 
}
