#include "add_line_hierarchy.h"

void add_line(struct list *caches, int entry, int w) {
  struct list *current_list = caches;
  struct cache *current_cache;

  int res = 0;
  while (res == 0 && current_list != NULL) {
    current_cache = current_list->cache;
    res = add_line_cache(current_cache, entry, w);
    current_list = current_list->next;
  }
}

/* Return 1 if there is entry is somewhere else in level */
int share_level(struct list *level, struct cache *cache, int entry) {

}



void load_line_hierarchy(struct list **caches, int nb_threads, struct list *cache, int entry, int use_in_store) {
  struct line *line;

  /* Hit: Ok! */
  if (is_in_cache(cache->cache, entry)) {
    line = line_in_cache(cache->cache, entry);
    cache->cache->hits++;
  }

  /* Miss: 
     if data is not in any cache: Exclusive
     else Shared, as other values in caches
  */
  else {
    int i;
    int res = 0;
    struct list *current;
    struct cache *cache_bis;

    for (i=0; i<nb_threads; i++) {
      current = caches[i];

      while (current != NULL) {
      cache_bis = current->cache;

	if (cache_bis != cache->cache) {
	  if (is_in_cache(cache_bis, entry)) {
	    line = line_in_cache(cache_bis, entry);
	    if (line->written && (use_in_store==0)) {
	      cache_bis->writes_back++;
	      line->written = 0;
	    }
	    share_line(line);
	    res ++;
	  }
	}
	current = current->next;
      }

    }
    
    add_line(cache, entry, 0);
    if (res) {
      line = line_in_cache(cache->cache, entry);
      share_line(line);
    }
  }
}


/* Warning: caches are supposed to be inclusive -> if store L1 then store L2, L3... */
void store_line_hierarchy(struct list **caches, int nb_threads, struct list *cache, int entry) {
  struct line *line;
    int i;
    struct list *current;
    struct cache *cache_bis;
    struct line *line_to_modify[128]; //TODO change!
    int tab_id = 0;

  /* Hit:
     if modified line, nothing to do
     if exclusive line, add state written
     if shared line, add state written and invalidate others lines
   */
  if (is_in_cache(cache->cache, entry)) {
    line = line_in_cache(cache->cache, entry);
    modify_line(line);
    cache->cache->hits++;

    if (line->shared) {
      for (i=0; i<nb_threads; i++) {
	current = caches[i];
	
	while (current != NULL) {
	  cache_bis = current->cache;
	  
	  if (cache_bis != cache->cache) {
	    if (is_in_cache(cache_bis, entry)) {
	      line = line_in_cache(cache_bis, entry);
	      invalid_line(line);
	    }
	  }
	  current = current->next;
	}	
      }      
    }
  }
  
  /* Miss:
     if no copy, no problem!
     if modified copy, write back
     if exclusive/shared copy, add written line
  */
  else {
    load_line_hierarchy(caches, nb_threads, cache, entry, 1);

    line = line_in_cache(cache->cache, entry);
    modify_line(line);

    /* Problem: same line not threated once */
    for (i=0; i<nb_threads; i++) {
      current = caches[i];
      
      while (current != NULL) {
	cache_bis = current->cache;
	
	if (cache_bis != cache->cache) {
	  if (is_in_cache(cache_bis, entry)) {
	    line = line_in_cache(cache_bis, entry);

	    if (line->written) {
	      cache_bis->writes_back++;
	      line->valid = 0;
	    }

	    /* Save lines to modify */
	    else {
	      line_to_modify[tab_id] = line;
	      tab_id ++;
	    }
	  }
	}
	current = current->next;
      }
    }
   
    for (i=0; i<tab_id; i++) {
      modify_line(line_to_modify[i]);
    }
  }
}
