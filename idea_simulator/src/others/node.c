/**
 * \file node.c
 * \brief Defines the node structure linking the caches
 * \author ~gmarait
 * \version 1.0
 * \date 29th january 2014
 * 
 * 
 *
 */

#include <string.h>
#include "node.h"

struct node * get_parent(struct node * n){
  return n->parent;
}

struct node * get_sibling(struct node * n){
  return n->next_sibling;
}

struct cache * get_cache(struct node * n){
  return n->cache;
}
