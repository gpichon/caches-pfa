#ifndef ADD_LINE_HIERARCHY_H
#define ADD_LINE_HIERARCHY_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/*
  Module utilisé pour la gestion d'une ligne mémoire au sein de la hiérarchie de caches
  Gère lecture/écriture de cette ligne dans l'ensemble des caches.
*/

void add_line(struct list *, int, int);

void load_line_hierarchy(struct list **, int, struct list *, int, int);
void store_line_hierarchy(struct list **, int, struct list *, int);

#endif
