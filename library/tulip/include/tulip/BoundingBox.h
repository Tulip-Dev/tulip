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
#include <limits>

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
	if(coord[2]<first[2])
	  first[2]=coord[2];
	if(coord[0]>second[0])
	  second[0]=coord[0];
	if(coord[1]>second[1])
	  second[1]=coord[1];
	if(coord[2]>second[2])
	  second[2]=coord[2];
      }
    }

    void getCompleteBB(Coord* bb) const{
      bb[0]=first;
      bb[1]=first;
      bb[1][0]=second[0];
      bb[2]=bb[1];
      bb[2][1]=second[1];
      bb[3]=first;
      bb[3][1]=second[1];
      bb[4]=bb[0];
      bb[4][2]=second[2];
      bb[5]=bb[1];
      bb[5][2]=second[2];
      bb[6]=bb[2];
      bb[6][2]=second[2];
      bb[7]=bb[3];
      bb[7][2]=second[2];
    } 

    bool isValid;
  };
  
}

#endif // Tulip_BOUNDINGBOX_H
