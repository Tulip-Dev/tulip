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

#ifndef PARALLEL_AXIS_BASE_H
#define PARALLEL_AXIS_BASE_H

#include <tulip/Graph.h>
#include <tulip/GlComposite.h>
#include <tulip/GlScene.h>
#include <vector>

#include "ParallelAxis.h"
#include "ParallelCoordinatesGraphProxy.h"
#include "ParallelTools.h"

using namespace std;
namespace tlp {

class ParallelCoordinatesView : public GlComposite {
  
public :

  ParallelCoordinatesView(ParallelCoordinatesGraphProxy *graphProxy, Coord begin, unsigned int width, unsigned int height, viewType vType=VIEW_3D );

  ~ParallelCoordinatesView();

  unsigned int nbParallelAxis(); 
  void swapAxis(string axisName1, string axisName2); 
  ParallelAxis *getAxisByIndex(unsigned int i);

  void deleteDataRepresentation(const unsigned int dataId);
  
  int getDataIdFromGlEntity(GlEntity *glEntity);
  void selectData(const unsigned int dataId);
  void unselectData(const unsigned int dataId);
  
  void setViewType(viewType vType);
  viewType getViewType() {return _viewType;}
  
  void redrawView();

  const vector<string> getAxisNames();

 private :
  
  void createAxis(bool willChangeAxisOrder=true);
  void plotAllData(); 
  void plotData(unsigned int dataIdx, const Color &color);

  void erase();
  void eraseData();

  int getPos(string name);

  unsigned int _nbAxis;
  Coord _begin;
  unsigned int _width, _height;
  
  vector<string> _axisOrder;
  map<string, ParallelAxis *> _axis;
  
  map<unsigned int, vector<GlEntity *> > _dataPlotElts;
  
  ParallelCoordinatesGraphProxy *_graphProxy;

  viewType _viewType;

  GlGraphInputData *_data;
  GlGraphRenderingParameters _parameters;

};

}



#endif
