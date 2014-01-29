/**
 * \file node.h
 * \brief Defines the node structure
 * \author ~gmarait
 * \version 1.0
 * \date 29th january 2014
 * 
 * 
 *
 */


#ifndef NODE_H
#define NODE_H

#include "cache.h"

struct node {
  struct node * parent;
  struct node * next_sibling;
  struct node ** child;
  int nb_children;
  int id;
  struct cache *;
};

/**
 * \brief Get the parent node
 * \param n The current node
 * \return The parent node or NULL for the root
 */
struct node * get_parent(struct node * n);

/**
 * \brief Get the next node at the same level, in a circular way : if you parse all the nodes at the same level, you will end up on the node you started the parsing (be careful to check the id of the node to avoid infinite loops)
 * \param n The current node
 * \return The next sibling
 */
struct node * get_sibling(struct node * n);

/**
 * \brief Get the cache structure
 * \param n The current node
 * \return The cache structure
 */
struct cache * get_cache(struct node * n);

#endif
