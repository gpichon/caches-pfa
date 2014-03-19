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
struct block **init_block(int nb_blocks, int nb_ways, int linesize, enum cache_coherence type) {
  struct block **blocks = malloc(nb_blocks * sizeof(struct block *));
  int i;
  for (i=0; i<nb_blocks; i++) {
    struct block *block = malloc(sizeof(struct block));
    block->nb_ways = nb_ways;
    struct line ** lines = init_lines(nb_ways, type);
    block->lines = lines;
    block->linesize = linesize;
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

int id_line_to_replace_LFU(struct block *block, int priority) {
  int nb_ways = block->nb_ways;
  int use = INT_MAX;
  int id = 0;
  int line_use;

  int i;
  for (i=0; i<nb_ways; i++) {
    line_use = block->lines[i]->use;
    if (line_use < use && block->lines[i]->priority==priority) {
      use = line_use;
      id = i;
    }
  }
  return id;
}

int id_line_to_replace_FIFO(struct block *block, int priority) {
  int nb_ways = block->nb_ways;
  int use = 0;
  int id = 0;
  int line_use;

  int i;
  for (i=0; i<nb_ways; i++) {
    line_use = block->lines[i]->use;
    if (line_use > use && block->lines[i]->priority==priority) {
      use = line_use;
      id = i;
    }
    if (line_use == 0) {
      return i;
    }
  }
  return id;
}

int id_line_to_replace_LRU(struct block *block, int priority) {
  int nb_ways = block->nb_ways;
  int use = INT_MAX;
  int id = 0;
  int line_use;

  int i;
  for (i=0; i<nb_ways; i++) {
    line_use = block->lines[i]->use;
    if (line_use < use && block->lines[i]->priority==priority) {
      use = line_use;
      id = i;
    }
    if (line_use == 0) {
      return i;
    }
  }
  return id;
}

struct line *add_line_block(struct block *block, struct line *line, int (*replacement)(struct block *, int), int priority) {
  int id_line = replacement(block, priority);
  int i;
  for (i=0; i<block->nb_ways; i++) {
    if (!is_valid(block->lines[i])){
      id_line = i;
      break;
    }
  }
  struct line *del_line = block->lines[id_line];
  block->lines[id_line] = line;
  return del_line;
}


void update_FIFO(struct block *block, int nb_ways, unsigned long entry) {
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

void update_LFU(struct block *block, int nb_ways, unsigned long entry) {
  int i;
  struct line *line;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(line) && (line->first_case == entry / block->linesize * block->linesize)) {
      line->use++;
    }
  }
}  

void update_LRU(struct block *block, int nb_ways, unsigned long entry) {
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
    if (is_valid(line) && (line->first_case == entry / block->linesize * block->linesize)) {
      line->use++;
    }
  } 
}
