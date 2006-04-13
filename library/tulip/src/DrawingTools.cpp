#include "tulip/DrawingTools.h"
#include "tulip/SuperGraph.h"
#include "tulip/Layout.h"
#include "tulip/Sizes.h"
#include "tulip/Metric.h"
#include "tulip/Selection.h"
#include <climits>

using namespace std;
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
}

pair<Coord, Coord> tlp::computeBoundingBox(SuperGraph *graph, Layout *layout, Sizes *size, Metric *rotation, Selection *selection) {
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




