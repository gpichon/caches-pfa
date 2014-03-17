/**
 * \file line.c
 * \brief Manage line : initialization, removal, return informations.
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
 * \brief Initialization of \a nb_ways new lines. 
 * \return Return a tabular of \a nb_ways structure line pointers.
 */
struct line** init_lines(int nb_ways, enum cache_coherence type);

/**
 * \brief Data removal.
 */
void delete_lines(struct line **lines, int nb_ways);

/* Getters */
bool is_dirty(struct line *line);
bool is_valid(struct line *line);

#endif
