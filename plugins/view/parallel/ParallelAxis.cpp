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

#include "ParallelAxis.h"

#include <tulip/GlLabel.h>
#include <tulip/GlBox.h>

#include <tulip/TlpTools.h>


#define BITMAPS_PATH TulipBitmapDir

using namespace std;

namespace tlp {

ParallelAxis::ParallelAxis(Coord baseCoord, float height, string name, unsigned int dataCount, viewType vType) : 
  _baseCoord(baseCoord), _height(height), _name(name) , _viewType(vType) {
  _dataCoords.resize(dataCount);
  

  GlSimpleEntity *axis;
  if (_viewType == VIEW_2D || _viewType == VIEW_2D_SPLINE) {
    axis = createLine(_baseCoord, 
		      Coord(_baseCoord.getX(), _baseCoord.getY() + _height),
		      Color(0, 0, 0), Color(0, 0, 0));   
    
  } else {

    axis = new GlBox(Coord(_baseCoord.getX() - 1, _baseCoord.getY() + height, 1), 
		     Coord(_baseCoord.getX() + 1, _baseCoord.getY(), -1), 
		     Color(0,0,0));
  }
  
  addGlEntity(axis, name + " axis");
  _axisElts.push_back(axis);
  addCaption(name);
  
}

ParallelAxis::~ParallelAxis() {
  vector<GlSimpleEntity *>::iterator it;
  for (it = _axisElts.begin() ; it != _axisElts.end() ; ++it) {
    deleteGlEntity(*it);
    delete *it;
  }
}

void 
ParallelAxis::addLabelDrawing(string labelName, float yCoord) {
  float xCoord = _baseCoord.getX();

  float z;
  GlSimpleEntity *line1;
  GlSimpleEntity *line2;
  if (_viewType == VIEW_2D || _viewType == VIEW_2D_SPLINE) {

    line1 = createLine(Coord(xCoord - 3, yCoord), Coord(xCoord + 3, yCoord),
			   Color(0, 0, 0), Color(0, 0, 0));
    line2 = createLine(Coord(xCoord, yCoord, -3), Coord(xCoord, yCoord, 3), Color(0,0,0), Color(0,0,0));
    z = 1;
  } else {
    line1 = new GlBox(Coord(xCoord - 5, yCoord + 1, 1), Coord(xCoord + 5, yCoord - 1, -1), Color(0,0,0, 100));
    line2 = new GlBox(Coord(xCoord - 1, yCoord + 1, 5), Coord(xCoord + 1, yCoord -1, -5), Color(0,0,0, 100));
    z = 1;
  }
  
  addGlEntity(line1, "label line " + labelName);
  addGlEntity(line2, "2nd label line " + labelName);
  _axisElts.push_back(line1);
  _axisElts.push_back(line2);
  float labelWidth = 2.5*labelName.length();
  GlLabel *lab = new GlLabel(BITMAPS_PATH,
			     Coord(xCoord + (labelWidth*1.5)/2. + 5, yCoord, z), Coord (labelWidth*1.5, 10), Color (0,0,0));
  lab->setText(labelName);
  addGlEntity(lab, "label text " + labelName);
  _axisElts.push_back(lab);
}

void ParallelAxis::addCaption(string caption) {
  float captionWidth = 2.5*caption.size();
  GlLabel *lab = new GlLabel(BITMAPS_PATH,
			     Coord(_baseCoord.getX(), _baseCoord.getY() - 10), Coord (captionWidth *1.5, 15), Color (0,0,0));
  lab->setText(caption);
  addGlEntity(lab, "caption " + caption);
  _axisElts.push_back(lab);
}

Coord
ParallelAxis::getPointCoordOnAxisForData(const unsigned int dataIdx) {
  return _dataCoords[dataIdx];
}

Coord
ParallelAxis::getBaseCoord(){
  return _baseCoord;
}

void
ParallelAxis::setBaseCoord(Coord newCoord){
  _baseCoord = newCoord;
}

void ParallelAxis::translate(const Coord &c) {
  
  setBaseCoord(_baseCoord + c);
  for (unsigned int i = 0 ; i < _dataCoords.size() ; ++i) {
    _dataCoords[i] += c;
  }
  GlComposite::translate(c);
  
}

}
