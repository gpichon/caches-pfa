#ifndef COHERENCE_H
#define COHERENCE_H

#include "common_types.h"
#include "node.h"

/* Setters */
void cohrence_invalid_line(struct line *line);
void cohrence_modify_line(struct line *line);
void cohrence_share_line(struct line *line);
void cohrence_exclusive_line(struct line *line);
void cohrence_forward_line(struct line *line);
void cohrence_owned_line(struct line *line);
void cohrence_dirty_line(struct line *line, int w);


void coherence_init(struct coherence*, enum cache_coherence);

#endif
