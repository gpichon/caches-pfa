#ifndef __COHERENCE_H__
#define __COHERENCE_H__

#include "coherence_sm.h"


struct coherence
{
	struct coherenceContext _fsm;
};

int is_dirty();

int exist_somewhere();

int exist_state(char);

void coherence_init(struct coherence*, int);

#endif
