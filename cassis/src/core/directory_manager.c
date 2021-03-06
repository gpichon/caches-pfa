/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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

/* Get the number of all sons in hierarchy */
int get_nb_sons(struct node *node){
  unsigned int i;
  int res = 0;
  res = node->nb_children;
  for (i=0; i<node->nb_children; i++){
    res += get_nb_sons(get_child(node, i));
  }
  return res;
}

/* Create the caches reference in the directory */
void assign_sons(struct directory *dir, struct node *node, struct cache **caches, int *index){
  unsigned int i;
  for (i=0; i<node->nb_children; i++){
    caches[*index] = get_cache(get_child(node, i));
    (*index)++;
  }
  for (i=0; i<node->nb_children; i++){
    assign_sons(dir, get_child(node, i), caches, index);
  }
}

void init_directories(struct node *root){
  unsigned int i;
  struct cache *cache = get_cache(root);
  cache->dir = init_directory(root);
  for (i=0; i<root->nb_children; i++){
    init_directories(get_child(root, i));
  }
}

void delete_directories(struct node *root){
  unsigned int i;
  if (get_cache(root)->directory){
    delete_directory(get_cache(root)->dir);
  }
  for (i=0; i<root->nb_children; i++){
    delete_directories(get_child(root, i));
  }
}

struct directory *init_directory(struct node *node){
  struct cache *cache = get_cache(node);
  struct directory *dir;
  if (!cache->directory){
    dir = NULL;
    return dir;
  }

  dir = malloc(sizeof(struct directory));
  dir->nb_sons = get_nb_sons(node);
  struct cache **caches = malloc(dir->nb_sons * sizeof(struct cache *));

  int current = 0;
  assign_sons(dir, node, caches, &current);
  dir->sons_caches = caches;
  return dir;
}

void delete_directory(struct directory *dir){
  free(dir->sons_caches);
  free(dir);
}

int delete_from_directory(struct directory *dir, struct block *block){
  int nb_ways = block->nb_ways;
  int min_priority = INT_MAX;
  int i, j;
  /* At the beginning, all priorities are set to 0 */
  for (j=0; j<nb_ways; j++){
    block->lines[j]->priority = 0;
  }

  for (i=0; i<dir->nb_sons; i++){
    for (j=0; j<nb_ways; j++){
      if (is_in_cache(dir->sons_caches[i], block->lines[j]->first_case))
	block->lines[j]->priority++;
    }
  }

  for (j=0; j<nb_ways; j++){
    if (block->lines[j]->priority < min_priority) {
      min_priority = block->lines[j]->priority;
    }
  }
  return min_priority;
}

int from_node(struct node *node, struct cache *cache){
  int nb_children = node->nb_children;
  int i;
  for (i=0; i<nb_children; i++){
    if (get_cache(node->child[i]) == cache){
      return 1;
    }
    from_node(node->child[i], cache);
  }
  return 0;
}

int search_from_directory(struct directory *dir, unsigned long entry, struct node *node){
  int nb_sons = dir->nb_sons;
  int i = 0;

  while(i<nb_sons){
    if (is_in_cache(dir->sons_caches[i], entry)){
      if (from_node(node, dir->sons_caches[i])){
	return 1;
      }
    }
    i++;
  }
  return 0;
}
