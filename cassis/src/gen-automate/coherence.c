#include "coherence.h"

int is_dirty(){
  return 0;
}

int exist_somewhere() {
  return 0;
}

int exist_state(char state) {
  switch (state){
  case 'O':
    return 0;
    break;
  case 'M':
    return 0;
    break;
  case 'S':
    return 0;
    break;
  case 'I':
    return 0;
    break;
  case 'E':
    return 0;
    break;
  case 'F':
    return 0;
    break;
  default:
    return 0;
    break;
  }
}


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
  case 2: //MOSI
    coherenceContext_MOSI(&this->_fsm);
    break;
  case 3: //MOESI
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

