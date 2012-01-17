#include <tulip/Algorithm.h>
#include <tulip/LayoutProperty.h>
#include <tulip/Graph.h>
#include <tulip/MethodFactory.h>

#include "cdt.h"

class Delaunay : public tlp::Algorithm {

public :

  Delaunay(tlp::AlgorithmContext context) : Algorithm(context) {}

  bool run() {
    tlp::Observable::holdObservers();
    tlp::Graph* delaunaySubGraph = graph->addCloneSubGraph("Delaunay");

    tlp::edge e;
    forEach(e, delaunaySubGraph->getEdges()) {
      delaunaySubGraph->delEdge(e);
    }

    std::vector<p2t::Point*> outline(4);

    std::map<p2t::Point*, tlp::node> points;
    tlp::LayoutProperty* layout = delaunaySubGraph->getProperty<tlp::LayoutProperty>("viewLayout");

    outline[0] = new p2t::Point(layout->getMin().getX(), layout->getMin().getY());
    outline[1] = new p2t::Point(layout->getMin().getX(), layout->getMax().getY());
    outline[2] = new p2t::Point(layout->getMax().getX(), layout->getMax().getY());
    outline[3] = new p2t::Point(layout->getMax().getX(), layout->getMin().getY());

    tlp::node tempNode0 = delaunaySubGraph->addNode();
    tlp::node tempNode1 = delaunaySubGraph->addNode();
    tlp::node tempNode2 = delaunaySubGraph->addNode();
    tlp::node tempNode3 = delaunaySubGraph->addNode();
    layout->setNodeValue(tempNode0, tlp::Coord(layout->getMin()));
    layout->setNodeValue(tempNode1, tlp::Coord(layout->getMin().getX(), layout->getMax().getY()));
    layout->setNodeValue(tempNode2, tlp::Coord(layout->getMax()));
    layout->setNodeValue(tempNode3, tlp::Coord(layout->getMax().getX(), layout->getMin().getY()));
    points.insert(std::pair<p2t::Point*, tlp::node>(outline[0], tempNode0));
    points.insert(std::pair<p2t::Point*, tlp::node>(outline[1], tempNode1));
    points.insert(std::pair<p2t::Point*, tlp::node>(outline[2], tempNode2));
    points.insert(std::pair<p2t::Point*, tlp::node>(outline[3], tempNode3));

    p2t::CDT triangulator(outline);
    tlp::node n;
    forEach(n, delaunaySubGraph->getNodes()) {
      tlp::Coord nodeCoord = layout->getNodeValue(n);

      if(n != tempNode0 && n != tempNode1 && n != tempNode2 && n != tempNode3) {
        p2t::Point* point = new p2t::Point(nodeCoord.getX(), nodeCoord.getY());
        points.insert(std::pair<p2t::Point*, tlp::node>(point, n));
        triangulator.AddPoint(point);
      }
    }

    triangulator.Triangulate();

    std::vector<p2t::Triangle*> triangles = triangulator.GetTriangles();

    for(std::vector<p2t::Triangle*>::const_iterator it = triangles.begin(); it != triangles.end(); ++it) {
      p2t::Point* p0 = (*it)->GetPoint(0);
      p2t::Point* p1 = (*it)->GetPoint(1);
      p2t::Point* p2 = (*it)->GetPoint(2);

      tlp::node n0 = points[p0];
      tlp::node n1 = points[p1];
      tlp::node n2 = points[p2];

      if(!delaunaySubGraph->existEdge(n0, n1).isValid()) {
        delaunaySubGraph->addEdge(n0, n1);
      }

      if(!delaunaySubGraph->existEdge(n0, n2).isValid()) {
        delaunaySubGraph->addEdge(n0, n2);
      }

      if(!delaunaySubGraph->existEdge(n1, n2).isValid()) {
        delaunaySubGraph->addEdge(n1, n2);
      }
    }

    for(std::map<p2t::Point*, tlp::node>::const_iterator it = points.begin(); it != points.end(); ++it) {
      delete it->first;
    }

    delaunaySubGraph->delNode(tempNode0, true);
    delaunaySubGraph->delNode(tempNode1, true);
    delaunaySubGraph->delNode(tempNode2, true);
    delaunaySubGraph->delNode(tempNode3, true);

    tlp::Observable::unholdObservers();

    return true;
  }
};

ALGORITHMPLUGIN(Delaunay,"Delaunay triangulation","","","","1.0");
