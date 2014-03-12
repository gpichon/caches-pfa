/**
 * \file option.c
 * \brief define options
 * \author ~ndubois
 * \version 1.0
 * \date 5th march 2014
 * 
 * 
 *
 */
#include "option.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

void get_options(int argc, char *argv[]) {
  char c;
  char *s;

  while ((c = getopt(argc, argv, "f:t:hb:v:wi:d")) != -1){
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
      tracking_type = TRACKING_LIGHT;
      tracking_count = 2;
      break;
    case 'h':	/* -h */
      help = 1;
      break;
    case 'v':	/* -v 1*/
      verbose_mode = atoi(optarg);
      if (verbose_mode < 1 || verbose_mode > 4){
	printf("Invalid verbose mode\n");
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
    }
  }
}
