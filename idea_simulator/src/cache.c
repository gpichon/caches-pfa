#include "cache.h"

/* Data allocations */
struct cache* init_cache(int size, int linesize, int nb_ways, int nb_blocks) {
  struct cache *cache   = malloc(sizeof(struct cache));
  cache->size           = size;
  cache->linesize       = linesize;
  cache->nb_ways        = nb_ways;
  cache->nb_blocks      = nb_blocks;
  struct block ** blocks = init_block(nb_blocks, nb_ways);
  cache->blocks = blocks;
  return cache;
}

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

struct line** init_line(int nb_ways) {
  struct line **lines = malloc(nb_ways * sizeof(struct line *));
  int i;
  for (i=0; i<nb_ways; i++) {
    struct line *line = malloc(sizeof(struct line));
    line->label = 0;
    line->first_case = 0;
    line->valid = 0;
    line->use = 0;
    line->writed = 0;
    lines[i] = line;
  }
  return lines;
}

/* Data removal */
void delete_cache(struct cache *cache) {
  delete_blocks(cache->blocks, cache->nb_blocks);
  free(cache);
}

void delete_blocks(struct block **blocks, int nb_blocks) {
  int i;
  for (i=0; i<nb_blocks; i++) {
    delete_lines(blocks[i]->lines, blocks[i]->nb_ways);
    free(blocks[i]);
  }
  free(blocks);
}

void delete_lines(struct line **lines, int nb_ways) {
  int i;
  for (i=0; i<nb_ways; i++) {
    free(lines[i]);
  }
  free(lines);
}

int block_id(struct cache *cache, int entry) {
  entry = entry / ARCH; //Line in principal memory

  int id_block = entry % cache->nb_blocks;
  return id_block;
}

int is_in_cache(struct cache *cache, int entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
#ifdef DEBUG
    printf("Entry:%d, Line in cache:%d\n", entry, line->first_case);
#endif
    if (line->valid && (line->first_case == entry / ARCH * ARCH)) {
      line->use++;
      return 1;
    }
  }    
  return 0;
}

int add_line_cache(struct cache *cache, int entry, int w) {
  int id_block = block_id(cache, entry);
  
  struct line *line = malloc(sizeof(struct line));
  line->label = 0;
  line->first_case = entry / ARCH * ARCH;
  line->valid = 1;
  line->use = 1;
  line->writed = w;

#ifdef DEBUG
  printf("Block:%d, Line:%d\n", id_block, line->first_case);
#endif
  if (!is_in_cache(cache, entry)) {
    add_line_block(cache->blocks[id_block], line);
    /* misses++; */
    return 0;
  }
  else { //gérer le cas du store plus précisement!
    /* hits++; */
    return 1;
  }
}

int add_line_cache_L1(struct cache *cache, int entry, int w) {
  int id_block = block_id(cache, entry);
  
  struct line *line = malloc(sizeof(struct line));
  line->label = 0;
  line->first_case = entry / ARCH * ARCH;
  line->valid = 1;
  line->use = 1;
  line->writed = w;

#ifdef DEBUG
  printf("Block:%d, Line:%d\n", id_block, line->first_case);
#endif
  if (!is_in_cache(cache, entry)) {
    add_line_block(cache->blocks[id_block], line);
    misses++;
    return 0;
  }
  else { //gérer le cas du store plus précisement!
    hits++;
    return 1;
  }
}

int add_line_cache_L2(struct cache *cache, int entry, int w) {
  int id_block = block_id(cache, entry);
  
  struct line *line = malloc(sizeof(struct line));
  line->label = 0;
  line->first_case = entry / ARCH * ARCH;
  line->valid = 1;
  line->use = 1;
  line->writed = w;

#ifdef DEBUG
  printf("Block:%d, Line:%d\n", id_block, line->first_case);
#endif
  if (!is_in_cache(cache, entry)) {
    add_line_block(cache->blocks[id_block], line);
    misses_L2++;
    return 0;
  }
  else { //gérer le cas du store plus précisement!
    hits_L2++;
    return 1;
  }
}

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
#ifdef DEBUG
  printf("Line to replace:%d\n", id);
#endif
  return id;
}

void add_line_block(struct block *block, struct line *line) {
  int id_line = id_line_to_replace(block);
  struct line *del_line = block->lines[id_line];
  if (del_line->writed) {
    writes ++;
  }
  free(del_line);  
  block->lines[id_line] = line;
}

int is_in_higher_cache(struct cache *cache, int entry, int w) {
  int r = is_in_cache(cache, entry);
  if (r) {
    hits_L2 ++;
  }
  else {
    add_line_cache(cache, entry, w);
    misses_L2 ++;
  }
}

void add_line_2caches(struct cache *cache1, struct cache *cache2, int entry, int w) {
  if (!add_line_cache_L1(cache1, entry, w)) {
    add_line_cache_L2(cache2, entry, w);
    /* is_in_higher_cache(cache2, entry, w); */
  }
}

void print_infos() {
  printf("Caches misses:%d\n", misses);
  printf("Caches hits:%d\n", hits);
  printf("Caches writes back:%d\n", writes);
  printf("Caches L2 misses:%d\n", misses_L2);
  printf("Caches L2 hits:%d\n", hits_L2);
  printf("Caches L2 writes back:%d\n", writes_L2);
}
