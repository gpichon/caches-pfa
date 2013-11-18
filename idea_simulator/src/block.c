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

#ifdef LFU
int id_line_to_replace(struct block *block) {
  int nb_ways = block->nb_ways;
  int use = 1000;
  int id = 0;

  int i;
  for (i=0; i<nb_ways; i++) {
    if (block->lines[i]->use < use) {
      use = block->lines[i]->use;
      id = i;
    }
  }
  return id;
}
#else
int id_line_to_replace(struct block *block) {
  int nb_ways = block->nb_ways;
  int use = 0;
  int id = 0;

  int i;
  for (i=0; i<nb_ways; i++) {
    if (block->lines[i]->use > use) {
      use = block->lines[i]->use;
      id = i;
    }
  }
  return id;
}
#endif

int add_line_block(struct block *block, struct line *line) {
  int id_line = id_line_to_replace(block);
  struct line *del_line = block->lines[id_line];
  block->lines[id_line] = line;
  if (del_line->written) {
    free(del_line);
    return 1;
  }
  free(del_line);
  return 0;
}
