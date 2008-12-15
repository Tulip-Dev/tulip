#include "tulip/GlEdge.h"

#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GraphProperty.h>

#include "tulip/GlTools.h"
#include "tulip/GlyphManager.h"
#include "tulip/Curves.h"
#include "tulip/GlGraphStaticData.h"
#include "tulip/GlLines.h"
#include "tulip/GlDisplayListManager.h"
#include "tulip/OcclusionTest.h"
#include "tulip/TextRenderer.h"
#include "tulip/GlTLPFeedBackBuilder.h"
#include "tulip/GlSceneVisitor.h"
#include "tulip/GlGraphRenderingParameters.h"
#include "tulip/Camera.h"

#include <iostream>

using namespace std;

namespace tlp {

  static const Color COLORSELECT= Color(255, 102, 255, 255);

  BoundingBox GlEdge::getBoundingBox(GlGraphInputData* data) {
    edge e=edge(id);
    BoundingBox bb;

    const node source = data->graph->source(e);
    const node target = data->graph->target(e);
    Coord srcCoord = data->elementLayout->getNodeValue(source);
    Coord tgtCoord = data->elementLayout->getNodeValue(target);

    const LineType::RealType &bends = data->elementLayout->getEdgeValue(e);

    if(bends.size()!=0) {

      const Size &srcSize  = data->elementSize->getNodeValue(source);
      const Size &tgtSize  = data->elementSize->getNodeValue(target);
      double srcRot = data->elementRotation->getNodeValue(source);
      double tgtRot = data->elementRotation->getNodeValue(target);

      // set srcAnchor, tgtAnchor. tmpAnchor will be on the point just before tgtAnchor
      Coord srcAnchor, tgtAnchor, endLineAnchor, tmpAnchor;

      int srcGlyphId = data->elementShape->getNodeValue(source);
      Glyph *sourceGlyph = data->glyphs.get(srcGlyphId);
      tmpAnchor = (bends.size() > 0) ? bends.front() : tgtCoord;
      srcAnchor = sourceGlyph->getAnchor(srcCoord, tmpAnchor, srcSize, srcRot);

      int tgtGlyphId = 1; //cube outlined
      if (!data->getGraph()->isMetaNode(target))
	tgtGlyphId = data->elementShape->getNodeValue(target);
      Glyph *targetGlyph = data->glyphs.get(tgtGlyphId);
      //this time we don't take srcCoord but srcAnchor to be oriented to where the line comes from
      tmpAnchor = (bends.size() > 0) ? bends.back() : srcAnchor;
      tgtAnchor = targetGlyph->getAnchor(tgtCoord, tmpAnchor, tgtSize, tgtRot);

      vector<Coord> tmp =
	tlp::computeCleanVertices(bends, srcCoord, tgtCoord, srcAnchor, tgtAnchor);

      for(vector<Coord>::iterator it=tmp.begin();it!=tmp.end();++it)
	bb.check(*it);
    }

    bb.check(srcCoord);
    bb.check(tgtCoord);

    //cout << bb.first << " * " << bb.second << endl;
    return bb;
  }

  void GlEdge::acceptVisitor(GlSceneVisitor *visitor) {
    visitor->visit(this);
  }

  void GlEdge::draw(float lod,GlGraphInputData* data,Camera* camera) {
    edge e=edge(id);
    bool selected = data->elementSelected->getEdgeValue(e);

    if(selected) {
      glDisable(GL_DEPTH_TEST);
      glStencilFunc(GL_LEQUAL,data->parameters->getSelectedEdgesStencil(),0xFFFF);
    }else{
      glEnable(GL_DEPTH_TEST);
      glStencilFunc(GL_LEQUAL,data->parameters->getEdgesStencil(),0xFFFF);
    }

    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    const node source = data->graph->source(e);
    const node target = data->graph->target(e);
    Coord srcCoord = data->elementLayout->getNodeValue(source);
    Coord tgtCoord = data->elementLayout->getNodeValue(target);

    Color fillColor = data->elementColor->getEdgeValue(e);
    Color strokeColor = data->elementBorderColor->getEdgeValue(e);
    Color textColor = data->elementLabelColor->getEdgeValue(e);

    if(data->parameters->getFeedbackRender()) {
      glPassThrough(TLP_FB_COLOR_INFO);
      glPassThrough(fillColor[0]);glPassThrough(fillColor[1]);glPassThrough(fillColor[2]);
      glPassThrough(strokeColor[0]);glPassThrough(strokeColor[1]);glPassThrough(strokeColor[2]);
      glPassThrough(textColor[0]);glPassThrough(textColor[1]);glPassThrough(textColor[2]);

      glPassThrough(TLP_FB_BEGIN_EDGE);
      glPassThrough(id); //id of the node for the feed back mode
    }

    Color srcCol,tgtCol;
    if (selected) {
      srcCol = COLORSELECT;
      tgtCol = COLORSELECT;
    } else {
      if (data->parameters->isEdgeColorInterpolate()) {
	srcCol = data->elementColor->getNodeValue(source);
	tgtCol = data->elementColor->getNodeValue(target);
      }
      else {
	srcCol = tgtCol = data->elementColor->getEdgeValue(e);
      }
    }

    if(lod<5) {
      setColor(srcCol);
      glPointSize(1);
      glBegin(GL_POINTS);
        glVertex3f(srcCoord[0], srcCoord[1], srcCoord[2]);
      glEnd();
      return;
    }

    const LineType::RealType &bends = data->elementLayout->getEdgeValue(e);
    unsigned nbBends = bends.size();

    if (nbBends==0 && (source==target)) { //a loop without bends
      //draw a nice loop;
      //TODO !!
      return;
    }

    if (bends.size()==0 && (srcCoord - tgtCoord).norm() < 1E-4)
      return; //two nodes very closed
    Matrix<float,4> transformMatrix;
    camera->getTransformMatrix(transformMatrix);
    //take source and target information for edge clipping
    const Size &srcSize  = data->elementSize->getNodeValue(source);
    const Size &tgtSize  = data->elementSize->getNodeValue(target);
    double srcRot = data->elementRotation->getNodeValue(source);
    double tgtRot = data->elementRotation->getNodeValue(target);

    //if first bend is inside the glyph (srcCoord is an anchor point it must be  replaced the node position)
    //if last bend is in the glyph (Coord is an anchor point it should replace the node position)
    //Be carefull if there is only one node.
    if (bends.size() > 0) {
      Coord firstBend = bends.front();
      Coord lastBend  = bends.back();
    }

    // set srcAnchor, tgtAnchor. tmpAnchor will be on the point just before tgtAnchor
    Coord srcAnchor, tgtAnchor, endLineAnchor, tmpAnchor;

    //compute anchor, (clip line with the glyph)
    int srcGlyphId = data->elementShape->getNodeValue(source);
    Glyph *sourceGlyph = data->glyphs.get(srcGlyphId);
    tmpAnchor = (nbBends > 0) ? bends.front() : tgtCoord;
    srcAnchor = sourceGlyph->getAnchor(srcCoord, tmpAnchor, srcSize, srcRot);

    //compute anchor, (clip line with the glyph)
      int tgtGlyphId = 1; //cube outlined
    //if (data->elementGraph->getNodeValue(target)==0)
      tgtGlyphId = data->elementShape->getNodeValue(target);
      Glyph *targetGlyph = data->glyphs.get(tgtGlyphId);
      //this time we don't take srcCoord but srcAnchor to be oriented to where the line comes from
      tmpAnchor = (nbBends > 0) ? bends.back() : srcAnchor;
      tgtAnchor = targetGlyph->getAnchor(tgtCoord, tmpAnchor, tgtSize, tgtRot);

    Size edgeSize; //the edge radius and arrow radius
    if (data->parameters->isEdgeSizeInterpolate()) {
      edgeSize[0] = std::min(srcSize[0], srcSize[1]) / 16.;
      edgeSize[1] = std::min(tgtSize[0], tgtSize[1]) / 16.;
      if (data->parameters->isViewArrow()) {
	edgeSize[2] = std::min(tgtSize[0], tgtSize[1]) / 4.;
      }
    }
    else {
      edgeSize = data->elementSize->getEdgeValue(e);
    }

    if (selected) {
      edgeSize[0] += 0.05;
      edgeSize[1] += 0.05;
    }

    //draw Arrow
    if(data->parameters->isViewArrow()) {
      if(GlDisplayListManager::getInst().beginNewDisplayList("arrow")) {
	tlp::solidCone();
	GlDisplayListManager::getInst().endNewDisplayList();
      }

      float sizeT = edgeSize.getD();
      sizeT = std::min(sizeT, (float)(tmpAnchor- tgtAnchor).norm()/2.0f);
      MatrixGL matrix(makeArrowMatrix(tmpAnchor, tgtAnchor));
      glPushMatrix();
      glMultMatrixf((GLfloat *)&matrix);
      glScalef(sizeT, sizeT, sizeT);
      setColor(tgtCol);
      //tlp::solidCone();
      GlDisplayListManager::getInst().callDisplayList("arrow");
      glPopMatrix();
      if (selected) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
	glMultMatrixf((GLfloat *)&matrix);
	glScalef(sizeT+0.1, sizeT+0.1, sizeT+0.1);
	setMaterial(COLORSELECT);
	//tlp::solidCone();
	GlDisplayListManager::getInst().callDisplayList("arrow");
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
      endLineAnchor = tmpAnchor - tgtAnchor;
      float nrm = endLineAnchor.norm();
      if( nrm > 0.00000001f ) {
	endLineAnchor *= sizeT / nrm;
	endLineAnchor += tgtAnchor;
      } else {
	endLineAnchor = tgtAnchor;
      }
      tgtCoord = tgtAnchor; //this defines in drawEdge the arrow head as being the final node


    }
    else {
      endLineAnchor = tgtAnchor;
    }

    Matrix<float, 4u> projectionMatrix;
    Matrix<float, 4u> modelviewMatrix;
    camera->getProjectionMatrix(projectionMatrix);
    camera->getModelviewMatrix(modelviewMatrix);
    float lodSize = projectSize(srcCoord, edgeSize[0], projectionMatrix, modelviewMatrix, camera->getViewport());

    //draw Edge
    drawEdge(srcCoord, tgtCoord, srcAnchor, endLineAnchor, bends, srcCol, tgtCol,edgeSize, data->elementShape->getEdgeValue(e),data->parameters->isEdge3D(),lodSize);

    if(data->parameters->getFeedbackRender()) {
      glPassThrough(TLP_FB_END_EDGE);
    }

    glEnable(GL_LIGHTING);

  }

  #define L3D_BIT (1<<9)
  void GlEdge::drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos,
			const Coord &startPoint, const Coord &endPoint, const LineType::RealType &bends,
			const Color &startColor, const Color &endColor, const Size &size, int shape, bool edge3D, float lod) {
    bool drawLine = true;
    bool drawPoly = true;

    //================================

    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    if (edge3D)
      shape |= L3D_BIT;
    if (shape & L3D_BIT) {
      if (shape >= (L3D_BIT+16)) {
	shape  = shape & ~L3D_BIT; //no 3D model defined, falling back to Line model
      }
    }
    Coord srcDir(srcNodePos);
    Coord tgtDir(tgtNodePos);
    vector<Coord> tmp =
      tlp::computeCleanVertices(bends, startPoint, endPoint, srcDir, tgtDir);

    if (tmp.size()<2) {
      return;
    }

    switch (shape) {
    case POLYLINESHAPE:
      if(lod>0.05 || lod<-0.05)
	tlp::polyQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
      else
        tlp::polyLine(tmp, startColor, endColor);
      //glDepthFunc(GL_LESS);
      /*if (drawLine) {
	tlp::polyLine(tmp, startColor, endColor);
      }*/
      break;
    case BEZIERSHAPE:
      if(lod>0.05 || lod<-0.05)
	tlp::bezierQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
      else
	tlp::bezierLine(tmp, startColor, endColor);
      break;
    case SPLINESHAPE:
      if (lod>0.05 || lod<-0.05)
	tlp::splineQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
      else
	tlp::splineLine(tmp, startColor, endColor);
      break;
      //3D lines
    case L3D_BIT + POLYLINESHAPE:
      GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size, GlLines::TLP_PLAIN,
			       GlLines::LINEAR, startColor, endColor);
      glDepthFunc(GL_LESS);
      if (drawLine) tlp::polyLine(tmp, startColor, endColor);
      break;
    case L3D_BIT + BEZIERSHAPE:
      GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size, GlLines::TLP_PLAIN,
			       GlLines::BEZIER, startColor, endColor); break;
    case L3D_BIT + SPLINESHAPE:
      GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size, GlLines::TLP_PLAIN,
			       GlLines::SPLINE3, startColor, endColor); break;
    default:
      if (lod>0.05 || lod<-0.05)
	tlp::polyQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
      else
	tlp::polyLine(tmp,startColor,endColor);
      break;
    }

    glDepthFunc(GL_LEQUAL);
  }

  void GlEdge::drawLabel(bool drawSelect,bool drawNodesLabel,bool drawEdgesLabel,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data) {
    if(!drawEdgesLabel)
      return;

    edge e=edge(id);

    const string &tmp = data->elementLabel->getEdgeValue(e);
    if (tmp.length() < 1) {
      return;
    }

    bool select=data->elementSelected->getEdgeValue(e);
    if(drawSelect!=select)
      return;

    if(select)
      renderer->setContext(data->parameters->getFontsPath() + "font.ttf", 20, 0, 0, 255);
    else
      renderer->setContext(data->parameters->getFontsPath() + "font.ttf", 18, 255, 255, 255);

    const Coord & srcCoord = data->elementLayout->getNodeValue(data->graph->source(e));
    const Coord & tgtCoord = data->elementLayout->getNodeValue(data->graph->target(e));
    const LineType::RealType &bends = data->elementLayout->getEdgeValue(e);
    Coord position;
    if (bends.empty()) {
      position = (srcCoord+tgtCoord) / 2.0;
    }
    else {
      if (bends.size()%2 == 0)
	position = (bends[bends.size()/2-1]+bends[bends.size()/2]) / 2.0;
      else
	position = bends[bends.size()/2];
    }

    Color fontColor;
    if (data->elementSelected->getEdgeValue(e))
      fontColor.set(255,0,0,255);
    else {
      fontColor = data->elementLabelColor->getEdgeValue(e);
    }

    float w_max = 300;
    float w,h;
    int rastPos[4];
    unsigned int labelsBorder = data->parameters->getLabelsBorder();

    setColor(Color(fontColor[0], fontColor[1], fontColor[2], 255));

    glRasterPos3f(position[0], position[1], position[2]);

    glGetIntegerv(GL_CURRENT_RASTER_POSITION, (GLint *) rastPos);

    if(test->testRectangle(RectangleInt2D(rastPos[0] - labelsBorder - 5,
					  rastPos[1]  - labelsBorder - 5,
					  rastPos[0]  + labelsBorder + 5,
					  rastPos[1]  + labelsBorder + 5))) {
      return;
    }

    renderer->setMode(TLP_PIXMAP);
    renderer->setString(tmp, VERBATIM);
    //fontRenderer->setString(str, XML);

    renderer->setColor(fontColor[0], fontColor[1], fontColor[2]);
    //  w_max = width;
    renderer->getBoundingBox(w_max, h, w);

    if(!test->addRectangle(RectangleInt2D(rastPos[0]-(int)(w/2.0) - labelsBorder,
					  rastPos[1]-(int)(h/2.0) - labelsBorder,
					  rastPos[0]+(int)(w/2.0) + labelsBorder,
					  rastPos[1]+(int)(h/2.0) + labelsBorder))) {
      renderer->draw(w, w, ON_CENTER);
    }
  }
}
