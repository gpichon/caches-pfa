#include "cache.h"
#include "block.h"

/* Data allocations */
struct cache* init_cache(int size, int linesize, int nb_ways, int nb_blocks) {
  struct cache *cache   = malloc(sizeof(struct cache));
  cache->size           = size;
  cache->linesize       = linesize;
  cache->nb_ways        = nb_ways;
  cache->nb_blocks      = nb_blocks;
  struct block **blocks = init_block(nb_blocks, nb_ways);
  cache->blocks         = blocks;
  cache->misses         = 0;
  cache->hits           = 0;
  cache->writes_back    = 0;
  return cache;
}

/* Data removal */
void delete_cache(struct cache *cache) {
  delete_blocks(cache->blocks, cache->nb_blocks);
  free(cache);
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
  int res=0;

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
#ifndef LFU
    update_line(line);
#endif 
    if (is_valid(line) && (line->first_case == entry / ARCH * ARCH)) {
#ifdef LFU
      update_line(line);
#endif
      res = 1;
    }
  }    
  return res;
}

void write_in_cache(struct cache *cache, int entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(line) && (line->first_case == entry / ARCH * ARCH)) {
      modify_line(line);
      return;
    }
  }    
}

int add_line_cache(struct cache *cache, int entry, int w) {
  int id_block = block_id(cache, entry);
  
  if (!is_in_cache(cache, entry)) {
    struct line *line = malloc(sizeof(struct line));
    line->first_case = entry / ARCH * ARCH;
    line->use = 1;

    if (w) {
      modify_line(line);
    }
    else {
      exclusive_line(line);
    }

    if (add_line_block(cache->blocks[id_block], line)) {
      cache->writes_back++;
    }
    cache->misses++;
    return 0;
  }
  else {
    if (w) {
      write_in_cache(cache, entry);
    }
    cache->hits++;
    return 1;
  }
}

void print_infos(struct cache *cache) {
  printf("misses:%d\n", cache->misses);
  printf("hits:%d\n", cache->hits);
  printf("writes back:%d\n\n", cache->writes_back);
}

struct line *line_in_cache(struct cache *cache, int entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(line) && (line->first_case == entry / ARCH * ARCH)) {
      return line;
    }
  }    
  assert(0);
}
