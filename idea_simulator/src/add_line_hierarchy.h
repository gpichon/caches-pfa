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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"
#include "architecture.h"


/**
 * \brief Used for a miss when load or a hit when store.    
 * \param \a cache is the cache which launched a share level procedure on parameter level.
 * \return Return 1 if there is entry is somewhere else in level.  
 */
int share_level(struct list *level, struct cache *cache, int entry, void (*action)(struct line *));

/**
 * \brief Loads the entry in the cache and applies coherence protocol. 
 */
void load_line_hierarchy(struct architecture *archi, struct list *cache, int entry);

/**
 * \brief Stores the entry in the cache and applies coherence protocol.
 */
void store_line_hierarchy(struct architecture *archi, struct list *cache, int entry);

#endif
