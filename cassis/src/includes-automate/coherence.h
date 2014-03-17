#ifndef COHERENCE_H
#define COHERENCE_H

#include "common_types.h"
#include "node.h"
#include "cache.h"

void coherence_init(struct coherence*, enum cache_coherence);

#endif
