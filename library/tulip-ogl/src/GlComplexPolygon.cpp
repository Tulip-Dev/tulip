#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifndef CALLBACK
#define CALLBACK
#endif

#ifdef __APPLE_CC__
  #if __APPLE_CC__ < 5400
// Tiger
    typedef GLvoid (*GLUTesselatorFunction)(...);
    #else
// Leopard
    typedef GLvoid (*GLUTesselatorFunction)();
  #endif
#elif defined( __mips ) || defined( __linux__ ) || defined( __FreeBSD_kernel__) || defined( __FreeBSD__ ) || defined( __OpenBSD__ ) || defined( __sun ) || defined (__CYGWIN__)
    typedef GLvoid (*GLUTesselatorFunction)();
#elif defined ( WIN32)
// inspired from ftgl/FTGL.h & ftgl/FTVectoriser.cpp
    #define  WIN32_LEAN_AND_MEAN
    #include <windows.h>
    namespace tlp {
      void CALLBACK beginCallback(GLenum which);
      void CALLBACK endCallback(void);
      void CALLBACK errorCallback(GLenum errorCode);
      void CALLBACK vertexCallback(GLvoid *vertex);
    }
#else
    #error "Error - need to define type GLUTesselatorFunction for this platform/compiler"
#endif

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
    Color color=Color((unsigned char) pointer[3], (unsigned char)pointer[4],
		      (unsigned char) pointer[5], (unsigned char)pointer[6]);
    setMaterial(color);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(pointer[0], pointer[1]);
    glVertex3dv((GLdouble *)vertex);
  }

  GlComplexPolygon::GlComplexPolygon(vector<Coord> &coords,Color fcolor,int bezier,const string &textureName):
    currentVector(0),
    outlined(false),
    fillColor(fcolor),
    outlineSize(1),
    textureName(textureName){
    createPolygon(coords,bezier);
  }
  //=====================================================
  GlComplexPolygon::GlComplexPolygon(vector<Coord> &coords,Color fcolor,Color ocolor,int bezier,const string &textureName):
    currentVector(0),
    outlined(true),
    fillColor(fcolor),
    outlineColor(ocolor),
    outlineSize(1),
    textureName(textureName) {
    createPolygon(coords,bezier);
  }
  //=====================================================
  GlComplexPolygon::GlComplexPolygon(vector<vector<Coord> >&coords,Color fcolor,int bezier,const string &textureName):
    currentVector(0),
    outlined(false),
    fillColor(fcolor),
    outlineSize(1),
    textureName(textureName){
    for(unsigned int i=0;i<coords.size();++i) {
      createPolygon(coords[i],bezier);
      beginNewHole();
    }
  }
  //=====================================================
  GlComplexPolygon::GlComplexPolygon(vector<vector<Coord> >&coords,Color fcolor,Color ocolor,int bezier,const string &textureName):
    currentVector(0),
    outlined(true),
    fillColor(fcolor),
    outlineColor(ocolor),
    outlineSize(1),
    textureName(textureName) {
    for(unsigned int i=0;i<coords.size();++i) {
      createPolygon(coords[i],bezier);
      beginNewHole();
    }
  }
  //=====================================================
  GlComplexPolygon::~GlComplexPolygon() {
  }
  //=====================================================
  void GlComplexPolygon::createPolygon(vector<Coord> &coords,int bezier) {
    points.push_back(vector<Coord>());
    if(bezier==0) {
      for(vector<Coord>::iterator it=coords.begin();it!=coords.end();++it) {
	addPoint(*it);
      }
    }else{
      double dCoords[coords.size()][3];
      int i=0;
      for(vector<Coord>::iterator it=coords.begin();it!=coords.end();++it) {
	dCoords[i][0]=(*it)[0];dCoords[i][1]=(*it)[1];dCoords[i][2]=(*it)[2];
	i++;
      }
      addPoint(coords[0]);

      double result[3];
      double dec=1./(bezier*coords.size());
      
      for(i=0;i+3<coords.size();i+=3) {
        for(double j=dec;j<1;j+=dec) {
	      Bezier(result,&(dCoords[i]),4,j);
	      addPoint(Coord(result[0],result[1],result[2]));
        }
      }

      addPoint(coords[coords.size()-1]);
    }
  }
  //=====================================================
  void GlComplexPolygon::setOutlineMode(const bool outlined) {
    this->outlined = outlined;
  }
  //=====================================================
  void GlComplexPolygon::setOutlineSize(double size) {
    outlineSize=size;
  }
  //=====================================================
  void GlComplexPolygon::addPoint(const Coord& point) {
    points[currentVector].push_back(point);
    boundingBox.expand(point);
  }
  //=====================================================
  void GlComplexPolygon::beginNewHole() {
    currentVector++;
  }
  //=====================================================
  void GlComplexPolygon::draw(float lod,Camera *camera) {
    glDisable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);

    if(textureName!=""){
      if(GlTextureManager::getInst().activateTexture(textureName))
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    }

    GLUtesselator *tobj;
    tobj = gluNewTess();

#ifdef WIN32
    gluTessCallback(tobj, GLU_TESS_VERTEX,
        (void (__stdcall*)(void))vertexCallback);
    gluTessCallback(tobj, GLU_TESS_BEGIN,
        (void (__stdcall*)(void))beginCallback);
    gluTessCallback(tobj, GLU_TESS_END,
        (void (__stdcall*)(void))endCallback);
    gluTessCallback(tobj, GLU_TESS_ERROR,
        (void (__stdcall*)(void))errorCallback);
#else
    gluTessCallback(tobj, GLU_TESS_VERTEX,
		    (GLUTesselatorFunction)vertexCallback);
    gluTessCallback(tobj, GLU_TESS_BEGIN,
		    (GLUTesselatorFunction)beginCallback);
    gluTessCallback(tobj, GLU_TESS_END,
		    (GLUTesselatorFunction)endCallback);
    gluTessCallback(tobj, GLU_TESS_ERROR,
		    (GLUTesselatorFunction)errorCallback);
#endif

    //Compute number of points to compute and create a big tab to store points' informations
    unsigned int numberOfPoints=0;
    for(unsigned int v=0;v<points.size();++v) {
      numberOfPoints+=points[v].size();
    }
    GLdouble *pointsData = new GLdouble[7*numberOfPoints];

    unsigned int pointNumber=0;
    glShadeModel(GL_SMOOTH);
    gluTessBeginPolygon(tobj, NULL);
    for(unsigned int v=0;v<points.size();++v) {
      gluTessBeginContour(tobj);
      for(unsigned int i=0; i < points[v].size(); ++i) {
        pointsData[pointNumber*7]=points[v][i][0];
        pointsData[pointNumber*7+1]=points[v][i][1];
        pointsData[pointNumber*7+2]=points[v][i][2];
        pointsData[pointNumber*7+3]=fillColor[0];
        pointsData[pointNumber*7+4]=fillColor[1];
        pointsData[pointNumber*7+5]=fillColor[2];
        pointsData[pointNumber*7+6]=fillColor[3];
        gluTessVertex(tobj,&pointsData[pointNumber*7],&pointsData[pointNumber*7]);
        pointNumber++;
      }
      gluTessEndContour(tobj);
    }
    gluTessEndPolygon(tobj);
    gluDeleteTess(tobj);
    delete [] pointsData;
    if(textureName!="")
      GlTextureManager::getInst().desactivateTexture();

    if (outlined) {
      glLineWidth(outlineSize);
      for(unsigned int v=0;v<points.size();++v) {
        glBegin(GL_LINE_LOOP);
        for(unsigned int i=0; i < points[v].size(); ++i) {
          setColor(outlineColor);
          glVertex3fv((float *)&points[v][i]);
        }
        glEnd();
      }
    }

    glTest(__PRETTY_FUNCTION__);
  }
  //===========================================================
  void GlComplexPolygon::translate(const Coord& vec) {
    boundingBox.translate(vec);
    for(vector<vector<Coord> >::iterator it = points.begin(); it != points.end(); ++it){
      for(vector<Coord>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
        (*it2) += vec;
      }
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

    GlXMLTools::getXML(dataNode,"numberOfVector",points.size());
    for(unsigned int i=0;i<points.size();++i){
      stringstream str;
      str << i ;
      GlXMLTools::getXML(dataNode,"points"+str.str(),points[i]);
    }
    GlXMLTools::getXML(dataNode,"fillColor",fillColor);
    GlXMLTools::getXML(dataNode,"outlineColor",outlineColor);
    GlXMLTools::getXML(dataNode,"outlined",outlined);
    GlXMLTools::getXML(dataNode,"outlineSize",outlineSize);
    GlXMLTools::getXML(dataNode,"textureName",textureName);
  }
  //============================================================
  void GlComplexPolygon::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      int numberOfVector;
      GlXMLTools::setWithXML(dataNode,"numberOfVector",numberOfVector);
      for(int i=0;i<numberOfVector;++i) {
        stringstream str;
        str << i ;
        points.push_back(vector<Coord>());
        GlXMLTools::setWithXML(dataNode,"points"+str.str(),points[i]);
      }

      GlXMLTools::setWithXML(dataNode,"fillColor",fillColor);
      GlXMLTools::setWithXML(dataNode,"outlineColor",outlineColor);
      GlXMLTools::setWithXML(dataNode,"outlined",outlined);
      GlXMLTools::setWithXML(dataNode,"outlineSize",outlineSize);
      GlXMLTools::setWithXML(dataNode,"textureName",textureName);

      for(vector<vector<Coord> >::iterator it= points.begin();it!=points.end();++it) {
        for(vector<Coord>::iterator it2=(*it).begin();it2!=(*it).end();++it2) {
          boundingBox.expand(*it2);
        }
      }
    }
  }
}
