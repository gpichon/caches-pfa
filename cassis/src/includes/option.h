/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file option.h
 * \brief Define options and tracking -- Header
 * \author ~ndubois
 * \version 1.1
 * \date 18th march 2014
 * 
 * 
 *
 */

#ifndef OPTION_H
#define OPTION_H

enum TrackingType { TRACKING_DISABLED = 0x0, TRACKING_BOUND = 0x1, TRACKING_INSTRUCTION = 0x2 };

extern unsigned int *tracking_instrs;		/**< The instructions tracked. */
extern unsigned int tracking_instr_count;	/**< Number of instructions tracked. */
extern unsigned int tracking_count;  		/**< Count of tracking values (=2 when -b and/or -i, =1 otherwise). */
extern int tracking_type;			/**< Tracking type used during the simulation. */
extern unsigned long tracking_lower_bound[2];	/**< Lower bound of address tracking. */
extern unsigned long tracking_upper_bound[2];	/**< Upper bound of address tracking. */
extern int help;				/**< help=1 when -h flag is set. */
extern char *trace_file;			/**< The artecture configuration file to use. If not set, a default file is used. */
extern char *lua_file;				/**< The lua file to use for threads interweaving. */
extern char *trace_directory;	        	/**< The directory where the programme open trace files. */
extern unsigned int verbose_mode;  		/**< Verbose when printing results. */
extern int ignore_warning;                      /**< 0 if the fatal warnings stop the program. */
extern int nb_instr_thread;                     /**< Number of instructions before changing thread. */
extern unsigned int nb_threads;                 /**< Number of threads. */
extern unsigned int debug_mode;                 /**< Debug mode. */
extern unsigned int print_mode;                 /**< Tracked values to be printed. */

/**
 * \brief Returns 1 if tracking_instrs contains instr_num.
 */
int is_instr_tracked(unsigned int instr_num);

/**
 * \brief Fill options variables with program's arguments.
 */
void get_options(int argc, char *argv[]);

/**
 * \brief Free variables allocated by options.
 */
void free_options();

#endif /* OPTION_H */
