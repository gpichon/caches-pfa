#include <stdio.h>
#include "../src/architecture.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <assert.h>

#define FILENAME "tests/architest.xml"

int main(int argc, char** argv){
  
  struct architecture A = parse_archi_file(FILENAME);

  print_archi(&A);

  return EXIT_SUCCESS;
}
