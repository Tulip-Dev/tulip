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
#include <tulip/Polynome.h>
#include <tulip/tulipconf.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace tlp;

#define PI 3.14159265

Polynome::Polynome() {
  a = 0;
  b = 0;
  c = 0;
  d = 0;
}

Polynome::Polynome(float a, float b, float c, float d) {
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
}

Polynome::~Polynome() {
}

void Polynome::resolv(float result[3], int &nResult) {
  // Thanks to Mr. Serge Mehl for his webpage on
  // the third degree equations :
  // http://serge.mehl.free.fr/anx/equ_deg3.html
    
  float h, q, p;
    
  h =  -(b / (3 * a));
  p =   ((c / a) - ((b * b) / (3 * a * a)));
  q =   (((2 * b * b * b) / (27 * a * a * a)) + (d / a) - ((b * c) / (3 * a * a)));
    
  float Delta = ((q * q) / 4) + ((p * p * p) / 27);
  float u, v;
    
  if (Delta > 0)
    {
      u = cbrt((-q / 2) - sqrt(Delta));
      v = cbrt((-q / 2) + sqrt(Delta));
	
      result[0] = u + v + h; 
	
      nResult = 1;
    } 
  else 
    { 
      float k = 0;
      float r = 0;
	
      if (p != 0)
	{
	  k = -q / (2 * sqrt(-p * p * p / 27));
	  r = sqrt(-p / 3);
	}
	
      float alpha = 0;
	
      if (abs((int) k) == 1)
	alpha = - PI * (k - 1) / 2;
      else 
	alpha = acos(k); 
	
      for(int i=0; i < 3; i++)
	result[i] = 2 * r * cos((alpha + 2 * i * PI) / 3.0) + h;
	
      nResult = 3;
    } 
}
