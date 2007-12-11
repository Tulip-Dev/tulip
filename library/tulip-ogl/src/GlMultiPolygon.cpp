#include "tulip/GlMultiPolygon.h"

using namespace std;

namespace tlp {

  static const Color oColor(255,0,0,255);
  static const Color fColor(0,0,0,0);

  void GlMultiPolygon::addPolygon(const vector<Coord> &poly) {
    polygons.push_back(GlPolygon(false,true));
    GlPolygon *newPolygon=&(*(--polygons.end()));
    for(vector<Coord>::const_iterator it=poly.begin();it!=poly.end();++it) {
      newPolygon->addPoint((*it),fColor,oColor);
    }

    BoundingBox bb=newPolygon->getBoundingBox();
    boundingBox.check(bb.first);
    boundingBox.check(bb.second);
  }

  void GlMultiPolygon::draw(float lod) {
    for(vector<GlPolygon>::iterator it=polygons.begin();it!=polygons.end();++it) {
      (*it).draw(lod);
    }
  }

}
