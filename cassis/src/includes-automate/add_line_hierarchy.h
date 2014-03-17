/**
 * \file add_line_hierarchy.h
 * \brief Communication between caches' level -- Header
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */

#ifndef ADD_LINE_HIERARCHY_H
#define ADD_LINE_HIERARCHY_H

#include "node.h"


int is_in_level(struct node *node,unsigned long entry, enum status status);

/**
 * \brief Used for a miss when load or a hit when store.    
 * \param cache Is the cache which launched a share level procedure on its level.
 * \return Return 1 if there is entry is somewhere else in level.  
 */
void share_level(struct node *node, unsigned long entry,void (*action)(struct coherenceContext*, struct node*, unsigned long, struct line*));

/**
 * \brief Loads the entry in the cache and applies coherence protocol. 
 */
void load_line_hierarchy(struct node *cache, unsigned long entry);

/**
 * \brief Stores the entry in the cache and applies coherence protocol.
 */
void store_line_hierarchy(struct node *cache, unsigned long entry);

/**
 * \brief Invalid all line in the caches below, which contain the entry, for inclusive strategy.
 */
void invalid_back(struct node *cache, unsigned long entry);

/**
 * \brief Add a line in the cache.
 * \param w If w = 1, modified line. 
 * \note Call add_line_block.
 */
void add_line_cache(struct node *cache, unsigned long entry);

#endif
