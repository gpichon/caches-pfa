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
  int first_case;
  int use;

  int valid;
  int writed;
  int shared; //1 if shared, 0 if exclusive
};

/* Data allocation */
struct line** init_line(int);

/* Data removal */
void delete_lines(struct line **, int);

void update_line(struct line *);
void invalid_line(struct line *);
void modify_line(struct line *);
void share_line(struct line *);
void exclusive_line(struct line *);

#endif
