//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 24/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_BOUNDINGBOX_H
#define Tulip_BOUNDINGBOX_H

#include <utility>

#include "tulip/Coord.h"

namespace tlp {

  struct BoundingBox : public std::pair<Coord,Coord> {
  
    BoundingBox():isValid(false) {}
    BoundingBox(const Coord& min,const Coord& max) : isValid(true) {
      first=min;
      second=max;
    }
    
    void check(const Coord& coord) {
      if(!isValid) {
	first=coord;
	second=coord;
	isValid=true;
      }else {
	if(coord[0]<first[0])
	  first[0]=coord[0];
	if(coord[1]<first[1])
	  first[1]=coord[1];
	if(coord[0]>second[0])
	  second[0]=coord[0];
	if(coord[1]>second[1])
	  second[1]=coord[1];
      }
    }

    bool isValid;
  };
  
}

#endif // Tulip_BOUNDINGBOX_H
