/**
 * \file test_architecture.c
 * \brief Test the architecture generation -- Header
 * \author ~gmarait
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */

#ifndef TEST_ARCHITECTURE_H
#define TEST_ARCHITECTURE_H

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <assert.h>
#include <unistd.h>

#include "architecture.h"

/**
 * \brief Check if the parsed architecture builds the wanted hierarchy.    
 * \note Should print "Tests architecture OK".
 */
int test_architecture(int argc, char** argv);

#endif
