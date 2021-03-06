/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file directory_manager.h
 * \brief A way for a cache to trace its sons data -- Header
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
 * \brief Represents a directory manager.
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
 * \brief Used to delete all directory managers.
 * \param The root of the hierarchy.
 */
void delete_directories(struct node *root);

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
 * \param The node where the data was initially searched.
 * \return 1 if the entry can be found in the hierarchy below, 0 otherwise.
 */
int search_from_directory(struct directory *dir, unsigned long entry, struct node *node);

#endif
