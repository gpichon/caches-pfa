#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

/*
  Module de liste pour hiérarchie de cache
*/

struct list {
  struct cache *cache;
  struct list *next;
};


/* Data allocation */
struct list* init_list(struct cache *cache);

/* List removal without delete data */
void delete_list(struct list *list);

/* List removal */
void delete_list_def(struct list *list);

/* Add a cache in the list */
void add_list(struct list *list, struct cache *cache);

#endif
