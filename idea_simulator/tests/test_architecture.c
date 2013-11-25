#include <stdio.h>
#include "../src/architecture.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <assert.h>

#define FILENAME "data/architest.xml"

int main(int argc, char** argv){
  (void) argc;
  (void) argv;

  struct architecture A = parse_archi_file(FILENAME);

  print_archi(&A);

  delete_archi(&A);
  return EXIT_SUCCESS;
}
