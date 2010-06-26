/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
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
namespace tlp {
  bool operator==(Context& a, Context& b){
    return ( (a.size == b.size) && // taille
	     (a.c[0] == b.c[0]) && // rouge
	     (a.c[1] == b.c[1]) && // vert
	     (a.c[2] == b.c[2]) && //bleu
	     (a.font_name == b.font_name) );
  }
}
//---------------------------------------------------------------------------
Renderer& Context::getRenderer() const {
  return *renderer;
}
