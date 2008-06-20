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

#include "NominalParallelAxis.h"

using namespace std;

namespace tlp {

NominalParallelAxis::NominalParallelAxis(Coord base_coord, float height, string name, vector<string> *labels, viewType vType) : 
  ParallelAxis(base_coord, height, name, labels->size(), vType) {
  setLabels(labels);
}

void 
NominalParallelAxis::setLabels(vector<string> *labels) {

  unsigned int idx = 1;
  unsigned int nbLabs = 0;
  
  if (_labels.size() > 0) {
    for (unsigned int i = 0; i < _labels.size(); ++i) {
      deleteGlEntity(_labels[i]);
    }
    _dataCoords.clear();
  }

  for (unsigned int i = 0 ; i < labels->size() ; ++i) {
    _labels.push_back((*labels)[i]);
    if ((*labels)[i] != "") {
      _labelsCoords[(*labels)[i]] = Coord(0,0,0);
    }
  }
  
  nbLabs = _labelsCoords.size();
  float ySpace = _height/(nbLabs + 1);
  map<string, Coord>::iterator it2;
  for(it2 = _labelsCoords.begin() ; it2 != _labelsCoords.end() ; ++it2) {
    float xCoord = _baseCoord.getX();
    float yCoord = _baseCoord.getY() + (idx++)*ySpace;
    
    addLabelDrawing(it2->first, yCoord);
    it2->second = Coord(xCoord, yCoord);

  }
}

Coord NominalParallelAxis::getPointCoordOnAxisForData(const unsigned int dataIdx) {
  return _labelsCoords[_labels[dataIdx]];
}

void NominalParallelAxis::translate(const Coord &c) {
  setBaseCoord(_baseCoord + c);
  map<string, Coord>::iterator it;
  for (it = _labelsCoords.begin() ; it !=_labelsCoords.end() ; ++it) {
    it->second += c;
  }
  GlComposite::translate(c);
}

}
