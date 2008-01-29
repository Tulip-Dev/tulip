#include "tulip/GlCircle.h"

namespace tlp {

  //===================================================
  GlCircle::GlCircle(const Coord &center, float radius, 
		     const Color &outlineColor,
		     const Color &fillColor,
		     bool filled, bool outlined ,
		     float startAngle,
		     unsigned int segments) :
    GlPolygon(segments, 1u, 1u, filled, outlined) {
    fcolor(0) = fillColor;
    ocolor(0) = outlineColor;
    set(center, radius, startAngle);
  }
  //===========================================================
  void  GlCircle::set(const Coord &center, float radius, float startAngle) {
    float delta = (2.0f * M_PI) / (float)_points.size();
    for (unsigned int i=0; i < _points.size(); ++i) {
      float deltaX = cos(startAngle + i * delta) * (radius);
      float deltaY = sin(startAngle + i * delta) * (radius);
      _points[i] = Coord(center[0] + deltaX, center[1] + deltaY, center[2]);
      boundingBox.check(_points[i]);
    }
  }
  //===========================================================
  void GlCircle::getXML(xmlNodePtr rootNode) {

    GlXMLTools::createProperty(rootNode, "type", "GlCircle");

    getXMLOnlyData(rootNode);
  }
}
