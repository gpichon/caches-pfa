/**
 * \file add_line_hierarchy.c
 * \brief Communication between caches' level.
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */

#include "add_line_hierarchy.h"

/* Return 1 if there is entry is somewhere else in level
   Used for a miss when load or a hit when store 
   the parameter node is the cache which launched a share level procedure on its level */
int share_level(struct node *node, long entry, void (*action)(struct line *)) {
  struct node *current_node = get_sibling(node);
  struct cache *current_cache;
  struct line *line;

  int res = 0;
  /* Get next cache in level */
  while (current_node != node){
    current_cache = get_cache(current_node);
    if (is_in_cache(current_cache, entry)) {
      line = line_in_cache(current_cache, entry);
	
      if (is_modified(line)) {
	action(line);
	UP_WRITE_BACKS(current_cache);
	return 1;
      }
      if (current_cache->treat_special_flags(line, action)) {
	return 1;
      }
      
      if (is_shared(line)) {
	action(line);
	res = 1;
      }
    }
    current_node = get_sibling(current_node);
  }
  return res;
}

void load_line_hierarchy(struct node *node, long entry) {
  struct node *current_node = node;
  struct cache *current_cache = get_cache(node);

  /* Hit: Ok! */
  if (is_in_cache(current_cache, entry)) {
    update_lines(current_cache, entry);
    UP_HITS(current_cache);
  }

  /* Miss:
     if data is not in any cache: Exclusive
     else Shared, as other values in caches
  */
  else {
    int res = 0;
    int v;
    struct line *line = NULL;
    
    /* While entry is not found and hierarchy not ended */
    while (res == 0 && current_node != NULL) {
      current_cache = get_cache(current_node);
      res = add_line_cache(current_node, entry, 0);
      update_lines(current_cache, entry);
      
      v = share_level(current_node, entry, &share_line);
      UP_BROADCASTS(current_cache);

      /* If line was previously in the cache, keep it as it was! */
      if (res == 0) {
	line = line_in_cache(current_cache, entry);
	current_cache->set_flags_new_line(v, line);
      }

      /* Exclusive case: invalid line. Impossible for L1, which is always inclusive */
      else{
      	if (is_cache_exclusive(current_cache)){
      	  line = line_in_cache(current_cache, entry);
      	  invalid_line(line);
      	}
      /* Snooping case: get the data from a level cache if possible */
      	if (is_snooping(current_cache) && v){
      	  res = 1;
      	}
      }      
            
      current_node = get_parent(current_node);
    }    
  }
}

/* Warning: caches are supposed to be inclusive -> if store L1 then store L2, L3... */
void store_line_hierarchy(struct node *node, long entry) {
  struct node *current_node = node;
  struct cache *current_cache = get_cache(node); 
  struct line *line = NULL;

  /* Hit: Ok! */
  if (is_in_cache(current_cache, entry)) {
    line = line_in_cache(current_cache, entry);
    modify_line(line);
    UP_HITS(current_cache);
    update_lines(current_cache, entry);
    
    while (current_node != NULL) {
      current_cache = get_cache(current_node);
      line = line_in_cache(current_cache, entry);
      modify_line(line);
      share_level(current_node, entry, &invalid_line);

      UP_BROADCASTS(current_cache);
      current_node = get_parent(current_node);
    }    
  }
  
  /* Miss:
     if data is not in any cache: Exclusive
     else Shared, as other values in caches
  */
  else {
    int res = 0;
    int v;
    while (res == 0 && current_node != NULL) {
      current_cache = get_cache(current_node);
      res = add_line_cache(current_node, entry, 1);

      v = share_level(current_node, entry, &invalid_line);
      UP_BROADCASTS(current_cache);
      line = line_in_cache(current_cache, entry);
      modify_line(line);
      update_lines(current_cache, entry);
      current_node = get_parent(current_node);

      /* Exclusive case: invalid line. Impossible for L1, which is always inclusive */
      if (is_cache_exclusive(current_cache)){
	line = line_in_cache(current_cache, entry);
	invalid_line(line);
      }
      
      /* Snooping case: get the data from a level cache if possible */
      if (is_snooping(current_cache) && v){
	res = 1;
      }
    }

    while (current_node != NULL) {
      current_cache = get_cache(current_node);
      line = line_in_cache(current_cache, entry);
      modify_line(line);
      share_level(current_node, entry, &invalid_line);

      UP_BROADCASTS(current_cache);
      current_node = get_parent(current_node);
    }
  }
}

void invalid_back(struct node *node, long entry) {
  struct node *current_node;
  struct cache *current_cache;
  struct line *line;
  unsigned int i;
  for (i=0; i<node->nb_children; i++){
    current_node = get_child(node, i);
    current_cache = get_cache(current_node);
    if (is_in_cache(current_cache, entry)){
      current_cache->invalid_back++;
      line = line_in_cache(current_cache, entry);
      invalid_line(line);
      invalid_back(current_node, entry);
    }
  }
}

int add_line_cache(struct node *node, long entry, int w) {
  struct cache *cache = get_cache(node);
  int id_block = block_id(cache, entry);
  struct line *line;
  struct line *del_line;

  if (!is_in_cache(cache, entry)) {
    line = malloc(sizeof(struct line));
    line->first_case = entry / cache->linesize * cache->linesize;
    line->use = 0;
    if (w) {
      modify_line(line);
    }
    else {
      exclusive_line(line);
    }

    del_line = add_line_block(cache->blocks[id_block], line, cache->replacement);
    if (del_line != NULL) {

      /* Exclusive case: add in higher level */
      struct node *parent = get_parent(node);
      if (parent != NULL){
	struct cache *parent_cache = get_cache(parent);
	if (is_cache_exclusive(parent_cache)){
	  add_line_cache(parent, entry, w);
	  invalid_back(parent, del_line->first_case);
	}
      }

      /* Inclusive case: invalid in lower levels */
      if (is_cache_inclusive(cache))
	invalid_back(node, del_line->first_case);

      if (is_modified(del_line)) {
	cache->writes_back++;
      }
      free(del_line);
    }

    cache->misses++;
    return 0;
  }
  else {
    if (w) {
      line = line_in_cache(cache, entry);
      modify_line(line);
    }
    cache->hits++;
    return 1;
  }
}
