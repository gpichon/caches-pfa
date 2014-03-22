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

/**
 * \brief Used to know if a data is stored in the level.    
 * \param node Is a cache of this level.
 * \return If status > I, return 1 if the entry exist in the level with this status,
 * if status == I, return 1 if the entry exist in the level with the same parent, -1 if with another parent,
 * return 0 otherwise. 
 */

int is_in_level(struct node *node,unsigned long entry, enum status status);

/**
 * \brief Used for a miss when load or a hit when store.    
<<<<<<< HEAD
 * \param node Is the cache which launched a share level procedure on its level.
 * \param action Is a transition function for the state machine : a_read or a_modify
 * \return Return 1 if there is entry is somewhere else in level.  
=======
 * \param cache Is the cache which launched a share level procedure on its level.
 * \return Return 1 if there is entry is somewhere else in level with same parent, -1 with another parent.  
>>>>>>> ba95df15ee66fd9496cfe2d1ad67595822a59a1a
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
 * \param cache Cache which must add this line. 
 * \param not_rm value which is to be kept.
 * \note This function call replacement function (beacause the cache may be full). 
 */
void add_line_cache(struct node *cache, unsigned long entry, void (*action)(struct coherenceContext*, struct node*, unsigned long, struct line*), unsigned long not_rm);

#endif
