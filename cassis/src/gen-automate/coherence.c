#include "coherence.h"

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
