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
struct list* init_list(struct cache *);

/* Data removal */
void delete_list(struct list *);

void add_list(struct list *, struct cache *);

#endif
