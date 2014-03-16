/**
 * \file line.c
 * \brief Common types.
 * \author ~ahonorat
 * \version 1.0
 * \date 16th march 2014
 *
 * 
 *
 */


#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#define NO_COHERENCE_SM_MACRO
#include "coherence_sm.h"

/* fsm */

struct coherence
{
	struct coherenceContext _fsm;
};


/* line */

/**
 * \enum status
 * \brief Status for coherency protocols.
 */ 
enum status{
  I = 0, /**< Invalid */
  S = 1, /**< Shared */
  E = 2, /**< Exclusive */
  M = 3, /**< Modified */
  O = 4, /**< Owned */
  F = 5, /**< Forward */
};

/**
 * \struct line
 * \brief Abstract Data Type for a line.
 */ 
struct line {
  unsigned long first_case; /**< First element of the cache line.*/
  int use;                  /**< number of utilizations since the line is in the block */
  enum status status;       /**< 0 invalid, 1 shared, 2 exclusive, 3 modified */
  int priority;             /**< Priority to delete data. A data line with high priority don't seem to bedeleted */
  int dirty;                /**< 1 if the line is dirty, 0 otherwise */
  struct coherence * coher; /**< type for state machine */
};

/* block */

/**
 * \struct block
 * \brief Abstract Data Type of a block.
 */
struct block {
  int nb_ways;         /**< Number of lines in the block. */
  int linesize;        /**< Line size. */
  struct line **lines; /**< Tabular of line structure pointer. */
};


/* cache */

/* A cache of size size, with nb_blocks blocks.
   Each block contains nb_ways lines and a line size is linesize */

/**
 * \enum stat_type
 * \brief Stat type: 
 */
enum stat_type {
  MISS,
  HIT,
  WRITE_BACK,
  COHERENCE_EVINCTION,
  CAPACITY_EVINCTION,
  TYPES_EVINCTION,
  VALUE_BY_SNOOPING,
  VALUE_ABOVE,
  VALUE_BELOW,
  COHERENCE_BROADCAST,
  SNOOPING_BROADCAST
};

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
 * \enum cache_coherence
 * \brief Cache type: MSI, MESI, MOESI, MESIF, MOSI.
 */
enum cache_coherence {
  MSI = 0,
  MESI = 1,
  MOESI = 2,
  MESIF = 3,
  MOSI = 4
};


/**
 * \struct cache
 * \brief Abstract Data Type for a cache.
 */
struct cache {
  int depth;             /**< Level of the cache. Range ?*/
  int size;              /**< Size of the cache. Unity = byte ? */
  int linesize;          /**< Size of the line. Unity = byte ?*/
  int nb_ways;           /**< Number of lines in a block. */
  int nb_blocks;         /**< Number of blocks. Range ?*/
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


  enum cache_type type;  /**< Type of cache: inclusive, exclusive, NIIO, NIEO */
  enum cache_coherence policy; /**< Coherence policy : MSI, MESI, MOESI, MESIF, MOSI */
  bool snooping;         /**< Can this cache use snooping to find data? */
  bool directory;        /**< Can this cache use a directory manager to trace its sons data? */
  struct directory *dir; /**< The directory */

  int (*replacement)(struct block *, int); /**< Function pointer to replace a line for a special priority in a block. */
  void (*update_line)(struct block *, int, unsigned long); /**< Function pointer to update line stat in a block.  */
};






#endif
