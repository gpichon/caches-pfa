/**
 * \file trace.h
 * \brief I don't now know -- Header
 * \author ~ndubois
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */


#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>

/**
 * \def MAX_ITERATOR_LENGTH
 * \brief 
 */ 
#define MAX_ITERATOR_LENGTH 10
/**
 * \def MAX_WORD_LENGTH
 * \brief 
 */ 
#define MAX_WORD_LENGTH 100
/**
 * \def MAX_INSTRUCTIONS_LENGTH
 * \brief 
 */ 
#define MAX_INSTRUCTIONS_LENGTH 1000
/**
 * \def TRACE_END_OF_INSTRUCTION
 * \brief 
 */ 
#define TRACE_END_OF_INSTRUCTION 1
/**
 * \def TRACE_INSTRUCTION_NOT_FINISHED
 * \brief 
 */ 
#define TRACE_INSTRUCTION_NOT_FINISHED 0
/**
 * \def TRACE_INVALID_INSTRUCTION
 * \brief 
 */ 
#define TRACE_INVALID_INSTRUCTION 0x0000 /* barely NULL as an integer --> What ?*/

/**
 * \enum instruction_type
 * \brief Define the different instructions types.
 */
enum instruction_type {INSTRUCTION_INVALID, INSTRUCTION_END_OF_THREAD, INSTRUCTION_END_OF_LOOP, INSTRUCTION_LOAD, INSTRUCTION_STORE};

/**
 * \struct instruction
 * \brief Abstract Data Type of an instruction.
 */
struct instruction {
  int type; /**< Instruction type, an int in the range of the instruction_type enumeration. */
  unsigned int addr; /**< Address to be used. */
};

/**
 * \struct thread
 * \brief Abstract Data Type of a thread.
 * \todo Manage a thread with lua.
 */
struct thread {
  int id; /**< A unique number identifiying a thread. */
  int file_fd; /**< File descriptor of the trace associated with the thread/core. */
  char *data; /**< File read with mmap. */
  size_t length; /**< mmap length. */
  size_t cursor; /**< To read the file at the right place. */
  struct loop *loops; /**< Pointer to the current loop. */
};

/**
 * \struct loop
 * \brief Abstract Data Type for a loop, in order to describe MAQAO compression.
 */
struct loop {
  int start, end, inc, current_value; /**<  inc=1 or inc=-1 */
  char iterator[MAX_ITERATOR_LENGTH]; /**< ??? */
  char instructions[MAX_INSTRUCTIONS_LENGTH]; /**< ??? */
  unsigned int length; /**< Length of instructions, i.e. ? */
  unsigned int cursor; /**< Used to read instructions. */
  struct loop *next; /**< If there is a loop inside the loop */
};

/**
 * \brief Return the id of the thread in which is the next instruction to execute.
 */
int get_next_thread_id();

/**
 * \brief Test function.
 * \test Tests the word recognition.
 * \todo Has to be removed (from here).
 */
void test_get_next_word();

/**
 * \brief Test function.
 * \test Tests the word replacement.
 * \todo Has to be removed (from here).
 */
void test_string_replace();

/**
 * \brief Test function.
 * \test Tests the whole trace reading.
 * \todo Has to be removed (from here).
 */
void test_trace();

/**
 * \brief Replace all occurences of s_to_replace in base, by s_to_copy. Base has to be long enough.
 */
void string_replace(char *base, const char *s_to_replace, const char *s_to_copy);

/**
 * \return Returns 1 if the thread's end is reached, 0 otherwise.
 */
int is_end_of_thread_reached(struct thread *t);

/**
 * \return Returns 1 if the loop's end is reached, 0 otherwise.
 */
int is_end_of_loop_reached(struct loop *l);

/**
 * \return Returns 1 if the loop iteration's end is reached, 0 otherwise.
 */
int is_end_of_loop_iteration_reached(struct loop *l);

/**
 * \brief Copy the next instruction of the thread t in the buffer instr.
 */
void get_next_thread_instruction(struct thread *t, char *instr);

/**
 * \return Return the next char of the thread or '\0' if is_end_of_thread_reached(t) == 1.
 */
char get_next_thread_char(struct thread *t);

/**
 * \return Return the next char of the loop or '\0' if is_end_of_loop_iteration_reached(t) == 1.
 */
char get_next_loop_char(struct loop *l);

/**
 * \brief Copy the next instruction from the current loop of the thread t in the buffer instr.
 */
void get_next_loop_instruction(struct loop *l, char *instr);

/**
 * \brief Create a loop structure. 
 * \param l If l == NULL read loop's instructions from the thread instructions.
 *          Otherwise, read loop's instructions from the loop instruction.
 */
void create_loop(struct thread *t, char *instruction, struct loop *l);

/**
 * \brief Execute an instruction, calling load or store functions.
 * \return Return a correct instruction structure.
 * \todo Change explaination of this function. Seems wrong.
 */
struct instruction execute_instruction(char *instruction);

/**
 * \brief Get the next word from the instruction and puts it into \a word.
 * \return Returns TRACE_INSTRUCTION_NOT_FINISHED or TRACE_END_OF_INSTRUCTION.
 */
int get_next_word(char *instruction, int *offset, char *word);

/**
 * \todo Document this function. What is \a value ?
 */
int add_value_to_address(unsigned int address, char *value);

/**
 * \brief Creates a thread structure.
 * \return Returns NULL when the thread creation failed.
 */
struct thread* create_thread(char *path);

/**
 * \brief Free a thread.
 * \return Returns 0 when successfully done.
 */
int free_thread(struct thread *t);

/**
 * \brief Execute the next thread instruction.
 * \return Returns value is different than 0 when the thread is finished or a error occurs.
 * \todo Change explaination of this function. Seems wrong.
 */
struct instruction next_instruction(struct thread *t);

/**
 * \brief Free a loop structure.
 * \return Returns 0 when successfully done.
 */
int free_loops(struct loop *l);

#endif 
