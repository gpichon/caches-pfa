/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file block.h
 * \brief Manage a block: update replacement flags and add a line in a block -- Header
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */


#ifndef BLOCK_H
#define BLOCK_H

#include <limits.h>
#include "line.h"



/**
 * \struct block
 * \brief Represents a cache block.
 */
struct block {
  int nb_ways;         /**< Number of lines in the block. */
  int linesize;        /**< Line size. */
  struct line **lines; /**< Tabular of line structure pointer. */
};



/**
 * \brief Initialization of several block and associated lines.
 * \return Return \a nb_blocks initialized blocks, 
 *  each contains \a nb_ways initialized lines.
 */
struct block **init_block(int nb_blocks, int nb_ways, int linesize, enum cache_coherence type);

/**
 * \brief Data removal. Contained lines are too free. 
 */
void delete_blocks(struct block **blocks, int nb_blocks);

/**
 * \brief Returns the number line to replace in the set.
 * \param The line to eject' priority.
 * \param not_rm value which is to be kept.
 * \note To be used when a block is full and uses LFU replacement policy.
 */
int id_line_to_replace_LFU(struct block *block, int priority, unsigned long not_rm);
/**
 * \brief Returns the number line to replace in the set. 
 * \param The line to eject' priority.
 * \param not_rm value which is to be kept.
 * \note To be used when a block is full and uses FIFO replacement policy.
 */
int id_line_to_replace_FIFO(struct block *block, int priority, unsigned long not_rm);
/**
 * \brief Returns the number line to replace in the set. 
 * \param The line to eject' priority.
 * \param not_rm value which is to be kept.
 * \note To be used when a block is full and uses LRU replacement policy.
 */
int id_line_to_replace_LRU(struct block *block, int priority, unsigned long not_rm);

/**
 * \brief Update replacement flag. 
 * \note To be used after a hit, and uses FIFO replacement policy.
 */
void update_FIFO(struct block *block, int nb_ways, unsigned long entry);
/**
 * \brief Update replacement flag. 
 * \note To be used after a hit, and uses LFU replacement policy.
 */
void update_LFU(struct block *block, int nb_ways, unsigned long entry);
/**
 * \brief Update replacement flag. 
 * \note To be used after a hit, and uses LRU replacement policy.
 */
void update_LRU(struct block *block, int nb_ways, unsigned long entry);

/**
 * \brief Load a line in the block. All structures must be initialized.
 * \param priority Priority of the data to be deleted.
 * \param not_rm value which is to be kept.
 * \note Pointer function in parameter returns 1 if write back (when deleting a modified line), else 0.
 */
struct line *add_line_block(struct block *block, struct line *line, int (*coherence)(struct block *, int, unsigned long), int priority, unsigned long not_rm);

#endif
