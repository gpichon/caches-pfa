#ifndef ADD_LINE_HIERARCHY_H
#define ADD_LINE_HIERARCHY_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"
#include "architecture.h"

/*
  Module utilisé pour la gestion d'une ligne mémoire au sein de la hiérarchie de caches
  Gère lecture/écriture de cette ligne dans l'ensemble des caches.
*/

/* Loads the entry in the cache and applies MESI protocol */
void load_line_hierarchy(struct architecture *archi, struct list *cache, int entry);

/* Stores the entry in the cache and applies MESI protocol */
void store_line_hierarchy(struct architecture *archi, struct list *cache, int entry);

#endif
