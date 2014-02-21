/**
 * \file line.c
 * \brief Manage line : initialization, removal, return informations.
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */

#include "line.h"

/* Data allocations */
struct line** init_lines(int nb_ways) {
  struct line **lines = malloc(nb_ways * sizeof(struct line *));
  int i;
  for (i=0; i<nb_ways; i++) {
    struct line *line = malloc(sizeof(struct line));
    line->first_case = 0;
    line->use = 0;
    line->status = 0;
    line->priority = 0;
    lines[i] = line;
  }
  return lines;
}

/* Data removal */
void delete_lines(struct line **lines, int nb_ways) {
  int i;
  for (i=0; i<nb_ways; i++) {
    free(lines[i]);
  }
  free(lines);
}

void invalid_line(struct line *line) {
  line->status = I;
}

void modify_line(struct line *line) {
  line->status = M;
}

void share_line(struct line *line) {
  line->status = S;
}

void exclusive_line(struct line *line) {
  line->status = E;
}

bool is_valid(struct line *line) {
  return (line->status > 0);
}

bool is_exclusive(struct line *line) {
  return (line->status == E);
}

bool is_modified(struct line *line) {
  return (line->status == M);
}

bool is_shared(struct line *line) {
  return (line->status == S);
}
