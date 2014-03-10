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

int tracking_count = 1;
int tracking_type = TRACKING_DISABLED;
unsigned long tracking_lower_bound[2] = {0, 0};
unsigned long tracking_upper_bound[2] = {0xFFFFFFFFFFFFFFFF, 0};
int help = 0;
char *trace_file = NULL;

void get_options(int argc, char *argv[]) {
  char c;
  char *s;
	
  while ((c = getopt(argc, argv, "f:hb:")) != -1)
    {
      switch (c)
	{
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
	}
    }
}
