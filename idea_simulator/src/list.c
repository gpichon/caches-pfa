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

