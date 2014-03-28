/**
 * \file main.c
 * \brief Run the simulator with the architecture and traces given.
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "add_line_hierarchy.h"
#include "architecture.h"
#include "trace.h"
#include "option.h"


#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

/**
 * \def DEFAULT_LUA_FILE
 * \brief Default path for the lua interweaving file.
 * Tricky method, source at :
 * http://stackoverflow.com/questions/195975/how-to-make-a-char-string-from-a-c-macros-value
 */
#ifndef DEFAULT_LUA_FILE
#define DEFAULT_LUA_FILE manage_threads.lua
#endif

#define LUA_FILE STR(DEFAULT_LUA_FILE)


int is_end(int *ends, int nb_threads){
  int i;
  for(i=0; i<nb_threads; i++){
    if (ends[i] == 0)
      return 0;
  }
  return 1;
}

int main(int argc, char *argv[]) {

  get_options(argc, argv);
  
  if (help){
    printf("To work with the architecture file: -f file.xml\n");
    printf("To precise number of threads: -t int\n");
    printf("To precise traces directory: -r file.xml\n");
    printf("To trace a special data set: -b 0x000000000000:0xffffffffffff or -b no_stack for all values excepting stack values (optional)\n");
    printf("To print more statistics: -v 1: basic stats (default)\n");
    printf("                          -v 2: evinctions (optional)\n");
    printf("                          -v 3: more detailled misses (optional)\n");
    printf("                          -v 4: all stats (optional)\n");
    printf("To ignore the architecture fatal warnings: -w (optional)\n");
    printf("To specify the interweaving lua file: -l file (optional if threads.lua exists)\n");
    printf("To track only some instructions: -i 1:3:42 (optional)\n");
    printf("To execute step by step (press s to run all instructions): -d (optional)\n");
    printf("To print only the tracked values stats: -o (optional)\n");
    printf("To print this help: -h\n");
    return EXIT_SUCCESS;
  }
  
  char filename[256];
  if(trace_file){
    strcpy(filename, trace_file);
  }
  
  char luafile[256];
  if(lua_file){
    strcpy(luafile, lua_file);
  }
  else {
    strcpy(luafile, LUA_FILE);
  }

  struct architecture A;
  parse_archi_file(filename, &A);
  struct architecture *archi = &A;
  if(archi->warning && (!ignore_warning)){
    delete_archi(archi);
    return EXIT_FAILURE;
  }

  if (archi->number_threads < nb_threads){
    printf("Invalid number of threads between traces and architecture: set to 1\n");
    return EXIT_FAILURE;
  }

  struct thread **threads;
  threads = malloc(nb_threads*sizeof(struct thread *));
  struct instruction *ins = malloc(sizeof(struct instruction));
  ins->type = INSTRUCTION_LOAD;
  
  /* Open trace file */
  char **trace_files = malloc(nb_threads*sizeof(char *));
  unsigned int i;  
  for (i=0; i<nb_threads; i++){
    trace_files[i] = malloc(100*sizeof(char));
    if (trace_directory)
      sprintf(trace_files[i], "%s/trace%d", trace_directory, i);
	else
      sprintf(trace_files[i], "./trace%d", i);
  }

  create_threads(threads, trace_files, nb_threads);
  
  /* Read trace */
  int count = 0;
  int current = nb_threads-1;
  int *ends = malloc(nb_threads*sizeof(int));
  char scan;
  for (i=0; i<nb_threads; i++){
    ends[i]=0;
  }

  /* lua initialization */
  lua_State *L = luaL_newstate();  
  luaL_openlibs(L);
  luaL_loadfile(L, luafile);
  lua_call(L, 0, 0);



  while (!is_end(ends, nb_threads)){
    lua_getglobal(L, "interweave");                  
    lua_pushnumber(L, current);                     
    lua_pushnumber(L, nb_threads);                    
    
    /* Next thread */
    lua_call(L, 2, 1);  

    current = lua_tonumber(L, -1);
    lua_pop(L, 1);  

    next_instruction(ins, threads, current);
    
    if (ins->type != INSTRUCTION_END_OF_THREAD) {
      if (ins->type == INSTRUCTION_LOAD) {
	load_line_hierarchy(archi->threads[current], ins->addr);
	count++;
	if (debug_mode){
	  printf("Load on entry 0x%ld, on core %d\n", ins->addr, current);
	  printf("Enter a character, s to stop debugging mode\n");
	  scanf("%c", &scan);
	  if (scan=='s'){
	    debug_mode = 0;
	  }
	  fflush(stdin);
	  print_caches(archi, 0);
	}
      }
      else if (ins->type == INSTRUCTION_STORE) {
	store_line_hierarchy(archi->threads[current], ins->addr);
	count++;
	if (debug_mode){
	  printf("Store on entry 0x%ld, on core %d\n", ins->addr, current);
	  printf("Enter a character, s to stop debugging mode\n");
	  scanf("%c", &scan);
	  if (scan=='s'){
	    debug_mode = 0;
	  }
	  fflush(stdin);
	  print_caches(archi, 0);
	}
      }
    }
    else{
      ends[current] = 1;
    }
  }


  /* Ends of reading */
  destroy_threads(threads, nb_threads);
  lua_close(L);

  /* Informations about caches */
  print_caches(archi, 1);
  delete_archi(archi);

  /* Options allocations */
  free_options();

  free(ins);
  free(ends);
  free(threads);
  for (i=0; i<nb_threads; i++){
    free(trace_files[i]);
  }
  free(trace_files);
  return EXIT_SUCCESS;
}
