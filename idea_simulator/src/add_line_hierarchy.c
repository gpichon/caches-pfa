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
