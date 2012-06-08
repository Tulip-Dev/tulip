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
#include <libxml/parser.h>
#include <iostream>

#include <FTGL/ftgl.h>

#include <tulip/GlLabel.h>
#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>

#include <tulip/GlTools.h>
#include <tulip/GlyphManager.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/OcclusionTest.h>
#include <tulip/GlTLPFeedBackBuilder.h>
#include <tulip/OcclusionTest.h>
#include <tulip/GlTextureManager.h>


using namespace std;

namespace tlp {

static const int SpaceBetweenLine=5;

std::map<std::string, FTGLPolygonFont*> GlLabel::fontMap;
std::map<std::string, FTOutlineFont*> GlLabel::borderFontMap;

GlLabel::GlLabel() {
  init();
}
GlLabel::GlLabel(Coord centerPosition,Coord labelSize,Color fontColor,bool leftAlign):centerPosition(centerPosition),size(Size(labelSize[0],labelSize[1],labelSize[2])),color(fontColor),leftAlign(leftAlign) {
  init();
}

GlLabel::GlLabel(Coord centerPosition,Size size,Color fontColor,bool leftAlign):centerPosition(centerPosition),size(size),color(fontColor),leftAlign(leftAlign) {
  init();
}


GlLabel::GlLabel(const string &,Coord centerPosition,Coord labelSize,Color fontColor,bool leftAlign):centerPosition(centerPosition),size(Size(labelSize[0],labelSize[1],labelSize[2])),color(fontColor),leftAlign(leftAlign) {
  init();
}

GlLabel::~GlLabel() {}
//============================================================
void GlLabel::init() {
  fontName=TulipBitmapDir + "font.ttf";

  if (fontMap.find(fontName) == fontMap.end()) {
    fontMap[fontName]=new FTPolygonFont(fontName.c_str());
    borderFontMap[fontName]=new FTGLOutlineFont(fontName.c_str());
  }

  font = fontMap[fontName];
  borderFont = borderFontMap[fontName];
  fontSize=20;

  outlineColor=Color(0,0,0,255);
  outlineSize=1.;
  renderingMode=0;
  translationAfterRotation=Coord(0,0,0);
  alignment=ON_CENTER;
  scaleToSize=true;
  useMinMaxSize=false;
  minSize=10;
  maxSize=30;
  depthTestEnabled=true;
  occlusionTester=NULL;
  xRot=0;
  yRot=0;
  zRot=0;
  useLOD=false;
  labelsDensity=100;
  billboarded=false;

  sizeForOutAlign = size;
}
//============================================================
void GlLabel::setText(const string& text) {
  this->text=text;
}
//============================================================
std::string GlLabel::getText() const {
  return this->text;
}
//============================================================
void GlLabel::setTextBeforeRendering(const std::string& text) {
  if(font->FaceSize()!=(unsigned int)fontSize) {
    font->FaceSize(fontSize);
    borderFont->FaceSize(fontSize);
  }

  // split each line
  textVector.clear();
  textWidthVector.clear();
  size_t lastPos=0;
  size_t pos=text.find_first_of("\n");

  while(pos!=string::npos) {
    textVector.push_back(text.substr(lastPos,pos-lastPos));
    lastPos=pos+1;
    pos=text.find_first_of("\n",pos+1);
  }

  textVector.push_back(text.substr(lastPos)+" ");
  //Text bounding box computation
  textBoundingBox=BoundingBox();

  float x1,y1,z1,x2,y2,z2,w1,w2;

  //Here we compute height of the text based on the char |
  stringstream strstr;
  strstr << "|" ;

  for(unsigned int i=0; i<textVector.size(); ++i)
    strstr << endl << "|" ;

  font->BBox(strstr.str().c_str(),x1,y1,z1,x2,y2,z2);

  // After we compute width of text
  for(vector<string>::iterator it=textVector.begin(); it!=textVector.end(); ++it) {
    font->BBox((*it).c_str(),x1,w1,z1,x2,w2,z2);

    textWidthVector.push_back(x2-x1);

    if(it==textVector.begin()) {
      textBoundingBox.expand(Coord(0,y1,z1));
      textBoundingBox.expand(Coord(x2-x1,y2,z2));
    }
    else {
      font->BBox((*it).c_str(),x1,y1,z1,x2,y2,z2);

      if(x2-x1>textBoundingBox[1][0])
        textBoundingBox[1][0]=(x2-x1);

      textBoundingBox[0][1]-=fontSize+SpaceBetweenLine;
    }
  }
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
BoundingBox GlLabel::getTextBoundingBox() {
  setTextBeforeRendering(text);

  return textBoundingBox;
}
//============================================================
void GlLabel::setSize(const Coord& size) {
  setSize(Size(size[0],size[1],size[2]));
}
//============================================================
void GlLabel::setSize(const Size &size) {
  this->size=size;
  this->sizeForOutAlign=size;
}
//============================================================
Coord GlLabel::getSize() {
  Size s = getLabelSize();
  return Coord(s[0],s[1],s[2]);
}
//============================================================
Size GlLabel::getLabelSize() const {
  return size;
}
//============================================================
void GlLabel::setSizeForOutAlign(const Coord &size) {
  setSizeForOutAlign(Size(size[0],size[1],size[2]));
}
//============================================================
void GlLabel::setSizeForOutAlign(const Size &size) {
  this->sizeForOutAlign=size;
}
//============================================================
Coord GlLabel::getSizeForOutAlign() {
  Size s = getLabelSizeForOutAlign();
  return Coord(s[0],s[1],s[2]);
}
//============================================================
Size GlLabel::getLabelSizeForOutAlign() const {
  return sizeForOutAlign;
}
//============================================================
void GlLabel::setBoldFont() {
  setFontName(TulipBitmapDir + "fontb.ttf");
  fontSize=18;
}
//============================================================
void GlLabel::setPlainFont() {
  setFontName(TulipBitmapDir + "font.ttf");
  fontSize=18;
}
//============================================================
void GlLabel::setFontName(const std::string &name) {
  if(fontName==name)
    return;

  fontName=name;

  if (fontMap.find(fontName) == fontMap.end()) {
    fontMap[fontName] = new FTGLPolygonFont(fontName.c_str());
    borderFontMap[fontName]=new FTOutlineFont(fontName.c_str());

    if(fontMap[fontName]->Error() || borderFontMap[fontName]->Error()) {
      if(fontName=="")
        cerr << "Error in font loading: no font name" << endl;
      else
        cerr << "Error in font loading: " << fontName << " cannot be loaded" << endl;

      delete fontMap[fontName];
      delete borderFontMap[fontName];
      fontMap.erase(fontName);
      borderFontMap.erase(fontName);

      fontName=TulipBitmapDir + "font.ttf";
    }
  }

  font = fontMap[fontName];
  borderFont = borderFontMap[fontName];
}
//============================================================
void GlLabel::setFontNameSizeAndColor(const std::string &name, const int &size, const Color &color) {
  setFontName(name);
  fontSize=size;
  this->color=color;
}
//============================================================
void GlLabel::setRenderingMode(int mode) {
  renderingMode=mode;
}
//============================================================
void GlLabel::drawWithStencil(float lod, Camera *camera) {
  glStencilFunc(GL_LEQUAL,stencil,0xFFFF);
  draw(lod,camera);
}
//============================================================
void GlLabel::draw(float lod, Camera *camera) {

  if(fontSize<=0)
    return;

  setTextBeforeRendering(text);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  if(depthTestEnabled)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  glPolygonMode(GL_FRONT, GL_FILL);
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);

  //Store width and height of the text
  float w=textBoundingBox[1][0]-textBoundingBox[0][0];
  float h=textBoundingBox[1][1]-textBoundingBox[0][1];

  //Compute scale of the text if text is scaled to a size
  float div_w, div_h;
  div_w = size[0]/w;
  div_h = size[1]/h;

  float screenH;
  float multiLineH=h;

  //Here the 4.5 number is the size of space between two lines
  if(textVector.size()>1)
    multiLineH=(h-(textVector.size()-1)*4.5)/textVector.size();

  if(camera) {
    //Here we compute the deformation of the bounding box when we have a rotation on the camera
    float a=acos(camera->getUp()[1]/sqrt(camera->getUp()[0]*camera->getUp()[0]+camera->getUp()[1]*camera->getUp()[1]));
    a=M_PI/4-a;

    if(a<-M_PI/4)
      a=-M_PI/2-a;

    //We compute the size of the text on the screen
    screenH=static_cast<float>(multiLineH*(lod/(cos(a)*sqrt((lodBoundingBox[1][0]-lodBoundingBox[0][0])*(lodBoundingBox[1][0]-lodBoundingBox[0][0])+(lodBoundingBox[1][1]-lodBoundingBox[0][1])*(lodBoundingBox[1][1]-lodBoundingBox[0][1])+(lodBoundingBox[1][2]-lodBoundingBox[0][2])*(lodBoundingBox[1][2]-lodBoundingBox[0][2])))))/2.f;
  }
  else {
    screenH=static_cast<float>(multiLineH)/2.f;
  }

  //Scale of the text
  float scaleToApply=1.;

  if(scaleToSize) {
    if(div_h * w > size[0]) {
      scaleToApply=div_w;
    }
    else {
      scaleToApply=div_h;
    }
  }
  else {
    scaleToApply=0.05f;
    float tmpScreenH=screenH*0.05f;

    if(useMinMaxSize) {
      if(tmpScreenH<minSize) {
        scaleToApply*=minSize/tmpScreenH;
      }

      if(tmpScreenH>maxSize) {
        scaleToApply*=maxSize/tmpScreenH;
      }
    }
  }

  //Occlusion part of the label
  if(occlusionTester && camera && labelsDensity!=100) {
    BoundingBox labelBoundingBox;
    Coord baseCoord=centerPosition;

    float wModified=w;
    float hModified=h;

    if(labelsDensity<=0) {
      wModified-=labelsDensity;
      hModified-=labelsDensity;
    }
    else {
      wModified=w-w*(((float)labelsDensity)/(100.f));
      hModified=h-h*(((float)labelsDensity)/(100.f));
    }

    switch(alignment) {
    case ON_CENTER:
      break;

    case ON_LEFT:
      baseCoord[0]-=sizeForOutAlign[0]/2+wModified*scaleToApply/2.f;
      break;

    case ON_RIGHT:
      baseCoord[0]+=sizeForOutAlign[0]/2+wModified*scaleToApply/2.f;
      break;

    case ON_TOP:
      baseCoord[1]+=sizeForOutAlign[1]/2+hModified*scaleToApply/2.f;
      break;

    case ON_BOTTOM:
      baseCoord[1]-=sizeForOutAlign[1]/2+hModified*scaleToApply/2.f;
      break;

    default:
      break;
    }

    Size occlusionSize(wModified*scaleToApply/2.f,hModified*scaleToApply/2.f,0);

    float angle=static_cast<float>(M_PI*zRot/180);

    if(zRot!=0) {
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

    BoundingBox bbTmp;
    bbTmp.expand(Coord(baseCoord[0]+occlusionSize[0],baseCoord[1]+occlusionSize[1],baseCoord[2]));
    bbTmp.expand(Coord(baseCoord[0]+occlusionSize[0],baseCoord[1]-occlusionSize[1],baseCoord[2]));
    bbTmp.expand(Coord(baseCoord[0]-occlusionSize[0],baseCoord[1]-occlusionSize[1],baseCoord[2]));
    bbTmp.expand(Coord(baseCoord[0]-occlusionSize[0],baseCoord[1]+occlusionSize[1],baseCoord[2]));

    labelBoundingBox.expand(projectPoint(Coord(baseCoord[0]+occlusionSize[0],baseCoord[1]+occlusionSize[1],baseCoord[2]),transformMatrix,camera->getViewport()));
    labelBoundingBox.expand(projectPoint(Coord(baseCoord[0]+occlusionSize[0],baseCoord[1]-occlusionSize[1],baseCoord[2]),transformMatrix,camera->getViewport()));
    labelBoundingBox.expand(projectPoint(Coord(baseCoord[0]-occlusionSize[0],baseCoord[1]-occlusionSize[1],baseCoord[2]),transformMatrix,camera->getViewport()));
    labelBoundingBox.expand(projectPoint(Coord(baseCoord[0]-occlusionSize[0],baseCoord[1]+occlusionSize[1],baseCoord[2]),transformMatrix,camera->getViewport()));

    if(occlusionTester->testRectangle(RectangleInt2D(static_cast<int>(labelBoundingBox[0][0]),static_cast<int>(labelBoundingBox[0][1]),static_cast<int>(labelBoundingBox[1][0]),static_cast<int>(labelBoundingBox[1][1])))) {
      glPopAttrib();
      return;
    }

    occlusionTester->addRectangle(RectangleInt2D(static_cast<int>(labelBoundingBox[0][0]),static_cast<int>(labelBoundingBox[0][1]),static_cast<int>(labelBoundingBox[1][0]),static_cast<int>(labelBoundingBox[1][1])));
  }

  glPushMatrix();

  //Translation and rotation
  glTranslatef(centerPosition[0],centerPosition[1], centerPosition[2]);

  if(xRot!=0.)
    glRotatef(xRot,1.,0.,0.);

  if(yRot!=0.)
    glRotatef(yRot,0.,1.,0.);

  if(zRot!=0.)
    glRotatef(zRot,0.,0.,1.);


  if(!billboarded) {
    //Alignement translation
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
  }
  else {

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

    MatrixGL invTransformMatrix(transformMatrix);
    invTransformMatrix.inverse();

    Coord baseCenter=projectPoint(centerPosition,transformMatrix,camera->getViewport());
    baseCenter=unprojectPoint(baseCenter,invTransformMatrix,camera->getViewport());

    BoundingBox billboardedBB;
    billboardedBB.expand(projectPoint(centerPosition+Coord(sizeForOutAlign[0]/2.,sizeForOutAlign[1]/2.,sizeForOutAlign[2]/2.),transformMatrix,camera->getViewport()));
    billboardedBB.expand(projectPoint(centerPosition+Coord(-sizeForOutAlign[0]/2.,sizeForOutAlign[1]/2.,sizeForOutAlign[2]/2.),transformMatrix,camera->getViewport()));
    billboardedBB.expand(projectPoint(centerPosition+Coord(sizeForOutAlign[0]/2.,-sizeForOutAlign[1]/2.,sizeForOutAlign[2]/2.),transformMatrix,camera->getViewport()));
    billboardedBB.expand(projectPoint(centerPosition+Coord(-sizeForOutAlign[0]/2.,-sizeForOutAlign[1]/2.,sizeForOutAlign[2]/2.),transformMatrix,camera->getViewport()));
    billboardedBB.expand(projectPoint(centerPosition+Coord(sizeForOutAlign[0]/2.,sizeForOutAlign[1]/2.,-sizeForOutAlign[2]/2.),transformMatrix,camera->getViewport()));
    billboardedBB.expand(projectPoint(centerPosition+Coord(-sizeForOutAlign[0]/2.,sizeForOutAlign[1]/2.,-sizeForOutAlign[2]/2.),transformMatrix,camera->getViewport()));
    billboardedBB.expand(projectPoint(centerPosition+Coord(sizeForOutAlign[0]/2.,-sizeForOutAlign[1]/2.,-sizeForOutAlign[2]/2.),transformMatrix,camera->getViewport()));
    billboardedBB.expand(projectPoint(centerPosition+Coord(-sizeForOutAlign[0]/2.,-sizeForOutAlign[1]/2.,-sizeForOutAlign[2]/2.),transformMatrix,camera->getViewport()));

    Coord billboardedTranlation(0,0,0);

    switch(alignment) {

    case ON_LEFT:
      billboardedTranlation=unprojectPoint(((Coord)billboardedBB.center())+Coord(-billboardedBB.width()/2.,0,0),invTransformMatrix,camera->getViewport())-baseCenter;
      break;

    case ON_RIGHT:
      billboardedTranlation=unprojectPoint(((Coord)billboardedBB.center())+Coord(billboardedBB.width()/2.,0,0),invTransformMatrix,camera->getViewport())-baseCenter;
      break;

    case ON_TOP:
      billboardedTranlation=unprojectPoint(((Coord)billboardedBB.center())+Coord(0,billboardedBB.height()/2.,0),invTransformMatrix,camera->getViewport())-baseCenter;
      break;

    case ON_BOTTOM:
      billboardedTranlation=unprojectPoint(((Coord)billboardedBB.center())+Coord(0,-billboardedBB.height()/2.,0),invTransformMatrix,camera->getViewport())-baseCenter;
      break;

    default:
      break;
    }

    glTranslatef(billboardedTranlation[0],billboardedTranlation[1],billboardedTranlation[2]);

    //Billboard computation
    float mdlM[16];
    glGetFloatv( GL_MODELVIEW_MATRIX, mdlM );
    glMatrixMode( GL_MODELVIEW );

    mdlM[0]  = 1.;
    mdlM[5]  = 1.;
    mdlM[10] = 1.;
    mdlM[1] = mdlM[2] = 0.0f;
    mdlM[4] = mdlM[6] = 0.0f;
    mdlM[8] = mdlM[9] = 0.0f;
    glLoadMatrixf( mdlM );
  }

  glScalef(scaleToApply,scaleToApply,1);
  screenH*=scaleToApply;

  if(screenH<0)
    screenH=-screenH;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //The label is too small to be readed, draw a line
  if(screenH < 4 && useLOD) {
    float wAlign=0;
    float hAlign=0;

    switch (alignment) {
    case ON_TOP:
      hAlign=h/2.f;
      break;

    case ON_BOTTOM:
      hAlign=-(h/2.f);
      break;

    case ON_LEFT:
      wAlign = -(w/2.f);
      break;

    case ON_RIGHT:
      wAlign = w/2.f;
      break;

    default:
      break;
    }

    glLineWidth(screenH);
    setMaterial(Color(color[0],color[1],color[2],128));
    OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();
    glBegin(GL_LINES);
    glVertex3f(-w/2.f+wAlign,hAlign,0);
    glVertex3f(w/2.f+wAlign,hAlign,0);
    glEnd();
    OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();
    glLineWidth(1);
  }
  else {
    //Draw labels
    glDisable(GL_DEPTH_TEST);

    // For left and right alignment
    float xAlignFactor=.5;

    switch (alignment) {
    case ON_LEFT:
      xAlignFactor = 1.;
      break;

    case ON_RIGHT:
      xAlignFactor = .0;
      break;

    default:
      break;
    }

    // Label shift when we have an alignement
    float xShiftFactor=0.;
    float yShiftFactor=0.;

    switch (alignment) {
    case ON_LEFT:
      xShiftFactor = -0.5;
      break;

    case ON_RIGHT:
      xShiftFactor = 0.5;
      break;

    case ON_TOP:
      yShiftFactor = 0.5;
      break;

    case ON_BOTTOM:
      yShiftFactor = -0.5;
      break;

    default:
      break;
    }

    // space between lines
    float yShift=0.;

    float x1,y1,z1,x2,y2,z2,w1,w2;
    font->BBox("|",x1,y1,z1,x2,y2,z2);

    vector<float>::iterator itW=textWidthVector.begin();

    for(vector<string>::iterator it=textVector.begin(); it!=textVector.end(); ++it) {
      font->BBox((*it).c_str(),x1,w1,z1,x2,w2,z2);

      FTPoint shift(-(textBoundingBox[1][0]-textBoundingBox[0][0])/2.-x1+((textBoundingBox[1][0]-textBoundingBox[0][0])-(*itW))*xAlignFactor+(textBoundingBox[1][0]-textBoundingBox[0][0])*xShiftFactor,
                    -textBoundingBox[1][1]+(textBoundingBox[1][1]-textBoundingBox[0][1])/2.+yShift+(textBoundingBox[1][1]-textBoundingBox[0][1])*yShiftFactor);

      if(textureName!="")
        GlTextureManager::getInst().activateTexture(textureName);

      setMaterial(color);

      font->Render((*it).c_str(),-1,shift);

      if(textureName!="")
        GlTextureManager::getInst().desactivateTexture();

      if(screenH > 25 && useLOD) {
        glLineWidth(outlineSize);
        setMaterial(outlineColor);
      }

      borderFont->Render((*it).c_str(),-1,shift);
      yShift-=fontSize+5;
      itW++;
    }
  }

  glPopMatrix();
  glPopAttrib();

}
//===========================================================
void GlLabel::translate(const Coord& mouvement) {
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
  GlXMLTools::getXML(dataNode,"useMinMaxSize",useMinMaxSize);
  GlXMLTools::getXML(dataNode,"minSize",minSize);
  GlXMLTools::getXML(dataNode,"maxSize",maxSize);
  GlXMLTools::getXML(dataNode,"depthTestEnabled",depthTestEnabled);
  GlXMLTools::getXML(dataNode,"leftAlign",leftAlign);
  GlXMLTools::getXML(dataNode,"xRot",xRot);
  GlXMLTools::getXML(dataNode,"yRot",yRot);
  GlXMLTools::getXML(dataNode,"zRot",zRot);
  GlXMLTools::getXML(dataNode,"outlineColor",outlineColor);
  GlXMLTools::getXML(dataNode,"outlineSize",outlineSize);
  GlXMLTools::getXML(dataNode,"textureName",textureName);
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
    GlXMLTools::setWithXML(dataNode,"useMinMaxSize",useMinMaxSize);
    GlXMLTools::setWithXML(dataNode,"minSize",minSize);
    GlXMLTools::setWithXML(dataNode,"maxSize",maxSize);
    GlXMLTools::setWithXML(dataNode,"depthTestEnabled",depthTestEnabled);
    GlXMLTools::setWithXML(dataNode,"leftAlign",leftAlign);
    GlXMLTools::setWithXML(dataNode,"xRot",xRot);
    GlXMLTools::setWithXML(dataNode,"yRot",yRot);
    GlXMLTools::setWithXML(dataNode,"zRot",zRot);
    GlXMLTools::setWithXML(dataNode,"outlineColor",outlineColor);
    GlXMLTools::setWithXML(dataNode,"outlineSize",outlineSize);
    GlXMLTools::setWithXML(dataNode,"textureName",textureName);
  }
}

}
