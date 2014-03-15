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
  assert(lines!=NULL);
  int i;
  for (i=0; i<nb_ways; i++) {
    struct line *line = malloc(sizeof(struct line));
    assert(line!=NULL);
    line->first_case = 0;
    line->use = 0;
    line->status = 0;
    line->priority = 0;
    line->dirty = 0;
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
  line->dirty = 0;
}

void modify_line(struct line *line) {
  line->status = M;
  line->dirty = 1;
}

void share_line(struct line *line) {
  line->status = S;
  line->dirty = 0;
}

void exclusive_line(struct line *line) {
  line->status = E;
  line->dirty = 0;
}

void forward_line(struct line *line) {
  line->status = F;
}

void owned_line(struct line *line) {
  line->status = O;
}

void dirty_line(struct line *line, int w){
  line->dirty = w;
}

bool is_valid(struct line *line) {
  return (line->status > 0);
}

bool is_exclusive(struct line *line) {
  return (line->status == E);
}

bool is_modified(struct line *line) {
  return line->dirty;
}

bool is_shared(struct line *line) {
  return (line->status == S);
}


bool is_dirty(struct line *line) {
  return line->dirty;
}
