#include "tulip/GlEdge.h"

#include <GL/gl.h>

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

    /*vector<Coord> tmp =
      tlp::computeCleanVertices(bends, srcCoord, tgtCoord, srcCoord, tgtCoord);

      for(vector<Coord>::iterator it=tmp.begin();it!=tmp.end();++it)
      bb.check(*it);*/

    bb.check(srcCoord);
    bb.check(tgtCoord);

    //cout << bb.first << " * " << bb.second << endl;
    return bb;
  }

  void GlEdge::draw(float lod,GlGraphInputData* data,Camera* camera) {
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_LIGHTING);
    glStencilFunc(GL_LEQUAL,data->parameters->getEdgesStencil(),0xFFFF);

    edge e=edge(id);

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

    const LineType::RealType &bends = data->elementLayout->getEdgeValue(e);
    unsigned nbBends = bends.size();
  
    if (nbBends==0 && (source==target)) { //a loop without bends
      //draw a nice loop;
      //TODO !!
      return;
    }

    /*if(lod<10)
      return;*/

    if (bends.size()==0 && (srcCoord - tgtCoord).norm() < 1E-4) 
      return; //two nodes very closed
    /*if (nbBends == 0 && 
    	(segmentVisible(srcCoord, tgtCoord, data->camera->transformMatrix, data->camera->getViewport()) < 15.)) {
    //cerr << ".";
    return;
    }*/
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
    /*float lod;
    lod = projectSize(srcCoord, srcSize, getLayout()->getCamera()->projectionMatrix, getLayout()->getCamera()->modelviewMatrix, getLayout()->getCamera()->getViewport());
    */
    /*if (lod > 10) { //clip edges with the glyph*/
    int srcGlyphId = data->elementShape->getNodeValue(source);
    Glyph *sourceGlyph = data->glyphs.get(srcGlyphId);
    tmpAnchor = (nbBends > 0) ? bends.front() : tgtCoord;
    srcAnchor = sourceGlyph->getAnchor(srcCoord, tmpAnchor, srcSize, srcRot);
      /*} else {
	srcAnchor = srcCoord;*/
      //}

    //compute anchor, (clip line with the glyph)
    /*lod = projectSize(tgtCoord, tgtSize, getLayout()->getCamera()->projectionMatrix, getLayout()->getCamera()->modelviewMatrix, getLayout()->getCamera()->getViewport()); 
      if (lod > 10) { //clip edges with the glyph*/
      int tgtGlyphId = 1; //cube outlined
      if (data->elementGraph->getNodeValue(target)==0)
      tgtGlyphId = data->elementShape->getNodeValue(target);
      Glyph *targetGlyph = data->glyphs.get(tgtGlyphId);
      //this time we don't take srcCoord but srcAnchor to be oriented to where the line comes from
      tmpAnchor = (nbBends > 0) ? bends.back() : srcAnchor;
      tgtAnchor = targetGlyph->getAnchor(tgtCoord, tmpAnchor, tgtSize, tgtRot);
      /*} 
      else {*/
      //tgtAnchor = tgtCoord;
    //}

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

    Color srcCol,tgtCol;
    bool selected = data->elementSelected->getEdgeValue(e);
    if (selected) {
      srcCol = COLORSELECT;
      tgtCol = COLORSELECT;
      edgeSize[0] += 0.05;
      edgeSize[1] += 0.05;
    } else {
      if (data->parameters->isEdgeColorInterpolate()) {
	srcCol = data->elementColor->getNodeValue(source);
	tgtCol = data->elementColor->getNodeValue(target);
      }
      else {
	srcCol = tgtCol = data->elementColor->getEdgeValue(e);
      }
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
      glEnable(GL_LIGHTING);
      glPushMatrix();
      glMultMatrixf((GLfloat *)&matrix);
      glScalef(sizeT, sizeT, sizeT);
      setMaterial(tgtCol);
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
      glDisable(GL_LIGHTING);
    }
    else {
      endLineAnchor = tgtAnchor;
    }
    
    //  cerr << "DRAW EDGE" << endl;
    //draw Edge
    drawEdge(srcCoord, tgtCoord, srcAnchor, endLineAnchor, bends, srcCol, tgtCol,edgeSize, data->elementShape->getEdgeValue(e),data->parameters->isEdge3D());
    //  cerr << this << "::" << "[END]" << endl;
    if(data->parameters->getFeedbackRender()) {
      glPassThrough(TLP_FB_END_EDGE);
    }
    //glEnable(GL_LIGHTING);
  }

  #define L3D_BIT (1<<9)
  void GlEdge::drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos,
			const Coord &startPoint, const Coord &endPoint, const LineType::RealType &bends,
			const Color &startColor, const Color &endColor, const Size &size, int shape, bool edge3D) {
    bool drawLine = true;
    bool drawPoly = true;

    //================================
    bool lightingOn=glIsEnabled(GL_LIGHTING);
    bool colorMaterialOn=glIsEnabled(GL_COLOR_MATERIAL);
    if(lightingOn)
      glDisable(GL_LIGHTING);
    if(!colorMaterialOn)
      glEnable(GL_COLOR_MATERIAL);
   
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
      if (drawPoly) {
	tlp::polyQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
      }
      if (drawLine) {
	tlp::polyLine(tmp, startColor, endColor);
      }
      break;
    case BEZIERSHAPE:
      if (drawPoly)
	tlp::bezierQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
      if (drawLine) 
	tlp::bezierLine(tmp, startColor, endColor);
      break;
    case SPLINESHAPE:
      if (drawPoly)
	tlp::splineQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
      if (drawLine) 
	tlp::splineLine(tmp, startColor, endColor);
      break;
      //3D lines
    case L3D_BIT + POLYLINESHAPE: 
      GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size, GlLines::TLP_PLAIN,
			       GlLines::LINEAR, startColor, endColor); 
      if (drawLine) tlp::polyLine(tmp, startColor, endColor);
      break;
    case L3D_BIT + BEZIERSHAPE:
      GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size, GlLines::TLP_PLAIN,
			       GlLines::BEZIER, startColor, endColor); break;
    case L3D_BIT + SPLINESHAPE:
      GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size, GlLines::TLP_PLAIN,
			       GlLines::SPLINE3, startColor, endColor); break;
    default:
      if (drawPoly)
	tlp::polyQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
      if (drawLine) 
	tlp::polyLine(tmp,startColor,endColor);
      break;
    }
    
    if(lightingOn != glIsEnabled(GL_LIGHTING)) {
      if(lightingOn)
	glEnable(GL_LIGHTING);
      else
	glDisable(GL_LIGHTING);
    }
    
    if(colorMaterialOn != glIsEnabled(GL_COLOR_MATERIAL)) {
      if(colorMaterialOn)
	glEnable(GL_COLOR_MATERIAL);
      else
	glDisable(GL_COLOR_MATERIAL);
    }
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
    float div_w, div_h;
    int rastPos[4];
    unsigned int labelsBorder = data->parameters->getLabelsBorder();

    glColor4ub(fontColor[0], fontColor[1], fontColor[2], 255);

    glRasterPos3f(position[0], position[1], position[2]);

    glGetIntegerv(GL_CURRENT_RASTER_POSITION, rastPos);

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
