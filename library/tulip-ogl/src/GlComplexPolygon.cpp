#include <GL/gl.h>
#include <GL/glu.h>
#include "tulip/GlComplexPolygon.h"
#include "tulip/GlTools.h"
#include "tulip/GlLayer.h"
#include "tulip/GlTextureManager.h"

#include <tulip/TlpTools.h>



using namespace std;

namespace tlp {

  void beginCallback(GLenum which)
  {
    glBegin(which);
  }
  
  void errorCallback(GLenum errorCode)
  {
    const GLubyte *estring;
    
    estring = gluErrorString(errorCode);
    cout << "Tessellation Error: " << estring << endl;
  }

  void endCallback(void)
  {
    glEnd();
  }

  void vertexCallback(GLvoid *vertex)
  {
    const GLdouble *pointer;
    
    pointer = (GLdouble *) vertex;
    Color color=Color(pointer[3],pointer[4],pointer[5],pointer[6]);
    setMaterial(color);
    glColor4ubv((unsigned char *)&color);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(pointer[0]/10., pointer[1]/10.);
    glVertex3dv((GLdouble *)vertex);
  }


  GlComplexPolygon::GlComplexPolygon(const bool filled,
				     const bool outlined,
				     const std::string &textureName):
    _filled(filled),
    _outlined(outlined),
    textureName(textureName){ 
  }
  //=====================================================
  GlComplexPolygon::~GlComplexPolygon() {
  }
  //=====================================================
  void GlComplexPolygon::setFillMode(const bool filled) {
    _filled = filled;
  }
  //=====================================================
  void GlComplexPolygon::setOutlineMode(const bool outlined) {
    _outlined = outlined;
  }
  //=====================================================
  const tlp::Coord& GlComplexPolygon::point(const unsigned int i) const {
    return _points[i];
  }
  //=====================================================
  tlp::Coord& GlComplexPolygon::point(const unsigned int i) {
    return _points[i];
  }
  //=====================================================
  void GlComplexPolygon::addPoint(const Coord& point,const Color& fcolor,const Color& ocolor) {
    _points.push_back(point);
    _fillColors.push_back(fcolor);
    _outlineColors.push_back(ocolor);
    boundingBox.check(point);
  }
  //=====================================================
  const tlp::Color& GlComplexPolygon::fcolor(const unsigned int i) const {
    return _fillColors[i];
  }
  //=====================================================
  tlp::Color& GlComplexPolygon::fcolor(const unsigned int i) {
    return _fillColors[i];
  }
  //=====================================================
  const tlp::Color& GlComplexPolygon::ocolor(const unsigned int i) const {
    return _outlineColors[i];
  }
  //=====================================================
  tlp::Color& GlComplexPolygon::ocolor(const unsigned int i) {
    return _outlineColors[i];
  }
  //=====================================================
  void GlComplexPolygon::draw(float lod,Camera *camera) {
    glDisable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);

    if(_filled) {
      if(GlTextureManager::getInst().activateTexture(textureName));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      
      GLUtesselator *tobj;
      tobj = gluNewTess();
      
      gluTessCallback(tobj, GLU_TESS_VERTEX, 
		      (void (*)())vertexCallback);
      gluTessCallback(tobj, GLU_TESS_BEGIN, 
		      (void (*)())beginCallback);
      gluTessCallback(tobj, GLU_TESS_END, 
		      (void (*)())endCallback);
      gluTessCallback(tobj, GLU_TESS_ERROR, 
		      (void (*)())errorCallback);
      
      glShadeModel(GL_SMOOTH);  
      gluTessProperty(tobj, GLU_TESS_WINDING_RULE,
		      GLU_TESS_WINDING_POSITIVE);
      
      gluTessBeginPolygon(tobj, NULL);
      gluTessBeginContour(tobj);
      for(unsigned int i=0; i < _points.size(); ++i) {
	GLdouble *tmp=new GLdouble[7];
	tmp[0]=_points[i][0];
	tmp[1]=_points[i][1];
	tmp[2]=_points[i][2];
	tmp[3]=_fillColors[i][0];
	tmp[4]=_fillColors[i][1];
	tmp[5]=_fillColors[i][2];
	tmp[6]=_fillColors[i][3];
	gluTessVertex(tobj,tmp,tmp);
      }
      gluTessEndContour(tobj);
      gluTessEndPolygon(tobj);
      gluDeleteTess(tobj);
      GlTextureManager::getInst().desactivateTexture();
    }
    if (_outlined) {
      glBegin(GL_LINE_LOOP);
      for(unsigned int i=0; i < _points.size(); ++i) {
	if (i < _outlineColors.size()) {
	  setMaterial(_outlineColors[i]);
	  glColor4ubv((unsigned char *)&_outlineColors[i]);
	}
	glVertex3fv((float *)&_points[i]);
      }
      glEnd();
    }
    /*vector<Coord> newPoints(_points.size());
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
      }*/
    
    glTest(__PRETTY_FUNCTION__);
  }
  //===========================================================
  void GlComplexPolygon::translate(const Coord& mouvement) {
    boundingBox.first+=mouvement;
    boundingBox.second+=mouvement;
    
    for(vector<Coord>::iterator it=_points.begin();it!=_points.end();++it){
      (*it)+=mouvement;
    }
  } 
  //===========================================================
  void GlComplexPolygon::getXML(xmlNodePtr rootNode) {

    GlXMLTools::createProperty(rootNode, "type", "GlComplexPolygon");

    getXMLOnlyData(rootNode);
    
  }
  //===========================================================
  void GlComplexPolygon::getXMLOnlyData(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;
    
    GlXMLTools::getDataNode(rootNode,dataNode);
    
    GlXMLTools::getXML(dataNode,"points",_points);
    GlXMLTools::getXML(dataNode,"fillColors",_fillColors);
    GlXMLTools::getXML(dataNode,"outlineColors",_outlineColors);
    GlXMLTools::getXML(dataNode,"filled",_filled);
    GlXMLTools::getXML(dataNode,"outlined",_outlined);
  }
  //============================================================
  void GlComplexPolygon::setWithXML(xmlNodePtr rootNode) {
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
