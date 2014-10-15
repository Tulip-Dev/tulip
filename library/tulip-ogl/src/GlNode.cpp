/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/GlNode.h>

#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlMetaNodeRenderer.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Glyph.h>
#include <tulip/GlTools.h>
#include <tulip/GlTLPFeedBackBuilder.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlLabel.h>
#include <tulip/GlBox.h>
#include <tulip/GlGlyphRenderer.h>

//====================================================
tlp::GlLabel* tlp::GlNode::label=NULL;
tlp::GlBox* tlp::GlNode::selectionBox=NULL;

using namespace std;

namespace tlp {

GlNode::GlNode(unsigned int id):id(id) {
  if(!label)
    label=new GlLabel();

  if(!selectionBox) {
    selectionBox=new GlBox(Coord(0,0,0),Size(1,1,1),Color(0,0,255,255),Color(0,255,0,255),false,true);
    selectionBox->setOutlineSize(3);
  }
}

BoundingBox GlNode::getBoundingBox(const GlGraphInputData* data) {
  node n(id);
  int nodeRot = data->getElementRotation()->getNodeValue(n);
  const Size& nodeSize = data->getElementSize()->getNodeValue(n);
  const Coord& nodeCoord = data->getElementLayout()->getNodeValue(n);

  if(nodeRot==0) {
    BoundingBox box;
    box.expand(nodeCoord-nodeSize/2.f);
    box.expand(nodeCoord+nodeSize/2.f);
    assert(box.isValid());
    return box;
  }
  else {
    float cosAngle=cos((float)nodeRot/180.*M_PI);
    float sinAngle=sin((float)nodeRot/180.*M_PI);
    Coord tmp1(nodeSize/2.f);
    Coord tmp2(tmp1[0] ,-tmp1[1], tmp1[2]);
    Coord tmp3(-tmp1[0],-tmp1[1],-tmp1[2]);
    Coord tmp4(-tmp1[0], tmp1[1],-tmp1[2]);
    tmp1=Coord(tmp1[0]*cosAngle-tmp1[1]*sinAngle,tmp1[0]*sinAngle+tmp1[1]*cosAngle,tmp1[2]);
    tmp2=Coord(tmp2[0]*cosAngle-tmp2[1]*sinAngle,tmp2[0]*sinAngle+tmp2[1]*cosAngle,tmp2[2]);
    tmp3=Coord(tmp3[0]*cosAngle-tmp3[1]*sinAngle,tmp3[0]*sinAngle+tmp3[1]*cosAngle,tmp3[2]);
    tmp4=Coord(tmp4[0]*cosAngle-tmp4[1]*sinAngle,tmp4[0]*sinAngle+tmp4[1]*cosAngle,tmp4[2]);
    BoundingBox bb;
    bb.expand(nodeCoord+tmp1);
    bb.expand(nodeCoord+tmp2);
    bb.expand(nodeCoord+tmp3);
    bb.expand(nodeCoord+tmp4);
    return bb;
  }
}

void GlNode::acceptVisitor(GlSceneVisitor *visitor) {
  visitor->visit(this);
}

void GlNode::draw(float lod,const GlGraphInputData* data,Camera* camera) {
  const Color& colorSelect2=data->parameters->getSelectionColor();

  glEnable(GL_CULL_FACE);

  node n=node(id);

  ColorProperty *colP=data->getElementColor();

  if(data->getGraph()->isMetaNode(n)) {
    data->getMetaNodeRenderer()->render(n,lod,camera);
  }

  const Coord &nodeCoord = data->getElementLayout()->getNodeValue(n);

  Size nodeSize = data->getElementSize()->getNodeValue(n);

  const Color& fillColor = colP->getNodeValue(n);

  const Color& strokeColor = data->getElementBorderColor()->getNodeValue(n);

  const Color& textColor = data->getElementLabelColor()->getNodeValue(n);

  GlTextureManager::getInst().setAnimationFrame(data->getElementAnimationFrame()->getNodeValue(n));

  if(data->parameters->getFeedbackRender()) {
    glPassThrough(TLP_FB_COLOR_INFO);
    glPassThrough(fillColor[0]);
    glPassThrough(fillColor[1]);
    glPassThrough(fillColor[2]);
    glPassThrough(fillColor[3]);
    glPassThrough(strokeColor[0]);
    glPassThrough(strokeColor[1]);
    glPassThrough(strokeColor[2]);
    glPassThrough(strokeColor[3]);
    glPassThrough(textColor[0]);
    glPassThrough(textColor[1]);
    glPassThrough(textColor[2]);
    glPassThrough(textColor[3]);

    glPassThrough(TLP_FB_BEGIN_NODE);
    glPassThrough(id); //id of the node for the feed back mode
  }

  bool selected = data->getElementSelected()->getNodeValue(n);

  if (lod < 10.0) { //less than four pixel on screen, we use points instead of glyphs
    if (lod < 1) lod = 1;

    if(data->getGlVertexArrayManager()->renderingIsBegin()) {
      data->getGlVertexArrayManager()->activatePointNodeDisplay(this, selected);
    }
    else {

      Color pointColor = data->getElementColor()->getNodeValue(n);

      if (data->getElementBorderWidth()->getNodeValue(n) > 0) {
        pointColor = data->getElementBorderColor()->getNodeValue(n);
      }

      glDisable(GL_LIGHTING);
      setColor(selected ? colorSelect2 : pointColor);
      glPointSize(4);
      glBegin(GL_POINTS);
      glVertex3f(nodeCoord[0], nodeCoord[1], nodeCoord[2]+nodeSize[2]/2.);
      glEnd();
      glEnable(GL_LIGHTING);
    }

    return;

  }

  if (!data->parameters->isDisplayNodes())
    return;

  float rot = data->getElementRotation()->getNodeValue(n);

  // If node size in z is equal to 0 we have to scale with FLT_EPSILON to preserve normal
  // (because if we do a scale of (x,y,0) and if we have a normal like (0,0,1) the new normal after scale will be (0,0,0) and we will have light problem)
  if(nodeSize[2]==0)
    nodeSize[2] = FLT_EPSILON;

  // Some glyphs can not benefit from the shader rendering optimization
  // due to the use of quadrics or modelview matrix modification or lighting effect
  static set<int> noShaderGlyphs;

  if (noShaderGlyphs.empty()) {
    noShaderGlyphs.insert(NodeShape::Billboard);
    noShaderGlyphs.insert(NodeShape::ChristmasTree);
    noShaderGlyphs.insert(NodeShape::Cone);
    noShaderGlyphs.insert(NodeShape::Cylinder);
    noShaderGlyphs.insert(NodeShape::GlowSphere);
    noShaderGlyphs.insert(NodeShape::HalfCylinder);
    noShaderGlyphs.insert(NodeShape::Sphere);
    noShaderGlyphs.insert(NodeShape::Cube);
    noShaderGlyphs.insert(NodeShape::CubeOutlined);
  }

  int glyph = data->getElementShape()->getNodeValue(n);

  if (data->getGlGlyphRenderer()->renderingHasStarted() && noShaderGlyphs.find(glyph) == noShaderGlyphs.end()) {
    data->getGlGlyphRenderer()->addNodeGlyphRendering(data->glyphs.get(glyph),
        n, lod, nodeCoord, nodeSize, rot, selected);
  }
  else {

    if (data->getElementSelected()->getNodeValue(n)) {
      glStencilFunc(GL_LEQUAL,data->parameters->getSelectedNodesStencil(),0xFFFF);
    }
    else {
      glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil(),0xFFFF);
    }

    //draw a glyph or make recursive call for meta nodes
    glPushMatrix();
    glTranslatef(nodeCoord[0], nodeCoord[1], nodeCoord[2]);
    glRotatef(rot, 0., 0., 1.);

    glScalef(nodeSize[0], nodeSize[1],nodeSize[2]);

    if (selected) {
      selectionBox->setStencil(data->parameters->getSelectedNodesStencil()-1);
      selectionBox->setOutlineColor(colorSelect2);
      selectionBox->draw(10,NULL);
    }

    data->glyphs.get(data->getElementShape()->getNodeValue(n))->draw(n,lod);

    glPopMatrix();

  }

  GlTextureManager::getInst().setAnimationFrame(0);

  if(data->parameters->getFeedbackRender()) {
    glPassThrough(TLP_FB_END_NODE);
  }
}

void GlNode::drawLabel(bool drawSelect,OcclusionTest* test,const GlGraphInputData* data,float lod) {
  node n=node(id);
  bool selected=data->getElementSelected()->getNodeValue(n);

  if(drawSelect!=selected)
    return;

  drawLabel(test,data,lod);
}

void GlNode::drawLabel(OcclusionTest* test,const GlGraphInputData* data) {
  GlNode::drawLabel(test,data,1000.);
}

void GlNode::drawLabel(OcclusionTest* test,const GlGraphInputData* data,float lod,Camera *camera) {

  node n=node(id);

  // If glyph can't render label : return
  if(data->glyphs.get(data->getElementShape()->getNodeValue(n))->renderLabel())
    return;

  // node is selected
  bool selected=data->getElementSelected()->getNodeValue(n);
  // Color of the label : selected or not
  const Color& fontColor = selected ? data->parameters->getSelectionColor() :data->getElementLabelColor()->getNodeValue(n);
  const Color& fontBorderColor = selected ? data->parameters->getSelectionColor() : data->getElementLabelBorderColor()->getNodeValue(n);
  float fontBorderWidth = data->getElementLabelBorderWidth()->getNodeValue(n);

  // If we have transparent label : return
  if(fontColor.getA()==0 && (fontBorderColor.getA()==0 || fontBorderWidth==0))
    return;

  // Node text
  const string &tmp = data->getElementLabel()->getNodeValue(n);

  if (tmp.length() < 1)
    return;


  if(selected) label->setStencil(data->parameters->getSelectedNodesStencil());
  else label->setStencil(data->parameters->getNodesLabelStencil());

  int fontSize=data->getElementFontSize()->getNodeValue(n);

  if(fontSize<=0)
    return;

  if(selected)
    fontSize+=2;

  const Coord &nodeCoord = data->getElementLayout()->getNodeValue(n);
  const Size  &nodeSize  = data->getElementSize()->getNodeValue(n);
  int labelPos = data->getElementLabelPosition()->getNodeValue(n);

  BoundingBox includeBB;
  data->glyphs.get(data->getElementShape()->getNodeValue(n))->getTextBoundingBox(includeBB,n);
  Coord centerBB(includeBB.center());
  Vec3f sizeBB = includeBB[1]-includeBB[0];

  label->setFontNameSizeAndColor(data->getElementFont()->getNodeValue(n),fontSize,fontColor);
  label->setOutlineColor(fontBorderColor);
  label->setOutlineSize(fontBorderWidth);
  label->setText(tmp);
  label->setTranslationAfterRotation(centerBB*nodeSize);
  label->setSize(Size(nodeSize[0]*sizeBB[0],nodeSize[1]*sizeBB[1],0));
  label->setSizeForOutAlign(Size(nodeSize[0],nodeSize[1],0));
  label->rotate(0,0,data->getElementRotation()->getNodeValue(n));
  label->setAlignment(labelPos);
  label->setScaleToSize(data->parameters->isLabelScaled());
  label->setUseLODOptimisation(true,this->getBoundingBox(data));
  label->setLabelsDensity(data->parameters->getLabelsDensity());
  label->setUseMinMaxSize(!data->parameters->isLabelFixedFontSize());
  label->setMinSize(data->parameters->getMinSizeOfLabel());
  label->setMaxSize(data->parameters->getMaxSizeOfLabel());
  label->setOcclusionTester(test);
  label->setBillboarded(data->parameters->getLabelsAreBillboarded());

  if(includeBB[1][2]!=0 && !data->parameters->getLabelsAreBillboarded())
    label->setPosition(Coord(nodeCoord[0],nodeCoord[1],nodeCoord[2]+nodeSize[2]/2.));
  else
    label->setPosition(Coord(nodeCoord[0],nodeCoord[1],nodeCoord[2]));

  label->drawWithStencil(lod,camera);
}

void GlNode::getPointAndColor(GlGraphInputData *inputData,std::vector<Coord> &pointsCoordsArray,std::vector<Color> &pointsColorsArray) {
  node n=node(id);
  const Coord &nodeCoord = inputData->getElementLayout()->getNodeValue(n);
  const Color& fillColor = inputData->getElementColor()->getNodeValue(n);
  const Color& borderColor = inputData->getElementBorderColor()->getNodeValue(n);
  pointsCoordsArray.push_back(nodeCoord);

  if (inputData->getElementBorderWidth()->getNodeValue(n) > 0) {
    pointsColorsArray.push_back(borderColor);
  }
  else {
    pointsColorsArray.push_back(fillColor);
  }
}

void GlNode::getColor(GlGraphInputData *inputData,std::vector<Color> &pointsColorsArray) {
  node n=node(id);
  const Color& fillColor = inputData->getElementColor()->getNodeValue(n);
  const Color& borderColor = inputData->getElementBorderColor()->getNodeValue(n);

  if (inputData->getElementBorderWidth()->getNodeValue(n) > 0) {
    pointsColorsArray.push_back(borderColor);
  }
  else {
    pointsColorsArray.push_back(fillColor);
  }
}
}
