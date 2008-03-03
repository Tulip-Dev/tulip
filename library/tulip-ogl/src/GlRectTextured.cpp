#include "tulip/GlRectTextured.h"

#include <GL/gl.h>
#include <limits>

#include <tulip/Color.h>
#include "tulip/GlTools.h"

#include "tulip/GlTextureManager.h"

using namespace std;

namespace tlp {
  GlRectTextured::GlRectTextured(float top,float bottom,float left, float right,
				 const std::string& textureName,
				 bool inPercent)
    :top(top),bottom(bottom),left(left),right(right),inPercent(inPercent),textureName(textureName)
  {
    GlTextureManager::getInst().loadTexture(textureName);
  }

  void GlRectTextured::reloadData() {
    GlTextureManager::getInst().loadTexture(textureName);
  }

  BoundingBox GlRectTextured::getBoundingBox() {
    BoundingBox bb;

    if(!inPercent){
      bb.check(Coord(left,bottom,0));
      bb.check(Coord(right,top,0));
    }else{
      bb.check(Coord(numeric_limits<float>::min(),numeric_limits<float>::min(),0));
      bb.check(Coord(numeric_limits<float>::max(),numeric_limits<float>::max(),0));
    }
    return bb;
  }

  void GlRectTextured::draw(float lod,Camera *camera) {
    glDisable(GL_COLOR_MATERIAL);
    //glEnable(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3ub(255,255,255);
    if(GlTextureManager::getInst().activateTexture(textureName))
      setMaterial(Color(255,255,255,0));
    if(!inPercent) {
      glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(left, top, 0);				// Top Left
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(right, top, 0);			// Top Right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(right, bottom, 0);			// Bottom Right
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(left, bottom, 0);			// Bottom Left
      glEnd();
    }else{
      Vector<int,4> viewport=camera->getViewport();
      float xMin=viewport[0]+left*(viewport[2]-viewport[0]);
      float xMax=viewport[0]+right*(viewport[2]-viewport[0]);
      float yMin=viewport[1]+bottom*(viewport[3]-viewport[1]);
      float yMax=viewport[1]+top*(viewport[3]-viewport[1]);
      glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(xMin, yMax, 0);				// Top Left
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(xMax, yMax, 0);			// Top Right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(xMax, yMin, 0);			// Bottom Right
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(xMin, yMin, 0);			// Bottom Left
      glEnd();
    }
    GlTextureManager::getInst().desactivateTexture();
  }
  //===========================================================
  void GlRectTextured::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::createProperty(rootNode, "type", "GlRectTextured");

    GlXMLTools::getDataNode(rootNode,dataNode);
    
    GlXMLTools::getXML(dataNode,"top",top);
    GlXMLTools::getXML(dataNode,"bottom",bottom);
    GlXMLTools::getXML(dataNode,"left",left);
    GlXMLTools::getXML(dataNode,"right",right);
    GlXMLTools::getXML(dataNode,"inPercent",inPercent);
    GlXMLTools::getXML(dataNode,"textureName",textureName);
    
  }
  //============================================================
  void GlRectTextured::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      GlXMLTools::setWithXML(dataNode, "top", top);
      GlXMLTools::setWithXML(dataNode, "bottom", bottom);
      GlXMLTools::setWithXML(dataNode, "left", left);
      GlXMLTools::setWithXML(dataNode, "right", right);
      GlXMLTools::setWithXML(dataNode, "inPercent", inPercent);
      GlXMLTools::setWithXML(dataNode, "textureName", textureName);
    }

    reloadData();
  }
}
