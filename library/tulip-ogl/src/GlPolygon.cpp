#include <GL/gl.h>
#include "tulip/GlPolygon.h"
#include "tulip/GlTools.h"
#include "tulip/GlLayer.h"



using namespace std;

namespace tlp {
  GlPolygon::GlPolygon(const bool filled,
		       const bool outlined):
    _filled(filled),
    _outlined(outlined) { 
  }
  //=====================================================
  GlPolygon::GlPolygon(const vector<Coord> &points, 
		       const vector<Color> &fcolors,
		       const vector<Color> &ocolors,
		       const bool filled,
		       const bool outlined):
    _points(points),
    _fillColors(fcolors),
    _outlineColors(ocolors),
    _filled(filled),
    _outlined(outlined) {
    assert(points.size() >= 3);
    for(vector<Coord>::iterator it= _points.begin();it!=_points.end();++it)
      boundingBox.check(*it);
  }
  //=====================================================
  GlPolygon::GlPolygon(const unsigned int nbPoints,
		       const unsigned int nbFillColors,
		       const unsigned int nbOutlineColors,
		       const bool filled,
		       const bool outlined):
    _points(nbPoints),
    _fillColors(nbFillColors),
    _outlineColors(nbOutlineColors),
    _filled(filled),
    _outlined(outlined) {
  }
  //=====================================================
  GlPolygon::~GlPolygon() {
  }
  //=====================================================
  void GlPolygon::setFillMode(const bool filled) {
    _filled = filled;
  }
  //=====================================================
  void GlPolygon::setOutlineMode(const bool outlined) {
    _outlined = outlined;
  }
  //=====================================================
  void GlPolygon::resizePoints(const unsigned int nbPoints) {
    assert(nbPoints >= 3);
    _points.resize(nbPoints);
  }
  //=====================================================
  void GlPolygon::resizeColors(const unsigned int nbColors) {
    assert(nbColors >= 1);
    _points.resize(nbColors);
  }
  //=====================================================
  const tlp::Coord& GlPolygon::point(const unsigned int i) const {
    return _points[i];
  }
  //=====================================================
  tlp::Coord& GlPolygon::point(const unsigned int i) {
    return _points[i];
  }
  //=====================================================
  void GlPolygon::addPoint(const Coord& point,const Color& fcolor,const Color& ocolor) {
    _points.push_back(point);
    _fillColors.push_back(fcolor);
    _outlineColors.push_back(ocolor);
    boundingBox.check(point);
  }
  //=====================================================
  const tlp::Color& GlPolygon::fcolor(const unsigned int i) const {
    return _fillColors[i];
  }
  //=====================================================
  tlp::Color& GlPolygon::fcolor(const unsigned int i) {
    return _fillColors[i];
  }
  //=====================================================
  const tlp::Color& GlPolygon::ocolor(const unsigned int i) const {
    return _outlineColors[i];
  }
  //=====================================================
  tlp::Color& GlPolygon::ocolor(const unsigned int i) {
    return _outlineColors[i];
  }
  //=====================================================
  void GlPolygon::draw(float lod,Camera *camera) {
    glDisable(GL_CULL_FACE);
    vector<Coord> newPoints(_points.size());
    for(unsigned int i=0; i < _points.size(); ++i) {
      newPoints[i] = _points[i];
    }
    if (_filled){
      if (_points.size() == 3) {
	glBegin(GL_TRIANGLES);
      }else{
	if (_points.size() == 4){
	  glBegin(GL_QUADS);
	}else{
	  glBegin(GL_POLYGON);
	}
      }
      for(unsigned int i=0; i < _points.size(); ++i) {
	if (i < _fillColors.size()) {
	  setMaterial(_fillColors[i]);
	  glColor4ubv((unsigned char *)&_fillColors[i]);
	}
	glVertex3fv((float *)&newPoints[i]);
      }
      glEnd();
    }
    
    if (_outlined) {
      glBegin(GL_LINE_LOOP);
      for(unsigned int i=0; i < _points.size(); ++i) {
	if (i < _outlineColors.size()) {
	  setMaterial(_outlineColors[i]);
	  glColor4ubv((unsigned char *)&_outlineColors[i]);
	}
	glVertex3fv((float *)&newPoints[i]);
      }
      glEnd();
    }
    
    glTest(__PRETTY_FUNCTION__);
  }
  //===========================================================
  void GlPolygon::translate(const Coord& mouvement) {
    boundingBox.first+=mouvement;
    boundingBox.second+=mouvement;
    
    for(vector<Coord>::iterator it=_points.begin();it!=_points.end();++it){
      (*it)+=mouvement;
    }
  } 
  //===========================================================
  void GlPolygon::getXML(xmlNodePtr rootNode) {

    GlXMLTools::createProperty(rootNode, "type", "GlPolygon");

    getXMLOnlyData(rootNode);
    
  }
  //===========================================================
  void GlPolygon::getXMLOnlyData(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;
    
    GlXMLTools::getDataNode(rootNode,dataNode);
    
    GlXMLTools::getXML(dataNode,"points",_points);
    GlXMLTools::getXML(dataNode,"fillColors",_fillColors);
    GlXMLTools::getXML(dataNode,"outlineColors",_outlineColors);
    GlXMLTools::getXML(dataNode,"filled",_filled);
    GlXMLTools::getXML(dataNode,"outlined",_outlined);
  }
  //============================================================
  void GlPolygon::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      GlXMLTools::setWithXML(dataNode,"points",_points);
      GlXMLTools::setWithXML(dataNode,"fillColors",_fillColors);
      GlXMLTools::setWithXML(dataNode,"outlineColors",_outlineColors);
      GlXMLTools::setWithXML(dataNode,"filled",_filled);
      GlXMLTools::setWithXML(dataNode,"outlined",_outlined);

      for(vector<Coord>::iterator it= _points.begin();it!=_points.end();++it)
	boundingBox.check(*it);
    }
  }
}
