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

int tracking_type = TRACKING_DISABLED;
int tracking_lower_bound;
int tracking_upper_bound;
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
				tracking_lower_bound = strtol(optarg, &s, 16);
				tracking_upper_bound = strtol(s+1, NULL, 16);
				tracking_type = TRACKING_LIGHT;
				break;
			case 'h':	/* -h */
				help = 1;
				break;
		}
	}
}
