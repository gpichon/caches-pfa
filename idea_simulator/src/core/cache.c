/**
 * \file cache.c
 * \brief Manage a block : initiate, removal, print data, coherence and replacement policy.
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */

#include "cache.h"

/* Data allocations */
struct cache* init_cache(int size, int linesize, int nb_ways, int nb_blocks, int depth, void (*replace)(struct cache *), void (*coherence)(struct cache *), int type, bool snooping) {
  struct cache *cache   = malloc(sizeof(struct cache));
  cache->size           = size;
  cache->linesize       = linesize;
  cache->nb_ways        = nb_ways;
  cache->nb_blocks      = nb_blocks;
  struct block **blocks = init_block(nb_blocks, nb_ways, linesize);
  cache->blocks         = blocks;
  cache->misses         = 0;
  cache->hits           = 0;
  cache->writes_back    = 0;
  cache->broadcasts     = 0;
  cache->invalid_back   = 0;
  cache->depth          = depth;
  cache->type           = type;
  cache->snooping       = snooping;
  replace(cache);
  coherence(cache);
  return cache;
}

/* Data removal */
void delete_cache(struct cache *cache) {
  delete_blocks(cache->blocks, cache->nb_blocks);
  free(cache);
}

int block_id(struct cache *cache, long entry) {
  entry = entry / cache->linesize; //Line in principal memory

  int id_block = entry % cache->nb_blocks;
  return id_block;
}

int is_in_cache(struct cache *cache, long entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(line) && (line->first_case == entry / cache->linesize * cache->linesize)) {
      return 1;
    }
  }    
  return 0;
}

struct line *line_in_cache(struct cache *cache, long entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(line) && (line->first_case == entry / cache->linesize * cache->linesize)) {
      return line;
    }
  }    
  assert(0);
}

void update_lines(struct cache *cache, long entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;
  cache->update_line(block, nb_ways, entry);
}

void replacement_LFU(struct cache *cache) {
  cache->replacement = id_line_to_replace_LFU;
  cache->update_line = update_LFU;
}

void replacement_FIFO(struct cache *cache) {
  cache->replacement = id_line_to_replace_FIFO;
  cache->update_line = update_FIFO;
}

void replacement_LRU(struct cache *cache) {
  cache->replacement = id_line_to_replace_LRU;
  cache->update_line = update_LRU;
}

void coherence_MESI(struct cache *cache) {
  cache->treat_special_flags = &flags_MESI;
  cache->set_flags_new_line = &flags_new_line_MESI;
}

void coherence_MSI(struct cache *cache) {
  cache->treat_special_flags = &flags_MSI;
  cache->set_flags_new_line = &flags_new_line_MSI;
}

int flags_MESI(struct line *line, void (*action) (struct line*)) {
  if (is_exclusive(line)) {
    action(line);
    return 1;
  }
  return 0;
}

void flags_new_line_MESI(int ret, struct line *line) {
  if (ret) {
    share_line(line);
  }
  else {
    exclusive_line(line);
  }  
}

int flags_MSI(struct line *line, void (*action) (struct line*)) {
  (void) line;
  (void) action;
  return 0;
}

void flags_new_line_MSI(int ret, struct line *line) {
  (void) ret;
  share_line(line);
}


bool is_cache_inclusive(struct cache *cache){
  return (cache->type == Inclusive);
}

bool is_cache_exclusive(struct cache *cache){
  return (cache->type == Exclusive);
}

bool is_inclusive_like(struct cache *cache){
  return (cache->type == Inclusive || cache->type == NIOI);
}

bool is_snooping(struct cache *cache){
  return (cache->snooping);
}
