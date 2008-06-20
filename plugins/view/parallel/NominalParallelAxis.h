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

#ifndef NOMINAL_PARALLEL_AXIS_H
#define NOMINAL_PARALLEL_AXIS_H

#include "ParallelAxis.h"

namespace tlp {

class NominalParallelAxis : public ParallelAxis {
  
 public :
  
  NominalParallelAxis(Coord base_coord, float height, std::string name, std::vector<std::string> *labels, viewType vType=VIEW_3D);
  
  Coord getLabelCoord(std::string label);
  
  Coord getPointCoordOnAxisForData(const unsigned int dataIdx);

  void translate(const Coord &c);

private :
  
  void setLabels(std::vector<std::string> *labels);
  
  std::map<std::string, Coord> _labelsCoords; 

};

}

#endif
