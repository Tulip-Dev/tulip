
#include "tulip/GlPolygon.h"
#include "tulip/GlTools.h"
#include "tulip/GlLayer.h"



using namespace std;

namespace tlp {
  GlPolygon::GlPolygon(const bool filled,
		       const bool outlined,
		       const int outlineSize):
    filled(filled),
    outlined(outlined),
    outlineSize(outlineSize){
  }
  //=====================================================
  GlPolygon::GlPolygon(const vector<Coord> &points,
		       const vector<Color> &fcolors,
		       const vector<Color> &ocolors,
		       const bool filled,
		       const bool outlined,
		       const int outlineSize):
    points(points),
    fillColors(fcolors),
    outlineColors(ocolors),
    filled(filled),
    outlined(outlined),
    outlineSize(outlineSize) {
    assert(points.size() >= 3);
    for(vector<Coord>::const_iterator it=points.begin();it!=points.end();++it)
      boundingBox.check(*it);
  }
  //=====================================================
  GlPolygon::GlPolygon(const unsigned int nbPoints,
		       const unsigned int nbFillColors,
		       const unsigned int nbOutlineColors,
		       const bool filled,
		       const bool outlined,
		       const int outlineSize):
    points(nbPoints),
    fillColors(nbFillColors),
    outlineColors(nbOutlineColors),
    filled(filled),
    outlined(outlined),
    outlineSize(outlineSize) {
  }
  //=====================================================
  GlPolygon::~GlPolygon() {
  }
  //=====================================================
  bool GlPolygon::getFillMode() {
    return filled;
  }
  //=====================================================
  void GlPolygon::setFillMode(const bool filled) {
    this->filled = filled;
  }
  //=====================================================
  bool GlPolygon::getOutlineMode() {
    return outlined;
  }
  //=====================================================
  void GlPolygon::setOutlineMode(const bool outlined) {
    this->outlined = outlined;
  }
  //=====================================================
  void GlPolygon::resizePoints(const unsigned int nbPoints) {
    assert(nbPoints >= 3);
    points.resize(nbPoints);
  }
  //=====================================================
  void GlPolygon::resizeColors(const unsigned int nbColors) {
    assert(nbColors >= 1);
    points.resize(nbColors);
  }
  //=====================================================
  const tlp::Coord& GlPolygon::point(const unsigned int i) const {
    return points[i];
  }
  //=====================================================
  tlp::Coord& GlPolygon::point(const unsigned int i) {
    return points[i];
  }
  //=====================================================
  void GlPolygon::addPoint(const Coord& point,const Color& fcolor,const Color& ocolor) {
    points.push_back(point);
    fillColors.push_back(fcolor);
    outlineColors.push_back(ocolor);
    boundingBox.check(point);
  }
  //=====================================================
  const Color& GlPolygon::fcolor(const unsigned int i) const {
    return fillColors[i];
  }
  //=====================================================
  Color& GlPolygon::fcolor(const unsigned int i) {
    return fillColors[i];
  }
  //=====================================================
  void GlPolygon::setFColor(const unsigned int i,const Color &color) {
    fillColors[i]=color;
  }
  //=====================================================
  const Color& GlPolygon::ocolor(const unsigned int i) const {
    return outlineColors[i];
  }
  //=====================================================
  Color& GlPolygon::ocolor(const unsigned int i) {
    return outlineColors[i];
  }
  //=====================================================
  void GlPolygon::setOColor(const unsigned int i,const Color &color) {
    outlineColors[i]=color;
  }
  //=====================================================
  void GlPolygon::draw(float lod,Camera *camera) {
    glDisable(GL_CULL_FACE);

    if (filled){
      vector<Coord> normalPoints;
      normalPoints.push_back(points[0]);
      for(int i=1;i<points.size() && normalPoints.size()<3;++i){
        for(int j=0;j<normalPoints.size();++j){
          if(normalPoints[j]!=points[i])
            normalPoints.push_back(points[i]);
        }
      }

      // Ok we have a valid filled polygon
      if(normalPoints.size()==3){
        Coord normal=normalPoints[0]-normalPoints[1];
        normal^=(normalPoints[2]-normalPoints[1]);
        normal/=normal.norm();
        if(normal[2]<0)
          normal=Coord(-normal[0],-normal[1],-normal[2]);

        if (points.size() == 3) {
          glBegin(GL_TRIANGLES);
        }else{
          if (points.size() == 4){
            glBegin(GL_QUADS);
          }else{
            glBegin(GL_POLYGON);
          }
        }
        glNormal3fv((float*)&normal);
        for(unsigned int i=0; i < points.size(); ++i) {
          if (i < fillColors.size()) {
            setMaterial(fillColors[i]);
          }
          glVertex3fv((float *)&points[i]);
        }
        glEnd();
      }
    }

    if (outlined) {
      if(outlineSize!=1)
        glLineWidth(outlineSize);
      glBegin(GL_LINE_LOOP);
      for(unsigned int i=0; i < points.size(); ++i) {
        if (i < outlineColors.size()) {
          setMaterial(outlineColors[i]);
        }
        glVertex3fv((float *)&points[i]);
      }
      glEnd();
      if(outlineSize!=1)
        glLineWidth(1);
    }

    glTest(__PRETTY_FUNCTION__);
  }
  //===========================================================
  void GlPolygon::translate(const Coord& mouvement) {
    boundingBox.first+=mouvement;
    boundingBox.second+=mouvement;

    for(vector<Coord>::iterator it=points.begin();it!=points.end();++it){
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

    GlXMLTools::getXML(dataNode,"points",points);
    GlXMLTools::getXML(dataNode,"fillColors",fillColors);
    GlXMLTools::getXML(dataNode,"outlineColors",outlineColors);
    GlXMLTools::getXML(dataNode,"filled",filled);
    GlXMLTools::getXML(dataNode,"outlined",outlined);
    GlXMLTools::getXML(dataNode,"outlineSize",outlineSize);
  }
  //============================================================
  void GlPolygon::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      points.clear();
      GlXMLTools::setWithXML(dataNode,"points",points);
      fillColors.clear();
      GlXMLTools::setWithXML(dataNode,"fillColors",fillColors);
      outlineColors.clear();
      GlXMLTools::setWithXML(dataNode,"outlineColors",outlineColors);
      GlXMLTools::setWithXML(dataNode,"filled",filled);
      GlXMLTools::setWithXML(dataNode,"outlined",outlined);
      GlXMLTools::setWithXML(dataNode,"outlineSize",outlineSize);

      for(vector<Coord>::iterator it= points.begin();it!=points.end();++it)
        boundingBox.check(*it);
    }
  }
}
