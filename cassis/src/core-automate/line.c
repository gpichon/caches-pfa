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
struct line** init_lines(int nb_ways, enum cache_coherence type) {
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
    struct coherence *policy = malloc(sizeof(struct coherence));
    line->coher = policy;
    coherence_init(policy,type);
    lines[i] = line;
  }
  return lines;
}

/* Data removal */
void delete_lines(struct line **lines, int nb_ways) {
  int i;
  for (i=0; i<nb_ways; i++) {
    free(lines[i]->coher);
    free(lines[i]);
  }
  free(lines);
}

bool is_valid(struct line *line) {
  return (line->status > 0);
}

bool is_dirty(struct line *line) {
  return (line->status == M); 	/* TODO delete */
  return line->dirty;
}
