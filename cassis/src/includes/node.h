/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file node.h
 * \brief Defines the node structure -- Header
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

/**
 * \struct node
 * \brief Represents a cache in the hierarchy, contains also this cache.
 */
struct node {
  struct node *parent; /**< Node (cache) above. */
  struct node *next_sibling; /**< Sibling in the same level. Possibly with a different parent.*/
  struct node **child; /**< Child list. */
  unsigned int nb_children; /**< Child number. */
  unsigned int id; /**< Identifier. */
  struct cache *data; /**< Cache object. */
};

/**
 * \brief Allocate and initiate values of a new node
 * \return The new node
 */
struct node *init_node();

/**
 * \brief Get the parent node
 * \param n The current node
 * \return The parent node or NULL for the root
 */
struct node *get_parent(struct node *n);

/**
 * \brief Get the next node at the same level, in a circular way : if you parse all the nodes at the same level, you will end up on the node you started the parsing (be careful to check the id of the node to avoid infinite loops)
 * \param n The current node
 * \return The next sibling
 */
struct node *get_sibling(struct node *n);

/**
 * \brief Get the i-th child of the node
 * \param n The current node
 * \param i The number of the child (0 is the fist child)
 * \return The i-th child or NULL
 */
struct node *get_child(struct node *n, unsigned int i);

/**
 * \brief Get the cache structure
 * \param n The current node
 * \return The cache structure
 */
struct cache *get_cache(struct node *n);

/**
 * \brief Get the root of the tree
 * \param n A node in the tree
 * \return The root of the tree
 */
struct node *get_root(struct node *n);

/**
 * \brief Set the parent node
 * \param child The current node
 * \param parent The parent node
 * \return The parent node
 */
struct node *set_parent(struct node *child, struct node *parent);

/**
 * \brief Set the next sibling
 * \param n The current node
 * \param sibling The next sibling
 * \return The next sibling
 */
struct node *set_next_sibling(struct node *n, struct node *sibling);

/**
 * \brief Add a child to a node
 * \param parent The parent node
 * \param child The child node to add
 * \return 0 in case of error
 */
int add_child(struct node *parent, struct node *child);

/**
 * \brief Set the cache data to the node
 * \param n The current node
 * \param c The cache to set
 * \return The cache structure
 */
struct cache *set_data(struct node *n, struct cache *c);

/**
 * \brief Free the node
 * \param n The node to free
 */
void free_node(struct node *n);

#endif
