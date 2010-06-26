/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <assert.h>
#include "Random.h"

const char * paramHelp[] = {
  // 3D
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Boolean" ) \
  HTML_HELP_BODY() \
  "If true the layout is in 3D else it is computed in 2D" \
  HTML_HELP_CLOSE()
};

LAYOUTPLUGINOFGROUP(Random,"Random","David Auber","01/12/1999","Ok","1.1","Basic");

using namespace std;
using namespace tlp;

Random::Random(const PropertyContext &context):LayoutAlgorithm(context){
  addParameter<bool>("3D layout", paramHelp[0], "true");
}

Random::~Random() {}

bool Random::run() {
  bool is3D = true ;
  if ( dataSet!=0 ) {
    dataSet->get("3D layout", is3D);
  }

  layoutResult->setAllEdgeValue(vector<Coord>(0));
  graph->getLocalProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if(is3D)
      layoutResult->setNodeValue(itn,Coord(rand()%1024,rand()%1024,rand()%1024));
    else
      layoutResult->setNodeValue(itn,Coord(rand()%1024,rand()%1024,0));
  } delete itN;
  return true;
}

bool Random::check(string &erreurMsg) {
  erreurMsg="";
  return (true);
}
