/**
 * \file trace.h
 * \brief Driver for reading MAQAO' traces.
 * \authors ~ndubois, ~gpichon
 * \version 2.0
 * \date 7th march 2014
 *
 * 
 *
 */

#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "add_line_hierarchy.h"

/**
 * \brief The last instruction identifier read with next_instruction().
 */
extern unsigned int instruction_number;

/**
 * \enum instruction_type
 * \brief Define the different instructions types.
 */
enum instruction_type {INSTRUCTION_END_OF_THREAD, INSTRUCTION_LOAD, INSTRUCTION_STORE};

/**
 * \struct instruction
 * \brief Represents an instruction.
 */
struct instruction {
  int type;           /**< Instruction type, an int in the range of the instruction_type enumeration. */
  unsigned long addr; /**< Address to be used. */
  unsigned int number;	/**< The instruction identifier, given by MAQAO in the trace file. */
};

/**
 * \struct thread
 * \brief Represents a thread.
 */
struct thread {
  int fd; /**< File descriptor of a trace. */
};


/**
 * \brief Initialization of \a nb_threads threads whit threads[i]->fd linked to trace_files. 
 */
void create_threads(struct thread **threads, char **trace_files, int nb_threads);

/**
 * \brief Destruction of \a nb_threads.
 */
void destroy_threads(struct thread **threads, int nb_threads);

/**
 * \brief Get the next instruction for a thread.
 */
void next_instruction(struct instruction *ins, struct thread **threads, int id_thread);

#endif 
