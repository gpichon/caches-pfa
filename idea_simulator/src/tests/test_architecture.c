#include "test_architecture.h"

#define FILENAME "data/architest.xml"

int test_architecture(int argc, char** argv){

  struct architecture A;
  int i, k;
  struct list * l;
  if(argc == 2){
    parse_archi_file(argv[1], &A);
    print_archi(&A);
  }
  else{
    parse_archi_file(FILENAME, &A);
    assert(A.nb_bits == 64);
    assert(A.number_threads == 4);
    assert(A.number_levels == 3);
    print_archi(&A);
    /* TODO
       Tests */
    
    printf("Tests architecture OK\n");
  }

  delete_archi(&A);
  return EXIT_SUCCESS;
}
