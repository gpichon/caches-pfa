/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
