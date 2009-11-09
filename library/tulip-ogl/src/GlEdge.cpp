#include "tulip/GlewManager.h"
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

#include <iostream>

using namespace std;

namespace tlp {

//static const Color COLORSELECT = Color(255, 102, 255, 255);

//BoundingBox eeGlyphBoundingBox(const Coord& anchor, const Coord& dest, const Coord& size) {
//
//	//Vecteur AB
//	Vector<float, 3> vAB;
//	//Vecteur V
//	Vector<float, 3> vV;
//	//Vecteur W
//	Vector<float, 3> vW;
//
//	vAB = dest - anchor;
//	float nAB; //|AB|
//	nAB = vAB.norm();
//	if (fabs(nAB) > 1E-6)
//		vAB /= nAB;
//
//	//Compute norm
//	//vAB * vV = xAB * xV + yAB*yV + zAB * zV = |AB| * |V| * cos(alpha) = 0;
//	if (fabs(vAB[2]) < 1E-6) {
//		vV[0] = 0;
//		vV[1] = 0;
//		vV[2] = 1.0;
//	} else if (fabs(vAB[1]) < 1E-6) {
//		vV[0] = 0;
//		vV[1] = 1.0;
//		vV[2] = 0;
//	} else {
//		vV[0] = 0;
//		vV[1] = 1. / vAB[1];
//		vV[2] = -1. / vAB[2];
//		vV /= vV.norm();
//	}
//
//	vW = vAB ^ vV;
//	float nW = vW.norm();
//	if (fabs(nW) > 1E-6)
//		vW /= nW;
//
//	Coord mid = anchor + vAB * size[0] * .5;
//
//	BoundingBox box;
//	box.check(anchor);
//	box.check(dest);
//	box.check(mid + vV * size[1] * .5);
//	box.check(mid - vV * size[1] * .5);
//	box.check(mid + vW * size[2] * .5);
//	box.check(mid - vW * size[2] * .5);
//
//	return box;
//}

BoundingBox GlEdge::eeGlyphBoundingBox(const Coord& anchor, const Coord& tgt, float glyphNrm,
		const MatrixGL& transformation, const MatrixGL& size) {

	Coord vect = tgt - anchor;
	float nrm = vect.norm();
	if (fabs(nrm) > 1E-6)
		vect /= nrm;

	Coord mid = anchor + vect * glyphNrm * .5;

	Coord vAB(transformation[0][0], transformation[0][1], transformation[0][2]);
	Coord vV(transformation[0][1], transformation[1][1], transformation[2][1]);
	Coord vW(transformation[0][2], transformation[1][2], transformation[2][2]);

	BoundingBox box;
	box.check(mid + vAB * (-size[0][0]) * .5);
	box.check(mid + vAB * (size[0][0]) * .5);
	box.check(mid + vV * (-size[1][1]) * .5);
	box.check(mid + vV * (size[1][1]) * .5);
	box.check(mid + vW * (-size[2][2]) * .5);
	box.check(mid + vW * (size[2][2]) * .5);

	return box;
}

BoundingBox GlEdge::getBoundingBox(GlGraphInputData* data) {
	edge e = edge(id);
	BoundingBox bb;

	const node source = data->graph->source(e);
	const node target = data->graph->target(e);
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

void GlEdge::draw(float lod, GlGraphInputData* data, Camera* camera) {
  Color selectionColor=PreferenceManager::getInst().getSelectionColor();
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

	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	const node source = data->graph->source(e);
	const node target = data->graph->target(e);
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

	Color* srcCol, *tgtCol;
	if (selected) {
		srcCol = &selectionColor;
		tgtCol = &selectionColor;
	} else {
		if (data->parameters->isEdgeColorInterpolate()) {
			srcCol = &(Color&) data->elementColor->getNodeValue(source);
			tgtCol = &(Color&) data->elementColor->getNodeValue(target);
		} else {
			srcCol = tgtCol = &(Color&) data->elementColor->getEdgeValue(e);
		}
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

	//We block the max radius of the edge and extremities to node max Size
	float maxSrcSize = std::max(srcSize[0], srcSize[1]);
	float maxTgtSize = std::max(tgtSize[0], tgtSize[1]);

	Size edgeSize; //the edge radius and arrow radius
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

	if (selected) {
	  Coord p1=camera->screenTo3DWorld(Coord(0,0,0));
	  Coord p2=camera->screenTo3DWorld(Coord(2,0,0));
	  edgeSize[0] += (p2-p1).norm();
	  edgeSize[1] += (p2-p1).norm();
	}

	int startEdgeGlyph = data->elementSrcAnchorShape->getEdgeValue(e);
	int endEdgeGlyph = data->elementTgtAnchorShape->getEdgeValue(e);

	if (startEdgeGlyph != 0) {
		const Size &sizeRatio = data->elementSrcAnchorSize->getEdgeValue(e);

		//Correct begin tmp Anchor
		if (beginTmpAnchor == tgtCoord) {
			beginTmpAnchor = tgtAnchor;
		}

		beginLineAnchor = beginTmpAnchor - srcAnchor;
		float nrm = beginLineAnchor.norm();
		float maxGlyphSize;
		if (endEdgeGlyph != 0 && nbBends == 0)
			maxGlyphSize = nrm * .5;
		else
			maxGlyphSize = nrm;

		Size size;
		if (data->parameters->isEdgeSizeInterpolate()) {
			size[0] = sizeRatio[0] * edgeSize[0];
			if (size[0] > maxGlyphSize)
				size[0] = maxGlyphSize;
			size[1] = std::min(maxSrcSize, edgeSize[0] * sizeRatio[1]);
			size[2] = std::min(maxSrcSize, edgeSize[0] * sizeRatio[2]);
		} else {
			size[0] = sizeRatio[0] > maxGlyphSize ? maxGlyphSize : sizeRatio[0];
			size[1] = std::min(maxSrcSize, sizeRatio[1]);
			size[2] = std::min(maxSrcSize, sizeRatio[2]);
			if (selected)
				size += 0.05;
		}

		EdgeExtremityGlyph *extremityGlyph = data->extremityGlyphs.get(startEdgeGlyph);
		assert(extremityGlyph);

		MatrixGL srcTransformationMatrix;
		MatrixGL srcScalingMatrix;

		extremityGlyph->getTransformationMatrix(beginTmpAnchor, srcAnchor, size,
				srcTransformationMatrix, srcScalingMatrix);

		BoundingBox box = eeGlyphBoundingBox(srcAnchor, beginTmpAnchor, size[0],
				srcTransformationMatrix, srcScalingMatrix);

		Coord boxSize = box.second - box.first;
		Coord middle = box.first + (size) / 2;

		for (int i = objectScale.size() - 1; i >= 0; --i) {
			middle += objectTranslate[i];
			middle = objectCoord[i] - (objectCoord[i] - middle) * objectScale[i];
			boxSize *= objectScale[i];
		}

		box.first = middle - boxSize / 2;
		box.second = middle + boxSize / 2;

		float lod = 0;
		if (camera->is3D()) {
			Coord eyes = camera->getEyes() + (camera->getEyes() - camera->getCenter())
					/ camera->getZoomFactor();
			lod = calculateAABBSize(box, eyes, transformMatrix, camera->getViewport(),
					camera->getViewport());
		} else {
			lod = calculate2DLod(box, camera->getViewport(), camera->getViewport());
		}

		if (lod < 10.0) { //less than four pixel on screen, don't draw it
			beginLineAnchor = srcAnchor;
		} else {

			glPushMatrix();
			glMultMatrixf((GLfloat *) &srcTransformationMatrix);
			glMultMatrixf((GLfloat *) &srcScalingMatrix);
			extremityGlyph->draw(e, source, (selected ? selectionColor : *srcCol), lod);
			glPopMatrix();

			//Compute new Anchor

			if (nrm > 0.00000001f) {
				beginLineAnchor /= nrm;
				beginLineAnchor *= size[0];
				beginLineAnchor += srcAnchor;
			} else {
				beginLineAnchor = srcAnchor;
			}
		}
	} else {
		beginLineAnchor = srcAnchor;
	}

	if (endEdgeGlyph != 0) {
		const Size& sizeRatio = data->elementTgtAnchorSize->getEdgeValue(e);
		endLineAnchor = endTmpAnchor - tgtAnchor;
		float nrm = endLineAnchor.norm();
		float maxGlyphSize;
		if (startEdgeGlyph != 0 && nbBends == 0)
			maxGlyphSize = nrm * .5;
		else
			maxGlyphSize = nrm;

		Size size;
		if (data->parameters->isEdgeSizeInterpolate()) {
			size[0] = sizeRatio[0] * edgeSize[1];
			if (size[0] > maxGlyphSize)
				size[0] = maxGlyphSize;
			size[1] = std::min(maxTgtSize, edgeSize[1] * sizeRatio[1]);
			size[2] = std::min(maxTgtSize, edgeSize[1] * sizeRatio[2]);
		} else {
			size[0] = sizeRatio[0] > maxGlyphSize ? maxGlyphSize : sizeRatio[0];
			size[1] = std::min(maxTgtSize, sizeRatio[1]);
			size[2] = std::min(maxTgtSize, sizeRatio[2]);
			if (selected)
				size += 0.05;
		}

		EdgeExtremityGlyph * extremityGlyph = data->extremityGlyphs.get(endEdgeGlyph);
		assert(extremityGlyph);
		MatrixGL tgtTransformationMatrix;
		MatrixGL tgtScalingMatrix;

		extremityGlyph->getTransformationMatrix(endTmpAnchor, tgtAnchor, size,
				tgtTransformationMatrix, tgtScalingMatrix);

		Coord nsize(tgtScalingMatrix[0][0], tgtScalingMatrix[1][1], tgtScalingMatrix[2][2]);
		BoundingBox box = eeGlyphBoundingBox(tgtAnchor, endTmpAnchor, size[0],
				tgtTransformationMatrix, tgtScalingMatrix);

		Coord boxSize = box.second - box.first;
		Coord middle = box.first + (size) / 2;

		for (int i = objectScale.size() - 1; i >= 0; --i) {
			middle += objectTranslate[i];
			middle = objectCoord[i] - (objectCoord[i] - middle) * objectScale[i];
			boxSize *= objectScale[i];
		}

		box.first = middle - boxSize / 2;
		box.second = middle + boxSize / 2;
		float lod = 0;
		if (camera->is3D()) {
			Coord eyes = camera->getEyes() + (camera->getEyes() - camera->getCenter())
					/ camera->getZoomFactor();
			lod = calculateAABBSize(box, eyes, transformMatrix, camera->getViewport(),
					camera->getViewport());
		} else {
			lod = calculate2DLod(box, camera->getViewport(), camera->getViewport());
		}
		if (lod < 10.0) { //less than four pixel on screen, don't draw it
			endLineAnchor = tgtAnchor;
		} else {
			glPushMatrix();
			glMultMatrixf((GLfloat *) &tgtTransformationMatrix);
			glMultMatrixf((GLfloat *) &tgtScalingMatrix);
			extremityGlyph->draw(e, target, (selected ? selectionColor : *tgtCol), lod);
			glPopMatrix();

			//Compute new Anchor
			if (nrm > 0.00000001f) {
				endLineAnchor /= nrm;
				endLineAnchor *= size[0];
				endLineAnchor += tgtAnchor;
			} else {
				endLineAnchor = tgtAnchor;
			}
		}
	} else {
		endLineAnchor = tgtAnchor;
	}

	if (lod < 5) {
		if (GlewManager::getInst().canUseGlew() && GlPointManager::getInst().renderingIsBegin()) {
			GlPointManager::getInst().addPoint(srcCoord, *srcCol, 1);
		} else {
			setColor(*srcCol);
			glPointSize(1);
			glBegin(GL_POINTS);
			glVertex3f(srcCoord[0], srcCoord[1], srcCoord[2]);
			glEnd();
		}
		return;
	}

	//Reset in case of drawing extremity glyph that can alterate value
	glLineWidth(1);

	Matrix<float, 4u> projectionMatrix;
	Matrix<float, 4u> modelviewMatrix;
	camera->getProjectionMatrix(projectionMatrix);
	camera->getModelviewMatrix(modelviewMatrix);
	float lodSize = projectSize(srcCoord, edgeSize[0], projectionMatrix, modelviewMatrix,
			camera->getViewport());

	//draw Edge
	drawEdge(srcCoord, tgtCoord, beginLineAnchor, endLineAnchor, bends, *srcCol, *tgtCol, edgeSize,
			data->elementShape->getEdgeValue(e), data->parameters->isEdge3D(), lodSize);

	if (data->parameters->getFeedbackRender()) {
		glPassThrough(TLP_FB_END_EDGE);
	}

	glEnable(GL_LIGHTING);

}

#define L3D_BIT (1<<9)
void GlEdge::drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos, const Coord &startPoint,
		const Coord &endPoint, const LineType::RealType &bends, const Color &startColor,
		const Color &endColor, const Size &size, int shape, bool edge3D, float lod) {
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);

	if (edge3D)
		shape |= L3D_BIT;
	if (shape & L3D_BIT) {
		if (shape >= (L3D_BIT + 16)) {
			shape = shape & ~L3D_BIT; //no 3D model defined, falling back to Line model
		}
	}
	Coord srcDir(srcNodePos);
	Coord tgtDir(tgtNodePos);
	vector<Coord> tmp = tlp::computeCleanVertices(bends, startPoint, endPoint, srcDir, tgtDir);

	if (tmp.size() < 2) {
		return;
	}

	switch (shape) {
	case POLYLINESHAPE:
		if (lod > 0.05 || lod < -0.05)
			tlp::polyQuad(tmp, startColor, endColor, size[0] * .5, size[1] * .5, srcDir, tgtDir);
		else
			tlp::polyLine(tmp, startColor, endColor);
		break;
	case BEZIERSHAPE:
		if (lod > 0.05 || lod < -0.05) {
			//tlp::bezierQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
			GlBezierCurve bezier(tmp, startColor, endColor, size[0], size[1]);
			bezier.draw(0, 0);

		} else {
			//tlp::bezierLine(tmp, startColor, endColor);
			GlBezierCurve bezier(tmp, startColor, endColor, 1, 1);
			bezier.draw(0, 0);
		}
		break;
	case SPLINESHAPE:
		if (lod > 0.05 || lod < -0.05) {
			//tlp::splineQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
			GlCatmullRomCurve catmull(tmp, startColor, endColor, size[0], size[1]);
			catmull.draw(0, 0);
		} else {
			//tlp::splineLine(tmp, startColor, endColor);
			GlCatmullRomCurve catmull(tmp, startColor, endColor, 1, 1);
			catmull.draw(0, 0);
		}
		break;
		//3D lines
	case L3D_BIT + POLYLINESHAPE:
		GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size,
				GlLines::TLP_PLAIN, GlLines::LINEAR, startColor, endColor);
		glDepthFunc(GL_LESS);
		tlp::polyLine(tmp, startColor, endColor);
		break;
	case L3D_BIT + BEZIERSHAPE:
		GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size,
				GlLines::TLP_PLAIN, GlLines::BEZIER, startColor, endColor);
		break;
	case L3D_BIT + SPLINESHAPE:
		GlLines::glDrawExtrusion(srcDir, tgtDir, startPoint, bends, endPoint, 10, size,
				GlLines::TLP_PLAIN, GlLines::SPLINE3, startColor, endColor);
		break;
	default:
		if (lod > 0.05 || lod < -0.05)
			tlp::polyQuad(tmp, startColor, endColor, size[0], size[1], srcDir, tgtDir);
		else
			tlp::polyLine(tmp, startColor, endColor);
		break;
	}

	glDepthFunc(GL_LEQUAL);
}

void GlEdge::drawLabel(bool drawSelect, OcclusionTest* test, TextRenderer* renderer,
		GlGraphInputData* data) {

	edge e = edge(id);

	const string &tmp = data->elementLabel->getEdgeValue(e);
	if (tmp.length() < 1) {
		return;
	}

	bool select = data->elementSelected->getEdgeValue(e);
	if (drawSelect != select)
		return;

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

	const Coord & srcCoord = data->elementLayout->getNodeValue(data->graph->source(e));
	const Coord & tgtCoord = data->elementLayout->getNodeValue(data->graph->target(e));
	const LineType::RealType &bends = data->elementLayout->getEdgeValue(e);
	Coord position;
	if (bends.empty()) {
		position = (srcCoord + tgtCoord) / 2.0;
	} else {
		if (bends.size() % 2 == 0)
			position = (bends[bends.size() / 2 - 1] + bends[bends.size() / 2]) / 2.0;
		else
			position = bends[bends.size() / 2];
	}

	Color fontColor;
	if (data->elementSelected->getEdgeValue(e))
		fontColor.set(255, 0, 0, 255);
	else {
		fontColor = data->elementLabelColor->getEdgeValue(e);
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
}
