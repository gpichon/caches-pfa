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
#define FILENAME "data/architest.xml"
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
  
  struct thread *t;
  struct instruction i = {.type = INSTRUCTION_INVALID};
  
  /* Open trace file */
  char trace_file[] = "data/traces/s1113_trace";
  t = create_thread(trace_file);
  
  if (!t) {
    fprintf(stderr, "Cannot open trace file %s\n", trace_file);
    return EXIT_SUCCESS;
  }
  
  /* Read trace */
  int count = 0;
  while (i.type != INSTRUCTION_END_OF_THREAD) {
    i = next_instruction(t);
    if (i.type == INSTRUCTION_LOAD) {
      load_line_hierarchy(archi, archi->threads[0], i.addr);
      count++;
    }
    else if (i.type == INSTRUCTION_STORE) {
      store_line_hierarchy(archi, archi->threads[0], i.addr);
      count++;
    }
  }
  
  free_thread(t);
  printf("Total values: %d\n", count);
  /* End of reading */

  /* Informations about caches */
  print_caches(archi);
  delete_archi(archi);

  return EXIT_SUCCESS;
}
