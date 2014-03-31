/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file test_replacement.h
 * \brief Test the replacement policies -- Header
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */

#ifndef TEST_REPLACEMENT_H
#define TEST_REPLACEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "add_line_hierarchy.h"

/**
 * \brief Launch tests on LFU, FIFO and LRU.    
 * \note Should print "Tests LFU OK", "Tests FIFO OK" and "Tests FIFO OK".
 */
int test_replacement(int argc, char** argv);

#endif
