/**
 * \file main.c
 * \brief Run the simulator with the architecture and traces given.
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * \note At the moment, architecture and traces cannot be given in command line. 
 * \todo Allow to choose architecture and traces in command line. 
 *       Manage options with -letter or --word.
 *       Create an usage option.
 *
 * # How to use Markdown ?? #
 * ## This is really simple. #
 * + firstly, you use some special characters
 *   - as #
 *   - or -
 *   - or +
 *   - or :
 * + secondly, RTFM
 * 
 *   [It's here](http://www.stack.nl/~dimitri/doxygen/manual/markdown.html)
 * 
 * ## We can also make a tabular. #
 * Emacs user | Not an emacs user
 * -----------|------------------:
 * We        | Not we
 * This is a long text|And a right-justified text
 *
 * ## But what about code ? #
 * ~~~~~~~~~~~~~{.c}
 * #define CODE "my_code"
 * int copyright(char * blop){
 *   return strcmp(blop, my_code);
 * }
 * ~~~~~~~~~~~~~
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "add_line_hierarchy.h"
#include "architecture.h"
#include "trace.h"

/**
 * \def FILENAME
 * \brief Path to the automatically generated XML configuration file, if none specified.
 */
#define FILENAME "data/architest_archi.xml"
/**
 * \def TEST
 * \brief Path to the result file of tests.
 */
#define TEST "../gentraces/file.txt"

int main(int argc, char *argv[]) {
  char filename[50];
  if(argc == 2){
    strcpy(filename, argv[1]);
  }
  else {
    strcpy(filename, FILENAME); /* Trouver un nom plus générique */
  }

  struct architecture A;
  parse_archi_file(filename, &A);
  struct architecture *archi = &A;
  
  struct thread **t;
  t = malloc(4*sizeof(struct thread *));
  struct instruction i = {.type = INSTRUCTION_INVALID};
  
  /* Open trace file */
  char trace_file1[] = "data/traces/trace1";
  char trace_file2[] = "data/traces/trace2";
  char trace_file3[] = "data/traces/trace3";
  char trace_file4[] = "data/traces/trace4";


  t[0] = create_thread(trace_file1);
  t[1] = create_thread(trace_file2);
  t[2] = create_thread(trace_file3);
  t[3] = create_thread(trace_file4);
  
  if (!t[0] || !t[1] || !t[2] || !t[3]) {
    fprintf(stderr, "Cannot open trace files\n");
    return EXIT_SUCCESS;
  }
  
  /* Read trace */
  int count = 0;
  int j;
  int current = 3;
  int end[4] = {0};

  while (!end[0] || !end[1] || !end[2] || !end[3]){
    current = (current+1)%4; 	/* Next thread */
    for (j=0; j<10; j++){ 	/* 10 instructions per thread */
      
      if (i.type != INSTRUCTION_END_OF_THREAD) {
	i = next_instruction(t[current]);
	if (i.type == INSTRUCTION_LOAD) {
	  load_line_hierarchy(archi, archi->threads[current], i.addr);
	  count++;
	}
	else if (i.type == INSTRUCTION_STORE) {
	  store_line_hierarchy(archi, archi->threads[current], i.addr);
	  count++;
	}
      }
      else{
	end[current] = 1;
	i.type = INSTRUCTION_INVALID;
      }
    }
  }

  
  free_thread(t[current]);
  for (j=0; j<4; j++){
    if (j!=current){
      free(t[j]);
    }
  }
  free(t);

  printf("Total values: %d\n", count);
  /* End of reading */

  /* Informations about caches */
  print_caches(archi);
  delete_archi(archi);

  return EXIT_SUCCESS;
}
