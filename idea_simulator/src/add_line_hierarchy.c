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

void load_line_hierarchy(struct list **caches, int nb_threads, struct list *cache, int entry) {
  struct line *line;
  if (is_in_cache(cache->cache, entry)) {
    line = line_in_cache(cache->cache, entry);
    cache->cache->hits++;
  }
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
	    if (line->writed) {
	      cache_bis->writes_back++;
	      line->writed = 0;
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
