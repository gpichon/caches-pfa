/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file option.c
 * \brief define options and tracking
 * \author ~ndubois
 * \version 1.1
 * \date 18th march 2014
 * 
 * 
 *
 */
#include "option.h"

#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int *tracking_instrs = NULL;
unsigned int tracking_instr_count;
unsigned int tracking_count = 1;
int tracking_type = TRACKING_DISABLED;
unsigned long tracking_lower_bound[2] = {0, 0};
unsigned long tracking_upper_bound[2] = {0xFFFFFFFFFFFFFFFFul, 0};
int help = 0;
unsigned int verbose_mode = 1;
char *trace_file = NULL;
char *lua_file = NULL;
char *trace_directory = NULL;
int ignore_warning = 0;
int nb_instr_thread = 42;
unsigned int nb_threads = 1;
unsigned int debug_mode = 0;
unsigned int print_mode = 0;

int is_instr_tracked(unsigned int instr_num) {
  unsigned int i = 0;
  for (i = 0; i < tracking_instr_count; i++) {
    if (tracking_instrs[i] == instr_num)
      return 1;
  }
  return 0;
}

void get_options(int argc, char *argv[]) {
  char c;
  char *s, *p;
  unsigned int i;
  tracking_type = TRACKING_DISABLED;

  int tot = 0;

  while ((c = getopt(argc, argv, "f:t:hb:v:wi:dor:l:")) != -1){
    switch (c){
    case 'f':	/* -f architecture_file */
      trace_file = optarg;
      tot++;
      break;
    case 'b':	/* -b 0x5555:0x6666 */
      s = optarg;
      if (strcmp(s, "no_stack") == 00) {
	tracking_lower_bound[1] = 0x0;
	tracking_upper_bound[1] = 0x6ffffffffffful;
	tracking_type += TRACKING_BOUND;
	tracking_count = 2;
	break;
      }
      if ((s = strchr(s, ':')) == NULL) {
	fprintf(stderr, "Bad parameter form");
	break;
      }
      tracking_lower_bound[1] = strtol(optarg, &s, 16);
      tracking_upper_bound[1] = strtol(s+1, NULL, 16);
      tracking_type += TRACKING_BOUND;
      tracking_count = 2;
      break;
    case 'h':	/* -h */
      help = 1;
      tot = 3;
      break;
    case 'v':	/* -v 1*/
      verbose_mode = strtol(optarg, NULL, 0);
      if (verbose_mode < 1 || verbose_mode > 4){
	fprintf(stderr, "Invalid verbose mode\n");
	exit(1);
      }
      break;
    case 'w':	/* -w */
      ignore_warning = 1;
      break;
    case 'i':	/* -i 42 */
      s = optarg;
      tracking_instr_count = 1;
      while ((s = strchr(s, ':')) != NULL) {
	tracking_instr_count++;
	s++;
      }
      s = optarg;
      p = optarg;
      i = 0;
      tracking_instrs = malloc(tracking_instr_count * sizeof(unsigned int));
      while ((s = strchr(s, ':')) != NULL) {
	tracking_instrs[i] = strtol(p, &s, 10);
	s++;
	i++;
	p = s;
      }
      tracking_instrs[i] = strtol(p, NULL, 10);
      tracking_type += TRACKING_INSTRUCTION;
      tracking_count = 2;
      break;
    case 't':	/* -t 4 */
      nb_threads = strtol(optarg, NULL, 0);
      if(nb_threads < 1){
	fprintf(stderr, "Number of threads (-t) invalid\n");
	nb_threads = 1;
      }
      tot++;
      break;
    case 'd':	/* -d */
      debug_mode = 1;
      break;
    case 'o':	/* -o */
      print_mode = 1;
      break;
    case 'l':	/* -l */
      lua_file = optarg;
      break;
    case 'r':	/* -r */
      trace_directory = optarg;
      tot++;
      break;
    }
  }
  if (tot<3){
    fprintf(stderr, "Please enter an architecture file with -f , a number of threads with -t and the trace directory with -r\n");
    exit(1);
  }    
}

void free_options() {
  free(tracking_instrs);
}
