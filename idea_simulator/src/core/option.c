#include "option.h"

#include <unistd.h>

int help = 0;
char *trace_file = NULL;

void get_options(int argc, char *argv[]) {
	char c;
	
	/* To add more options, add the letter and then ":" if the option is a string or nothing for a true/false statement */
	while ((c = getopt(argc, argv, "f:h")) != -1)
	{
		switch (c)
		{
			case 'f':
				trace_file = optarg;
				break;
			case 'h':
				help = 1;
				break;
		}
	}
}
