/**
 * \file block.h
 * \brief Manage a block : initialization, removal, replacement policy -- Header
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */


#ifndef BLOCK_H
#define BLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "line.h"


/**
 * \struct block
 * \brief Abstract Data Type of a block.
 */
struct block {
  int nb_ways; /**< Number of lines in the block. */
  int linesize; /**< Line size. */
  struct line **lines; /**< Tabular of line structure pointer. */
};

/**
 * \brief Initialization of several block and associated lines.
 * \return Return \a nb_blocks initialized blocks, 
 *  each contains \a nb_ways initialized lines.
 */
struct block **init_block(int nb_blocks, int nb_ways, int linesize);

/**
 * \brief Data removal. Contained lines are too free. 
 */
void delete_blocks(struct block **blocks, int nb_blocks);

/**
 * \brief Returns the number line to replace in the set. 
 * \note To be used when a block is full and uses LFU replacement policy.
 */
int id_line_to_replace_LFU(struct block *block);
/**
 * \brief Returns the number line to replace in the set. 
 * \note To be used when a block is full and uses FIFO replacement policy.
 */
int id_line_to_replace_FIFO(struct block *block);
/**
 * \brief Returns the number line to replace in the set. 
 * \note To be used when a block is full and uses LRU replacement policy.
 */
int id_line_to_replace_LRU(struct block *block);

/**
 * \brief Update replacement flag. 
 * \note To be used after a hit, and uses FIFO replacement policy.
 */
void update_FIFO(struct block *block, int nb_ways, long entry);
/**
 * \brief Update replacement flag. 
 * \note To be used after a hit, and uses LFU replacement policy.
 */
void update_LFU(struct block *block, int nb_ways, long entry);
/**
 * \brief Update replacement flag. 
 * \note To be used after a hit, and uses LRU replacement policy.
 */
void update_LRU(struct block *block, int nb_ways, long entry);

/**
 * \brief Load a line in the block. All structures must be initialized.
 * \note Pointer function in parameter returns 1 if write back (when deleting a modified line), else 0.
 */
struct line *add_line_block(struct block *block, struct line *line, int (*coherence)(struct block *));

#endif
