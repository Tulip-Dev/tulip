#include "tulip/GlNode.h"

#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>

#include "tulip/GlTools.h"
#include "tulip/GlyphManager.h"
#include "tulip/GlDisplayListManager.h"
#include "tulip/OcclusionTest.h"
#include "tulip/TextRenderer.h"
#include "tulip/GlTLPFeedBackBuilder.h"
#include "tulip/GlSceneVisitor.h"
#include "tulip/GlGraphRenderingParameters.h"

#include <iostream>

using namespace std;

namespace tlp {

  static const Color colorSelect2 = Color(255, 102, 255, 255);

  BoundingBox GlNode::getBoundingBox(GlGraphInputData* data) {
    node n=node(id);
    //cout << data->elementLayout->getNodeValue(n) << endl;
    return BoundingBox(data->elementLayout->getNodeValue(n)-data->elementSize->getNodeValue(n)/2,data->elementLayout->getNodeValue(n)+data->elementSize->getNodeValue(n)/2);
  }

  void GlNode::acceptVisitor(GlSceneVisitor *visitor) {
    visitor->visit(this);
  }

  void GlNode::draw(float lod,GlGraphInputData* data,Camera* camera) {
    glEnable(GL_CULL_FACE);
    GLenum error = glGetError();
    if(GlDisplayListManager::getInst().beginNewDisplayList("selection")) {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glDisable(GL_LIGHTING);
      glDepthFunc(GL_LEQUAL);
      glLineWidth(3);
      setColor(colorSelect2);
      tlp::cube(GL_LINE_LOOP);
      glPopAttrib();
      GlDisplayListManager::getInst().endNewDisplayList();
    }

    node n=node(id);
    if (data->elementSelected->getNodeValue(n)) {
      glDisable(GL_DEPTH_TEST);
      if(data->getGraph()->isMetaNode(n)) {
	glStencilFunc(GL_LEQUAL,data->parameters->getSelectedMetaNodesStencil(),0xFFFF);
      }else{
	glStencilFunc(GL_LEQUAL,data->parameters->getSelectedNodesStencil(),0xFFFF);
      }
    }else{
      glEnable(GL_DEPTH_TEST);
      if(data->getGraph()->isMetaNode(n)) {
	glStencilFunc(GL_LEQUAL,data->parameters->getMetaNodesStencil(),0xFFFF);
      }else{
	glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil(),0xFFFF);
      }
    }

    const Coord &nodeCoord = data->elementLayout->getNodeValue(n);
    const Size &nodeSize = data->elementSize->getNodeValue(n);

    Color fillColor = data->elementColor->getNodeValue(n);
    Color strokeColor = data->elementBorderColor->getNodeValue(n);
    Color textColor = data->elementLabelColor->getNodeValue(n);

    if(data->parameters->getFeedbackRender()) {
      glPassThrough(TLP_FB_COLOR_INFO);
      glPassThrough(fillColor[0]);glPassThrough(fillColor[1]);glPassThrough(fillColor[2]);
      glPassThrough(strokeColor[0]);glPassThrough(strokeColor[1]);glPassThrough(strokeColor[2]);
      glPassThrough(textColor[0]);glPassThrough(textColor[1]);glPassThrough(textColor[2]);

      glPassThrough(TLP_FB_BEGIN_NODE);
      glPassThrough(id); //id of the node for the feed back mode
    }

    if (lod < 10.0) { //less than four pixel on screen, we use points instead of glyphs
      if (lod < 1) lod = 1;
      glDisable(GL_LIGHTING);
      //const Color &nodeColor = data->elementColor->getNodeValue(n);
      if (!data->elementSelected->getNodeValue(n)) {
	setColor(fillColor);
	glPointSize(sqrt(lod));
	glBegin(GL_POINTS);
	  glVertex3f(nodeCoord[0], nodeCoord[1], nodeCoord[2]+nodeSize[2]);
	 glEnd();
      }
      else {
	setColor(colorSelect2);
	glPointSize(sqrt(lod)+1);
	glBegin(GL_POINTS);
	  glVertex3f(nodeCoord[0], nodeCoord[1], nodeCoord[2]+nodeSize[2]);
	glEnd();
      }
      glEnable(GL_LIGHTING);
    } else { //draw a glyph or make recursive call for meta nodes
      glPushMatrix();
      glTranslatef(nodeCoord[0], nodeCoord[1], nodeCoord[2]);
      glRotatef(data->elementRotation->getNodeValue(n), 0., 0., 1.);
      glScalef(nodeSize[0], nodeSize[1], nodeSize[2]);

      data->glyphs.get(data->elementShape->getNodeValue(n))->draw(n,lod);

      if (data->elementSelected->getNodeValue(n)) {
	//glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil()-1,0xFFFF);
	GlDisplayListManager::getInst().callDisplayList("selection");
      }
      glPopMatrix();
    }

    if (data->elementSelected->getNodeValue(n)) {
      glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil(),0xFFFF);
    }

    if(data->parameters->getFeedbackRender()) {
      glPassThrough(TLP_FB_END_NODE);
    }
    if ( error != GL_NO_ERROR)
    cerr << "end [OpenGL Error] => " << gluErrorString(error) << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;
  }

  void GlNode::drawLabel(bool drawSelect,bool drawNodesLabel,bool drawEdgesLabel,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data) {
    if(!drawNodesLabel)
      return;

    node n=node(id);

    bool select=data->elementSelected->getNodeValue(n);
        if(drawSelect!=select)
          return;

    const string &tmp = data->elementLabel->getNodeValue(n);
    if (tmp.length() < 1) {
      return;
    }

    if(!data->getGraph()->isMetaNode(n)){
      if(select)
        glStencilFunc(GL_LEQUAL,data->parameters->getSelectedNodesStencil() ,0xFFFF);
      else
        glStencilFunc(GL_LEQUAL,data->parameters->getNodesLabelStencil(),0xFFFF);
    }else{
      if(select)
        glStencilFunc(GL_LEQUAL,data->parameters->getSelectedMetaNodesStencil() ,0xFFFF);
      else
        glStencilFunc(GL_LEQUAL,data->parameters->getMetaNodesLabelStencil(),0xFFFF);
    }

    if(select)
      renderer->setContext(data->parameters->getFontsPath() + "font.ttf", 20, 0, 0, 255);
    else
      renderer->setContext(data->parameters->getFontsPath() + "font.ttf", 18, 255, 255, 255);

    const Coord &nodeCoord = data->elementLayout->getNodeValue(n);
    const Size  &nodeSize  = data->elementSize->getNodeValue(n);
    int labelPos = data->elementLabelPosition->getNodeValue(n);
    Coord nodePos(nodeCoord);
    switch (labelPos) {
    case ON_TOP:
      nodePos.setY(nodeCoord.getY() + nodeSize.getH()/2);
      break;
    case ON_BOTTOM:
      nodePos.setY(nodeCoord.getY() - nodeSize.getH()/2);
      break;
    case ON_LEFT:
      nodePos.setX(nodeCoord.getX() - nodeSize.getW()/2);
      break;
    case ON_RIGHT:
      nodePos.setX(nodeCoord.getX() + nodeSize.getW()/2);
    default:
      break;
    }
    //if (elementSelected->getNodeValue(n) != mode) return;

    Color fontColor = data->elementLabelColor->getNodeValue(n);

    if (select)
      fontColor = colorSelect2;

    float w_max = 300;
    float w,h;
    float div_w, div_h;
    BoundingBox includeBB;

    switch(data->parameters->getFontsType()){
    case 0:
      renderer->setMode(TLP_POLYGON);
      renderer->setColor(fontColor[0], fontColor[1], fontColor[2]);
      renderer->setString(tmp, VERBATIM);
      //      w_max = nodeSize.getW()*50.0;
      renderer->getBoundingBox(w_max, h, w);
      glPushMatrix();
      data->glyphs.get(data->elementShape->getNodeValue(n))->getIncludeBoundingBox(includeBB);
      if(includeBB.second[2]==0.)
        glTranslatef(nodePos[0], nodePos[1], nodePos[2]+0.01);
      else
        glTranslatef(nodePos[0], nodePos[1], nodePos[2]+nodeSize[2]/2.+0.01);
      glRotatef(data->elementRotation->getNodeValue(n), 0., 0., 1.);
      div_w = nodeSize.getW()/w;
      div_h = nodeSize.getH()/h;
      if(div_h > div_w)
	glScalef(div_w, div_w, 1);
      else
	glScalef(div_h, div_h, 1);
      glDepthFunc(GL_LEQUAL );
      renderer->draw(w,w, labelPos);
      glPopMatrix();
      break;
    case 1:
      drawPixmapFont(test,renderer,data,tmp, fontColor, nodePos, labelPos, data->elementSelected->getNodeValue(n), nodeSize.getW());
      break;
    case 2:
      //if (projectSize(nodeCoord, nodeSize, camera->projectionMatrix, camera->modelviewMatrix, camera->getViewport()) < 8.0) return;

      renderer->setMode(TLP_TEXTURE);
      renderer->setColor(fontColor[0], fontColor[1], fontColor[2]);
      renderer->setString(tmp, VERBATIM);

      //      w_max = nodeSize.getW();
      renderer->getBoundingBox(w_max, h, w);
      glPushMatrix();
      glTranslatef(nodePos[0], nodePos[1], nodePos[2]);
      glRotatef(data->elementRotation->getNodeValue(n), 0., 0., 1.);
      div_w = nodeSize.getW()/w;
      div_h = nodeSize.getH()/h;
      if(div_h > div_w)
	glScalef(div_w, div_w, 1);
      else
	glScalef(div_h, div_h, 1);
      glEnable( GL_TEXTURE_2D);
      glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_SRC_COLOR);
      renderer->draw(w, w, labelPos);
      glDisable( GL_TEXTURE_2D);
      glPopMatrix();
      break;
    default:
      cerr << "GlGraph::DrawNodes unknown fonts" << endl;
      break;
    }
  }

  void GlNode::drawPixmapFont(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data,const string &str, const Color &col,  const Coord &position,
			      int labelPos, bool selected, float width) {
    int rastPos[4];
    float w_max = 300;
    float w,h;
    unsigned int labelsBorder = data->parameters->getLabelsBorder();
    //need to be done before glRasterPos3f to set GL_RASTER_COLOR correctly
    //Gl_RASTER_COLOR is used by FTGL in BITMAP and PIXMAP mode instead of GL_COLOR
    setColor(Color(col[0],col[1],col[2],255));

    glRasterPos3f(position[0], position[1], position[2]);

    glGetIntegerv(GL_CURRENT_RASTER_POSITION, (GLint *) rastPos);
    if(test->testRectangle(RectangleInt2D(rastPos[0] - labelsBorder - 5,
					  rastPos[1]  - labelsBorder - 5,
					  rastPos[0]  + labelsBorder + 5,
					  rastPos[1]  + labelsBorder + 5)))
      return;

    renderer->setMode(TLP_PIXMAP);
    renderer->setString(str, VERBATIM);
    //renderer->setString(str, XML);

    renderer->setColor(col[0], col[1], col[2]);
    //  w_max = width;
    renderer->getBoundingBox(w_max, h, w);

    if(!test->addRectangle(RectangleInt2D(rastPos[0]-(int)(w/2.0) - labelsBorder,
					  rastPos[1]-(int)(h/2.0) - labelsBorder,
					  rastPos[0]+(int)(w/2.0) + labelsBorder,
					  rastPos[1]+(int)(h/2.0) + labelsBorder))) {
      renderer->draw(w, w, labelPos);
    }
  }
}
