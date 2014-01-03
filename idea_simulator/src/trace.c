/**
 * \file trace.c
 * \brief I don't now know.
 * \author ~ndubois
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */

#include "trace.h"
#include "add_line_hierarchy.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <ctype.h>

/* TO REMOVE AFTER -- TRACE TESTS */
void test_get_next_word();
void test_string_replace();
void test_trace();
/* END OF TO REMOVE AFTER */

int get_next_thread_id()
{
  static int next_thread_id = 0;
  return next_thread_id++;
}

void string_replace(char *base, const char *s_to_replace, const char *s_to_copy) {
  char buffer[MAX_INSTRUCTIONS_LENGTH];
  char *addr;
  
  while ((addr = strstr(base, s_to_replace)) != NULL) {
    strcpy(buffer, addr + strlen(s_to_replace));
    sprintf(addr, "%s%s", s_to_copy, buffer);
  }
}

int is_end_of_thread_reached(struct thread *t) {
  return t->cursor >= t->length;
}

int is_end_of_loop_reached(struct loop *l) {
  if (l->inc > 0)
    return l->current_value > l->end;
  else
    return l->current_value < l->end;
}

int is_end_of_loop_iteration_reached(struct loop *l) {
  return l->cursor > l->length;
}

struct instruction next_instruction(struct thread *t) {
  char instruction[MAX_INSTRUCTIONS_LENGTH];
  struct instruction instr = {.type = INSTRUCTION_INVALID};
  
  /* we're reading the next instruction, that can be in a loop or not */
  if (t->loops) {
    get_next_loop_instruction(t->loops, instruction);
  }
  else {
    get_next_thread_instruction(t, instruction);
  }
  
  if (is_end_of_thread_reached(t)) {
    instr.type = INSTRUCTION_END_OF_THREAD;
    return instr;
  }
  
  /* we're creating a loop or executing an instruction */
  if (strstr(instruction, "for ") != NULL) {
    create_loop(t, instruction, t->loops);
  }
  else
    instr = execute_instruction(instruction);
  
  if (t->loops && is_end_of_loop_reached(t->loops)) {
    instr.type = INSTRUCTION_END_OF_LOOP;
    struct loop *l = t->loops;
    t->loops = l->next;
    free(l);
    return instr;
  }
  
  return instr;
}

void get_next_thread_instruction(struct thread *t, char *instr) {
  char c;
  int i = 0;
  
  c = get_next_thread_char(t);
  while (!(is_end_of_thread_reached(t) || c == '\n' || c == '\0')) {
    instr[i++] = c;
    c = get_next_thread_char(t);
  }
  instr[i] = '\0';
}

char get_next_thread_char(struct thread *t) {
  return (is_end_of_thread_reached(t)?'\0':t->data[t->cursor++]);
}

char get_next_loop_char(struct loop *l) {
  return (is_end_of_loop_iteration_reached(l)?'\0':l->instructions[l->cursor++]);
}

void get_next_loop_instruction(struct loop *l, char *instr) {
  char c;
  int i = 0;
  char iterator[MAX_ITERATOR_LENGTH];
  
  c = get_next_loop_char(l);
  while (!(is_end_of_loop_iteration_reached(l) || c == '\n' || c == '\0')) {
    instr[i++] = c;
    c = get_next_loop_char(l);
  }
  instr[i] = '\0';
  sprintf(iterator, "%d", l->current_value);
  string_replace(instr, l->iterator, iterator);
  
  if (is_end_of_loop_iteration_reached(l)) {
    l->cursor = 0;
    l->current_value += l->inc;
  }
}

void create_loop(struct thread *t, char *instruction, struct loop *loop) {
  int cursor = 0, write = 0;
  char word[MAX_WORD_LENGTH], iterator[MAX_ITERATOR_LENGTH];
  char instructions[MAX_INSTRUCTIONS_LENGTH] = "", buffer[MAX_INSTRUCTIONS_LENGTH] = "";
  int start, end, for_loops;
  struct loop *l;
  
  if (get_next_word(instruction, &cursor, word) == TRACE_END_OF_INSTRUCTION)
    return;
  if (strcmp(word, "for") != 0)
    return;
  
  if (get_next_word(instruction, &cursor, word) == TRACE_END_OF_INSTRUCTION)
    return;
  strncpy(iterator, word, 9);
  iterator[9] = '\0';
  
  if (get_next_word(instruction, &cursor, word) == TRACE_END_OF_INSTRUCTION)
    return;
  if (strcmp(word, "=") != 0)
    return;
  
  if (get_next_word(instruction, &cursor, word) == TRACE_END_OF_INSTRUCTION)
    return;
  start = atoi(word);
  
  if (get_next_word(instruction, &cursor, word) == TRACE_END_OF_INSTRUCTION)
    return;
  if (strcmp(word, "to") != 0)
    return;
  
  if (get_next_word(instruction, &cursor, word) == TRACE_END_OF_INSTRUCTION && word[0] == '\0')
    return;
  end = atoi(word);
  
  if (!(get_next_word(instruction, &cursor, word) == TRACE_END_OF_INSTRUCTION && word[0] == '\0'))
    return;
  
  for_loops = 0;
  if (loop)
    get_next_loop_instruction(loop, buffer);
  else
    get_next_thread_instruction(t, buffer);
  while (!((strstr(buffer, "endfor") != NULL && for_loops == 0) || is_end_of_thread_reached(t))) {
    if (write)
      strcat(instructions, "\n");
    write = 1;
    if (strstr(buffer, "for ") != NULL)
      for_loops++;
    if (strstr(buffer, "endfor") != NULL)
      for_loops--;
    strcat(instructions, buffer);
    if (loop)
      get_next_loop_instruction(loop, buffer);
    else
      get_next_thread_instruction(t, buffer);
  }
  
  l = malloc(sizeof(struct loop));
  l->start = start;
  l->current_value = start;
  l->end = end;
  l->inc = (start < end?1:-1);
  strcpy(l->iterator, iterator);
  strcpy(l->instructions, instructions);
  l->length = strlen(l->instructions);
  l->cursor = 0;
  l->next = t->loops;
  t->loops = l;
}

struct instruction execute_instruction(char *instruction) {
  char word[MAX_WORD_LENGTH];
  int offset = 0, end_of_instruction;
  struct instruction instr = {.type = INSTRUCTION_INVALID};
  
  get_next_word(instruction, &offset, word);
  if (strcmp(word, "load") == 0)
    instr.type = INSTRUCTION_LOAD;
  else if (strcmp(word, "store") == 0)
    instr.type = INSTRUCTION_STORE;
  else
    return instr; /* unknowned instruction */
  
  /* Reading the value */
  end_of_instruction = get_next_word(instruction, &offset, word);
  instr.addr = strtoul(word, NULL, 16);
  
  if (end_of_instruction)
    return instr;
  
  /* if the value is modified by an operation we have to compute it */
  while (end_of_instruction != TRACE_END_OF_INSTRUCTION) {
    end_of_instruction = get_next_word(instruction, &offset, word);
    
    if (strcmp(word, "+") == 0) {
      end_of_instruction = get_next_word(instruction, &offset, word);
      instr.addr = add_value_to_address(instr.addr, word);
    }
  }
  
  return instr;
}

int add_value_to_address(unsigned int address, char *value) {
  int offset = 0, oleft = 0, oright = 0;
  char left[MAX_WORD_LENGTH] = "", op, right[MAX_WORD_LENGTH] = "", c;
  
  // Getting first operand
  c = value[offset++];
  if (c == '-') {
    left[oleft++] = c;
    c = value[offset++];
  }
  if (c == '\0')
    return address;
  while (isdigit(c)) {
    left[oleft++] = c;
    c = value[offset++];
    if (c == '\0')
      return address;
  }
  left[oleft] = '\0';
  
  if (c == '\0')
    return address;
  // Getting operator
  offset--;
  op = value[offset++];
  
  // Getting second operand
  c = value[offset++];
  if (c == '-') {
    right[oright++] = c;
    c = value[offset++];
  }
  if (c == '\0')
    return address;
  while (isdigit(c)) {
    right[oright++] = c;
    c = value[offset++];
  }
  right[oright] = '\0';
  
  if (op == '*')
    address += strtoul(left, NULL, 10) * strtoul(right, NULL, 10);
  
  return address;
}

int get_next_word(char *instruction, int *offset, char *word) {
  int j;
  
  for (j = 0; !(instruction[*offset] == '\n' || instruction[*offset] == '\0' || (instruction[*offset] == ' ' && j > 0)); (*offset)++) {
    if (instruction[*offset] != ' ')
      word[j++] = instruction[*offset];
  }
  
  word[j] = '\0';
  
  if  (instruction[*offset] == '\n' || instruction[*offset] == '\0') {
    return TRACE_END_OF_INSTRUCTION;
  }
  
  return TRACE_INSTRUCTION_NOT_FINISHED;
}

struct thread* create_thread(char *path) {
  struct thread *t = malloc(sizeof(struct thread));
  
  if (!t)
    return NULL;
  
  t->file_fd = open(path, O_RDONLY);
  if (t->file_fd == -1) {
    free(t);
    perror("openning trace file");
    return NULL;
  }
  
  t->cursor = lseek(t->file_fd, 0, SEEK_END);
  t->length = t->cursor;
  lseek(t->file_fd, 0, SEEK_SET);
  t->data = mmap(NULL, t->cursor, PROT_READ, MAP_SHARED, t->file_fd, 0);
  if (t->data == NULL) {
    close(t->file_fd);
    perror("mmap trace");
    return NULL;
  }
  
  t->loops = NULL;
  t->cursor = 0;
  
  return t;
}

int free_thread(struct thread *t) {
  munmap(t->data, t->length);
  close(t->file_fd);
  if (t->loops)
    free_loops(t->loops);
  free(t);
  return 0;
}

int free_loops(struct loop *l) {
  if (l->next)
    free_loops(l);
  return 0;
}

/* TESTS */
void test_get_next_word() {
  char instr[MAX_INSTRUCTIONS_LENGTH] = "test tes te t 3";
  char word[MAX_WORD_LENGTH];
  int offset = 0;
  
  get_next_word(instr, &offset, word);  printf("get_next_word(test):%s\n", word);
  get_next_word(instr, &offset, word);  printf("get_next_word(tes):%s\n", word);
  get_next_word(instr, &offset, word);  printf("get_next_word(te):%s\n", word);
  get_next_word(instr, &offset, word);  printf("get_next_word(t):%s\n", word);
  get_next_word(instr, &offset, word);  printf("get_next_word(3):%s\n", word);
} 

void test_string_replace() {
  char var[MAX_INSTRUCTIONS_LENGTH] = "test tes te t 3";
  
  string_replace(var, "te", "va");
  printf("string_replace(vast vas va t 3):%s\n", var);
  string_replace(var, "t", "hahaha");
  printf("string_replace(vashahaha vas va hahaha 3):%s\n", var);
}

void test_trace() {
  struct thread *t;
  struct instruction i = {.type = INSTRUCTION_INVALID};
  
  puts("START TRACE TEST");
  
  test_get_next_word();
  test_string_replace();
  
  puts("END TRACE TEST\n");
  
  puts("START TRACE TRY");
  t = create_thread("data/traces/s1113_trace");
  
  if (!t) return;
  
  while (i.type != INSTRUCTION_END_OF_THREAD) {
    i = next_instruction(t);
    printf("instruction:%d(%d)\n", i.type, i.addr);
  }
  
  free_thread(t);
  
  puts("END TRACE TRY");
}
