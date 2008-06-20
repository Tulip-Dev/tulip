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

#ifndef   	PARALLELTOOLS_H_
#define   	PARALLELTOOLS_H_

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/GlLine.h>

namespace tlp {

GlLine *createLine(const Coord & source, const Coord & target, 
		   const Color & c_source, const Color & c_target);


// compute the intersection point of two lines in the xOy plane
// return NULL if the lines are parallel
Coord *computeStraightLineIntersection(const Coord line1[2], const Coord line2[2]);

enum viewType {VIEW_2D, VIEW_3D, VIEW_2D_SPLINE};
enum DataLocation {NODES,EDGES};

const Color colorSelect2 = Color(255, 102, 255, 255);

}

#endif 	    /* !PARALLELTOOLS_H_ */
