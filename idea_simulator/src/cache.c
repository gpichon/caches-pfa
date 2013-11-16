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
  cache->writes         = 0;
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

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
#ifdef DEBUG
    printf("Entry:%d, Line in cache:%d\n", entry, line->first_case);
#endif
    if (line->valid && (line->first_case == entry / ARCH * ARCH)) {
      update_line(line);
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
    if (add_line_block(cache->blocks[id_block], line)) {
      cache->writes++;
    }
    cache->misses++;
    return 0;
  }
  else {
    cache->hits++;
    return 1;
  }
}

int is_in_higher_cache(struct cache *cache, int entry, int w) {
  int r = is_in_cache(cache, entry);
  if (r) {
    cache->hits ++;
    return 1;
  }
  else {
    if (add_line_cache(cache, entry, w)) {
      cache->writes++;
    }
    cache->misses ++;
    return 0;
  }
}

void add_line_2caches(struct cache *cache1, struct cache *cache2, int entry, int w) {
  if (!add_line_cache(cache1, entry, w)) {
    add_line_cache(cache2, entry, w);
    /* is_in_higher_cache(cache2, entry, w); */
  }
}

void print_infos(struct cache *cache) {
  printf("Caches misses:%d\n", cache->misses);
  printf("Caches hits:%d\n", cache->hits);
  printf("Caches writes back:%d\n", cache->writes);
}
