#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>

#define MAX_ITERATOR_LENGTH 10
#define MAX_WORD_LENGTH 100
#define MAX_INSTRUCTIONS_LENGTH 1000
#define TRACE_END_OF_INSTRUCTION 1
#define TRACE_INSTRUCTION_NOT_FINISHED 0
#define TRACE_INVALID_INSTRUCTION 0x0000 /* barely NULL as an integer */

// Type of instruction
enum {INSTRUCTION_INVALID, INSTRUCTION_END_OF_THREAD, INSTRUCTION_END_OF_LOOP, INSTRUCTION_LOAD, INSTRUCTION_STORE};

struct instruction {
  int type;
  unsigned int addr;
};

struct thread {
  int id; /* a unique number identifiying a thread */
  int file_fd;
  char *data; /* file read with mmap */
  size_t length; /* mmap length */
  size_t cursor; /* to read the file */
  struct loop *loops; /* the current loop */
};

struct loop {
  int start, end, inc, current_value; /*  inc=1 or inc=-1*/
  char iterator[MAX_ITERATOR_LENGTH];
  char instructions[MAX_INSTRUCTIONS_LENGTH];
  unsigned int length; /* length of instructions */
  unsigned int cursor; /* used to read instructions */
  struct loop *next; /* If there is a loop inside the loop */
};

int get_next_thread_id();

void test_trace();

/*
  replace all occurences of s_to_replace by s_to_copy, base has to be long enough
 */
void string_replace(char *base, const char *s_to_replace, const char *s_to_copy);

/*
  returns 1 if the thread's end is reached, 0 otherwise
 */
int is_end_of_thread_reached(struct thread *t);

/*
  returns 1 if the loop's end is reached, 0 otherwise
 */
int is_end_of_loop_reached(struct loop *l);

/*
  returns 1 if the loop iteration's end is reached, 0 otherwise
 */
int is_end_of_loop_iteration_reached(struct loop *l);

/*
  copy the next instruction of the thread t in the buffer instr
 */
void get_next_thread_instruction(struct thread *t, char *instr);

/*
  return the next char of the thread or '\0' if is_end_of_thread_reached(t) == 1
 */
char get_next_thread_char(struct thread *t);

/*
  return the next char of the loop or '\0' if is_end_of_loop_iteration_reached(t) == 1
 */
char get_next_loop_char(struct loop *l);

/*
  copy the next instruction from the current loop of the thread t in the buffer instr
 */
void get_next_loop_instruction(struct loop *l, char *instr);

/*
  Create a loop structure, if l == NULL read loop's instructions from the thread instructions.
  Otherwise, read loop's instructions from the loop instruction
 */
void create_loop(struct thread *t, char *instruction, struct loop *l);

/*
  execute an instruction, calling load or store functions
 */
struct instruction execute_instruction(char *instruction);

/*
  get the next word from the instruction and puts it into word
  returns TRACE_INSTRUCTION_NOT_FINISHED or TRACE_END_OF_INSTRUCTION
 */
int get_next_word(char *instruction, int *offset, char *word);

/*
 */
int add_value_to_address(unsigned int address, char *value);

/*
  returns NULL when the thread creation failed
 */
struct thread* create_thread(char *path);

/*
  free a thread
  returns 0 when successfully done
 */
int free_thread(struct thread *t);

/*
  execute the next thread instruction
  returns value is different than 0 when the thread is finished or a error occurs
 */
struct instruction next_instruction(struct thread *t);

/*
  returns 0 when successfully done
 */
int free_loops(struct loop *l);

#endif /* TRACE_H */
