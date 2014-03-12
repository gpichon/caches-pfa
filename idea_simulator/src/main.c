/**
 * \file main.c
 * \brief Run the simulator with the architecture and traces given.
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "add_line_hierarchy.h"
#include "architecture.h"
#include "trace.h"

/**
 * \def DEFAULT_ARCHITECTURE
 * \brief Path to the automatically generated XML configuration file, if none specified.
 */
#define DEFAULT_ARCHITECTURE "data/architest_archi.xml"

#include "option.h"

int main(int argc, char *argv[]) {

  get_options(argc, argv);
  
  if (help){
    printf("To work with the architecture file named arch: -f file\n");
    printf("To trace a special data set: -b 0x000000000000:0x6fffffffffff for all values excepting stack values\n");
    printf("To print more statistics: -v 1: basic stats\n");
    printf("                          -v 2: evinctions\n");
    printf("                          -v 3: more detailled misses\n");
    printf("                          -v 4: all stats\n");
    printf("To ignore the architecture fatal warnings; -w\n");
    printf("To process 10 instruction on a thread before moving to the next thread: -i 10\n");
    return EXIT_SUCCESS;
  }
  
  char filename[256];
  if(trace_file){
    strcpy(filename, trace_file);
  }
  else {
    strcpy(filename, DEFAULT_ARCHITECTURE);
  }

  struct architecture A;
  parse_archi_file(filename, &A);
  struct architecture *archi = &A;
  if(archi->warning && (!ignore_warning)){
    delete_archi(archi);
    return EXIT_FAILURE;
  }

  struct thread **threads;
  threads = malloc(4*sizeof(struct thread *));
  struct instruction *ins = malloc(sizeof(struct instruction));
  ins->type = INSTRUCTION_LOAD;
  
  /* Open trace file */
  char **trace_files = malloc(4*sizeof(char *));
  trace_files[0] = "MAQAO/trace0";
  trace_files[1] = "MAQAO/trace1";
  trace_files[2] = "MAQAO/trace2";
  trace_files[3] = "MAQAO/trace3";

  create_threads(threads, trace_files, 4);
  
  /* Read trace */
  int count = 0;
  int j;
  int current = 3;
  int end[4] = {0};

  while (!end[0] || !end[1] || !end[2] || !end[3]){
    current = (current+1)%4; 	/* Next thread */

    for (j=0; j<nb_instr_thread; j++){
      next_instruction(ins, threads, current);

      if (ins->type != INSTRUCTION_END_OF_THREAD) {
  	if (ins->type == INSTRUCTION_LOAD) {
  	  load_line_hierarchy(archi->threads[current], ins->addr);
  	  count++;
  	}
  	else if (ins->type == INSTRUCTION_STORE) {
  	  store_line_hierarchy(archi->threads[current], ins->addr);
  	  count++;
  	}
      }
      else{
  	end[current] = 1;
      }
    }
  }

  /* End of reading */
  /* printf("Total values: %d\n", count); */
  destroy_threads(threads, 4);

  /* Informations about caches */
  print_caches(archi);
  delete_archi(archi);

  free(ins);
  free(threads);
  free(trace_files);
  return EXIT_SUCCESS;
}
