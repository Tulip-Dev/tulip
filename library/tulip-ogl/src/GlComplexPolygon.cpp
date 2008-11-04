#include <GL/gl.h>
#include <GL/glu.h>
#include "tulip/GlComplexPolygon.h"
#include "tulip/GlTools.h"
#include "tulip/GlLayer.h"
#include "tulip/GlTextureManager.h"
#include "tulip/Bezier.h"

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
    glTexCoord2f(pointer[0], pointer[1]);
    glVertex3dv((GLdouble *)vertex);
  }

  GlComplexPolygon::GlComplexPolygon(vector<Coord> &coords,vector<Color> &fcolors,int bezier,const string &textureName):
    filled(true),
    outlined(false),
    textureName(textureName){
    createPolygon(coords,fcolors,bezier);
  }
  //=====================================================
  GlComplexPolygon::GlComplexPolygon(vector<Coord> &coords,vector<Color> &fcolors,Color ocolor,int bezier,const string &textureName):
    filled(true),
    outlined(true),
    textureName(textureName) {
    outlineColors.push_back(ocolor);
    createPolygon(coords,fcolors,bezier);
  }
  //=====================================================
  GlComplexPolygon::GlComplexPolygon(const bool filled,
				     const bool outlined,
				     const std::string &textureName):
    filled(filled),
    outlined(outlined),
    textureName(textureName){ 
  }
  //=====================================================
  GlComplexPolygon::~GlComplexPolygon() {
  }
  //=====================================================
  void GlComplexPolygon::createPolygon(vector<Coord> &coords,vector<Color> &fcolors,int bezier) {
    assert(coords.size()==fcolors.size());
    if(bezier==0) {
      vector<Color>::iterator itFC=fcolors.begin();
      for(vector<Coord>::iterator it=coords.begin();it!=coords.end();++it) {
	addPoint(*it,*itFC);
	++itFC;
      }
    }else{
      double dCoords[coords.size()][3];
      int i=0;
      for(vector<Coord>::iterator it=coords.begin();it!=coords.end();++it) {
	dCoords[i][0]=(*it)[0];dCoords[i][1]=(*it)[1];dCoords[i][2]=(*it)[2];
	i++;
      }
      double result[3];
      double dec=1./(bezier*coords.size());
      int ci=0;
      i=-1;
      addPoint(coords[0],fcolors[0]);
      for(double j=0;j<1;j+=dec) {
	if(i>=bezier){
	  ci++;
	  i=0;
	}
	Bezier(result,dCoords,coords.size(),j);
	addPoint(Coord(result[0],result[1],result[2]),fcolors[ci]);
	i++;
      }
      addPoint(coords[coords.size()-1],fcolors[fcolors.size()-1]);
    }
  }
  //=====================================================
  void GlComplexPolygon::setFillMode(const bool filled) {
    this->filled = filled;
  }
  //=====================================================
  void GlComplexPolygon::setOutlineMode(const bool outlined) {
    this->outlined = outlined;
  }
  //=====================================================
  const tlp::Coord& GlComplexPolygon::point(const unsigned int i) const {
    return points[i];
  }
  //=====================================================
  tlp::Coord& GlComplexPolygon::point(const unsigned int i) {
    return points[i];
  }
  //=====================================================
  void GlComplexPolygon::addPoint(const Coord& point,const Color& fcolor,const Color& ocolor) {
    points.push_back(point);
    fillColors.push_back(fcolor);
    outlineColors.push_back(ocolor);
    boundingBox.check(point);
  }
  //=====================================================
  void GlComplexPolygon::addPoint(const Coord& point,const Color& fcolor) {
    points.push_back(point);
    fillColors.push_back(fcolor);
    boundingBox.check(point);
  }
  //=====================================================
  const tlp::Color& GlComplexPolygon::fcolor(const unsigned int i) const {
    return fillColors[i];
  }
  //=====================================================
  tlp::Color& GlComplexPolygon::fcolor(const unsigned int i) {
    return fillColors[i];
  }
  //=====================================================
  const tlp::Color& GlComplexPolygon::ocolor(const unsigned int i) const {
    return outlineColors[i];
  }
  //=====================================================
  tlp::Color& GlComplexPolygon::ocolor(const unsigned int i) {
    return outlineColors[i];
  }
  //=====================================================
  void GlComplexPolygon::draw(float lod,Camera *camera) {
    glDisable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);

    if(filled) {
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
      for(unsigned int i=0; i < points.size(); ++i) {
	GLdouble *tmp=new GLdouble[7];
	tmp[0]=points[i][0];
	tmp[1]=points[i][1];
	tmp[2]=points[i][2];
	tmp[3]=fillColors[i][0];
	tmp[4]=fillColors[i][1];
	tmp[5]=fillColors[i][2];
	tmp[6]=fillColors[i][3];
	gluTessVertex(tobj,tmp,tmp);
      }
      gluTessEndContour(tobj);
      gluTessEndPolygon(tobj);
      gluDeleteTess(tobj);
      GlTextureManager::getInst().desactivateTexture();
    }
    if (outlined) {
      glBegin(GL_LINE_LOOP);
      for(unsigned int i=0; i < points.size(); ++i) {
	if (i < outlineColors.size()) {
	  setMaterial(outlineColors[i]);
	  glColor4ubv((unsigned char *)&outlineColors[i]);
	}
	glVertex3fv((float *)&points[i]);
      }
      glEnd();
    }
    
    glTest(__PRETTY_FUNCTION__);
  }
  //===========================================================
  void GlComplexPolygon::translate(const Coord& mouvement) {
    boundingBox.first+=mouvement;
    boundingBox.second+=mouvement;
    
    for(vector<Coord>::iterator it=points.begin();it!=points.end();++it){
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
    
    GlXMLTools::getXML(dataNode,"points",points);
    GlXMLTools::getXML(dataNode,"fillColors",fillColors);
    GlXMLTools::getXML(dataNode,"outlineColors",outlineColors);
    GlXMLTools::getXML(dataNode,"filled",filled);
    GlXMLTools::getXML(dataNode,"outlined",outlined);
  }
  //============================================================
  void GlComplexPolygon::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      GlXMLTools::setWithXML(dataNode,"points",points);
      GlXMLTools::setWithXML(dataNode,"fillColors",fillColors);
      GlXMLTools::setWithXML(dataNode,"outlineColors",outlineColors);
      GlXMLTools::setWithXML(dataNode,"filled",filled);
      GlXMLTools::setWithXML(dataNode,"outlined",outlined);

      for(vector<Coord>::iterator it= points.begin();it!=points.end();++it)
	boundingBox.check(*it);
    }
  }
}
