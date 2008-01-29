#include "tulip/GlRect.h"

using namespace std;

namespace tlp {

  GlRect::GlRect(const Coord &topLeftPos, const Coord &bottomRightPos, const Color &topLeftCol, const Color &bottomRightCol, bool filled, bool outlined):
    GlPolygon(4u,4u, 4u, filled, outlined) {
    point(0) = topLeftPos;
    point(1) = topLeftPos;
    point(1)[0] = bottomRightPos[0];
    point(2) = bottomRightPos ;
    point(3) = topLeftPos;
    point(3)[1] = bottomRightPos[1]; 

    boundingBox.check(topLeftPos);
    boundingBox.check(bottomRightPos);

    fcolor(0) = topLeftCol;
    fcolor(1) = topLeftCol;
    fcolor(2) = bottomRightCol ;
    fcolor(3) = bottomRightCol;
  }
  GlRect::GlRect(bool filled, bool outlined):GlPolygon(4u,4u,4u,filled,outlined) {
  }
  //=====================================================
  GlRect::~GlRect()  {
  }
  //=====================================================
  Coord GlRect::getCenter()  {
    return (point(0) + point(2)) / 2.0;
  }
  //=====================================================
  Coord GlRect::getTopLeftPos() {
    return point(0);
  }
  //=====================================================
  Coord GlRect::getBottomRightPos() {
    return point(2);
  }
  //=====================================================
  Color GlRect::getTopLeftColor() {
    return fcolor(0);
  }
  //=====================================================
  Color GlRect::getBottomRightColor() {
    return fcolor(2);
  }
  //=====================================================
  void GlRect::setTopLeftPos(const Coord &_topLeftPos) {
    point(0) = _topLeftPos;
    point(1)[1] = _topLeftPos[1];
    point(3)[0] = _topLeftPos[0];
    boundingBox.check(_topLeftPos);
  }
  //=====================================================
  void GlRect::setBottomRightPos(const Coord &_bottomRightPos) {
    point(2) = _bottomRightPos;
    point(1)[0] = _bottomRightPos[0];
    point(3)[1] = _bottomRightPos[1];
    boundingBox.check(_bottomRightPos);
  }
  //=====================================================
  void GlRect::setTopLeftColor(const Color &_topLeftCol) {
    fcolor(0) = _topLeftCol;
    fcolor(1) = _topLeftCol;
  }
  //=====================================================
  void GlRect::setBottomRightColor(const Color &_bottomRightCol) {
    fcolor(2) = _bottomRightCol;
    fcolor(3) = _bottomRightCol;
  }
  //=====================================================
  bool GlRect::inRect(double x, double y) {
    double minX = std::min(point(0).getX(), point(2).getX());
    double maxX = std::max(point(0).getX(), point(2).getX());
    double minY = std::min(point(0).getY(), point(2).getY());
    double maxY = std::max(point(0).getY(), point(2).getY());
    return ((x >= minX) && 
	    (x <= maxX) && 
	    (y >= minY) && 
	    (y <= maxY));
  }
  //=====================================================
  void GlRect::draw(float lod,Camera *camera) {
    GlPolygon::draw(lod,camera);
  }
  //===========================================================
  void GlRect::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::createProperty(rootNode, "type", "GlRect");

    GlPolygon::getXMLOnlyData(rootNode);
    
    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"topLeftPos",topLeftPos);
    GlXMLTools::getXML(dataNode,"bottomRightPos",bottomRightPos);
    GlXMLTools::getXML(dataNode,"topLeftCol",topLeftCol);
    GlXMLTools::getXML(dataNode,"bottomRightCol",bottomRightCol);
    
  }
  //============================================================
  void GlRect::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlPolygon::setWithXML(rootNode);

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      GlXMLTools::setWithXML(dataNode,"topLeftPos",topLeftPos);
      GlXMLTools::setWithXML(dataNode,"bottomRightPos",bottomRightPos);
      GlXMLTools::setWithXML(dataNode,"topLeftCol",topLeftCol);
      GlXMLTools::setWithXML(dataNode,"bottomRightCol",bottomRightCol);
    }
  }
}
