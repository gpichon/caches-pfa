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

    for(i=0;i<A.number_levels;i++){ //Checks the levels table
      l = A.levels[i];
      k = 0;
      while(l != NULL){ //Checks each cache has the good depth
	assert(l->cache->depth == i+1);
	l = l->next;
	k++;
      }
      switch(i){ //Checks the number of cache
      case 0:
	assert(k == 4);
	break;
      case 1:
	assert(k == 2);
	break;
      case 2:
	assert(k == 1);
	break;
      }
    }
    
    for(i=0;i<A.number_threads; i++){ //Checks the threads table
      l = A.threads[i];
      k = 0;
      while(l != NULL){ //Checks each cache has the good depth
	k++;
	assert(l->cache->depth == k);
	l = l->next;
      }
      assert(k == 3);
    }
    
    printf("Tests architecture OK\n");
  }

  delete_archi(&A);
  return EXIT_SUCCESS;
}
