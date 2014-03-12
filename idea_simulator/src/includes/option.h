/**
 * \file option.h
 * \brief define options
 * \author ~ndubois
 * \version 1.0
 * \date 5th march 2014
 * 
 * 
 *
 */

#ifndef OPTION_H
#define OPTION_H

enum TrackingType { TRACKING_DISABLED, TRACKING_LIGHT, TRACKING_FULL };

extern unsigned int tracking_count;  		/**< Count of tracking values. */
extern int tracking_type;			/**< Tracking type used during the simulation. */
extern unsigned long tracking_lower_bound[2];	/**< Lower bound of address tracking. */
extern unsigned long tracking_upper_bound[2];	/**< Upper bound of address tracking. */
extern int help;					/**< help=1 when -h flag is set */
extern char *trace_file;			/**< The artecture configuration file to use. If not set, a default file is used. */
extern unsigned int verbose_mode;  		/**< Verbose when printing results. */
extern int ignore_warning;                      /**< 0 if the fatal warnings stop the program */
extern int nb_instr_thread;                     /**< Number of instructions before changing thread */

/**
 * \brief Fill options variables with program's arguments
 */
void get_options(int argc, char *argv[]);

#endif /* OPTION_H */
