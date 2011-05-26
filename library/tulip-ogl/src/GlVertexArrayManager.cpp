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
#include <GL/glew.h>

#include <tulip/OpenGlConfigManager.h>

#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlEdge.h>
#include <tulip/GlNode.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Graph.h>
#include <tulip/Curves.h>
#include <tulip/GlShaderProgram.h>

using namespace std;

// macro to get access to the undelying memory array of a vector
#if (__GNUC__ < 4 || __GNUC_MINOR__ < 2)
#define VECTOR_DATA(vec) &vec[0]
#else
#define VECTOR_DATA(vec) vec.data()
#endif

namespace tlp {

GlVertexArrayManager::GlVertexArrayManager(GlGraphInputData *inputData)
:inputData(inputData),
 graph(inputData->getGraph()),
 graphObserverActivated(false),
 layoutObserverActivated(false),
 colorObserverActivated(false),
 activated(true),
 isBegin(false),
 toComputeAll(true),
 toComputeLayout(true),
 toComputeColor(true),
 vectorLayoutSizeInit(false),
 vectorColorSizeInit(false),
 vectorIndexSizeInit(false),
 edgesModified(false) {
	colorInterpolate=inputData->parameters->isEdgeColorInterpolate();
	sizeInterpolate=inputData->parameters->isEdgeSizeInterpolate();
}

GlVertexArrayManager::~GlVertexArrayManager(){
	clearObservers();
	clearData();
}

void GlVertexArrayManager::setInputData(GlGraphInputData *inputData){
	clearObservers();
	this->inputData=inputData;
	graph=inputData->getGraph();
	initObservers();
}

bool GlVertexArrayManager::haveToCompute(){

	if(toComputeAll || toComputeLayout || toComputeColor){
		return true;
	}
	if(inputData->parameters->isEdgeColorInterpolate()!=colorInterpolate){
		colorInterpolate=inputData->parameters->isEdgeColorInterpolate();
		clearColorData();
		toComputeColor = true;
		return true;
	}
	if(inputData->parameters->isEdgeSizeInterpolate()!=sizeInterpolate){
		sizeInterpolate=inputData->parameters->isEdgeSizeInterpolate();
		clearLayoutData();
		toComputeLayout = true;
		return true;
	}

	return false;
}

void GlVertexArrayManager::setHaveToComputeAll(bool compute){
	if(compute){
		clearObservers();
		clearData();
	}
	if(!compute)
		initObservers();

	toComputeAll=compute;
	toComputeLayout=compute;
	toComputeColor=compute;
}

void GlVertexArrayManager::setHaveToComputeLayout(bool compute){
	toComputeLayout=compute;
}

void GlVertexArrayManager::setHaveToComputeColor(bool compute){
	toComputeColor=compute;
}

void GlVertexArrayManager::beginRendering() {
	if(!activated)
		return;

	graph=inputData->getGraph();

	isBegin=true;
	linesRenderingStartIndexArray.clear();
	linesRenderingCountArray.clear();

	quadsRenderingCountArray.clear();
	quadsRenderingStartIndexArray.clear();
	quadsSelectedRenderingCountArray.clear();
	quadsSelectedRenderingStartIndexArray.clear();

	quadsOutlineRenderingIndexArray.clear();
	quadsOutlineRenderingCountArray.clear();
	quadsOutlineSelectedRenderingIndexArray.clear();
	quadsOutlineSelectedRenderingCountArray.clear();

	linesSelectedRenderingStartIndexArray.clear();
	points1PNodesRenderingIndexArray.clear();
	points1PNodesSelectedRenderingIndexArray.clear();
	points2PNodesRenderingIndexArray.clear();
	points2PNodesSelectedRenderingIndexArray.clear();
	points1PEdgesRenderingIndexArray.clear();
	points1PEdgesSelectedRenderingIndexArray.clear();

	linesIndexCountArray.reserve(graph->numberOfEdges());

	linesSelectedRenderingStartIndexArray.reserve(graph->numberOfEdges());
	points1PNodesRenderingIndexArray.reserve(graph->numberOfNodes());
	points1PNodesSelectedRenderingIndexArray.reserve(graph->numberOfNodes());
	points2PNodesRenderingIndexArray.reserve(graph->numberOfNodes());
	points2PNodesSelectedRenderingIndexArray.reserve(graph->numberOfNodes());

	points1PEdgesRenderingIndexArray.reserve(graph->numberOfEdges());
	points1PEdgesSelectedRenderingIndexArray.reserve(graph->numberOfEdges());

	if(!vectorLayoutSizeInit){
		linesCoordsArray.reserve(graph->numberOfEdges()*2);
		quadsCoordsArray.reserve(graph->numberOfEdges()*4);
		pointsCoordsArray.reserve(graph->numberOfEdges()+graph->numberOfNodes());

		vectorLayoutSizeInit=true;
	}

	if(!vectorColorSizeInit){
		linesColorsArray.reserve(graph->numberOfEdges()*2);
		quadsColorsArray.reserve(graph->numberOfEdges()*4);
		pointsColorsArray.reserve(graph->numberOfEdges()+graph->numberOfNodes());

		vectorColorSizeInit=true;
	}

	if(!vectorIndexSizeInit){
		linesIndexArray.reserve(graph->numberOfEdges()*2);
		linesIndexCountArray.reserve(graph->numberOfEdges());

		vectorIndexSizeInit=true;
	}
}

void GlVertexArrayManager::endRendering() {
	if(!isBegin)
		return;
	isBegin=false;

	OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);

	glEnableClientState(GL_VERTEX_ARRAY);

	//============ Selected graph elements rendering ============================

	Color selectionColor=inputData->parameters->getSelectionColor();
	glColor4ubv(reinterpret_cast<const GLubyte *>(&selectionColor));

	glDisable(GL_DEPTH_TEST);

	// Selected edges point rendering
	glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedEdgesStencil(), 0xFFFF);
	glPointSize(2);
	if(points1PEdgesSelectedRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glDrawElements(GL_POINTS, points1PEdgesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points1PEdgesSelectedRenderingIndexArray));
	}

	// Selected nodes point rendering
	glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedNodesStencil(), 0xFFFF);
	glPointSize(2);
	if(points1PNodesSelectedRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glDrawElements(GL_POINTS, points1PNodesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points1PNodesSelectedRenderingIndexArray));
	}
	glPointSize(4);
	if(points2PNodesSelectedRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glDrawElements(GL_POINTS, points2PNodesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points2PNodesSelectedRenderingIndexArray));
	}

	// Selected edges polyline rendering
	glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedEdgesStencil(), 0xFFFF);
	glLineWidth(4);
	if(linesSelectedRenderingStartIndexArray.size()!=0) {
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));
		glMultiDrawArrays(GL_LINE_STRIP, &linesSelectedRenderingStartIndexArray[0], &linesSelectedRenderingCountArray[0], linesSelectedRenderingStartIndexArray.size());
	}

	// Selected edges quad rendering
	if (!quadsSelectedRenderingStartIndexArray.empty()) {
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(quadsCoordsArray));
		OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();
		OpenGlConfigManager::getInst().activatePolygonAntiAliasing();
		glMultiDrawArrays(GL_QUAD_STRIP, &quadsSelectedRenderingStartIndexArray[0], &quadsSelectedRenderingCountArray[0], quadsSelectedRenderingStartIndexArray.size());
		OpenGlConfigManager::getInst().desactivatePolygonAntiAliasing();
		OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();
		std::map<float, vector<GLsizei> >::iterator it = quadsOutlineSelectedRenderingCountArray.begin();
		for (; it != quadsOutlineSelectedRenderingCountArray.end() ; ++it) {
			glLineWidth(it->first);
			glMultiDrawElements(GL_LINE_STRIP, &quadsOutlineSelectedRenderingCountArray[it->first][0], GL_UNSIGNED_INT, reinterpret_cast<const GLvoid **>(&quadsOutlineSelectedRenderingIndexArray[it->first][0]), quadsOutlineSelectedRenderingCountArray[it->first].size());
		}
		OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();
	}

	OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();

	//============ Graph elements rendering ============================

	OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();

	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_DEPTH_TEST);

	// Edges point rendering
	glStencilFunc(GL_LEQUAL, inputData->parameters->getEdgesStencil(), 0xFFFF);
	glPointSize(2);
	if(points1PEdgesRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
		glDrawElements(GL_POINTS, points1PEdgesRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points1PEdgesRenderingIndexArray));
	}

	// Nodes point rendering
	glStencilFunc(GL_LEQUAL, inputData->parameters->getNodesStencil(), 0xFFFF);
	if(points1PNodesRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
		glDrawElements(GL_POINTS, points1PNodesRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points1PNodesRenderingIndexArray));
	}
	glPointSize(4);
	if(points2PNodesRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
		glDrawElements(GL_POINTS, points2PNodesRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points2PNodesRenderingIndexArray));
	}

	// Edges polyline rendering
	glStencilFunc(GL_LEQUAL, inputData->parameters->getEdgesStencil(), 0xFFFF);
	glLineWidth(1.4);
	if (!linesRenderingStartIndexArray.empty()) {
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(linesColorsArray));
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));
		glMultiDrawArrays(GL_LINE_STRIP, &linesRenderingStartIndexArray[0], &linesRenderingCountArray[0], linesRenderingStartIndexArray.size());
	}

	OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();

	// Edges quad rendering
	if (!quadsRenderingStartIndexArray.empty()) {
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(quadsCoordsArray));
		OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();
		if (!inputData->parameters->isEdgeColorInterpolate()) {
			glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsOutlineColorsArray));
		} else {
			glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsColorsArray));
		}
		std::map<float, vector<GLsizei> >::iterator it = quadsOutlineRenderingCountArray.begin();
		for (; it != quadsOutlineRenderingCountArray.end() ; ++it) {
			glLineWidth(it->first);
			glMultiDrawElements(GL_LINE_STRIP, &quadsOutlineRenderingCountArray[it->first][0], GL_UNSIGNED_INT, reinterpret_cast<const GLvoid **>(&quadsOutlineRenderingIndexArray[it->first][0]), quadsOutlineRenderingCountArray[it->first].size());
		}
		OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();

		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsColorsArray));
		OpenGlConfigManager::getInst().activatePolygonAntiAliasing();
		glMultiDrawArrays(GL_QUAD_STRIP, &quadsRenderingStartIndexArray[0], &quadsRenderingCountArray[0], quadsRenderingStartIndexArray.size());
		OpenGlConfigManager::getInst().desactivatePolygonAntiAliasing();
	}

	glDisableClientState(GL_COLOR_ARRAY);

	glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedEdgesStencil(), 0xFFFF);


	glDisableClientState(GL_VERTEX_ARRAY);

	glPointSize(1);
	glLineWidth(1);
}

void GlVertexArrayManager::pauseRendering(bool pause){
	if(pause)
		isBegin=false;
	else
		isBegin=true;
}

void GlVertexArrayManager::activate(bool act){
	activated=act;
}

void GlVertexArrayManager::addEdge(GlEdge *edge){

	node src = graph->source(tlp::edge(edge->id));
	node tgt = graph->target(tlp::edge(edge->id));

	const Size &srcSize = inputData->getElementSize()->getNodeValue(src);
	const Size &tgtSize = inputData->getElementSize()->getNodeValue(tgt);

	const Color &edgeColor = inputData->getElementColor()->getEdgeValue(tlp::edge(edge->id));
	const Color &srcColor = inputData->getElementColor()->getNodeValue(src);
	const Color &tgtColor = inputData->getElementColor()->getNodeValue(tgt);
	const Color &borderColor = inputData->getElementBorderColor()->getEdgeValue(tlp::edge(edge->id));

	if(toComputeAll || toComputeLayout){

		size_t lastIndex=linesCoordsArray.size();
		edge->getVertices(inputData, linesCoordsArray);
		size_t numberOfVertices=linesCoordsArray.size()-lastIndex;

		if(edgeToLineIndexVector.size()<edge->id+1)
			edgeToLineIndexVector.resize(edge->id+1);

		if(edgeToPointIndexVector.size()<edge->id+1)
			edgeToPointIndexVector.resize(edge->id+1);

		if(edgeToQuadIndexVector.size()<edge->id+1)
			edgeToQuadIndexVector.resize(edge->id+1);

		if(edgeToBottomOulineIndexVector.size()<edge->id+1)
			edgeToBottomOulineIndexVector.resize(edge->id+1);

		if(edgeToTopOutlineIndexVector.size()<edge->id+1)
			edgeToTopOutlineIndexVector.resize(edge->id+1);

		if(edgeToPointIndexVector.size()<edge->id+1)
			edgeToPointIndexVector.resize(edge->id+1);

		vector<Coord> vertices;
		vector<Color> colors;
		if(numberOfVertices!=0){
			edgeToLineIndexVector[edge->id]=pair<unsigned int,unsigned int>(linesIndexArray.size(),linesIndexCountArray.size());
			linesIndexArray.push_back(lastIndex);
			for(size_t i=0;i<numberOfVertices;++i){
				vertices.push_back(linesCoordsArray[lastIndex+i]);
			}

			edge->getColors(inputData, vertices,colors);
			linesColorsArray.insert(linesColorsArray.end(), colors.begin(), colors.end());
			linesIndexCountArray.push_back(numberOfVertices);

			pointsCoordsArray.push_back(linesCoordsArray[lastIndex]);
			pointsColorsArray.push_back(linesColorsArray[lastIndex]);
			edgeToPointIndexVector[edge->id]=pointsCoordsArray.size()-1;

			Size edgeSize;
			float maxSrcSize, maxTgtSize;
			if(srcSize[0]>=srcSize[1])
				maxSrcSize=srcSize[0];
			else
				maxSrcSize=srcSize[1];
			if(tgtSize[0]>=tgtSize[1])
				maxTgtSize=tgtSize[0];
			else
				maxTgtSize=tgtSize[1];

			edge->getEdgeSize(inputData, tlp::edge(edge->id),srcSize,tgtSize,maxSrcSize,maxTgtSize,edgeSize);

			vector<float> edgeSizes;
			getSizes(vertices, edgeSize[0]/2.0f, edgeSize[1]/2.0f, edgeSizes);

			edgeToQuadIndexVector[edge->id]=pair<unsigned int,unsigned int>(quadsIndexArray.size(),quadsIndexCountArray.size());
			edgeToBottomOulineIndexVector[edge->id]=quadsBottomOutlineIndexArray.size();
			edgeToTopOutlineIndexVector[edge->id]=quadsTopOutlineIndexArray.size();

			size_t lastQuadIndex=quadsCoordsArray.size();
			quadsIndexArray.push_back(lastQuadIndex);

			vector<Coord> quadVertices;
			buildCurvePoints(vertices, edgeSizes, inputData->elementLayout->getNodeValue(src), inputData->elementLayout->getNodeValue(tgt), quadVertices);

			vector<Coord> centerLine;
			centerLine.reserve(quadVertices.size()/2);
			vector<GLuint> quadBottomOutlineIndices;
			vector<GLuint> quadTopOutlineIndices;
			for (size_t i = 0 ; i < quadVertices.size() / 2 ; ++i) {
				centerLine.push_back((quadVertices[2*i]+quadVertices[2*i+1])/2);
				quadBottomOutlineIndices.push_back(lastQuadIndex+2*i);
				quadTopOutlineIndices.push_back(lastQuadIndex+2*i+1);
			}

			quadsBottomOutlineIndexArray.push_back(quadBottomOutlineIndices);
			quadsTopOutlineIndexArray.push_back(quadTopOutlineIndices);

			vector<Color> colors;
			getColors(&centerLine[0], centerLine.size(), srcColor, tgtColor,colors);

			for (size_t i = 0 ; i < colors.size() ; ++i) {
				if (inputData->parameters->isEdgeColorInterpolate()) {
					quadsColorsArray.push_back(colors[i]);
					quadsColorsArray.push_back(colors[i]);
				} else {
					quadsColorsArray.push_back(edgeColor);
					quadsColorsArray.push_back(edgeColor);
				}
				quadsOutlineColorsArray.push_back(borderColor);
				quadsOutlineColorsArray.push_back(borderColor);
			}

			quadsCoordsArray.insert(quadsCoordsArray.end(), quadVertices.begin(), quadVertices.end());
			quadsIndexCountArray.push_back(quadVertices.size());

		}else{
			linesIndexCountArray.push_back(0);
			edgeToPointIndexVector[edge->id]=-1;
			quadsIndexCountArray.push_back(0);
			edgeToLineIndexVector[edge->id]=make_pair(-1,linesIndexCountArray.size()-1);
			edgeToQuadIndexVector[edge->id] = make_pair(-1,quadsIndexCountArray.size()-1);
		}
	}else{

		size_t lastIndex=linesColorsArray.size();
		pair<unsigned int,unsigned int> index=edgeToLineIndexVector[edge->id];
		pair<unsigned int,unsigned int> indexQuad=edgeToQuadIndexVector[edge->id];
		GLsizei numberOfVertices=linesIndexCountArray[index.second];

		if(numberOfVertices!=0){

			vector<Color> colors;
			edge->getColors(inputData, &linesCoordsArray[linesIndexArray[index.first]],numberOfVertices,colors);
			linesColorsArray.insert(linesColorsArray.end(), colors.begin(), colors.end());

			GLsizei numberQuadVertices=quadsIndexCountArray[indexQuad.second];

			unsigned int baseQuadIndex=quadsIndexArray[indexQuad.first];

			vector<Coord> centerLine;
			centerLine.reserve(numberQuadVertices/2);
			for (int i = 0 ; i < numberQuadVertices / 2 ; ++i) {
				centerLine.push_back((quadsCoordsArray[baseQuadIndex+2*i]+quadsCoordsArray[2*i+1])/2);
			}
			colors.clear();
			getColors(&centerLine[0], centerLine.size(), srcColor, tgtColor,colors);

			for (size_t i = 0 ; i < colors.size() ; ++i) {
				if (inputData->parameters->isEdgeColorInterpolate()) {
					quadsColorsArray.push_back(colors[i]);
					quadsColorsArray.push_back(colors[i]);
				} else {
					quadsColorsArray.push_back(edgeColor);
					quadsColorsArray.push_back(edgeColor);
				}

				quadsOutlineColorsArray.push_back(borderColor);
				quadsOutlineColorsArray.push_back(borderColor);
			}

			pointsColorsArray.push_back(linesColorsArray[lastIndex]);
		}
	}
}

void GlVertexArrayManager::addNode(GlNode *node){
	if(toComputeAll || toComputeLayout){
		node->getPointAndColor(inputData, pointsCoordsArray,pointsColorsArray);
		if(nodeToPointIndexVector.size()<node->id+1)
			nodeToPointIndexVector.resize(node->id+1);
		nodeToPointIndexVector[node->id]=pointsCoordsArray.size()-1;
	}else{
		node->getColor(inputData, pointsColorsArray);
	}
}

void GlVertexArrayManager::activateLineEdgeDisplay(GlEdge *edge,bool selected){
	pair<unsigned int,unsigned int> index=edgeToLineIndexVector[edge->id];
	unsigned int numberOfVertices=linesIndexCountArray[index.second];
	if(numberOfVertices==0)
		return;

	unsigned int baseIndex=linesIndexArray[index.first];
	if(!selected){
		linesRenderingStartIndexArray.push_back(baseIndex);
		linesRenderingCountArray.push_back(numberOfVertices);
	}else{
		linesSelectedRenderingStartIndexArray.push_back(baseIndex);
		linesSelectedRenderingCountArray.push_back(numberOfVertices);
	}
}

void GlVertexArrayManager::activateQuadEdgeDisplay(GlEdge *edge, bool selected) {
	pair<unsigned int,unsigned int> index=edgeToQuadIndexVector[edge->id];
	unsigned int numberOfVertices=quadsIndexCountArray[index.second];
	unsigned int bottomOutlineIndicesIdx = edgeToBottomOulineIndexVector[edge->id];
	unsigned int topOutlineIndicesIdx = edgeToTopOutlineIndexVector[edge->id];

	float borderWidth = inputData->getElementBorderWidth()->getEdgeValue(tlp::edge(edge->id));


	if(numberOfVertices==0)
		return;

	unsigned int baseIndex=quadsIndexArray[index.first];
	if (!selected) {
		quadsRenderingStartIndexArray.push_back(baseIndex);
		quadsRenderingCountArray.push_back(numberOfVertices);

		if (borderWidth > 0) {

			if (quadsOutlineRenderingIndexArray.find(borderWidth) == quadsOutlineRenderingIndexArray.end()) {
				quadsOutlineRenderingIndexArray[borderWidth] = vector<const GLuint *>();
				quadsOutlineRenderingCountArray[borderWidth] = vector<GLsizei>();
			}

			quadsOutlineRenderingIndexArray[borderWidth].push_back(&quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx][0]);
			quadsOutlineRenderingCountArray[borderWidth].push_back(quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx].size());
			quadsOutlineRenderingIndexArray[borderWidth].push_back(&quadsTopOutlineIndexArray[topOutlineIndicesIdx][0]);
			quadsOutlineRenderingCountArray[borderWidth].push_back(quadsTopOutlineIndexArray[topOutlineIndicesIdx].size());
		}
	} else {
		quadsSelectedRenderingStartIndexArray.push_back(baseIndex);
		quadsSelectedRenderingCountArray.push_back(numberOfVertices);

		if (borderWidth > 0) {

			if (quadsOutlineSelectedRenderingIndexArray.find(borderWidth) == quadsOutlineSelectedRenderingIndexArray.end()) {
				quadsOutlineSelectedRenderingIndexArray[borderWidth] = vector<const GLuint *>();
				quadsOutlineSelectedRenderingCountArray[borderWidth] = vector<GLsizei>();
			}

			quadsOutlineSelectedRenderingIndexArray[borderWidth].push_back(&quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx][0]);
			quadsOutlineSelectedRenderingCountArray[borderWidth].push_back(quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx].size());
			quadsOutlineSelectedRenderingIndexArray[borderWidth].push_back(&quadsTopOutlineIndexArray[topOutlineIndicesIdx][0]);
			quadsOutlineSelectedRenderingCountArray[borderWidth].push_back(quadsTopOutlineIndexArray[topOutlineIndicesIdx].size());
		}
	}
}

void GlVertexArrayManager::activatePointEdgeDisplay(GlEdge *edge,bool selected){
	unsigned int index=edgeToPointIndexVector[edge->id];
	if(index==(unsigned int)(-1))
		return;

	if(!selected){
		points1PEdgesRenderingIndexArray.push_back(index);
	}else{
		points1PEdgesSelectedRenderingIndexArray.push_back(index);
	}
}

void GlVertexArrayManager::activatePointNodeDisplay(GlNode *node,bool onePixel,bool selected){
	unsigned int index=nodeToPointIndexVector[node->id];
	if(index==(unsigned int)(-1))
		return;

	if(!selected){
		if(onePixel)
			points1PNodesRenderingIndexArray.push_back(index);
		else
			points2PNodesRenderingIndexArray.push_back(index);
	}else{
		if(onePixel)
			points1PNodesSelectedRenderingIndexArray.push_back(index);
		else
			points2PNodesSelectedRenderingIndexArray.push_back(index);
	}
}

void GlVertexArrayManager::addEdge(Graph *,const edge){
	clearData();
	clearObservers();
}

void GlVertexArrayManager::addNode(Graph *,const node){
	clearData();
	clearObservers();
}

void GlVertexArrayManager::propertyValueChanged(PropertyInterface *property){
	if(inputData->getElementLayout()==property || inputData->getElementSize()==property){
		setHaveToComputeLayout(true);
		clearLayoutData();
		inputData->getElementLayout()->removePropertyObserver(this);
		layoutObserverActivated=false;
	}
	if(edgesModified || inputData->getElementColor()==property || inputData->getElementBorderColor()==property ){
		setHaveToComputeColor(true);
		clearColorData();
		inputData->getElementColor()->removePropertyObserver(this);
		inputData->getElementBorderColor()->removePropertyObserver(this);
		colorObserverActivated=false;
	}
	edgesModified = false;
}

void GlVertexArrayManager::beforeSetAllNodeValue(PropertyInterface *property) {
	propertyValueChanged(property);
}

void GlVertexArrayManager::beforeSetAllEdgeValue(PropertyInterface *property) {
	if (inputData->getElementLayout()==property) {
		edgesModified = true;
	}
	propertyValueChanged(property);
}

void GlVertexArrayManager::beforeSetNodeValue(PropertyInterface *property, const node){
	propertyValueChanged(property);
}

void GlVertexArrayManager::beforeSetEdgeValue(PropertyInterface *property, const edge){
	if (inputData->getElementLayout()==property) {
		edgesModified = true;
	}
	propertyValueChanged(property);
}

void GlVertexArrayManager::destroy(Graph *){
	clearData();
	clearObservers();
}

void GlVertexArrayManager::destroy(PropertyInterface*){
	clearData();
	clearObservers();
}

void GlVertexArrayManager::addLocalProperty(Graph *, const std::string &name){
	if(name==inputData->getElementColorPropName() || name==inputData->getElementLayoutPropName() || name ==inputData->getElementSizePropName()){
		clearData();
		clearObservers();
	}
}

void GlVertexArrayManager::delLocalProperty(Graph *, const std::string &name){
	if(name==inputData->getElementColorPropName() || name==inputData->getElementLayoutPropName() || name ==inputData->getElementSizePropName()){
		clearData();
		clearObservers();
	}
}

void GlVertexArrayManager::clearLayoutData() {
	toComputeLayout=true;

	linesCoordsArray.clear();
	pointsCoordsArray.clear();
	quadsCoordsArray.clear();

	vectorLayoutSizeInit=false;
}

void GlVertexArrayManager::clearColorData() {
	toComputeColor=true;

	linesColorsArray.clear();
	pointsColorsArray.clear();
	quadsColorsArray.clear();
	quadsOutlineColorsArray.clear();

	vectorColorSizeInit=false;
}

void GlVertexArrayManager::clearData() {
	toComputeAll=true;

	linesCoordsArray.clear();
	linesColorsArray.clear();
	linesIndexArray.clear();
	linesIndexCountArray.clear();
	edgeToLineIndexVector.clear();
	edgeToQuadIndexVector.clear();
	edgeToTopOutlineIndexVector.clear();
	edgeToBottomOulineIndexVector.clear();

	quadsCoordsArray.clear();
	quadsColorsArray.clear();
	quadsOutlineColorsArray.clear();
	quadsIndexArray.clear();
	quadsIndexCountArray.clear();
	quadsTopOutlineIndexArray.clear();
	quadsBottomOutlineIndexArray.clear();

	pointsCoordsArray.clear();
	pointsColorsArray.clear();
	edgeToPointIndexVector.clear();
	nodeToPointIndexVector.clear();

	vectorLayoutSizeInit=false;
	vectorColorSizeInit=false;
	vectorIndexSizeInit=false;
}

void GlVertexArrayManager::initObservers() {
	if(!graphObserverActivated){
		graph->addGraphObserver(this);
		graphObserverActivated=true;
	}
	if(!layoutObserverActivated){
		inputData->getElementLayout()->addPropertyObserver(this);
		inputData->getElementSize()->addPropertyObserver(this);
		layoutObserverActivated=true;
	}
	if(!colorObserverActivated){
		inputData->getElementColor()->addPropertyObserver(this);
		inputData->getElementBorderColor()->addPropertyObserver(this);
		colorObserverActivated=true;
	}
}

void GlVertexArrayManager::clearObservers() {

	if(graphObserverActivated){
		graph->removeGraphObserver(this);
		graphObserverActivated=false;
	}
	if(layoutObserverActivated){
		inputData->getElementLayout()->removePropertyObserver(this);
		inputData->getElementSize()->removePropertyObserver(this);
		layoutObserverActivated=false;
	}
	if(colorObserverActivated){
		inputData->getElementColor()->removePropertyObserver(this);
		inputData->getElementBorderColor()->removePropertyObserver(this);
		colorObserverActivated=false;
	}
}

}
