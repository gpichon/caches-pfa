#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "add_line_hierarchy.h"
#include "architecture.h"
#include "trace.h"

#define FILENAME "data/architest.xml"
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
