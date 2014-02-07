/**
 * \file directory_manager.h
 * \brief A way for a cache to trace its sons data.
 * \author ~gpichon
 * \version 1.0
 * \date 5th february 2014
 * 
 * 
 *
 */

#ifndef DIRECTORY_MANAGER_H
#define DIRECTORY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "node.h"

/**
 * \struct directory
 * \brief Abstract Data Type for a directory manager.
 */

struct directory {
  int nb_sons; 			/**< Number of sons in the hierarchy */
  struct cache **sons_caches;	/**< References to the caches sons */
};

/**
 * \brief Used to create all directory managers.
 * \param The root of the hierarchy.
 */
void init_directories(struct node *root);

/**
 * \brief Used to create a directory manager.
 * \param The node in the hierarchy.
 */
struct directory *init_directory(struct node *node);

/**
 * \brief Used to delete a directory manager.
 */
void delete_directory(struct directory *dir);

/**
 * \brief Gives priorities to delete data in the parameter block.
 * \param The directory and the block to delete from.
 * \return The minimal priority in the parameter block.
 */
int delete_from_directory(struct directory *dir, struct block *block);

/**
 * \brief Search a data in hierarchy thanks to the directory.
 * \param The directory in the hierarchy.
 * \param The data to search.
 * \return A cache where the entry can be found.
 */
struct cache *search_from_directory(struct directory *dir, long entry);

#endif
