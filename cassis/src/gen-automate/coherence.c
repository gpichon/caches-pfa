#include "coherence.h"

void coherence_up_stat(struct coherence *this, struct node *node, unsigned long entry, int stats_type){
  struct cache *cache = get_cache(node);
  up_stat(cache, entry, stats_type);
} 	


void coherence_invalid_line(struct coherence *this, struct line *line) {
  line->status = I;
}

void coherence_modify_line(struct coherence *this, struct line *line) {
  line->status = M;
}

void coherence_share_line(struct coherence *this, struct line *line) {
  line->status = S;
}

void coherence_exclusive_line(struct coherence *this, struct line *line) {
  line->status = E;
}

void coherence_forward_line(struct coherence *this, struct line *line) {
  line->status = F;
}

void coherence_owned_line(struct coherence *this, struct line *line) {
  line->status = O;
}

void coherence_dirty_line(struct coherence *this, struct line *line, int w){
  line->dirty = w;
}


void coherence_init(struct coherence *this, enum cache_coherence coher)
{
  coherenceContext_Init(&this->_fsm, this);
  switch (coher){
  case MSI:
    coherenceContext_MSI(&this->_fsm);
    break;
  case MESI:
    coherenceContext_MESI(&this->_fsm);
    break;
  case MOSI:
    coherenceContext_MOSI(&this->_fsm);
    break;
  case MOESI: 
    coherenceContext_MOESI(&this->_fsm);
    break;
  case MESIF:
    coherenceContext_MESIF(&this->_fsm);
    break;
  default:
    coherenceContext_MSI(&this->_fsm);
    break;
  }
}
