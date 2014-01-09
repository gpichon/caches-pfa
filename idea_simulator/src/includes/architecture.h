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

#include "list.h"

/**
 * \struct architecture 
 * \brief Abstract Data Type of the global architecture.
 */
struct architecture {
  int nb_bits; /**< architecture 32 or 64 bits */
  char name[30]; /**< for example "x86_64" */
  char CPU_name[100]; /**< for example "Intel(R) Core(TM) i3-3120M CPU @ 2.50GHz" */
  int number_threads; /**< Range ? */
  int number_levels; /**< Range ? */
  struct list ** threads; /**< Caches linked by level for each thread.*/
  struct list ** levels; /**< Caches linked by thread for each level.*/
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
 */
void print_caches(struct architecture *archi);

/**
 * Frees the structues used in the architecture structure.
 * \param archi The architecture structure to delete
 */
void delete_archi(struct architecture * archi);

#endif