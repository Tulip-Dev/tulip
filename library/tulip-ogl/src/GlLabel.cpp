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
#include "tulip/GlLabel.h"

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
#include "tulip/GlRenderer.h"
#include "tulip/OcclusionTest.h"

#include <iostream>

using namespace std;

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::TextRenderer* tlp::GlLabel::renderer=0;
#endif
#else
tlp::TextRenderer *tlp::GlLabel::renderer=0;
#endif

namespace tlp {

  GlLabel::GlLabel() {
    init();
    if(!renderer){
      renderer=new TextRenderer;
      renderer->setContext(fontName, 20, 0, 0, 255);
      renderer->setMode(TLP_TEXTURE);
    }
  }
  GlLabel::GlLabel(Coord centerPosition,Coord size,Color fontColor,bool leftAlign):centerPosition(centerPosition),size(size),color(fontColor),leftAlign(leftAlign) {
    init();
  }

  GlLabel::GlLabel(const string &,Coord centerPosition,Coord size,Color fontColor,bool leftAlign):centerPosition(centerPosition),size(size),color(fontColor),leftAlign(leftAlign){
    init();
  }

  GlLabel::~GlLabel() {
  }
  //============================================================
  void GlLabel::init(){
    fontName=TulipBitmapDir + "font.ttf";
    renderingMode=0;
    translationAfterRotation=Coord(0,0,0);
    alignment=ON_CENTER;
    scaleToSize=true;
    minSize=10;
    maxSize=30;
    depthTestEnabled=true;
    occlusionTester=NULL;
    xRot=0;
    yRot=0;
    zRot=0;
    useLOD=false;
    occlusionBorderSize=0;
  }
  //============================================================
  void GlLabel::setText(const string& text) {
    this->text=text;
  }
  //============================================================
  void GlLabel::setPosition(const Coord &position) {
    centerPosition=position;
  }
  //============================================================
  Coord GlLabel::getPosition() {
    return centerPosition;
  }
  //============================================================
  BoundingBox GlLabel::getBoundingBox() {
    if(!leftAlign)
      return BoundingBox(centerPosition-size/2.f,centerPosition+size/2.f);
    else
      return BoundingBox(centerPosition-Coord(0,size[1]/2.f,0),centerPosition+Coord(size[0],size[1]/2.f,0));
  }
  //============================================================
  void GlLabel::setSize(const Coord &size) {
    this->size=size;
    this->sizeForOutAlign=size;
  }
  //============================================================
  Coord GlLabel::getSize() {
    return size;
  }
  //============================================================
  void GlLabel::setSizeForOutAlign(const Coord &size) {
    this->sizeForOutAlign=size;
  }
  //============================================================
  Coord GlLabel::getSizeForOutAlign() {
    return sizeForOutAlign;
  }
  //============================================================
  void GlLabel::setBoldFont() {
    renderer->setContext(TulipBitmapDir + "fontb.ttf", 20, 0, 0, 255);
  }
  //============================================================
  void GlLabel::setPlainFont() {
    renderer->setContext(TulipBitmapDir + "font.ttf", 20, 0, 0, 255);
  }
  //============================================================
  void GlLabel::setFontName(const std::string &name){
    if(GlRenderer::checkFont(name))
      fontName=name;
  }
  //============================================================
  void GlLabel::setFontNameSizeAndColor(const std::string &name, const int &size, const Color &color){
    if(GlRenderer::checkFont(name))
      fontName=name;

    renderer->setContext(fontName,size,color[0],color[1],color[2]);
    this->color=color;
  }
  //============================================================
  void GlLabel::setRenderingMode(int mode) {
    renderingMode=mode;
  }
  //============================================================
  void GlLabel::drawWithStencil(float lod, Camera *camera){
    glStencilFunc(GL_LEQUAL,stencil,0xFFFF);
    draw(lod,camera);
  }
  //============================================================
  void GlLabel::draw(float lod, Camera *camera) {

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if(depthTestEnabled)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);

    renderer->setColor(color[0], color[1], color[2]);
    renderer->setString(text, VERBATIM);

    renderer->setMode(TLP_POLYGON);

    glPolygonMode(GL_FRONT, GL_FILL);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    float w_max = 300;
    float w,h;
    float div_w, div_h;

    renderer->getBoundingBox(w_max, h, w);



    float screenH=size[1]/(sqrt(size[0]*size[0]+size[1]*size[1])/lod);

    float labelSize=(screenH/size[1])/2.5;
    if(labelSize<0)
      labelSize=-labelSize;

    float sizeScale=1.;
    if(!scaleToSize && labelSize<minSize)
      sizeScale=minSize/labelSize;
    if(!scaleToSize && labelSize>maxSize)
      sizeScale=maxSize/labelSize;

    div_w = size[0]/w;
    div_h = size[1]/h;

    if(occlusionTester && camera){
      BoundingBox labelBoundingBox;
      Coord baseCoord=centerPosition;

      switch(alignment) {
      case ON_CENTER:break;
      case ON_LEFT:baseCoord[0]-=sizeForOutAlign[0];break;
      case ON_RIGHT:baseCoord[0]+=sizeForOutAlign[0];break;
      case ON_TOP:baseCoord[1]+=sizeForOutAlign[1];break;
      case ON_BOTTOM:baseCoord[1]-=sizeForOutAlign[1];break;
      default:break;
      }

      w+=occlusionBorderSize;
      h+=occlusionBorderSize;

      Size occlusionSize;
      if(!scaleToSize){
        occlusionSize=Size(w*0.025*sizeScale,h*0.025*sizeScale,0);
      }else{
        if(div_h * w > size[0]){
          occlusionSize=Size(w*div_w*0.5,h*div_w*0.5,0);
        }else{
          occlusionSize=Size(w*div_h*0.5,h*div_h*0.5,0);
        }
      }

      float angle=M_PI*zRot/180;

      if(zRot!=0){
        BoundingBox tmpBB;
        float size0Cos=occlusionSize[0]*cos(angle);
        float size0Sin=occlusionSize[0]*sin(angle);
        float size1Cos=occlusionSize[1]*cos(angle);
        float size1Sin=occlusionSize[1]*sin(angle);
        tmpBB.expand(Coord(size0Cos-size1Sin,size0Sin+size1Cos,0));
        tmpBB.expand(Coord(size0Cos+size1Sin,size0Sin-size1Cos,0));
        tmpBB.expand(Coord(-size0Cos+size1Sin,-size0Sin-size1Cos,0));
        tmpBB.expand(Coord(-size0Cos-size1Sin,-size0Sin+size1Cos,0));

        occlusionSize[0]=tmpBB[1][0];
        occlusionSize[1]=tmpBB[1][1];
      }

      baseCoord[0]+=translationAfterRotation[0]*cos(angle)-translationAfterRotation[1]*sin(angle);
      baseCoord[1]+=translationAfterRotation[0]*sin(angle)+translationAfterRotation[1]*cos(angle);

      Matrix<float, 4> modelviewMatrix, projectionMatrix, transformMatrix;

      glGetFloatv (GL_MODELVIEW_MATRIX, (GLfloat*)&modelviewMatrix);
      glGetFloatv (GL_PROJECTION_MATRIX, (GLfloat*)&projectionMatrix);

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glMultMatrixf((GLfloat*)&projectionMatrix);
      glMultMatrixf((GLfloat*)&modelviewMatrix);
      glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&transformMatrix);
      glPopMatrix();

      labelBoundingBox.expand(projectPoint(Coord(baseCoord[0]+occlusionSize[0],baseCoord[1]+occlusionSize[1],baseCoord[2]),transformMatrix,camera->getViewport()));
      labelBoundingBox.expand(projectPoint(Coord(baseCoord[0]+occlusionSize[0],baseCoord[1]-occlusionSize[1],baseCoord[2]),transformMatrix,camera->getViewport()));
      labelBoundingBox.expand(projectPoint(Coord(baseCoord[0]-occlusionSize[0],baseCoord[1]-occlusionSize[1],baseCoord[2]),transformMatrix,camera->getViewport()));
      labelBoundingBox.expand(projectPoint(Coord(baseCoord[0]-occlusionSize[0],baseCoord[1]+occlusionSize[1],baseCoord[2]),transformMatrix,camera->getViewport()));

      if(occlusionTester->testRectangle(RectangleInt2D(labelBoundingBox[0][0],labelBoundingBox[0][1],labelBoundingBox[1][0],labelBoundingBox[1][1]))){
        glPopAttrib();
        return;
      }

      occlusionTester->addRectangle(RectangleInt2D(labelBoundingBox[0][0],labelBoundingBox[0][1],labelBoundingBox[1][0],labelBoundingBox[1][1]));
    }

    glPushMatrix();

    glTranslatef(centerPosition[0],centerPosition[1], centerPosition[2]);

    if(xRot!=0.)
      glRotatef(xRot,1.,0.,0.);
    if(yRot!=0.)
      glRotatef(yRot,0.,1.,0.);
    if(zRot!=0.)
      glRotatef(zRot,0.,0.,1.);

    switch(alignment) {
    case ON_CENTER:
      glTranslatef(translationAfterRotation[0],translationAfterRotation[1],translationAfterRotation[2]);
      break;
    case ON_LEFT:
      glTranslatef(-sizeForOutAlign[0]/2,0,0);
      break;
    case ON_RIGHT:
      glTranslatef(sizeForOutAlign[0]/2,0,0);
      break;
    case ON_TOP:
      glTranslatef(0,sizeForOutAlign[1]/2,0);
      break;
    case ON_BOTTOM:
      glTranslatef(0,-sizeForOutAlign[1]/2,0);
      break;
    default:
      break;
    }

    if(scaleToSize){
      if(div_h * w > size[0])        // too wide, so make it fit and maintain aspect ratio
        glScalef(div_w, div_w, 1);
      else
        glScalef(div_h, div_h, 1);
    }else{
      glScalef(0.05*sizeScale,0.05*sizeScale,1);
    }


    if(scaleToSize){
      float sizeRatio=size[0]/size[1];
      float fontRatio=w/h;
      if(fontRatio>sizeRatio){
        screenH=screenH/(fontRatio/sizeRatio);
      }
    }else{
      screenH*=((w*0.05*sizeScale)/size[0]);
    }

    if(screenH<0)
      screenH=-screenH;

    if(screenH < 15 && useLOD){
      float wAlign=0;
      float hAlign=0;

      switch (alignment) {
      case ON_TOP:hAlign=h/2.;break;
      case ON_BOTTOM:hAlign=-(h/2.);break;
      case ON_LEFT:wAlign = -(w/2.);break;
      case ON_RIGHT:wAlign = w/2.;break;
      default:break;
      }

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glLineWidth(screenH/5.);
      setMaterial(Color(color[0],color[1],color[2],128));
      OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();
      glBegin(GL_LINES);
      glVertex3f(-w/2.+wAlign,hAlign,0);
      glVertex3f(w/2.+wAlign,hAlign,0);
      glEnd();
      OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();
      glLineWidth(1);
    }else{
      glDisable(GL_DEPTH_TEST);
      glTranslatef(-0.75,0.,0.);
      glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_SRC_COLOR);
      renderer->draw(w, w, alignment);
    }
    glPopMatrix();
    glPopAttrib();
  }
  //===========================================================
  void GlLabel::translate(const Coord& mouvement){
    centerPosition+=mouvement;
  }
  //===========================================================
  void GlLabel::rotate(float xRot, float yRot, float zRot) {
    this->xRot=xRot;
    this->yRot=yRot;
    this->zRot=zRot;
  }
  //===========================================================
  void GlLabel::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    xmlNewProp(rootNode,BAD_CAST "type",BAD_CAST "GlLabel");

    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"text",text);
    GlXMLTools::getXML(dataNode,"renderingMode",renderingMode);
    GlXMLTools::getXML(dataNode,"fontName",fontName);
    GlXMLTools::getXML(dataNode,"centerPosition",centerPosition);
    GlXMLTools::getXML(dataNode,"translationAfterRotation",translationAfterRotation);
    GlXMLTools::getXML(dataNode,"size",size);
    GlXMLTools::getXML(dataNode,"color",color);
    GlXMLTools::getXML(dataNode,"alignment",alignment);
    GlXMLTools::getXML(dataNode,"scaleToSize",scaleToSize);
    GlXMLTools::getXML(dataNode,"minSize",minSize);
    GlXMLTools::getXML(dataNode,"maxSize",maxSize);
    GlXMLTools::getXML(dataNode,"depthTestEnabled",depthTestEnabled);
    GlXMLTools::getXML(dataNode,"leftAlign",leftAlign);
    GlXMLTools::getXML(dataNode,"xRot",xRot);
    GlXMLTools::getXML(dataNode,"yRot",yRot);
    GlXMLTools::getXML(dataNode,"zRot",zRot);
  }
  //============================================================
  void GlLabel::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      GlXMLTools::setWithXML(dataNode,"text",text);
      GlXMLTools::setWithXML(dataNode,"renderingMode",renderingMode);
      GlXMLTools::setWithXML(dataNode,"fontName",fontName);
      GlXMLTools::setWithXML(dataNode,"centerPosition",centerPosition);
      GlXMLTools::setWithXML(dataNode,"translationAfterRotation",translationAfterRotation);
      GlXMLTools::setWithXML(dataNode, "size", size);
      GlXMLTools::setWithXML(dataNode,"color",color);
      GlXMLTools::setWithXML(dataNode,"alignment",alignment);
      GlXMLTools::setWithXML(dataNode,"scaleToSize",scaleToSize);
      GlXMLTools::setWithXML(dataNode,"minSize",minSize);
      GlXMLTools::setWithXML(dataNode,"maxSize",maxSize);
      GlXMLTools::setWithXML(dataNode,"depthTestEnabled",depthTestEnabled);
      GlXMLTools::setWithXML(dataNode,"leftAlign",leftAlign);
      GlXMLTools::setWithXML(dataNode,"xRot",xRot);
      GlXMLTools::setWithXML(dataNode,"yRot",yRot);
      GlXMLTools::setWithXML(dataNode,"zRot",zRot);
    }
  }

}
