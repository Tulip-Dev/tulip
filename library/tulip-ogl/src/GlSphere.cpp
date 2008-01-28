#include "tulip/GlSphere.h"

#include <GL/gl.h>

#include <tulip/Color.h>

#include "tulip/GlDisplayListManager.h"
#include "tulip/GlTextureManager.h"
#include "tulip/GlTools.h"

using namespace std;

namespace tlp {

  GlSphere::GlSphere(const Coord &position, float radius,const Color& color,float rotX,float rotY,float rotZ):position(position),radius(radius),color(color),rot(rotX,rotY,rotZ) { 
    boundingBox.first=Coord(position[0]-radius,position[1]-radius,position[2]-radius);
    boundingBox.second=Coord(position[0]+radius,position[1]+radius,position[2]+radius);
  }

  GlSphere::GlSphere(const Coord &position, float radius,const string& textureFile,int alpha,float rotX,float rotY,float rotZ):position(position),radius(radius),color(255,255,255,alpha),textureFile(textureFile),rot(rotX,rotY,rotZ) { 
    boundingBox.first=Coord(position[0]-radius,position[1]-radius,position[2]-radius);
    boundingBox.second=Coord(position[0]+radius,position[1]+radius,position[2]+radius);
  }

  void GlSphere::draw(float lod, Camera *camera) {
    glPushMatrix();
    glTranslatef(position[0],position[1],position[2]);
    glRotatef(rot[0],1,0,0);
    glRotatef(rot[1],0,1,0);
    glRotatef(rot[2],0,0,1);
    if (textureFile != "") {
      GlTextureManager::getInst().activateTexture(textureFile);
    }
    
    setMaterial(color);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f); 
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    GlTextureManager::getInst().desactivateTexture();
    glPopMatrix();
  }

  //===========================================================
  void GlSphere::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    xmlNewProp(rootNode,BAD_CAST "type",BAD_CAST "GlSphere");
    
    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"position",position);
    GlXMLTools::getXML(dataNode,"radius",radius);
    GlXMLTools::getXML(dataNode,"color",color);
    GlXMLTools::getXML(dataNode,"textureFile",textureFile);
    GlXMLTools::getXML(dataNode,"rotation",rot);
    
  }
  //============================================================
  void GlSphere::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {

      GlXMLTools::setWithXML(dataNode,"position",position);
      GlXMLTools::setWithXML(dataNode,"radius",radius);
      GlXMLTools::setWithXML(dataNode,"color",color);
      GlXMLTools::setWithXML(dataNode,"textureFile",textureFile);
      GlXMLTools::setWithXML(dataNode,"rotation",rot);

      boundingBox.first=Coord(position[0]-radius,position[1]-radius,position[2]-radius);
      boundingBox.second=Coord(position[0]+radius,position[1]+radius,position[2]+radius);
    }
  }

}
