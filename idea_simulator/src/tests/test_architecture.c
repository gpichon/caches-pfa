#include "test_architecture.h"

#define FILENAME "data/architest.xml"

int test_architecture(int argc, char** argv){

  struct node * n1, * n2;
  int i;
  struct architecture A;
  if(argc == 2){
    parse_archi_file(argv[1], &A);
    print_archi(&A);
  }
  else{
    parse_archi_file(FILENAME, &A);
    assert(A.nb_bits == 64);
    assert(A.number_threads == 4);
    assert(A.number_levels == 3);
    assert(A.threads[0]);
    assert(A.threads[1]);
    assert(A.threads[2]);
    assert(A.threads[3]);

    //Check parent
    //-- L1
    assert(A.threads[0]->parent->id == A.threads[1]->parent->id);
    assert(A.threads[2]->parent->id == A.threads[3]->parent->id);
    //-- L2
    n1 = A.threads[0]->parent;
    n2 = A.threads[2]->parent;
    assert(n1->parent->id == n2->parent->id);
    //-- L3
    assert(n1->parent->parent == NULL);

    //Check next sibling
    //-- L1
    assert(A.threads[0]->next_sibling->id == A.threads[1]->id);
    assert(A.threads[1]->next_sibling->id == A.threads[2]->id);
    assert(A.threads[2]->next_sibling->id == A.threads[3]->id);
    assert(A.threads[3]->next_sibling->id == A.threads[0]->id);
    //-- L2
    assert(n1->next_sibling->id == n2->id);
    assert(n2->next_sibling->id == n1->id);
    //-- L3
    assert(n1->parent->next_sibling->id == n1->parent->id);

    //Check number_children
    //-- L1
    for(i=0;i<4;i++)
      assert(A.threads[i]->nb_children == 0);
    //-- L2
    assert(n1->nb_children == 2);
    assert(n2->nb_children == 2);
    //-- L3
    assert(n1->parent->nb_children == 2);

    //Check children
    //-- L1
    assert(get_child(A.threads[0], 0) == NULL);
    assert(get_child(A.threads[1], 0) == NULL);
    assert(get_child(A.threads[2], 0) == NULL);
    assert(get_child(A.threads[3], 0) == NULL);
    assert(get_child(A.threads[2], 1) == NULL);
    //-- L2
    assert(get_child(n1, 0) == A.threads[0]);
    assert(get_child(n1, 1) == A.threads[1]);
    assert(get_child(n1, 2) == NULL);
    assert(get_child(n2, 0) == A.threads[2]);
    assert(get_child(n2, 1) == A.threads[3]);
    assert(get_child(n2, 2) == NULL);
    //-- L3
    assert(get_child(n1->parent, 0) == n1);
    assert(get_child(n1->parent, 1) == n2);
    assert(get_child(n1->parent, 2) == NULL);

    printf("Tests architecture OK\n");
  }

  delete_archi(&A);
  return EXIT_SUCCESS;
}
