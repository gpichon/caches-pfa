#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define ARCH 64

/*
  Module utilisé pour la gestion d'une ligne.
  Gère lecture/écriture au sein d'une ligne.
*/

/* Status: 0 invalid, 1 shared, 2 exclusive, 3 modified */
struct line {
  int first_case;
  int use;
  int status;
};

/* Data allocation: nb_ways new lines */
struct line** init_line(int nb_ways);

/* Data removal */
void delete_lines(struct line **lines, int nb_ways);

/* Update replacement flag */
void update_FIFO(struct line *line, int entry);
void update_LFU(struct line *line, int entry);

/* Setters */
void invalid_line(struct line *line);
void modify_line(struct line *line);
void share_line(struct line *line);
void exclusive_line(struct line *line);

/* Getters */
int is_valid(struct line *line);
int is_exclusive(struct line *line);
int is_modified(struct line *line);
int is_shared(struct line *line);

#endif
