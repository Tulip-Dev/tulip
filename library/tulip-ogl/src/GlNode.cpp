/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "tulip/OpenGlConfigManager.h"

#include "tulip/GlNode.h"

#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/PreferenceManager.h>

#include "tulip/GlTools.h"
#include "tulip/GlyphManager.h"
#include "tulip/GlDisplayListManager.h"
#include "tulip/OcclusionTest.h"
#include "tulip/TextRenderer.h"
#include "tulip/GlTLPFeedBackBuilder.h"
#include "tulip/GlSceneVisitor.h"
#include "tulip/GlGraphRenderingParameters.h"
#include "tulip/GlRenderer.h"
#include "tulip/GlTextureManager.h"
#include "tulip/GlVertexArrayManager.h"

#include <iostream>

using namespace std;

namespace tlp {

  BoundingBox GlNode::getBoundingBox(GlGraphInputData* data) {
    node n=node(id);
    if(data->elementRotation->getNodeValue(n)==0){
      BoundingBox box;
      box.expand(data->elementLayout->getNodeValue(n)-data->elementSize->getNodeValue(n)/2.f);
      box.expand(data->elementLayout->getNodeValue(n)+data->elementSize->getNodeValue(n)/2.f);
      assert(box.isValid());
      return box;
    }else{
      float cosAngle=cos((float)data->elementRotation->getNodeValue(n)/180.*M_PI);
      float sinAngle=sin((float)data->elementRotation->getNodeValue(n)/180.*M_PI);
      Coord tmp1(data->elementSize->getNodeValue(n)/2.f);
      Coord tmp2(tmp1[0] ,-tmp1[1], tmp1[2]);
      Coord tmp3(-tmp1[0],-tmp1[1],-tmp1[2]);
      Coord tmp4(-tmp1[0], tmp1[1],-tmp1[2]);
      tmp1=Coord(tmp1[0]*cosAngle-tmp1[1]*sinAngle,tmp1[0]*sinAngle+tmp1[1]*cosAngle,tmp1[2]);
      tmp2=Coord(tmp2[0]*cosAngle-tmp2[1]*sinAngle,tmp2[0]*sinAngle+tmp2[1]*cosAngle,tmp2[2]);
      tmp3=Coord(tmp3[0]*cosAngle-tmp3[1]*sinAngle,tmp3[0]*sinAngle+tmp3[1]*cosAngle,tmp3[2]);
      tmp4=Coord(tmp4[0]*cosAngle-tmp4[1]*sinAngle,tmp4[0]*sinAngle+tmp4[1]*cosAngle,tmp4[2]);
      BoundingBox bb;
      bb.expand(data->elementLayout->getNodeValue(n)+tmp1);
      bb.expand(data->elementLayout->getNodeValue(n)+tmp2);
      bb.expand(data->elementLayout->getNodeValue(n)+tmp3);
      bb.expand(data->elementLayout->getNodeValue(n)+tmp4);
      return bb;
    }
  }

  void GlNode::acceptVisitor(GlSceneVisitor *visitor) {
    visitor->visit(this);
  }

  void GlNode::draw(float lod,GlGraphInputData* data,Camera*) {
    const Color& colorSelect2=data->parameters->getSelectionColor();

    glEnable(GL_CULL_FACE);
    GLenum error = glGetError();
    if(GlDisplayListManager::getInst().beginNewDisplayList("selection")) {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glDisable(GL_LIGHTING);
      glDepthFunc(GL_LEQUAL);
      glLineWidth(3);
      tlp::cube(GL_LINE_LOOP);
      glPopAttrib();
      GlDisplayListManager::getInst().endNewDisplayList();
    }

    node n=node(id);
    /*if(data->parameters->isElementZOrdered()){
      if(data->elementColor->getNodeValue(n)[3]!=255){
        GlPointManager::getInst().endRendering();
        GlPointManager::getInst().beginRendering();
      }
    }*/

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

    const Color& fillColor = data->elementColor->getNodeValue(n);
    const Color& strokeColor = data->elementBorderColor->getNodeValue(n);
    const Color& textColor = data->elementLabelColor->getNodeValue(n);
    GlTextureManager::getInst().setAnimationFrame(data->elementAnimationFrame->getNodeValue(n));

    if(data->parameters->getFeedbackRender()) {
      glPassThrough(TLP_FB_COLOR_INFO);
      glPassThrough(fillColor[0]);glPassThrough(fillColor[1]);glPassThrough(fillColor[2]);glPassThrough(fillColor[3]);
      glPassThrough(strokeColor[0]);glPassThrough(strokeColor[1]);glPassThrough(strokeColor[2]);glPassThrough(strokeColor[3]);
      glPassThrough(textColor[0]);glPassThrough(textColor[1]);glPassThrough(textColor[2]);glPassThrough(textColor[3]);

      glPassThrough(TLP_FB_BEGIN_NODE);
      glPassThrough(id); //id of the node for the feed back mode
    }

    bool selected = data->elementSelected->getNodeValue(n);
    if (lod < 10.0) { //less than four pixel on screen, we use points instead of glyphs
      if (lod < 1) lod = 1;
      int size= sqrt(lod);

      if(data->getGlVertexArrayManager()->renderingIsBegin()){
        if(size<2)
          data->getGlVertexArrayManager()->activatePointNodeDisplay(this,true,selected);
        else
          data->getGlVertexArrayManager()->activatePointNodeDisplay(this,false,selected);
      }else{
        if(size>2)
          size=2;
        const Color& color = selected ? colorSelect2 : fillColor;

        OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();
        glDisable(GL_LIGHTING);
        setColor(color);
        glPointSize(size);
        glBegin(GL_POINTS);
        glVertex3f(nodeCoord[0], nodeCoord[1], nodeCoord[2]+nodeSize[2]/2.);
        glEnd();
        glEnable(GL_LIGHTING);
        OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();
      }
      return;

    }
    //draw a glyph or make recursive call for meta nodes
    glPushMatrix();
    glTranslatef(nodeCoord[0], nodeCoord[1], nodeCoord[2]);
    glRotatef(data->elementRotation->getNodeValue(n), 0., 0., 1.);
    glScalef(nodeSize[0], nodeSize[1], nodeSize[2]);

    data->glyphs.get(data->elementShape->getNodeValue(n))->draw(n,lod);

    if (selected) {
      //glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil()-1,0xFFFF);
      setColor(colorSelect2);
      OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();
      GlDisplayListManager::getInst().callDisplayList("selection");
      OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();
    }
    glPopMatrix();

    if (selected) {
      glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil(),0xFFFF);
    }

    GlTextureManager::getInst().setAnimationFrame(0);

    if(data->parameters->getFeedbackRender()) {
      glPassThrough(TLP_FB_END_NODE);
    }
    if ( error != GL_NO_ERROR)
    cerr << "end [OpenGL Error] => " << gluErrorString(error) << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;
  }

  void GlNode::drawLabel(bool drawSelect,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data,float lod) {
    node n=node(id);
    bool selected=data->elementSelected->getNodeValue(n);
    if(drawSelect!=selected)
      return;

    drawLabel(test,renderer,data,lod);
  }

  void GlNode::drawLabel(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data) {
    GlNode::drawLabel(test,renderer,data,0.);
  }

  void GlNode::drawLabel(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data,float lod) {
    const Color& colorSelect2=data->parameters->getSelectionColor();

    node n=node(id);

    if(data->glyphs.get(data->elementShape->getNodeValue(n))->renderLabel())
      return;

    bool selected=data->elementSelected->getNodeValue(n);

    const Color& fontColor = selected ? colorSelect2 :
      data->elementLabelColor->getNodeValue(n);

    if(fontColor.getA()==0)
      return;

    const string &tmp = data->elementLabel->getNodeValue(n);
    if (tmp.length() < 1) {
      return;
    }

    Size size=data->elementSize->getNodeValue(n);

    if(!data->getGraph()->isMetaNode(n)){
      if(selected)
        glStencilFunc(GL_LEQUAL,data->parameters->getSelectedNodesStencil() ,0xFFFF);
      else
        glStencilFunc(GL_LEQUAL,data->parameters->getNodesLabelStencil(),0xFFFF);
    }else{
      if(selected)
        glStencilFunc(GL_LEQUAL,data->parameters->getSelectedMetaNodesStencil() ,0xFFFF);
      else
        glStencilFunc(GL_LEQUAL,data->parameters->getMetaNodesLabelStencil(),0xFFFF);
    }

    string fontName=data->elementFont->getNodeValue(n);
    int fontSize=data->elementFontSize->getNodeValue(n);
    if(!GlRenderer::checkFont(fontName))
        fontName=data->parameters->getFontsPath()+"font.ttf";
    if(fontSize==0)
      fontSize=18;

    if(selected)
      renderer->setContext(fontName, fontSize+2, 0, 0, 255);
    else
      renderer->setContext(fontName, fontSize, 255, 255, 255);

    const Coord &nodeCoord = data->elementLayout->getNodeValue(n);
    const Size  &nodeSize  = data->elementSize->getNodeValue(n);
    int labelPos = data->elementLabelPosition->getNodeValue(n);
    Coord nodePos(0,0,0);
    switch (labelPos) {
    case ON_TOP:
      nodePos.setY(nodeSize.getH()/2);
      break;
    case ON_BOTTOM:
      nodePos.setY(-nodeSize.getH()/2);
      break;
    case ON_LEFT:
      nodePos.setX(-nodeSize.getW()/2);
      break;
    case ON_RIGHT:
      nodePos.setX(nodeSize.getW()/2);
    default:
      break;
    }
    //if (elementSelected->getNodeValue(n) != mode) return;

    float w_max = 300;
    float w,h;
    float div_w, div_h;
    float screenH=20.;
    BoundingBox includeBB;
    int fontType=data->parameters->getFontsType();

    switch(fontType){
    case 0:
    case 2:{
      if(fontType==0)
        renderer->setMode(TLP_POLYGON);
      else
        renderer->setMode(TLP_TEXTURE);

      renderer->setColor(fontColor[0], fontColor[1], fontColor[2]);
      renderer->setString(tmp, VERBATIM);

      renderer->getBoundingBox(w_max, h, w);

      data->glyphs.get(data->elementShape->getNodeValue(n))->getIncludeBoundingBox(includeBB);
      Vec3f centerBB = includeBB.center();
      Vec3f sizeBB = includeBB[1]-includeBB[0];

      glPushMatrix();

      glTranslatef(nodeCoord[0], nodeCoord[1], nodeCoord[2]);
      glRotatef(data->elementRotation->getNodeValue(n), 0., 0., 1.);

      if(fontType==0){
        data->glyphs.get(data->elementShape->getNodeValue(n))->getIncludeBoundingBox(includeBB);
        if(includeBB[1][2]==0.)
          glTranslatef(nodePos[0], nodePos[1], nodePos[2]+0.01);
        else
          glTranslatef(nodePos[0], nodePos[1], nodePos[2]+nodeSize[2]/2.+0.01);
      }else{
        glTranslatef(nodePos[0], nodePos[1], nodePos[2]);
      }

      glTranslatef(centerBB[0]*nodeSize[0], centerBB[1]*nodeSize[1], 0.);
      glScalef(sizeBB[0],sizeBB[1],1.);

      div_w = nodeSize.getW()/w;
      div_h = nodeSize.getH()/h;
      if(div_h > div_w)
        glScalef(div_w, div_w, 1);
      else
        glScalef(div_h, div_h, 1);

      if(fontType==0)
        glDepthFunc(GL_LEQUAL );

      if(lod!=0){
        screenH=size[1]/(sqrt(size[0]*size[0]+size[1]*size[1])/lod);
        float sizeRatio=size[0]/size[1];
        float fontRatio=w/h;
        if(fontRatio>sizeRatio){
          screenH=screenH/(fontRatio/sizeRatio);
        }
      }

      if(screenH < 15){
        float wAlign=0;
        float hAlign=0;
        switch (labelPos) {
        case ON_TOP:
          hAlign=h/2.;
          break;
        case ON_BOTTOM:
          hAlign=-(h/2.);
          break;
        case ON_LEFT:
          wAlign = -(w/2.);
          break;
        case ON_RIGHT:
          wAlign = w/2.;
        default:
          break;
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(screenH/5.);
        setColor(Color(fontColor[0],fontColor[1],fontColor[2],128));
        OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();
        glBegin(GL_LINES);
        glVertex3f(-w/2.+wAlign,hAlign,0);
        glVertex3f(w/2.+wAlign,hAlign,0);
        glEnd();
        OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();
        glLineWidth(1);
      }else{
        if(fontType==0){
          renderer->draw(w,w, labelPos);
        }else{
          glEnable( GL_TEXTURE_2D);
          glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_SRC_COLOR);
          renderer->draw(w, w, labelPos);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glDisable( GL_TEXTURE_2D);
        }
      }
      glPopMatrix();
      break;
    }
    case 1:
      drawPixmapFont(test,renderer,data,tmp, fontColor, nodePos+nodeCoord, labelPos, selected, nodeSize.getW());
      break;
    default:
      cerr << "GlGraph::DrawNodes unknown fonts" << endl;
      break;
    }
  }

  void GlNode::drawPixmapFont(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data,const string &str, const Color &col,  const Coord &position,
			      int labelPos, bool, float) {
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

  void GlNode::getPointAndColor(GlGraphInputData *inputData,std::vector<Coord> &pointsCoordsArray,std::vector<Color> &pointsColorsArray){
    node n=node(id);
    const Coord &nodeCoord = inputData->elementLayout->getNodeValue(n);
    const Color& fillColor = inputData->elementColor->getNodeValue(n);
    pointsCoordsArray.push_back(nodeCoord);
    pointsColorsArray.push_back(fillColor);
  }
}
