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
#include "block.h"


/* A cache of size size, with nb_blocks blocks.
   Each block contains nb_ways lines and a line size is linesize */

/**
 * \def UP_HITS(cache)
 * \param cache Container of the data.
 * \brief Count one hit in cache stats.
 */
#define UP_HITS(cache) cache->hits++
/**
 * \def UP_MISSES(cache)
 * \param cache Container of the data.
 * \brief Count one miss in cache stats.
 */
#define UP_MISSES(cache) cache->misses++
/**
 * \def UP_WRITE_BACKS(cache)
 * \param cache Container of the data.
 * \brief Count one write back in cache stats.
 */
#define UP_WRITE_BACKS(cache) cache->writes_back++
/**
 * \def UP_BROADCASTS(cache)
 * \param cache Container of the data.
 * \brief Count one broadcast in cache stats.
 */
#define UP_BROADCASTS(cache) cache->broadcasts++


/**
 * \struct cache
 * \brief Abstract Data Type for a cache.
 */
struct cache {
  int depth; /**< Level of the cache. Range ?*/
  int size; /**< Size of the cache. Unity = byte ? */
  int linesize; /**< Size of the line. Unity = byte ?*/
  int nb_ways; /**< Number of lines in a block. */
  int nb_blocks; /**< Number of blocks. Range ?*/
  struct block **blocks; /**< Tabular of blocks pointer.*/

  int misses; /**< Count of misses for this cache. */
  int hits; /**< Count of hits for this cache. */
  int writes_back; /**< Count of write backs for this cache. */
  int broadcasts; /**< Count of broadcasts for this cache. */
  int invalid_back; /**< Count of invalid back for this cache. */

  int (*replacement)(struct block *); /**< Function pointer to replace a line in a block. */
  void (*update_line)(struct block *, int, int); /**< Function pointer to update line stat in a block.  */

  int (*treat_special_flags)(struct line *, void(*)(struct line *)); /**< Function pointer to manage special flags: E, O. */
  void (*set_flags_new_line)(int, struct line *); /**< Function pointer to create a new line with right flag : S or E with MESI. */
};

/**
 * \brief Cache initialization. 
 */
struct cache* init_cache(int size, int linesize, int nb_ways, int nb_blocks, int depth, void (*replacement)(struct cache *), void (*coherence)(struct cache *));

/**
 * \brief Cache removal.
 */
void delete_cache(struct cache *cache);

/**
 * \brief Return in which block the entry has to be store.
 * \param entry Range ? 
*/
int block_id(struct cache *cache, int entry);

/**
 * \brief Return whether or not the cache contains the entry. 
 * \return It is 1 when true and 0 otherwise.
 * \bug This function check all the ways, it should return true immediatly when the entry is found.
 */
int is_in_cache(struct cache *cache, int entry);


/**
 * \brief Prints informations about a cache. 
 */
void print_infos(struct cache *cache);

/**
 * \brief  Returns a pointer to the line which contains the entry in the cache. 
 */
struct line *line_in_cache(struct cache *cache, int line);

/**
 * \brief Call the update_line function on the right line determined with the entry.
 */
void update_lines(struct cache *cache, int entry);

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
 * \brief Set the MESI coherence policy for the cache.
 */
void coherence_MESI(struct cache *cache);
/**
 * \brief Set the MSI coherence policy for the cache.
 */
void coherence_MSI(struct cache *cache);

/**
 * \brief Call the given function if the flag allow it.
 * \return True (=1) if the given function has be called, 0 otherwise. 
 */
int flags_MESI(struct line *line, void (*action) (struct line*));
/**
 * \brief Initiate the right flag for the given line. 
 * \param ret What is this ?
 */
void flags_new_line_MESI(int ret, struct line *line);

/**
 * \brief Call the right function given if the flag allow it.
 * \return True (=1) if the given function has be called, 0 otherwise.
 */
int flags_MSI(struct line *line, void (*action) (struct line*));
/**
 * \brief Initiate the right flag for the given line. 
 * \param ret What is this ?
 */
void flags_new_line_MSI(int ret, struct line *line);

#endif
