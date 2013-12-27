#ifndef ARCHI_H
#include "list.h"

struct architecture {
  int nb_bits; // architecture 32 or 64 bits
  char name[30]; // for example x86_64
  char CPU_name[100];
  int number_threads;
  int number_levels;
  struct list ** threads; //table of each list : L1(k) -> L2(k) -> ...
  struct list ** levels; //table of each list : L1(k) -> L1(k+1) -> ...
};

/**
 * Sets the architecture parsing the file describing the architecture (xml)
 * @param filename Name of the xml file to parse
 * @param archi The architecture structure storing the result
 * @return EXIT_FAILURE or EXIT_SUCCESS
 */
int parse_archi_file(const char * filename, struct architecture * archi);

/**
 * Displays the architecture information on standard output
 * @param archi The architecture structure to print
 */
void print_archi(struct architecture * archi);

/**
 * Converts the hwloc xml file to the modified xml architecture description used by the program
 * @param file_in Name of the hwloc file
 * @param file_out Name of the file to be created/modified to store the modified xml architecture
 * @return EXIT_FAILURE or EXIT_SUCCESS
 */
int convert_archi_xml(const char * file_in, const char * file_out);

/**
 * Displays the results of the program : misses, hits, ... for each cache
 * @param archi The architecture structure
 */
void print_caches(struct architecture *archi);

/**
 * Frees the structues used in the architecture structure.
 * @param archi The architecture structure to delete
 */
void delete_archi(struct architecture * archi);

#define ARCHI_H
#endif
