/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

LAYOUTPLUGINOFGROUP(Random,"Random","David Auber","01/12/1999","Ok","1.0","Basic");

using namespace std;
using namespace tlp;

Random::Random(const PropertyContext &context):LayoutAlgorithm(context){}

Random::~Random() {}

bool Random::run() {
  layoutResult->setAllEdgeValue(vector<Coord>(0));
  graph->getLocalProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    layoutResult->setNodeValue(itn,Coord(rand()%1024,rand()%1024,rand()%1024));
  } delete itN;
  return true;
}

bool Random::check(string &erreurMsg) {
  erreurMsg="";
  return (true);
}
