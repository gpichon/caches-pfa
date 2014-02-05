/**
 * \file directory_manager.c
 * \brief A way for a cache to trace its sons data.
 * \author ~gpichon
 * \version 1.0
 * \date 5th february 2014
 * 
 * 
 *
 */

#include "directory_manager.h"

struct directory *init_directory(struct node *node){
  (void) node;
  struct directory *dir = malloc(sizeof(struct directory *));
  dir->nb_sons = 0;
  return dir;
}

void delete_directory(struct directory *dir){
  free(dir);
}

void delete_from_directory(struct directory *dir, struct block *block){
  int nb_ways = block->nb_ways;
  int i, j;
  /* At the beginning, all priorities are at 0 */
  for (j=0; j<nb_ways; j++){
    block->lines[j]->priority = 0;
  }

  for (i=0; i<dir->nb_sons; i++){
    for (j=0; j<nb_ways; j++){
	if (is_in_cache(dir->sons_caches[i], block->lines[j]->first_case))
	  block->lines[j]->priority++;
    }
  }
}

struct cache *search_from_directory(struct directory *dir, long entry){
  int nb_sons = dir->nb_sons;
  int i = 0;

  while(!is_in_cache(dir->sons_caches[i], entry) && (i++<nb_sons));
  if (i == nb_sons)
    return NULL;
  else 
    return dir->sons_caches[i];
}
