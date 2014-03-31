/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file architecture.h
 * \brief Initiate the XML configuration's file -- Header
 * \author ~gmarait
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */


#ifndef ARCHI_H
#define ARCHI_H

#include "node.h"
#include "directory_manager.h"

/**
 * \struct architecture 
 * \brief Represents the global architecture.
 */
struct architecture {
  unsigned int nb_bits; /**< architecture 32 or 64 bits */
  char name[30]; /**< for example "x86_64" */
  char CPU_name[100]; /**< for example "Intel(R) Core(TM) i3-3120M CPU @ 2.50GHz" */
  unsigned int number_threads; /**< Explicit */
  unsigned int number_levels; /**< Explicit */
  struct node ** threads; /**< Leaves of the cache hierarchy tree (corresponding to L1 cache)*/
  bool warning; /**< True if there is an important warning that could create a bug*/
};

/**
 * \brief Sets the architecture parsing the file describing the architecture (xml)
 * \param filename Name of the xml file to parse.
 * \param archi The architecture structure storing the result.
 * \return EXIT_FAILURE or EXIT_SUCCESS
 */
int parse_archi_file(const char * filename, struct architecture * archi);

/**
 * \brief Displays the architecture information on standard output.
 * \param archi The architecture structure to print.
 */
void print_archi(struct architecture * archi);

/**
 * \brief Converts the hwloc xml file to the modified xml architecture description used by the program
 * \param file_in Name of the hwloc file
 * \param file_out Name of the file to be created/modified to store the modified xml architecture
 * \return EXIT_FAILURE or EXIT_SUCCESS
 */
int convert_archi_xml(const char * file_in, const char * file_out);

/**
 * \brief Displays the results of the program : misses, hits, ... for each cache
 * \param archi The architecture structure
 * \param arch Whether of not to print caches types
 */
void print_caches(struct architecture *archi, int arch);

/**
 * \brief Displays warnings if the architecture seems wrong and can generate errors
 * \param archi The architecture structure
 */
void check_archi(struct architecture * archi);

/**
 * Frees the structues used in the architecture structure.
 * \param archi The architecture structure to delete
 */
void delete_archi(struct architecture * archi);

#endif
