#include "tulip/DrawingTools.h"
#include "tulip/Graph.h"
#include "tulip/LayoutProperty.h"
#include "tulip/SizeProperty.h"
#include "tulip/DoubleProperty.h"
#include "tulip/BooleanProperty.h"
#include <climits>

using namespace std;
using namespace tlp;
namespace {
  Coord maxCoord(const Coord &a, const Coord &b) {
    return Coord(std::max(a[0], b[0]),
		 std::max(a[1], b[1]),
		 std::max(a[2], b[2]));
  }

  Coord minCoord(const Coord &a, const Coord &b) {
    return Coord(std::min(a[0], b[0]),
		 std::min(a[1], b[1]),
		 std::min(a[2], b[2]));
  }

  void rotate(Coord &vec, double alpha) {
    Coord backupVec(vec);
    double zRot =  - 2.0*M_PI * alpha / 360.0;
    float cosz = cos(zRot);
    float sinz = sin(zRot);
    vec[0] = backupVec[0]*cosz - backupVec[1]*sinz;
    vec[1] = backupVec[0]*sinz + backupVec[1]*cosz;
  }

  void computePoint(pair<Coord, Coord> &boundingbox, const Coord &point, const Size & size, const double & rot) {
    //rotate size
    vector<Coord> points(4);
    points[0].set(size[0], size[1], size[2]);
    points[1].set(-size[0], -size[1], -size[2]);
    points[2].set(+size[0], -size[1], -size[2]);
    points[3].set(-size[0], +size[1], size[2]);
    for (unsigned int i=0; i<4; ++i) {
      rotate(points[i], rot);
      points[i] += point;
      boundingbox.first = maxCoord(boundingbox.first, points[i]);
      boundingbox.second =minCoord(boundingbox.second, points[i]);
    }
  }
  
  void computeRotatedPoints(vector<Coord> &rotatedPoints, 
			    const Coord &point, const Size & size, 
			    const double & rot) {
    //rotate size
    rotatedPoints.clear();
    rotatedPoints.resize(4);
    rotatedPoints[0].set(size[0], size[1], size[2]);
    rotatedPoints[1].set(-size[0], -size[1], -size[2]);
    rotatedPoints[2].set(+size[0], -size[1], -size[2]);
    rotatedPoints[3].set(-size[0], +size[1], size[2]);
    for (unsigned int i=0; i<4; ++i) {
      rotate(rotatedPoints[i], rot);
      rotatedPoints[i] += point;
    }
  }
}

pair<Coord, Coord> tlp::computeBoundingBox(Graph *graph, LayoutProperty *layout, SizeProperty *size, DoubleProperty *rotation, BooleanProperty *selection) {
  Coord curCoord;
  Size  curSize;
  double curRot;
  pair<Coord, Coord> result;
  if (graph->numberOfNodes()==0) {
    result.first.set(0, 0, 0);
    result.second.set(0, 0, 0);
    return result;
  }
  result.first.set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
  result.second.set(FLT_MAX, FLT_MAX, FLT_MAX);
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();

    if (selection == 0 || selection->getNodeValue(itn)) {
	curCoord = layout->getNodeValue(itn);
	curSize  = size->getNodeValue(itn) / 2.0;
	curRot = rotation->getNodeValue(itn);
	computePoint(result, curCoord, curSize, curRot);
      }
  } delete itN;

  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();

    if (selection == 0 || selection->getEdgeValue(ite)) {
	LineType::RealType::const_iterator itCoord;
	const LineType::RealType &bends = layout->getEdgeValue(ite);
	for (itCoord = bends.begin(); itCoord!=bends.end();++itCoord) {
	  result.first = maxCoord(result.first, *itCoord);
	  result.second = minCoord(result.second, *itCoord);
	}
    }
  } delete itE;
  return result;
}

pair<Coord, Coord> tlp::computeBoundingRadius(Graph *graph, 
					      LayoutProperty *layout, 
					      SizeProperty *size, 
					      DoubleProperty *rotation, 
					      BooleanProperty *selection) {
  Coord curCoord;
  Size  curSize;
  double curRot;
  pair<Coord, Coord> result;
  result.first.set(0, 0, 0);
  result.second.set(0, 0, 0);
  if (graph->numberOfNodes()==0) return result;
  pair <Coord, Coord> boundingBox = 
    tlp::computeBoundingBox (graph, layout, size, rotation, selection);
  Coord centre = (boundingBox.first + boundingBox.second)/2.0;
  result.first = result.second = centre;
  
  double maxRad = 0;
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn = itN->next();
    curCoord = layout->getNodeValue(itn);
    curSize  = size->getNodeValue(itn) / 2.0;
    curRot = rotation->getNodeValue(itn);
    if (selection == 0 || selection->getNodeValue(itn)) {
      double nodeRad = sqrt (curSize.getW()*curSize.getW() +
			     curSize.getH()*curSize.getH());
      Coord radDir = curCoord - centre;
      double curRad = nodeRad + radDir.norm();
      if (radDir.norm() < 1e-6) {
	curRad = nodeRad;
	radDir = Coord (1.0,0.0,0.0);
      }
      if (curRad > maxRad) {
	maxRad = curRad;
	radDir /= radDir.norm();
	radDir *= curRad;
	result.second = radDir + centre;	
      }
    }//end if
  } delete itN;

  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    if (selection == 0 || selection->getEdgeValue(ite)) {
      LineType::RealType::const_iterator itCoord;
      const LineType::RealType &bends = layout->getEdgeValue(ite);
      for (itCoord = bends.begin(); itCoord!=bends.end();++itCoord) {
	double curRad = (*itCoord - centre).norm();
	if (curRad > maxRad) {
	  maxRad = curRad;
	  result.second = *itCoord;
	}//end if
      }//end for
    }//end if
  } delete itE;
  return result;
}
