/**
 * \file main_test.c
 * \brief Run 3 unitary tests. Should print 5 OK. 
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include "test_replacement.h"
#include "test_architecture.h"
#include "test_without_parser.h"

int main(int argc, char** argv){

  test_replacement(argc, argv);
  test_without_parser(argc, argv);
  test_architecture(argc, argv);

  return EXIT_SUCCESS;
}
