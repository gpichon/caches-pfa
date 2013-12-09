#include "list.h"

/* Data allocation */
struct list* init_list(struct cache *cache) {
  struct list *list = malloc(sizeof(struct list));
  list->cache = cache;
  list->next = NULL;
  return list;
}

/* List removal
   Warning, data is not free!*/
void delete_list(struct list *list) {
  struct list *current = list;
  struct list *del;
  while(current->next != NULL) {
    del = current;
    current = current->next;
    free(del);
  }
  free(current);
}

/* List removal */
void delete_list_def(struct list *list) {
  struct list *current = list;
  struct list *del;
  while(current->next != NULL) {
    delete_cache(current->cache);
    del = current;
    current = current->next;
    free(del);
  }
  delete_cache(current->cache);
  free(current);
}


void add_list(struct list *list, struct cache *cache) {
  struct list *next = init_list(cache);

  struct list *try = list;
  while(try->next != NULL) {
    try = try->next;
  }
  try->next = next;
}

void invalid_back(struct list **caches, struct cache *cache, int entry) {
  struct list *current;
  struct line *line;
  int i;
  for (i=0; i<4; i++){
    current = caches[i];
    while (current->next != NULL) {
      if (current->next->cache == cache && is_in_cache(current->cache, entry)) {
	current->cache->invalid_back++;
	line = line_in_cache(current->cache, entry);
	invalid_line(line);
      }
    current = current->next;
    }
  }
}

int add_line_cache(struct list **caches, struct cache *cache, int entry, int w) {
  int id_block = block_id(cache, entry);
  struct line *line;
  struct line *del_line;

  if (!is_in_cache(cache, entry)) {
    line = malloc(sizeof(struct line));
    line->first_case = entry / ARCH * ARCH;
    line->use = 0;
    if (w) {
      modify_line(line);
    }
    else {
      exclusive_line(line);
    }

    del_line = add_line_block(cache->blocks[id_block], line, cache->replacement);
    if (del_line != NULL) {
      invalid_back(caches, cache, del_line->first_case);
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
