/**
 * \file line.h
 * \brief Manage line : initialization, removal, return informations -- Header
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */


#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common_types.h"
#include "coherence.h"



/**
 * \struct line
 * \brief Represents a cache line.
 */ 
struct line {
  unsigned long first_case; /**< First element of the cache line.*/
  int use;                  /**< number of utilizations since the line is in the block */
  enum status status;       /**< 0 invalid, 1 shared, 2 exclusive, 3 modified */
  int priority;             /**< Priority to delete data. A data line with high priority is to be kept. */
  int dirty;                /**< 1 if the line is dirty, 0 otherwise */
  struct coherence * coher; /**< type for state machine */
};


/**
 * \brief Initialization of \a nb_ways new lines. 
 * \return Return a tabular of \a nb_ways structure line pointers.
 */
struct line** init_lines(int nb_ways, enum cache_coherence type);

/**
 * \brief Data removal.
 */
void delete_lines(struct line **lines, int nb_ways);

/* Getters */

/**
 * \brief Check if the line is dirty.
 * \return 1 if drity, 0 otherwise.
 */
bool is_dirty(struct line *line);

/**
 * \brief Check if the line is valid i.e. not invalid.
 * \return 1 if not invalid, 0 otherwise.
 */
bool is_valid(struct line *line);

#endif
