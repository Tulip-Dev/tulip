#include "tulip/Gl2DRect.h"

#include <limits>

using namespace std;

namespace tlp {

  Gl2DRect::Gl2DRect()
    :GlRect(Coord(0.5,-0.5,0),Coord(-0.5,0.5,0),Color(255,255,255,255),Color(255,255,255,255),true,false){

  }

  Gl2DRect::Gl2DRect(float top,float bottom,float left, float right,
				 const std::string& textureName,
				 bool inPercent)
    :GlRect(Coord(0.5,-0.5,0),Coord(-0.5,0.5,0),Color(255,255,255,255),Color(255,255,255,255),true,false),
    top(top),bottom(bottom),left(left),right(right),inPercent(inPercent),xInv(false),yInv(false)
  {
    this->textureName=textureName;
  }

  Gl2DRect::Gl2DRect(float bottom,float left,float height, float width,const std::string& textureName,bool xInv, bool yInv)
    :GlRect(Coord(0.5,-0.5,0),Coord(-0.5,0.5,0),Color(255,255,255,255),Color(255,255,255,255),true,false),
    top(bottom),bottom(bottom+height),left(left),right(left+width),inPercent(false),xInv(xInv),yInv(yInv)
  {
    this->textureName=textureName;
  }

  BoundingBox Gl2DRect::getBoundingBox() {
    BoundingBox bb;

    if(!inPercent){
      bb.expand(Coord(left,bottom,0));
      bb.expand(Coord(right,top,0));
    }else{
      bb.expand(Coord(numeric_limits<float>::min(),numeric_limits<float>::min(),0));
      bb.expand(Coord(numeric_limits<float>::max(),numeric_limits<float>::max(),0));
    }
    return bb;
  }

  void Gl2DRect::draw(float lod,Camera *camera) {
    Vector<int,4> viewport=camera->getViewport();

    Coord center;
    Size size;
    if(inPercent){
      center=Coord(viewport[0]+(left*(viewport[2]-viewport[0])+right*(viewport[2]-viewport[0]))/2.,
                         viewport[1]+(bottom*(viewport[3]-viewport[1])+top*(viewport[3]-viewport[1]))/2.,
                         0);
      size=Size(right*(viewport[2]-viewport[0])-left*(viewport[2]-viewport[0]),
                     bottom*(viewport[3]-viewport[1])-top*(viewport[3]-viewport[1]),
                     0);
    }else{
      float xMin;
      float xMax;
      float yMin;
      float yMax;

      if(!xInv){
        xMin=left;
        xMax=right;
      }else{
        xMax=viewport[2]-left;
        xMin=viewport[2]-right;
      }
      if(!yInv){
        yMin=bottom;
        yMax=top;
      }else{
        yMax=viewport[3]-bottom;
        yMin=viewport[3]-top;
      }

      center=Coord((xMin+xMax)/2.,(yMin+yMax)/2.,0);
      size=Size(xMax-xMin,yMax-yMin,0);
    }

    glPushMatrix();

    glTranslatef(center[0],center[1],center[2]);
    glScalef(size[0],size[1],1);

    GlRect::draw(lod,0);

    glPopMatrix();
  }
  //===========================================================
  void Gl2DRect::translate(const Coord& vec) {
    if(!inPercent) {        
      boundingBox.translate(vec);
      top   += vec[1];
      bottom+= vec[1];
      left  += vec[0];
      right += vec[0];
    }
  }
  //===========================================================
  void Gl2DRect::setTexture(const std::string &name){
    textureName=name;
  }
  //===========================================================
  string Gl2DRect::getTexture(){
    return textureName;
  }
  //===========================================================
  void Gl2DRect::setCoordinates(float bottom, float left, float height,float width){
    this->top=bottom;
    this->left=left;
    this->bottom=top+height;
    this->right=left+width;
  }
  //===========================================================
  void Gl2DRect::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::createProperty(rootNode, "type", "Gl2DRect");

    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"top",top);
    GlXMLTools::getXML(dataNode,"bottom",bottom);
    GlXMLTools::getXML(dataNode,"left",left);
    GlXMLTools::getXML(dataNode,"right",right);
    GlXMLTools::getXML(dataNode,"inPercent",inPercent);
    GlXMLTools::getXML(dataNode,"textureName",textureName);
    GlXMLTools::getXML(dataNode,"xInv",xInv);
    GlXMLTools::getXML(dataNode,"yInv",yInv);
  }
  //============================================================
  void Gl2DRect::setWithXML(xmlNodePtr rootNode) {
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
      GlXMLTools::setWithXML(dataNode,"xInv",xInv,false);
      GlXMLTools::setWithXML(dataNode,"yInv",yInv,false);
    }
  }
}
