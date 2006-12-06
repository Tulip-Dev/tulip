#include "tulip/Context.h"

#include <iostream>
using namespace std;
using namespace tlp;

//***************************************************************************
// CLASSE Context
//***************************************************************************
//---------------------------------------------------------------------------
Context::Context(string str, int s, unsigned char r, unsigned char v, unsigned char b){
  font_name = str; size = s;
  c[0] = r;  c[1] = v;  c[2] = b;
}
//---------------------------------------------------------------------------
void Context::getColor(unsigned char& r, unsigned char& v, unsigned char& b) const{
  r = c[0];  v = c[1];  b = c[2];
}
//---------------------------------------------------------------------------
bool tlp::operator==(Context& a, Context& b){
  if( (a.size == b.size) && // taille
      (a.c[0] == b.c[0]) && // rouge
      (a.c[1] == b.c[1]) && // vert
      (a.c[2] == b.c[2]) && //bleu
      (a.font_name == b.font_name) )
    return true;
  else
    return false;
}
//---------------------------------------------------------------------------
Renderer& Context::getRenderer() const {
  return *renderer;
}
