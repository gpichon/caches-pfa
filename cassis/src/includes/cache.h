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

#include <stdbool.h>
#include "block.h"

/* A cache of size size, with nb_blocks blocks.
   Each block contains nb_ways lines and a line size is linesize */

/**
 * \enum cache_type
 * \brief Cache type: inclusive, exclusive, non inclusive like inclusive, non inclusive like exclusive.
 */
enum cache_type{
 Inclusive = 0,
 Exclusive = 1,
 NIIO = 2,
 NIEO = 3
};


/**
 * \struct cache
 * \brief Abstract Data Type for a cache.
 */
struct cache {
  int depth;             /**< Level of the cache. From 1 to 3 in general cases.*/
  int size;              /**< Size of the cache. Unity = byte */
  int linesize;          /**< Size of the line. Unity = byte */
  int nb_ways;           /**< Number of lines in a block. */
  int nb_blocks;         /**< Number of blocks.*/
  struct block **blocks; /**< Tabular of blocks pointer.*/

  int misses[2];       /**< Count of misses for this cache for the specific adresses. */
  int hits[2];         /**< Count of hits for this cache for the specific adresses. */
  int writes_back[2];  /**< Count of write backs for this cache for the specific adresses. */

  int evincted_coherence[2];    /**< Count of data evinctions due to coherence problems. */
  int evincted_capacity[2];     /**< Count of data evinctions due to capacity. */
  int evincted_caches_types[2]; /**< Count of data evinctions due to caches types. */

  int misses_snooping[2]; /**< Count of misses when the data is gotten by snooping. */
  int misses_above[2];    /**< Count of misses when the data is gotten above in the hierachy. */
  int misses_below[2];    /**< Count of misses when the data is gotten below in the hierachy. */

  int broadcast_coherence[2]; /**< Broadcast for coherence. */
  int broadcast_snooping[2];  /**< Broadcast to search data by snooping. */


  enum cache_type type;        /**< Type of cache: inclusive, exclusive, NIIO, NIEO */
  enum cache_coherence policy; /**< Coherence policy : MSI, MESI, MOESI, MESIF, MOSI */
  bool snooping;               /**< Can this cache use snooping to find data? */
  bool directory;              /**< Can this cache use a directory manager to trace its sons data? */
  struct directory *dir;       /**< The directory */

  int (*replacement)(struct block *, int, unsigned long);  /**< Function pointer to replace a line for a special priority in a block. */
  void (*update_line)(struct block *, int, unsigned long); /**< Function pointer to update line stat in a block. */
};



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
 * \brief Print informations about a cache. 
 */
void print_infos(struct cache *cache);

/**
 * \brief Return a pointer to the line which contains the entry in the cache. 
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
