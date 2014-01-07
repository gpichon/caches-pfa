/**
 * \file list.h
 * \brief Abstract Data Type : cache linked list and associated initialization, removal, add -- Header
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */


#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

/**
 * \struct list 
 * \brief Linked list of cache.
 * \warning This list structure is actually an element structure. The list is its first element.
 */
struct list {
  struct cache *cache; /**< Pointer to the cache. */ 
  struct list *next; /**< Pointer to the next element. NULL if none.*/
};


/**
 * \brief List/Element initialization with a cache pointer as data.
 */
struct list* init_list(struct cache *cache);

/**
 * \brief List removal without deleting data.
 */
void delete_list(struct list *list);

/**
 * \brief List removal with deleting data.
 */
void delete_list_def(struct list *list);

/**
 * \brief Add a cache at the end of the list. 
 * \param list List must be initialized.
 */
void add_list(struct list *list, struct cache *cache);

/**
 * \brief Invalid all line in all caches, which contain the entry.
 * \bug This function has nothing to do here.
 */
void invalid_back(struct list **caches, int nb_threads, struct cache *cache, int entry);


/**
 * \brief Add a line in the cache.
 * \param w If w = 1, modified line. 
 * \note Call add_line_block.
 * \bug This function has nothing to do here.
 */
int add_line_cache(struct list **caches, int nb_threads, struct cache *cache, int entry, int w);

#endif
