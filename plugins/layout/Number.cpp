#include "Number.h"

float Number::infini=-1 ;


bool Number::operator > (float b)
{
  if(b==Number::infini)
    return false;

  if(this->value == Number::infini) 
    return true; 
  else return (this->value > b);
} 
