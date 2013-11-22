#include "line.h"

/* Data allocations */
struct line** init_line(int nb_ways) {
  struct line **lines = malloc(nb_ways * sizeof(struct line *));
  int i;
  for (i=0; i<nb_ways; i++) {
    struct line *line = malloc(sizeof(struct line));
    line->first_case = 0;
    line->use = 0;
    line->status = 0;
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

void update_line(struct line *line) {
  line->use ++;
}

void invalid_line(struct line *line) {
  line->status = 0;
}

void modify_line(struct line *line) {
  line->status = 3;
}

void share_line(struct line *line) {
  line->status = 1;
}

void exclusive_line(struct line *line) {
  line->status = 2;
}

int is_valid(struct line *line) {
  return (line->status > 0);
}

int is_exclusive(struct line *line) {
  return (line->status == 2);
}

int is_modified(struct line *line) {
  return (line->status == 3);
}

int is_shared(struct line *line) {
  return (line->status == 1);
}

