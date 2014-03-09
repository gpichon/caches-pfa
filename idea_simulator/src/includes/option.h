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

/**
 * \def UP_TRACKING_HITS()
 * \brief Count one hit in tracking stats.
 */
#define UP_TRACKING_HITS() tracking_hits++
/**
 * \def UP_TRACKING_MISSES()
 * \brief Count one miss in tracking stats.
 */
#define UP_TRACKING_MISSES() tracking_misses++
/**
 * \def UP_TRACKING_WRITE_BACKS()
 * \brief Count one write back in tracking stats.
 */
#define UP_TRACKING_WRITE_BACKS() tracking_write_backs++
/**
 * \def UP_TRACKING_BROADCASTS()
 * \brief Count one broadcast in tracking stats.
 */
#define UP_TRACKING_BROADCASTS() tracking_broadcasts++

extern int tracking_misses;      	/**< Count of misses for tracking. */
extern int tracking_hits;        	/**< Count of hits for tracking. */
extern int tracking_write_backs; 	/**< Count of write backs for tracking. */
extern int tracking_broadcasts;  	/**< Count of broadcasts for tracking. */
extern int tracking_type;	/**< Tracking type used during the simulation. */
extern int tracking_lower_bound;	/**< Lower bound of address tracking. */
extern int tracking_upper_bound;	/**< Upper bound of address tracking. */
extern int help;					/**< help=1 when -h flag is set */
extern char *trace_file;			/**< The artecture configuration file to use. If not set, a default file is used. */

/**
 * \brief Fill options variables with program's arguments
 */
void get_options(int argc, char *argv[]);

#endif /* OPTION_H */
