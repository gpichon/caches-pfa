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
#include <stdbool.h>
#include <assert.h>

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
  int dirty;                /**< Is the line dirty */
};

/**
 * \brief Initialization of \a nb_ways new lines. 
 * \return Return a tabular of \a nb_ways structure line pointers.
 */
struct line** init_lines(int nb_ways);

/**
 * \brief Data removal.
 */
void delete_lines(struct line **lines, int nb_ways);

/* Setters */
void invalid_line(struct line *line);
void modify_line(struct line *line);
void share_line(struct line *line);
void exclusive_line(struct line *line);
void forward_line(struct line *line);
void owned_line(struct line *line);

void dirty_line(struct line *line, int w);

/* Getters */
bool is_valid(struct line *line);
bool is_exclusive(struct line *line);
bool is_modified(struct line *line);
bool is_shared(struct line *line);

bool is_dirty(struct line *line);

#endif
