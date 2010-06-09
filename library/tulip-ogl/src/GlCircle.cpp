#include "tulip/GlCircle.h"

namespace tlp {

  //===================================================
  GlCircle::GlCircle(const Coord &center, float radius, 
		     const Color &outlineColor,
		     const Color &fillColor,
		     bool filled, bool outlined ,
		     float startAngle,
		     unsigned int segments) :
    GlRegularPolygon(center,Size(radius,radius,0),segments,outlineColor,fillColor,filled,outlined) {
    assert(segments<=256);
    setStartAngle(startAngle);
    computePolygon(center,Size(radius,radius,0));
  }
  //===========================================================
  void  GlCircle::set(const Coord &center, float radius, float startAngle) {
    setStartAngle(startAngle);
    computePolygon(center,Size(radius,radius,0));
  }
  //===========================================================
  void GlCircle::getXML(xmlNodePtr rootNode) {

    GlXMLTools::createProperty(rootNode, "type", "GlCircle");

    getXMLOnlyData(rootNode);
  }
}
