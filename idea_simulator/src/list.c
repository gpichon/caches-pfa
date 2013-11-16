#include "list.h"

/* Data allocation */
struct list* init_list(struct cache *cache) {
  struct list *list = malloc(sizeof(struct list));
  list->cache = cache;
  list->next = NULL;
  return list;
}

/* Data removal */
void delete_list(struct list *list) {
  struct list *current = list;
  while(current->next != NULL) {
    delete_cache(current->cache);
    current = current->next;
  }
  delete_cache(current->cache);
  free(list);
}

void add_list(struct list *list, struct cache *cache) {
  struct list *next = init_list(cache);

  struct list *try = list;
  while(try->next->next != NULL) {
    try = try->next;
  }

  try->next = next;
}

