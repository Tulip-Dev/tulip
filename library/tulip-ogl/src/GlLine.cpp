
#include "tulip/GlLine.h"
#include "tulip/GlTools.h"


using namespace std;

namespace tlp {
  GlLine::GlLine(const vector<Coord> &points, const vector<Color> &colors):
    _points(points),
    _colors(colors){

    for(vector<Coord>::iterator it= _points.begin();it!=_points.end();++it)
      boundingBox.check(*it);
  }
  //=====================================================
  GlLine::~GlLine() {
  }
  //=====================================================
  void GlLine::resizePoints(const unsigned int nbPoints) {
    _points.resize(nbPoints);
  }
  //=====================================================
  void GlLine::resizeColors(const unsigned int nbColors) {
    assert(nbColors >= 1);
    _points.resize(nbColors);
  }
  //=====================================================
  const tlp::Coord& GlLine::point(const unsigned int i) const {
    return _points[i];
  }
  //=====================================================
  tlp::Coord& GlLine::point(const unsigned int i) {
    return _points[i];
  }
  //=====================================================
  void GlLine::addPoint(const Coord& point,const Color& color) {
    _points.push_back(point);
    _colors.push_back(color);
    boundingBox.check(point);
  }
  //=====================================================
  const tlp::Color& GlLine::color(const unsigned int i) const {
    return _colors[i];
  }
  //=====================================================
  tlp::Color& GlLine::color(const unsigned int i) {
    return _colors[i];
  }
  //=====================================================
  void GlLine::draw(float lod,Camera *camera) {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_STRIP);

    for(unsigned int i=0; i < _points.size(); ++i) {
      if (i < _colors.size()) {
        setColor(_colors[i]);
      }
      glVertex3fv((float *)&_points[i]);
    }
    glEnd();

    glEnable(GL_LIGHTING);
    glTest(__PRETTY_FUNCTION__);
  }
  //=====================================================
  void GlLine::translate(const Coord& mouvement){
    boundingBox.first+=mouvement;
    boundingBox.second+=mouvement;

    for(vector<Coord>::iterator it=_points.begin();it!=_points.end();++it) {
      (*it)+=mouvement;
    }
  }
  //=====================================================
  void GlLine::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::createProperty(rootNode, "type", "GlLine");

    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"points",_points);
    GlXMLTools::getXML(dataNode,"colors",_colors);

  }
  //============================================================
  void GlLine::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {

      GlXMLTools::setWithXML(dataNode, "points", _points);
      GlXMLTools::setWithXML(dataNode, "colors", _colors);

      for(vector<Coord>::iterator it= _points.begin();it!=_points.end();++it)
	boundingBox.check(*it);
    }
  }
}
