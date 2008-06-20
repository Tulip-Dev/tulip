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

#ifndef PARALLEL_AXIS_H
#define PARALLEL_AXIS_H



#include <tulip/GlComposite.h>

#include <vector>
#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include "ParallelTools.h"

namespace tlp {

class ParallelAxis : public GlComposite {
  
 public:

  virtual Coord getPointCoordOnAxisForData(const unsigned int dataIdx);

  virtual ~ParallelAxis();

  Coord getBaseCoord();

  void setBaseCoord(Coord newCoord);

  void translate(const Coord &c);

 protected :
  
  ParallelAxis(Coord baseCoord, float height, std::string name, unsigned int dataCount, viewType vType=VIEW_3D);

  void addLabelDrawing(std::string labelName, float yCoord);
  void addCaption(std::string caption);

  Coord _baseCoord;
  float _height;
  std::string _name;

  std::vector<std::string> _labels;
  std::vector<Coord> _dataCoords;
  std::vector<GlSimpleEntity *> _axisElts;

  viewType _viewType;
  
};

}

#endif
