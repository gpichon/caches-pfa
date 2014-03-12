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
#define DEFAULT_ARCHITECTURE "architecture/architest_archi.xml"

#include "option.h"

int is_end(int *ends, int nb_threads){
  int i;
  for(i=0; i<nb_threads; i++){
    if (ends[i] == 0)
      return 0;
  }
  return 1;
}

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
    delete_archi(archi)
      ;    return EXIT_FAILURE;
  }

  if (archi->number_threads < nb_threads){
    printf("Invalid number of threads between traces and architecture: set to 1\n");
    return EXIT_FAILURE;
  }

  struct thread **threads;
  threads = malloc(nb_threads*sizeof(struct thread *));
  struct instruction *ins = malloc(sizeof(struct instruction));
  ins->type = INSTRUCTION_LOAD;
  
  /* Open trace file */
  char **trace_files = malloc(nb_threads*sizeof(char *));
  unsigned int i;  
  for (i=0; i<nb_threads; i++){
    trace_files[i] = malloc(100*sizeof(char));
    sprintf(trace_files[i], "MAQAO/trace%d", i);
  }

  create_threads(threads, trace_files, nb_threads);
  
  /* Read trace */
  int count = 0;
  int j;
  int current = nb_threads-1;
  int *ends = malloc(nb_threads*sizeof(int));
  char scan;
  for (i=0; i<nb_threads; i++){
    ends[i]=0;
  }

  while (!is_end(ends, nb_threads)){
    current = (current+1)%nb_threads; 	/* Next thread */

    for (j=0; j<nb_instr_thread; j++){
      next_instruction(ins, threads, current);

      if (ins->type != INSTRUCTION_END_OF_THREAD) {
  	if (ins->type == INSTRUCTION_LOAD) {
	  if (debug_mode){
	    printf("Load on entry 0x%ld, on core %d\n", ins->addr, current);
	    printf("Enter a character, s to stop debugging mode\n");
	    scanf("%c", &scan);
	    if (scan=='s'){
	      debug_mode = 0;
	    }
	    fflush(stdin);
	    print_caches(archi, 0);
	  }
  	  load_line_hierarchy(archi->threads[current], ins->addr);
  	  count++;
  	}
  	else if (ins->type == INSTRUCTION_STORE) {
	  if (debug_mode){
	    printf("Load on entry 0x%ld, on core %d\n", ins->addr, current);
	    printf("Enter a character, s to stop debugging mode\n");
	    scanf("%c", &scan);
	    if (scan=='s'){
	      debug_mode = 0;
	    }
	    fflush(stdin);
	    print_caches(archi, 0);
	  }
  	  store_line_hierarchy(archi->threads[current], ins->addr);
  	  count++;
  	}
      }
      else{
  	ends[current] = 1;
      }
    }
  }

  /* Ends of reading */
  destroy_threads(threads, nb_threads);

  /* Informations about caches */
  print_caches(archi, 1);
  delete_archi(archi);

  free(ins);
  free(ends);
  free(threads);
  for (i=0; i<nb_threads; i++){
    free(trace_files[i]);
  }
  free(trace_files);
  return EXIT_SUCCESS;
}
