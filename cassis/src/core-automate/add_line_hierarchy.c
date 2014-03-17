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
#include "directory_manager.h"
#include "coherence.h"

/** 
 * If status > I, return 1 if the entry exist in the level with this status,
 * if status == I, return 1 if the entry exist in the level,
 * return 0 otherwise.
 */

int is_in_level(struct node *node, unsigned long entry, enum status status){
  struct node *current_node = get_sibling(node);
  struct cache *current_cache;
  struct line *line;
  
  /* Get next cache in level */
  while (current_node != node){
    current_cache = get_cache(current_node);
    if (is_in_cache(current_cache, entry)) {
      line = line_in_cache(current_cache, entry);
      if (status == I && line->status > I)
	return 1;
      if (line->status == status && status > I)
	return 1;
    }
    current_node = get_sibling(current_node);
  }
  return 0;
}

/* Return 1 if there is entry is somewhere else in level
   Used for a miss when load or a hit when store 
   the parameter node is the cache which launched a share level procedure on its level */
void share_level(struct node *node, unsigned long entry,void (*action)(struct coherenceContext*, struct node*, unsigned long, struct line*)) {
    
  struct node *current_node = get_sibling(node);
  struct cache *current_cache;
  struct line *line;

  /* Get next cache in level */
  while (current_node != node){
    current_cache = get_cache(current_node);
    if (is_in_cache(current_cache, entry)) {
      line = line_in_cache(current_cache, entry);
      action(&line->coher->_fsm, current_node, entry, line);
    }
    current_node = get_sibling(current_node);
  }
}

void load_line_hierarchy(struct node *node, unsigned long entry) {
  struct node *current_node = node;
  struct cache *current_cache = get_cache(node);
  struct line *line = NULL;
  int res = 0;
  
  /* While entry is not found and hierarchy not ended */
  while (res == 0 && current_node != NULL) {
    current_cache = get_cache(current_node);

    if (is_in_cache(current_cache, entry)){
      up_stat(current_cache, entry, HIT);

      line = line_in_cache(current_cache, entry);
      coherenceContext_i_read(&line->coher->_fsm, current_node, entry, line);
      share_level(current_node, entry, coherenceContext_a_read);

      /* Exclusive case: invalid line. Impossible for L1, which is always inclusive */
      if (is_cache_exclusive(current_cache)){
	up_stat(current_cache, entry, TYPES_EVINCTION);
	coherenceContext_i_del(&line->coher->_fsm, current_node, entry, line);
	share_level(current_node, entry, &coherenceContext_a_del);
      }
      res = 1;
    }
    
    else {
      up_stat(current_cache, entry, MISS);

      /* Snooping case: get the data from a level cache if possible */
      if (is_snooping(current_cache)){
	up_stat(current_cache, entry, SNOOPING_BROADCAST);
      }

      if (is_in_level(current_node, entry, I) && is_snooping(current_cache)){
	up_stat(current_cache, entry, VALUE_BY_SNOOPING);
	res = 1;
      }
	
      else if (current_cache->directory && search_from_directory(current_cache->dir, entry, node)){
	up_stat(current_cache, entry, VALUE_BELOW);
	res = 1;
      }

      else{
	up_stat(current_cache, entry, VALUE_ABOVE);
      }

      if (is_inclusive_like(current_cache)){
	add_line_cache(current_node, entry);
	line = line_in_cache(current_cache, entry);
	/* link with state machine */
	coherenceContext_i_read(&line->coher->_fsm, current_node, entry, line);
	share_level(current_node, entry, &coherenceContext_a_read);
      }
      
    }
    current_node = get_parent(current_node);
    update_lines(current_cache, entry);
  }    
}

void store_line_hierarchy(struct node *node, unsigned long entry) {
  struct node *current_node = node;
  struct cache *current_cache = get_cache(node); 
  struct line *line = NULL;

  int res = 0;
  while (res == 0 && current_node != NULL) {
    current_cache = get_cache(current_node);

    if (is_in_cache(current_cache, entry)){
      up_stat(current_cache, entry, HIT);
      res = 1;

      /* Exclusive case: invalid line. Impossible for L1, which is always inclusive */
      if(is_cache_exclusive(current_cache)){
	coherenceContext_i_del(&line->coher->_fsm, current_node, entry, line);
	share_level(current_node, entry, &coherenceContext_a_del);
	up_stat(current_cache, entry, TYPES_EVINCTION);
      }
      else{
	line = line_in_cache(current_cache, entry);
	coherenceContext_i_modify(&line->coher->_fsm, current_node, entry, line);
	share_level(current_node, entry, &coherenceContext_a_modify);
	update_lines(current_cache, entry);
      }
    }
    
    else {
      up_stat(current_cache, entry, MISS);

      /* Snooping case: get the data from a level cache if possible */
      if (is_snooping(current_cache)){
	up_stat(current_cache, entry, SNOOPING_BROADCAST);
      }

      if (is_in_level(current_node,entry, I) && is_snooping(current_cache)){
	up_stat(current_cache, entry, VALUE_BY_SNOOPING);
	res = 1;
      }
      
      else if (current_cache->directory && search_from_directory(current_cache->dir, entry, node)){
	up_stat(current_cache, entry, VALUE_BELOW);
	res = 1;
      }
      
      else {
	up_stat(current_cache, entry, VALUE_ABOVE);
      }

      if (is_inclusive_like(current_cache)){	
	add_line_cache(current_node, entry);
	line = line_in_cache(current_cache, entry);
	coherenceContext_i_modify(&line->coher->_fsm, current_node, entry, line);
	share_level(current_node, entry, &coherenceContext_a_modify);
	update_lines(current_cache, entry);
      }
    } 
    current_node = get_parent(current_node);
  }

  while (current_node != NULL) {
    current_cache = get_cache(current_node);
    if (is_in_cache(current_cache, entry)){
      line = line_in_cache(current_cache, entry);
      coherenceContext_i_modify(&line->coher->_fsm, current_node, entry, line);
      share_level(current_node, entry, &coherenceContext_a_modify);
      update_lines(current_cache, entry);
    }
    current_node = get_parent(current_node);
  }    
}

void invalid_back(struct node *node, unsigned long entry) {
  struct node *current_node;
  struct cache *current_cache;
  struct line *line;
  unsigned int i;
  for (i=0; i<node->nb_children; i++){
    current_node = get_child(node, i);
    current_cache = get_cache(current_node);
    if (is_in_cache(current_cache, entry)){
      line = line_in_cache(current_cache, entry);
      coherenceContext_i_del(&line->coher->_fsm, current_node, entry, line);
      share_level(current_node, entry, &coherenceContext_a_del);
      up_stat(current_cache, entry, TYPES_EVINCTION);
    }
    invalid_back(current_node, entry);
  }
}

void add_line_cache(struct node *node, unsigned long entry) {
  struct cache *cache = get_cache(node);
  int id_block = block_id(cache, entry);
  struct line *line;
  struct line *del_line;

  line = malloc(sizeof(struct line));
  assert(line!=NULL);
  line->first_case = entry / cache->linesize * cache->linesize;
  line->use = 0;
  line->priority = 0;
  line->dirty = 0;
  line->status = I;
  struct coherence *policy = malloc(sizeof(struct coherence));
  line->coher = policy;
  coherence_init(policy,cache->policy);
 
  int priority = 0;
  if (cache->directory){
    priority = delete_from_directory(cache->dir, cache->blocks[id_block]);
  }
  del_line = add_line_block(cache->blocks[id_block], line, cache->replacement, priority);

  unsigned long del_data = del_line->first_case;
  if (del_line != NULL) {
    if (is_valid(del_line)){
      up_stat(cache, entry, CAPACITY_EVINCTION);
      
      /* Inclusive case: invalid in lower levels */
      if (is_cache_inclusive(cache))
	invalid_back(node, del_line->first_case);
      
      /* Give the del_line to the parent */
      if (get_parent(node) != NULL){
	struct cache *parent = get_cache(get_parent(node));
	if (!is_in_cache(parent, del_data)){
	  add_line_cache(get_parent(node), del_data);
	  line = line_in_cache(parent, del_data);
	  if (!is_dirty(del_line)) {
	    coherenceContext_i_read(&line->coher->_fsm, get_parent(node), del_data, line);
	    share_level(get_parent(node), del_data, &coherenceContext_a_read);
	  }
	  else {
	    coherenceContext_i_modify(&line->coher->_fsm, get_parent(node), del_data, line);
	    share_level(get_parent(node), del_data, &coherenceContext_a_modify);	    
	  }
	} 
	else {
	  if (is_dirty(del_line)) {
	    line = line_in_cache(parent, del_data);
	    coherenceContext_i_modify(&line->coher->_fsm, get_parent(node), del_data, line);
	    share_level(get_parent(node), del_data, &coherenceContext_a_modify);
	  }
	}	
      }
      share_level(node, del_data, &coherenceContext_a_del);
    }
  }
  free(del_line->coher);
  free(del_line);
}
