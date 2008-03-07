#include "tulip/GlGrid.h"

#include <tulip/GlTools.h>

using namespace std;

namespace tlp {
  //============================================================
  GlGrid::GlGrid(const Coord& frontTopLeft, const Coord& backBottomRight, 
		 const Size& cell, const Color& color, bool displayDim[3]) :
    frontTopLeft(frontTopLeft), 
    backBottomRight(backBottomRight), 
    color(color), 
    cell(cell){
    for(unsigned int i=0; i < 3; i++)
      this->displayDim[i] = displayDim[i];
    boundingBox.check(frontTopLeft);
    boundingBox.check(backBottomRight);
  }
  //============================================================
  void GlGrid::draw(float lod,Camera *camera) {
    Coord delta = backBottomRight - frontTopLeft;
    Coord gridSize = backBottomRight - frontTopLeft;
    delta /= gridSize / cell;
  
    glDisable(GL_COLOR_MATERIAL);
    setMaterial(color);
    //glColor4ubv(((const GLubyte *)&color));

    glLineWidth(1);
    glBegin(GL_LINES);
    //xy-plane
    if (displayDim[0]) {
      Coord A(frontTopLeft);
      while (A[0] <= backBottomRight[0] + 1E-3) {
	glVertex3f(A[0], A[1], A[2]);
	glVertex3f(A[0], A[1] + gridSize[1] , A[2]);
	A[0] += delta[0];
      }
      A = frontTopLeft;
      while (A[1] <= backBottomRight[1] + 1E-3) {
	glVertex3f(A[0], A[1], A[2]);
	glVertex3f(A[0] + gridSize[0], A[1]  , A[2]);
	A[1] += delta[1];
      }
    }
    //yz-plane
    if (displayDim[1]) {
      Coord A(frontTopLeft);
      while (A[2] <= backBottomRight[2] + 1E-3) {
	glVertex3f(A[0], A[1], A[2]);
	glVertex3f(A[0], A[1] + gridSize[1] , A[2]);
	A[2] += delta[2];
      }
      A = frontTopLeft;
      while (A[1] <= backBottomRight[1] + 1E-3) {
	glVertex3f(A[0], A[1], A[2]);
	glVertex3f(A[0], A[1] , A[2] + gridSize[2]);
	A[1] += delta[1];
      }
    }
    //xz-plane
    if (displayDim[2]) {
      Coord A(frontTopLeft);
      while (A[2] <= backBottomRight[2] + 1E-3) {
	glVertex3f(A[0], A[1], A[2]);
	glVertex3f(A[0]+ gridSize[0], A[1]  , A[2]);
	A[2] += delta[2];
      }
      A = frontTopLeft;
      while (A[0] <= backBottomRight[0] + 1E-3) {
	glVertex3f(A[0], A[1], A[2]);
	glVertex3f(A[0], A[1], A[2] + gridSize[2]);
	A[0] += delta[0];
      }
    }
    glEnd();
  }
  //============================================================
  void GlGrid::getDisplayDim(bool displayDim[3]) const {
    for(int i=0; i < 3; i++)
      displayDim[i] = this->displayDim[i];
  }
  //============================================================
  void GlGrid::setDisplayDim(bool displayDim[3]) {
    for(int i=0; i < 3; i++)
      this->displayDim[i] = displayDim[i];
  }
  //============================================================
  void GlGrid::translate(const Coord& mouvement){
    boundingBox.first+=mouvement;
    boundingBox.second+=mouvement;

    frontTopLeft+=mouvement;
    backBottomRight+=mouvement;
  }
  //============================================================
  void GlGrid::getXML(xmlNodePtr rootNode){
    xmlNodePtr dataNode= NULL;

    GlXMLTools::createProperty(rootNode, "type", "GlGrid");
    
    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"displayDim0",displayDim[0]);
    GlXMLTools::getXML(dataNode,"displayDim1",displayDim[1]);
    GlXMLTools::getXML(dataNode,"displayDim2",displayDim[2]);
    GlXMLTools::getXML(dataNode,"frontTopLeft",frontTopLeft);
    GlXMLTools::getXML(dataNode,"backBottomRight",backBottomRight);
    GlXMLTools::getXML(dataNode,"color",color);
    GlXMLTools::getXML(dataNode,"cell",cell);

  }
   //====================================================
  void GlGrid::setWithXML(xmlNodePtr rootNode){
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      GlXMLTools::setWithXML(dataNode,"displayDim0",displayDim[0]);
      GlXMLTools::setWithXML(dataNode,"displayDim1",displayDim[1]);
      GlXMLTools::setWithXML(dataNode,"displayDim2",displayDim[2]);
      GlXMLTools::setWithXML(dataNode,"frontTopLeft",frontTopLeft);
      GlXMLTools::setWithXML(dataNode,"backBottomRight",backBottomRight);
      GlXMLTools::setWithXML(dataNode,"color",color);
      GlXMLTools::setWithXML(dataNode,"cell",cell);

      GlGrid(frontTopLeft, backBottomRight,cell, color, displayDim);
    }
  }
}
