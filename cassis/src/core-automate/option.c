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

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int *tracking_instrs = NULL;
unsigned int tracking_instr_count;
unsigned int tracking_count = 1;
int tracking_type = TRACKING_DISABLED;
unsigned long tracking_lower_bound[2] = {0, 0};
unsigned long tracking_upper_bound[2] = {0xFFFFFFFFFFFFFFFF, 0};
int help = 0;
unsigned int verbose_mode = 1;
char *trace_file = NULL;
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

  while ((c = getopt(argc, argv, "f:t:hb:v:wi:dor:")) != -1){
    switch (c){
    case 'f':	/* -f architecture_file */
      trace_file = optarg;
      break;
    case 'b':	/* -b 0x5555:0x6666 */
      s = optarg;
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
      break;
    case 'v':	/* -v 1*/
      verbose_mode = atoi(optarg);
      if (verbose_mode < 1 || verbose_mode > 4){
	fprintf(stderr, "Invalid verbose mode\n");
	exit(1);
      }
      break;
    case 'w':	/* -w */
      ignore_warning = 1;
      break;
    case 'i':	/* -i 42 */
      nb_instr_thread = atoi(optarg);
      if(nb_instr_thread < 1){
	fprintf(stderr, "Number of instruction per thread (-i) invalid\n");
	nb_instr_thread = 42;
      }
      break;
    case 't':	/* -t 4 */
      nb_threads = atoi(optarg);
      if(nb_threads < 1){
	fprintf(stderr, "Number of threads (-t) invalid\n");
	nb_threads = 1;
      }
      break;
    case 'd':	/* -d */
      debug_mode = 1;
      break;
    case 'o':	/* -o */
      print_mode = 1;
      break;
    case 'r':	/* -r */
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
    }
  }
}

void free_options() {
	free(tracking_instrs);
}
