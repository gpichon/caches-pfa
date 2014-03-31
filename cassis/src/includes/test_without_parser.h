/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file test_without_parser.h
 * \brief Global test with LFU and MESI on inclusive caches, without architecture functions -- Header
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */

#ifndef TEST_WITHOUT_PARSER_H
#define TEST_WITHOUT_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "add_line_hierarchy.h"

/**
 * \brief Check global working with LFU and MESI on inclusive caches.
 * \note Should print "Tests MESI OK".
 */
int test_without_parser(int argc, char** argv);

#endif
