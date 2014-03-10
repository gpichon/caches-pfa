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

/* Return 1 if there is entry is somewhere else in level
   Used for a miss when load or a hit when store 
   the parameter node is the cache which launched a share level procedure on its level */
int share_level(struct node *node, unsigned long entry, void (*action)(struct line *)) {
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
	/* STATS: if action=invalid_line -> evincted_coherence++ */
	up_stat(current_cache, entry, WRITE_BACK);
	res = 1;
      }

      if (current_cache->treat_special_flags(line, action)) {
	res = 1;
      }
      
      if (is_shared(line)) {
	/* STATS: if action=invalid_line -> evincted_coherence++ */
	action(line);
	res = 1;
      }
    }
    current_node = get_sibling(current_node);
  }
  return res;
}

void load_line_hierarchy(struct node *node, unsigned long entry) {
  struct node *current_node = node;
  struct cache *current_cache = get_cache(node);
  struct cache *current_son = get_cache(node);
  struct line *line = NULL;
  int res = 0;
  int v;
  
  /* While entry is not found and hierarchy not ended */
  while (res == 0 && current_node != NULL) {
    current_cache = get_cache(current_node);
    
    if (is_in_cache(current_cache, entry)){
      up_stat(current_cache, entry, HIT);
      /* Exclusive case: invalid line. Impossible for L1, which is always inclusive */
      if (is_cache_exclusive(current_cache)){
	line = line_in_cache(current_cache, entry);
	/* STATS: evincted_cache_type++ */
	invalid_line(line);
      }
      res = 1;
    }
    
    else {
      up_stat(current_cache, entry, MISS);
      v = share_level(current_node, entry, &share_line);

      if (is_inclusive_like(current_cache)){
	add_line_cache(current_node, entry, 0);
	up_stat(current_cache, entry, BROADCAST);
	/* STATS: broadcast_coherence++ */
	line = line_in_cache(current_cache, entry);
	current_cache->set_flags_new_line(v, line);
      }
	
      /* Snooping case: get the data from a level cache if possible */
      if (is_snooping(current_cache) && v){
	/* STATS: misses_snooping++ */
	/* STATS: broadcast_snooping++ !!even if !v!!*/
	res = 1;
      }

      else if (current_cache->directory && search_from_directory(current_cache->dir, entry, current_son)){
	/* STATS: misses_below++ */
	res = 1;
      }

      /* else{ */
      /* STATS: misses_above++ */
      /* } */
      
    }

    update_lines(current_cache, entry);
    current_son = current_cache;
    current_node = get_parent(current_node);
  }    
}

void store_line_hierarchy(struct node *node, unsigned long entry) {
  struct node *current_node = node;
  struct cache *current_cache = get_cache(node); 
  struct cache *current_son = get_cache(node); 
  struct line *line = NULL;

  int res = 0;
  int v;
  while (res == 0 && current_node != NULL) {
    current_cache = get_cache(current_node);

    if (is_in_cache(current_cache, entry)){
      up_stat(current_cache, entry, HIT);
      res = 1;

      line = line_in_cache(current_cache, entry);
      
      /* Exclusive case: invalid line. Impossible for L1, which is always inclusive */
      if(is_cache_exclusive(current_cache)){
	invalid_line(line);
	/* STATS: evincted_cache_type++ */
      }
      else{
	v = share_level(current_node, entry, &invalid_line);
	modify_line(line);
	/* STATS: broadcast_coherence++ */
      }
    }
    
    else{
      up_stat(current_cache, entry, MISS);
      if (is_inclusive_like(current_cache)){	
	add_line_cache(current_node, entry, 1);
	v = share_level(current_node, entry, &invalid_line);
	/* STATS: broadcast_coherence++ */
	line = line_in_cache(current_cache, entry);
	modify_line(line);
	update_lines(current_cache, entry);
      }
      /* Snooping case: get the data from a level cache if possible */
      if (is_snooping(current_cache) && v){
	res = 1;
	/* STATS: broadcast_snooping++ */
	/* STATS: misses_snooping++ */
      }
      else if (current_cache->directory && search_from_directory(current_cache->dir, entry, current_son)){
	res = 1;
	/* STATS: misses_below++ */
      }

      /* else{ */
      /* STATS: misses_above++ */
      /* } */

    }
    
    up_stat(current_cache, entry, BROADCAST);
    current_son = current_cache;
    current_node = get_parent(current_node);
  }
  
  while (current_node != NULL) {
    current_cache = get_cache(current_node);
    if (is_in_cache(current_cache, entry)){
      line = line_in_cache(current_cache, entry);
      modify_line(line);
    }
    
    /* Debug, should be threated by architecture */
    else if (is_cache_inclusive(current_cache)){
      fprintf(stderr, "Erreur de logique, snooping en dessous niveau inclusif...\n");
      exit(1);
    }
    
    share_level(current_node, entry, &invalid_line);
    up_stat(current_cache, entry, BROADCAST);
    /* STATS: broadcast_coherence++ */
    current_node = get_parent(current_node);
    update_lines(current_cache, entry);
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
      //current_cache->invalid_back++;
      line = line_in_cache(current_cache, entry);
      invalid_line(line);
      /* STATS: evincted_cache_type++ */
      invalid_back(current_node, entry);
    }
  }
}

void add_line_cache(struct node *node, unsigned long entry, int w) {
  struct cache *cache = get_cache(node);
  int id_block = block_id(cache, entry);
  struct line *line;
  struct line *del_line;

  line = malloc(sizeof(struct line));
  line->first_case = entry / cache->linesize * cache->linesize;
  line->use = 0;
  line->priority = 0;
  if (w) {
    modify_line(line);
  }
  else {
    exclusive_line(line);
  }

  int priority = 0;
  if (cache->directory){
    priority = delete_from_directory(cache->dir, cache->blocks[id_block]);
  }
  del_line = add_line_block(cache->blocks[id_block], line, cache->replacement, priority);

  int del_data = del_line->first_case;
  if (del_line != NULL) {
    /* STATS: evincted_capacity++ */
    
    /* Inclusive case: invalid in lower levels */
    if (is_cache_inclusive(cache))
      invalid_back(node, del_line->first_case);

    /* Give the del_line to the parent!!! */
    if (get_parent(node) != NULL){
      int r;
      struct cache *parent = get_cache(get_parent(node));
      if (is_in_cache(parent, del_data)){
    	line = line_in_cache(parent, del_data);
    	if (is_modified(del_line)) {
    	  up_stat(cache, entry, WRITE_BACK);
    	  modify_line(line);
	  share_level(get_parent(node), del_data, &invalid_line);
    	  /* STATS: broadcast_coherence++ */
    	}
      }
      else{
    	add_line_cache(get_parent(node), del_data, w);
    	if (is_modified(del_line)) {
	  share_level(get_parent(node), del_data, &invalid_line);
    	}
    	else {
	  r = share_level(get_parent(node), del_data, &share_line);
	  if (r){
	    line = line_in_cache(parent, del_data);
	    exclusive_line(line);
	  }
    	}
    	/* STATS: broadcast_coherence++ */
      }
    }
    free(del_line);
  }
}
