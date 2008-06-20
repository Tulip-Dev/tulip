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

#ifndef QUANTITATIVE_PARALLEL_AXIS
#define QUANTITATIVE_PARALLEL_AXIS

#include "ParallelAxis.h"
#include <vector>
#include <algorithm>
#include <string>
#include "ParallelTools.h"

namespace tlp {

template <typename T>
class QuantitativeParallelAxis : public ParallelAxis {

public :
  
  QuantitativeParallelAxis(Coord baseCoord, float height, string name, vector<T> *values, bool crescent = true, viewType vType=VIEW_3D); 
    
 private:
  
  void setLabels(vector<T> &values, bool crescent);
  void computeDataPointsCoord(vector<T> *values);

  T begin;
  T end;
  
  double scale;
  T increment;
  bool crescent;
  
  void bestFit(T min, T max);
  
};

#include "QuantitativeParallelAxis.cxx"

}

#endif
