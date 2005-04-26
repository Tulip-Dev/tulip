#include "tulip/DrawingTools.h"
#include "tulip/SuperGraph.h"
#include "tulip/LayoutProxy.h"
#include "tulip/SizesProxy.h"

using namespace std;


pair<Coord, Coord> tlp::computeBoundingBox(SuperGraph *graph, LayoutProxy *layout, SizesProxy *size) {
  Coord curCoord;
  Size  curSize;
  double maxX=0;
  double minX=0;
  double maxY=0;
  double minY=0;
  double maxZ=0;
  double minZ=0;
  Iterator<node> *itN=graph->getNodes();
  if  (itN->hasNext()) {
    node itn=itN->next();
    curCoord = layout->getNodeValue(itn);
    curSize  = size->getNodeValue(itn) / 2.0;
    maxX = curCoord[0] + curSize[0];
    minX = curCoord[0] - curSize[0];
    maxY = curCoord[1] + curSize[1];
    minY = curCoord[1] - curSize[1];
    maxZ = curCoord[2] + curSize[2];
    minZ = curCoord[2] - curSize[2];
  }
  while (itN->hasNext()) {
    node itn=itN->next();
    curCoord = layout->getNodeValue(itn);
    curSize  = size->getNodeValue(itn) / 2.0;
    maxX >?= curCoord[0] + curSize[0];
    minX <?= curCoord[0] - curSize[0];
    maxY >?= curCoord[1] + curSize[1];
    minY <?= curCoord[1] - curSize[1];
    maxZ >?= curCoord[2] + curSize[2];
    minZ <?= curCoord[2] - curSize[2];
  } delete itN;
  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    LineType::RealType::const_iterator itCoord;
    const LineType::RealType &bends = layout->getEdgeValue(ite);
    for (itCoord = bends.begin(); itCoord!=bends.end();++itCoord) {
      curCoord = *itCoord;
      maxX >?= curCoord[0];
      minX <?= curCoord[0];
      maxY >?= curCoord[1];
      minY <?= curCoord[1];
      maxZ >?= curCoord[2];
      minZ <?= curCoord[2];
    }
  } delete itE;
  Coord maxi(maxX, maxY, maxZ);
  Coord mini(minX, minY, minZ);
  return pair<Coord, Coord>(maxi, mini);
}
