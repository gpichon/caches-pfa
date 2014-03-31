/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file common_types.h
 * \brief Common types (between SMC and core).
 * \author ~ahonorat
 * \version 1.0
 * \date 16th march 2014
 *
 * 
 *
 */


#ifndef TYPES_H
#define TYPES_H

/**
 * \def NO_COHERENCE_SM_MACRO
 * \brief Allow to use SMC functions instead of macros.
 */
#define NO_COHERENCE_SM_MACRO
#include "coherence_sm.h"

/* fsm */

/**
 * \struct coherence
 * \brief Represents a coherence policies state machine.
 */ 
struct coherence{
  struct coherenceContext _fsm; /**< SMC object */
};

/* line */

/**
 * \enum status
 * \brief Status for coherency protocols.
 */ 
enum status{
  I = 0, /**< Invalid */
  S = 1, /**< Shared */
  E = 2, /**< Exclusive */
  M = 3, /**< Modified */
  O = 4, /**< Owned */
  F = 5, /**< Forward */
};



/* cache */

/**
 * \enum stat_type
 * \brief Stat type: 
 */
enum stat_type {
  MISS,
  HIT,
  WRITE_BACK,
  COHERENCE_EVINCTION,
  CAPACITY_EVINCTION,
  TYPES_EVINCTION,
  VALUE_BY_SNOOPING,
  VALUE_ABOVE,
  VALUE_BELOW,
  COHERENCE_BROADCAST,
  SNOOPING_BROADCAST
};

/**
 * \enum cache_coherence
 * \brief Cache type: MSI, MESI, MOESI, MESIF, MOSI.
 */
enum cache_coherence {
  MSI = 0,
  MESI = 1,
  MOESI = 2,
  MESIF = 3,
  MOSI = 4
};



#endif
