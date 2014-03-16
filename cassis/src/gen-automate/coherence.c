#include "coherence.h"

void coherence_init(struct coherence *this, int coher)
{
  coherenceContext_Init(&this->_fsm, this);
  switch (coher){
  case 0: //MSI
    coherenceContext_MSI(&this->_fsm);
    break;
  case 1: //MESI
    coherenceContext_MESI(&this->_fsm);
    break;
  case 2: //MOESI
    coherenceContext_MOSI(&this->_fsm);
    break;
  case 3: //MOSI
    coherenceContext_MOESI(&this->_fsm);
    break;
  case 4: //MESIF
    coherenceContext_MESIF(&this->_fsm);
    break;
  default:
    coherenceContext_MSI(&this->_fsm);
    break;
  }
}

void foo(){
  coherenceContext_a_aread();
}
