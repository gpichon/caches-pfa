#include <stdio.h>
#include <stdlib.h>

/*
  Module utilisé pour communiquer avec Hwloc et ainsi définir
  l'architecture se laquelle on se place.  Lie les différents caches
  pour les communications utlérieures.  Les allocations de
  blocs/lignes seront réalisées par les caches eux-mêmes.
*/

/*
  get_depth()
  add_cache(depth, links, size, line size, associativity)
*/
