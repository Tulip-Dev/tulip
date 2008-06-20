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

#ifndef PARALLELCOORDINATESGRAPHPROXY_H_INCLUDE
#define PARALLELCOORDINATESGRAPHPROXY_H_INCLUDE

#include <list>
#include <string>
#include <tulip/GraphDecorator.h>
#include <tulip/Color.h>
#include <tulip/Size.h>

#include "ParallelTools.h"



using namespace std;

namespace tlp {

class ParallelCoordinatesGraphProxy : public tlp::GraphDecorator {

public :

  ParallelCoordinatesGraphProxy(tlp::Graph *graph, DataLocation location=NODES);
  
  int getNumberOfSelectedProperties();

  list<string> getAllProperties();

  list<string> getSelectedProperties();

  void setSelectedProperties(list<string>& l);

  void fillVectorWithData(void *pVector, string property);

  DataLocation getDataLocation();

  void setDataLocation(DataLocation location) {dataLocation = location;}

  unsigned int getDataCount();

  Color getDataColor(unsigned int dataId);

  string getDataTexture(unsigned int dataId);

  Size getDataViewSize(unsigned int dataId);

  unsigned int getDataMaxId();

  bool isDataSelected(unsigned int dataId);

  void setDataSelected(unsigned int dataId, bool dataSelected);

  void pushObserver(GraphObserver *obs);

  void popObserver(GraphObserver *obs);

private:

  void fillPropertiesVector();

  list<string> selectedProxy;
  list<string> propertiesList;
  DataLocation dataLocation;
};

}

#endif // PARALLELCOORDINATESGRAPHPROXY_H_INCLUDE
