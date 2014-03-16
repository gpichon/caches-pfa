/**
 * \file cache.h
 * \brief Manage a block : initiate, removal, print data, coherence and replacement policy -- Header
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */


#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "block.h"

/* A cache of size size, with nb_blocks blocks.
   Each block contains nb_ways lines and a line size is linesize */

/**
 * \brief update cache stats
 */
void up_stat(struct cache *cache, unsigned long entry, int stats_type);

/**
 * \brief Cache initialization. 
 */
struct cache* init_cache(int size, int linesize, int nb_ways, int nb_blocks, int depth, void (*replacement)(struct cache *), enum cache_coherence policy, int type, bool snooping, bool manager);

/**
 * \brief Cache removal.
 */
void delete_cache(struct cache *cache);

/**
 * \brief Return in which block the entry has to be store.
 * \param entry Range ? 
*/
int block_id(struct cache *cache, unsigned long entry);

/**
 * \brief Return whether or not the cache contains the entry. 
 * \return It is 1 when true and 0 otherwise.
 */
int is_in_cache(struct cache *cache, unsigned long entry);


/**
 * \brief Prints informations about a cache. 
 */
void print_infos(struct cache *cache);

/**
 * \brief  Returns a pointer to the line which contains the entry in the cache. 
 */
struct line *line_in_cache(struct cache *cache, unsigned long entry);

/**
 * \brief Call the update_line function on the right line determined with the entry.
 */
void update_lines(struct cache *cache, unsigned long entry);

/* Replacement protocols */
/**
 * \brief Set the Least Frequently Used replacement policy for the cache.
 */
void replacement_LFU(struct cache *cache);
/**
 * \brief Set the First In First Out replacement policy for the cache.
 */
void replacement_FIFO(struct cache *cache);
/**
 * \brief Set the Least Recently Used replacement policy for the cache.
 */
void replacement_LRU(struct cache *cache);

/* Coherency protocols */

/**
 * \brief Return whether or not a cache is inclusive.
 */
bool is_cache_inclusive(struct cache *cache);

/**
 * \brief Return whether or not a cache is exclusive.
 */
bool is_cache_exclusive(struct cache *cache);

/**
 * \brief Return whether or not a cache add the data
 */
bool is_inclusive_like(struct cache *cache);

/**
 * \brief Return whether or not there is snooping on the cache's level.
 */
bool is_snooping(struct cache *cache);

#endif
