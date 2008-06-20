//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#include "ParallelTools.h"

#include <vector>

using namespace std;

namespace tlp {

GlLine *createLine(const Coord & source, const Coord & target, 
			  const Color & c_source, const Color & c_target){
  vector<Coord> position;
  position.push_back(source);
  position.push_back(target);
  
  vector<Color> colors;
  colors.push_back(c_source);
  colors.push_back(c_target);

  return new GlLine(position,colors);
}

Coord *computeStraightLineIntersection(const Coord line1[2], const Coord line2[2]) {

  Coord *intersectionPoint = NULL;
  bool line1ParallelToXaxis = false;
  bool line1ParallelToYaxis = false;
  bool line2ParallelToXaxis = false;
  bool line2ParallelToYaxis = false;
  bool parallelLines = false;
  float x, y;
  
  // compute the equation of the first line
  // y = al1 * x + bl1
  float xal1 = line1[0].getX();
  float xbl1 = line1[1].getX();
  float yal1 = line1[0].getY();
  float ybl1 = line1[1].getY();
  float al1;
  float bl1;
  if ((xbl1 - xal1) != 0) {
    al1 = (ybl1 - yal1) / (xbl1 - xal1);
    bl1 = ybl1 - al1 * xbl1;
  } else {
    line1ParallelToXaxis = true; 
  }
  
  // compute the equation of the second line
  // y = al2 * x + bl2
  float xal2 = line2[0].getX();
  float xbl2 = line2[1].getX();
  float yal2 = line2[0].getY();
  float ybl2 = line2[1].getY();
  float al2;
  float bl2;
  if ((xbl2 - xal2) != 0) {
    al2 = (ybl2 - yal2) / (xbl2 - xal2);
    bl2 = ybl2 - al2 * xbl2;
  } else {
    line2ParallelToXaxis = true;
  }
  
  if (!line1ParallelToXaxis && al1 == 0) {
    line1ParallelToYaxis = true;	 
  }
  
  if (!line2ParallelToXaxis && al2 == 0) {
    line2ParallelToYaxis = true;
  }
  
  // compute the intersection point of the two lines if any
  if (line1ParallelToXaxis && line2ParallelToYaxis) {
    x = xal1;
    y = yal2;
  } else if (line2ParallelToXaxis && line1ParallelToYaxis) {
    x = xal2;
    y = yal1;
  } else if (line1ParallelToXaxis && !line2ParallelToXaxis) { 
    x = xal1;
    y = al2 * x + bl2;
  } else if (line2ParallelToXaxis && !line1ParallelToXaxis) { 
    x = xal2;
    y = al1 * x + bl1;
  } else if ((!line1ParallelToXaxis && !line2ParallelToXaxis) &&
	     (!line1ParallelToYaxis && !line2ParallelToYaxis)) {
    
    float d1 = (bl2 - bl1);
    float d2 = (al1 - al2);
    
    if (d2 != 0) {
      x = d1 / d2;
      y = al1 *x + bl1;
    } else {
      parallelLines = true;
    }
  } else {
    parallelLines = true;
  }

  if (!parallelLines) {
    intersectionPoint = new Coord(x, y, 0);
  }
  
  return intersectionPoint;
}

}
