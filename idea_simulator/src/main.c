#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "add_line_hierarchy.h"
#include "architecture.h"

#define FILENAME "data/architest.xml"
#define TEST "../gentraces/file.txt"

int main(int argc, char *argv[]) {
  char filename[50];
  if(argc == 2){
    strcpy(filename, argv[1]);
  }
  else {
    strcpy(filename, FILENAME);
  }

  struct architecture A;
  parse_archi_file(filename, &A);
  struct architecture *archi = &A;

  int i = 0;
  int count = 0;

  int j, k;
  for (j=0; j<1000; j++){
    for (k=0; k<2048; k++){
      i++;
      i = i%4;
      store_line_hierarchy(archi->levels, archi->threads[i], 6365296+1024*j+4*k);
      count++;
    }
  }
  for (j=0; j<1000; j++){
    for (k=0; k<2048; k++){
      i++;
      i = i%4;
      load_line_hierarchy(archi->levels, archi->threads[i], 636856+1024*j+4*k);
      count++;
    }
  }
  for (j=0; j<1000; j++){
    for (k=0; k<2048; k++){
      i++;
      i = i%4;
      store_line_hierarchy(archi->levels, archi->threads[i], 6296+1024*j+4*k);
      count++;
    }
  }

  printf("Total values: %d\n", count);

  /* Informations about caches */
  print_caches(archi);
  delete_archi(archi);

  return EXIT_SUCCESS;
}
