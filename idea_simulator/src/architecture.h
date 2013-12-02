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

struct architecture parse_archi_file(const char * filename, int is_file_hwloc);
void print_archi(struct architecture * archi);
int convert_archi_xml(const char * file_in, const char * file_out);
void print_caches(struct architecture *archi);
void delete_archi(struct architecture * archi);

#define ARCHI_H
#endif
