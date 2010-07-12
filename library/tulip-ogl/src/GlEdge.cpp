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
#include "tulip/GlEdge.h"

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
#include "tulip/GlPointManager.h"
#include "tulip/GlBezierCurve.h"
#include "tulip/GlCatmullRomCurve.h"
#include "tulip/GlRenderer.h"
#include "tulip/GlOpenUniformCubicBSpline.h"
#include "tulip/GlTextureManager.h"

#include <iostream>

using namespace std;

namespace tlp {

BoundingBox GlEdge::eeGlyphBoundingBox(const Coord& anchor, const Coord& tgt, float glyphNrm,
		const MatrixGL& transformation, const MatrixGL& size) {

	Coord vect = tgt - anchor;
	float nrm = vect.norm();
	if (fabs(nrm) > 1E-6)
		vect /= nrm;

	Coord mid = anchor + vect * glyphNrm * .5f;

	Coord vAB(transformation[0][0], transformation[0][1], transformation[0][2]);
	Coord vV(transformation[0][1], transformation[1][1], transformation[2][1]);
	Coord vW(transformation[0][2], transformation[1][2], transformation[2][2]);

	BoundingBox box;
	box.expand(mid + vAB * (-size[0][0]) * .5f);
	box.expand(mid + vAB * (size[0][0]) * .5f);
	box.expand(mid + vV * (-size[1][1]) * .5f);
	box.expand(mid + vV * (size[1][1]) * .5f);
	box.expand(mid + vW * (-size[2][2]) * .5f);
	box.expand(mid + vW * (size[2][2]) * .5f);

	return box;
}

BoundingBox GlEdge::getBoundingBox(GlGraphInputData* data) {
	edge e = edge(id);
	BoundingBox bb;

	const std::pair<node, node>& eEnds = data->graph->ends(e);
	const node source = eEnds.first;
	const node target = eEnds.second;
	const Coord& srcCoord = data->elementLayout->getNodeValue(source);
	const Coord& tgtCoord = data->elementLayout->getNodeValue(target);

	const LineType::RealType &bends = data->elementLayout->getEdgeValue(e);

	if (bends.size() != 0) {

		const Size &srcSize = data->elementSize->getNodeValue(source);
		const Size &tgtSize = data->elementSize->getNodeValue(target);
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

		vector<Coord> tmp = tlp::computeCleanVertices(bends, srcCoord, tgtCoord, srcAnchor,
				tgtAnchor);

		for (vector<Coord>::iterator it = tmp.begin(); it != tmp.end(); ++it)
			bb.expand(*it);
	}

	bb.expand(srcCoord);
	bb.expand(tgtCoord);

	return bb;
}

void GlEdge::acceptVisitor(GlSceneVisitor *visitor) {
	visitor->visit(this);
}

void GlEdge::draw(float lod, GlGraphInputData* data, Camera* camera) {
  edge e = edge(id);

	if(data->parameters->isElementZOrdered()){
		if(data->elementColor->getEdgeValue(e)[3]!=255){
			GlPointManager::getInst().endRendering();
			GlPointManager::getInst().beginRendering();
		}
	}
	bool selected = data->elementSelected->getEdgeValue(e);

	if (selected) {
		glDisable(GL_DEPTH_TEST);
		glStencilFunc(GL_LEQUAL, data->parameters->getSelectedEdgesStencil(), 0xFFFF);
	} else {
		glEnable(GL_DEPTH_TEST);
		glStencilFunc(GL_LEQUAL, data->parameters->getEdgesStencil(), 0xFFFF);
	}

	glEnable(GL_COLOR_MATERIAL);

	const std::pair<node, node>& eEnds = data->graph->ends(e);
	const node source = eEnds.first;
	const node target = eEnds.second;
	const Coord& srcCoord = data->elementLayout->getNodeValue(source);
	const Coord& tgtCoord = data->elementLayout->getNodeValue(target);

	const Color& fillColor = data->elementColor->getEdgeValue(e);
	const Color& strokeColor = data->elementBorderColor->getEdgeValue(e);
	const Color& textColor = data->elementLabelColor->getEdgeValue(e);

	if (data->parameters->getFeedbackRender()) {
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

		glPassThrough(TLP_FB_BEGIN_EDGE);
		glPassThrough(id); //id of the node for the feed back mode
	}

	Color srcCol, tgtCol;
  getEdgeColor(data,e,source,target,selected,srcCol,tgtCol);

	if (lod < 5) {
		if (OpenGlConfigManager::getInst().canUseGlew() && GlPointManager::getInst().renderingIsBegin()) {
			GlPointManager::getInst().addPoint(srcCoord, srcCol, 1);
		} else {
			setColor(srcCol);
			glPointSize(1);
			glBegin(GL_POINTS);
			glVertex3f(srcCoord[0], srcCoord[1], srcCoord[2]);
			glEnd();
		}
		return;
	}

	const LineType::RealType &bends = data->elementLayout->getEdgeValue(e);
	unsigned nbBends = bends.size();

	if (nbBends == 0 && (source == target)) { //a loop without bends
		//draw a nice loop;
		//TODO !!
		return;
	}

	if (bends.size() == 0 && (srcCoord - tgtCoord).norm() < 1E-4)
		return; //two nodes very closed

	//Get added transformations
	vector<Coord> objectScale, objectTranslate, objectCoord;
	camera->getObjectTransformation(objectTranslate, objectScale, objectCoord);

	Matrix<float, 4> transformMatrix;
	camera->getTransformMatrix(transformMatrix);
	//take source and target information for edge clipping
	const Size &srcSize = data->elementSize->getNodeValue(source);
	const Size &tgtSize = data->elementSize->getNodeValue(target);
	double srcRot = data->elementRotation->getNodeValue(source);
	double tgtRot = data->elementRotation->getNodeValue(target);

	//if first bend is inside the glyph (srcCoord is an anchor point it must be  replaced the node position)
	//if last bend is in the glyph (Coord is an anchor point it should replace the node position)
	//Be carefull if there is only one node.
	if (bends.size() > 0) {
		Coord firstBend = bends.front();
		Coord lastBend = bends.back();
	}

	// set srcAnchor, tgtAnchor. tmpAnchor will be on the point just before tgtAnchor
	Coord srcAnchor, tgtAnchor, beginLineAnchor, endLineAnchor, beginTmpAnchor, endTmpAnchor;

	//compute anchor, (clip line with the glyph)
	int srcGlyphId = data->elementShape->getNodeValue(source);
	Glyph *sourceGlyph = data->glyphs.get(srcGlyphId);
	beginTmpAnchor = (nbBends > 0) ? bends.front() : tgtCoord;
	srcAnchor = sourceGlyph->getAnchor(srcCoord, beginTmpAnchor, srcSize, srcRot);

	//compute anchor, (clip line with the glyph)
	int tgtGlyphId = 1; //cube outlined
	tgtGlyphId = data->elementShape->getNodeValue(target);
	Glyph *targetGlyph = data->glyphs.get(tgtGlyphId);
	//this time we don't take srcCoord but srcAnchor to be oriented to where the line comes from
	endTmpAnchor = (nbBends > 0) ? bends.back() : srcAnchor;
	tgtAnchor = targetGlyph->getAnchor(tgtCoord, endTmpAnchor, tgtSize, tgtRot);

  Size edgeSize;
  float maxSrcSize, maxTgtSize;
  getEdgeSize(data,e,srcSize,tgtSize,edgeSize,maxSrcSize,maxTgtSize);

	if (selected) {
		glPushMatrix();
		Coord p1=camera->screenTo3DWorld(Coord(0,0,0));
		Coord p2=camera->screenTo3DWorld(Coord(2,0,0));
		edgeSize[0] += (p2-p1).norm();
		edgeSize[1] += (p2-p1).norm();
		glPopMatrix();
	}

	int startEdgeGlyph = data->elementSrcAnchorShape->getEdgeValue(e);
	int endEdgeGlyph = data->elementTgtAnchorShape->getEdgeValue(e);

	if (startEdgeGlyph != 0 && data->parameters->isViewArrow()) {
    displayArrow(data,e,source,edgeSize[0],srcCol,maxSrcSize,selected,startEdgeGlyph,endEdgeGlyph,
                 bends.size(),beginTmpAnchor,tgtCoord,srcAnchor,tgtAnchor,beginLineAnchor);
	} else {
		beginLineAnchor = srcAnchor;
	}

	if (endEdgeGlyph != 0 && data->parameters->isViewArrow()) {
    displayArrow(data,e,target,edgeSize[1],tgtCol,maxTgtSize,selected,endEdgeGlyph,startEdgeGlyph,
                 bends.size(),endTmpAnchor,srcCoord,tgtAnchor,srcAnchor,endLineAnchor);
	} else {
		endLineAnchor = tgtAnchor;
	}

	//Reset in case of drawing extremity glyph that can alterate value
	glLineWidth(1);

  float lodSize = getEdgeWidthLod(srcCoord,edgeSize,camera);

	GlTextureManager::getInst().setAnimationFrame(data->elementAnimationFrame->getEdgeValue(e));
	//draw Edge
	drawEdge(srcCoord, tgtCoord, beginLineAnchor, endLineAnchor, bends, srcCol, tgtCol,camera->getCenter()-camera->getEyes(),data->parameters->isEdgeColorInterpolate() ,strokeColor,edgeSize,
			data->elementShape->getEdgeValue(e), data->parameters->isEdge3D(), lodSize, data->elementTexture->getEdgeValue(e));
	GlTextureManager::getInst().setAnimationFrame(0);

	if (data->parameters->getFeedbackRender()) {
		glPassThrough(TLP_FB_END_EDGE);
	}

	glEnable(GL_LIGHTING);

}

#define L3D_BIT (1<<9)
void GlEdge::drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos, const Coord &startPoint,
		const Coord &endPoint, const LineType::RealType &bends, const Color &startColor,
		const Color &endColor, const Coord &lookDir, bool colorInterpolate, const Color &borderColor,
		const Size &size, int shape, bool edge3D, float lod,const string &textureName) {

	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);


	Coord srcDir(srcNodePos);
	Coord tgtDir(tgtNodePos);
  vector<Coord> tmp;
  computeCleanVertices(bends, startPoint, endPoint, srcDir, tgtDir,tmp);

	if (tmp.size() < 2) {
		return;
	}

    if (edge3D){
      shape |= L3D_BIT;
      glEnable(GL_LIGHTING);
    }else{
      glDisable(GL_LIGHTING);
    }


	switch (shape) {
	case POLYLINESHAPE:
		if (lod > 20 || lod < -20){
			tlp::polyQuad(tmp, startColor, endColor, size[0] * .5, size[1] * .5, srcDir, tgtDir,colorInterpolate,borderColor,textureName);
		}else if(lod > 0.05 || lod < -0.05){
			tlp::polyQuad(tmp, startColor, endColor, size[0] * .5, size[1] * .5, srcDir, tgtDir,true,borderColor,textureName);
		}else{
			tlp::polyLine(tmp, startColor, endColor);
		}
		break;
	case L3D_BIT + POLYLINESHAPE: {
		glDisable(GL_LIGHTING);
		simpleQuad(tmp, startColor, endColor, size[0] * .5, size[1] * .5, srcDir, tgtDir,lookDir,colorInterpolate,borderColor,textureName);
		glEnable(GL_LIGHTING);
		break;
	}
	case BEZIERSHAPE :
	case SPLINESHAPE :
	case CUBICBSPLINE :
	case BEZIERSHAPE + L3D_BIT:
	case SPLINESHAPE + L3D_BIT:
	case CUBICBSPLINE + L3D_BIT: {
		static GlBezierCurve bezier;
		static GlCatmullRomCurve catmull;
		static GlOpenUniformCubicBSpline bspline;
		AbstractGlCurve *curve = NULL;
		unsigned int nbCurvePoints = 100;

		if (shape == BEZIERSHAPE || shape == BEZIERSHAPE + L3D_BIT) {
			curve = &bezier;
		} else if (shape == SPLINESHAPE || shape == SPLINESHAPE + L3D_BIT) {
			curve = &catmull;
			nbCurvePoints = 200;
		} else {
			curve = &bspline;
			nbCurvePoints = 200;
		}

		curve->setOutlined(false);
		curve->setBillboardCurve(false);
		curve->setTexture(textureName);
		if (edge3D) {
			curve->setBillboardCurve(true);
			curve->setLookDir(lookDir);
		}
		float startSize = 1, endSize = 1;
		if (lod > 10 || lod < -10) {
			if(!colorInterpolate){
				curve->setOutlined(true);
				curve->setOutlineColor(borderColor);
			}
			startSize =  size[0]*.5;
			endSize = size[1]*.5;
		} else if (lod > 0.05 || lod < -0.05) {
			startSize =  size[0]*.5;
			endSize = size[1]*.5;;
		}
		curve->drawCurve(tmp, startColor, endColor, startSize, endSize, nbCurvePoints);
		break;
	}
	default:
		if (lod > 20 || lod < -20){
			tlp::polyQuad(tmp, startColor, endColor, size[0] * .5, size[1] * .5, srcDir, tgtDir,colorInterpolate,borderColor);
		}else if(lod > 0.05 || lod < -0.05){
			tlp::polyQuad(tmp, startColor, endColor, size[0] * .5, size[1] * .5, srcDir, tgtDir,true,borderColor);
		}else{
			tlp::polyLine(tmp, startColor, endColor);
		}
		break;
	}

	glDepthFunc(GL_LEQUAL);
}

void GlEdge::drawLabel(bool drawSelect,OcclusionTest* test, TextRenderer* renderer, GlGraphInputData* data) {
	edge e = edge(id);
	bool select = data->elementSelected->getEdgeValue(e);
	if(select!=drawSelect)
		return;

	drawLabel(test, renderer, data);
}

void GlEdge::drawLabel(OcclusionTest* test, TextRenderer* renderer, GlGraphInputData* data) {

	edge e = edge(id);

	const string &tmp = data->elementLabel->getEdgeValue(e);
	if (tmp.length() < 1) {
		return;
	}

	bool select = data->elementSelected->getEdgeValue(e);

    Color fontColor;
    if (data->elementSelected->getEdgeValue(e))
        fontColor.set(255, 0, 0, 255);
    else {
        fontColor = data->elementLabelColor->getEdgeValue(e);
    }

    if(fontColor.getA()==0)
      return;

	if(select)
		glStencilFunc(GL_LEQUAL,data->parameters->getSelectedEdgesStencil() ,0xFFFF);
	else
		glStencilFunc(GL_LEQUAL,data->parameters->getEdgesLabelStencil(),0xFFFF);

	string fontName=data->elementFont->getEdgeValue(e);
	int fontSize=data->elementFontSize->getEdgeValue(e);
	if(!GlRenderer::checkFont(fontName))
		fontName=data->parameters->getFontsPath()+"font.ttf";
	if(fontSize==0)
		fontSize=18;

	if (select)
		renderer->setContext(fontName, fontSize+2, 0, 0, 255);
	else
		renderer->setContext(fontName, fontSize, 255, 255, 255);

	const std::pair<node, node>& eEnds = data->graph->ends(e);
	const node source = eEnds.first;
	const node target = eEnds.second;
	const Coord & srcCoord = data->elementLayout->getNodeValue(source);
	const Coord & tgtCoord = data->elementLayout->getNodeValue(target);
	const LineType::RealType &bends = data->elementLayout->getEdgeValue(e);
	Coord position;
	if (bends.empty()) {
		position = (srcCoord + tgtCoord) / 2.f;
	} else {
		if (bends.size() % 2 == 0)
			position = (bends[bends.size() / 2 - 1] + bends[bends.size() / 2]) / 2.f;
		else
			position = bends[bends.size() / 2];
	}

	float w_max = 300;
	float w, h;
	int rastPos[4];
	unsigned int labelsBorder = data->parameters->getLabelsBorder();

	setColor(Color(fontColor[0], fontColor[1], fontColor[2], 255));

	glRasterPos3f(position[0], position[1], position[2]);

	glGetIntegerv(GL_CURRENT_RASTER_POSITION, (GLint *) rastPos);

	if (test->testRectangle(RectangleInt2D(rastPos[0] - labelsBorder - 5, rastPos[1] - labelsBorder
			- 5, rastPos[0] + labelsBorder + 5, rastPos[1] + labelsBorder + 5))) {
		return;
	}

	renderer->setMode(TLP_PIXMAP);
	renderer->setString(tmp, VERBATIM);
	//fontRenderer->setString(str, XML);

	renderer->setColor(fontColor[0], fontColor[1], fontColor[2]);
	//  w_max = width;
	renderer->getBoundingBox(w_max, h, w);

	if (!test->addRectangle(RectangleInt2D(rastPos[0] - (int) (w / 2.0) - labelsBorder, rastPos[1]
	                                                                                            - (int) (h / 2.0) - labelsBorder, rastPos[0] + (int) (w / 2.0) + labelsBorder,
	                                                                                            rastPos[1] + (int) (h / 2.0) + labelsBorder))) {
		renderer->draw(w, w, ON_CENTER);
	}
}

void GlEdge::getEdgeColor(GlGraphInputData *data,edge e,node source, node target, bool selected,Color &srcCol, Color &tgtCol){
  Color selectionColor=data->parameters->getSelectionColor();
  if (selected) {
    srcCol = selectionColor;
    tgtCol = selectionColor;
  } else {
    if (data->parameters->isEdgeColorInterpolate()) {
      srcCol = data->elementColor->getNodeValue(source);
      tgtCol = data->elementColor->getNodeValue(target);
    } else {
      srcCol = tgtCol = data->elementColor->getEdgeValue(e);
    }
  }
}

void GlEdge::getEdgeSize(GlGraphInputData *data,edge e,const Size &srcSize, const Size &tgtSize,Size &edgeSize, float &maxSrcSize, float &maxTgtSize){
  //We block the max radius of the edge and extremities to node max Size
  maxSrcSize = std::max(srcSize[0], srcSize[1]);
  maxTgtSize = std::max(tgtSize[0], tgtSize[1]);

  if (data->parameters->isEdgeSizeInterpolate()) {
    edgeSize[0] = std::min(srcSize[0], srcSize[1]) / 16.;
    edgeSize[1] = std::min(tgtSize[0], tgtSize[1]) / 16.;
  } else {
    const Size &size = data->elementSize->getEdgeValue(e);
    edgeSize[0]=size[0];
    edgeSize[1]=size[1];
    if(data->parameters->getEdgesMaxSizeToNodesSize()){
      edgeSize[0] = std::min(maxSrcSize, size[0]);
      edgeSize[1] = std::min(maxTgtSize, size[1]);
    }
    edgeSize[0]=edgeSize[0]/2.;
    edgeSize[1]=edgeSize[1]/2.;
  }
}

float GlEdge::getEdgeWidthLod(const Coord &edgeCoord,const Size &edgeSize,Camera *camera){
  Matrix<float, 4u> projectionMatrix;
  Matrix<float, 4u> modelviewMatrix;
  camera->getProjectionMatrix(projectionMatrix);
  camera->getModelviewMatrix(modelviewMatrix);
  return projectSize(edgeCoord, Size(edgeSize[0], edgeSize[0], edgeSize[0]), projectionMatrix, modelviewMatrix,
      camera->getViewport());
}

void GlEdge::displayArrow(GlGraphInputData *data,
                          edge e,
                          node source,
                          float edgeSize,
                          const Color &color,
                          float maxSize,
                          bool selected,
                          int srcEdgeGlyph,
                          int tgtEdgeGlyph,
                          size_t numberOfBends,
                          const Coord &anchor,
                          const Coord &tgtCoord,
                          const Coord &srcAnchor,
                          const Coord &tgtAnchor,
                          Coord &lineAnchor){
  const Size &sizeRatio = data->elementSrcAnchorSize->getEdgeValue(e);

  //Correct begin tmp Anchor
  Coord beginTmpAnchor = anchor;
  if (beginTmpAnchor == tgtCoord) {
    beginTmpAnchor = tgtAnchor;
  }

  lineAnchor = beginTmpAnchor - srcAnchor;
  float nrm = lineAnchor.norm();
  float maxGlyphSize;
  if (tgtEdgeGlyph != 0 && numberOfBends == 0)
    maxGlyphSize = nrm * .5;
  else
    maxGlyphSize = nrm;

  Size size;
  if (data->parameters->isEdgeSizeInterpolate()) {
    size[0] = (sizeRatio[0]*10.) * edgeSize;
    if (size[0] > maxGlyphSize)
      size[0] = maxGlyphSize;
    size[1] = std::min(maxSize, (float)(edgeSize * (sizeRatio[1]*10.)));
    size[2] = std::min(maxSize, (float)(edgeSize * (sizeRatio[2]*10.)));
  } else {
    size[0] = sizeRatio[0] > maxGlyphSize ? maxGlyphSize : sizeRatio[0];
    size[1] = std::min(maxSize, sizeRatio[1]);
    size[2] = std::min(maxSize, sizeRatio[2]);
    if (selected)
      size += 0.05;
  }

  EdgeExtremityGlyph *extremityGlyph = data->extremityGlyphs.get(srcEdgeGlyph);
  assert(extremityGlyph);

  MatrixGL srcTransformationMatrix;
  MatrixGL srcScalingMatrix;

  extremityGlyph->getTransformationMatrix(beginTmpAnchor, srcAnchor, size,
                                          srcTransformationMatrix, srcScalingMatrix);

  /*BoundingBox box = eeGlyphBoundingBox(srcAnchor, beginTmpAnchor, size[0],
                                       srcTransformationMatrix, srcScalingMatrix);
  Coord boxSize( box[1] - box[0] );
  Coord middle( box[0] + (size) / 2.f);
  for (int i = objectScale.size() - 1; i >= 0; --i) {
    middle += objectTranslate[i];
    middle = objectCoord[i] - (objectCoord[i] - middle) * objectScale[i];
    boxSize *= objectScale[i];
  }
  box[0] = middle - boxSize / 2.f;
  box[1] = middle + boxSize / 2.f;
  float lod = 0;
  if (camera->is3D()) {
    Coord eyes = camera->getEyes() + (camera->getEyes() - camera->getCenter())
                 / (float)camera->getZoomFactor();
    lod = calculateAABBSize(box, eyes, transformMatrix, camera->getViewport(),
                            camera->getViewport());
  } else {
    lod = calculate2DLod(box, camera->getViewport(), camera->getViewport());
  }
  if (lod < 10.0) { //less than four pixel on screen, don't draw it
    beginLineAnchor = srcAnchor;
  } else {*/

    glPushMatrix();
    glMultMatrixf((GLfloat *) &srcTransformationMatrix);
    glMultMatrixf((GLfloat *) &srcScalingMatrix);
    extremityGlyph->draw(e, source, color,(data->parameters->isEdgeColorInterpolate() ? color : data->elementBorderColor->getEdgeValue(e)), 100.);
    glPopMatrix();

    //Compute new Anchor

    if (nrm > 0.00000001f) {
      lineAnchor /= nrm;
      lineAnchor *= size[0];
      lineAnchor += srcAnchor;
    } else {
      lineAnchor = srcAnchor;
    }
}

}
