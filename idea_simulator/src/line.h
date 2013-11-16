#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
  Module utilisé pour la gestion d'une ligne.
  Gère lecture/écriture au sein d'une ligne.
*/

struct line {
  int label;
  int first_case;
  int valid;
  int use;
  int writed;
};

/* Data allocation */
struct line** init_line(int);

/* Data removal */
void delete_lines(struct line **, int);

void update_line(struct line *);

#endif
