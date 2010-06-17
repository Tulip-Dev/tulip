/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "tulip/Hline.h"
#include "tulip/Renderer.h"

using namespace tlp;

const int HAUTEUR=5; // défini l'espace avant et après la ligne horizontale

void Hline::draw(float w_max, float& w)const{
  renderer.setColor(couleur[0], couleur[1], couleur[2]);
  renderer.translate(0,-HAUTEUR,0); 
  renderer.drawLine(0, 0, 0, w_max, 0, 0);
  renderer.translate(0,-HAUTEUR,0);
  w = w_max;
}

void Hline::getBoundingBox(float w_max, float& h, float& w)const{
  h = 2*HAUTEUR;
  //  w = w_max;
  w = 1.0; 
}

void Hline::getColor(unsigned char& r, unsigned char& v, unsigned char& b) const{
  r = couleur[0]; v = couleur[1]; b = couleur[2];
}
void Hline::setColor(unsigned char r, unsigned char v, unsigned char b){
  couleur[0] = r; couleur[1] = v; couleur[2] = b;
}
