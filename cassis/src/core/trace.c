/**
 * \file trace.c
 * \brief Driver for reading MAQAO' traces.
 * \authors ~ndubois, ~gpichon
 * \version 2.0
 * \date 7rd march 2014
 *
 * 
 *
 */

#include "trace.h"

unsigned int instruction_number;

void create_threads(struct thread **threads, char **trace_files, int nb_threads){
  int i;
  for (i=0; i<nb_threads; i++){
    threads[i] = malloc(sizeof(struct thread));
    assert(threads[i]!=NULL);
    threads[i]->fd = open(trace_files[i], O_RDONLY);
    if (threads[i]->fd == -1){
      perror("Error when opening trace\n");
      exit(1);
    }
  }
}

void destroy_threads(struct thread **threads, int nb_threads){
  int i, err;
  for (i=0; i<nb_threads; i++){
    err = close(threads[i]->fd);
    if (err == -1){
      perror("Erreur when closing file\n");
      exit(1);
    }
    free(threads[i]);
  }
}

void next_instruction(struct instruction *ins, struct thread **threads, int id_thread){
  int fd = threads[id_thread]->fd;
  char buf[100];
  char save[100];
  int size = 0;

  int lu, i;

  for (i=0; i<4; i++){
    lu = read(fd, buf, 1);
    if (lu == 0){
      ins->type = INSTRUCTION_END_OF_THREAD;
      return;
    }

    while (buf[0] != ' '){
      if (buf[0] == '\n'){
	break;
      }

      sprintf(save+size, "%c", buf[0]);
      size++;
      lu = read(fd, buf, 1);
      if (lu == 0){
	ins->type = INSTRUCTION_END_OF_THREAD;
	return;
      }
    }

    if (i==0){
      if (strcmp(save, "LOAD")==0){
	ins->type = INSTRUCTION_LOAD;
      }
      else if (strcmp(save, "STORE")==0){
	ins->type = INSTRUCTION_STORE;
      }
      else{
	ins->type = INSTRUCTION_END_OF_THREAD;
      }
      size = 0;
    }

    else if (i==1){
      unsigned long i = strtol(save, NULL, 16);
      ins->addr = i;
    }

    else if (i==3) {
	  unsigned int ui = atoi(save);
	  ins->number = ui;
	  instruction_number = ins->number;
	}
  }
}
