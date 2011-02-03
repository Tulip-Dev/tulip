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

#include "tulip/OpenGlConfigManager.h"

#include "tulip/GlVertexArrayManager.h"
#include "tulip/GlEdge.h"
#include "tulip/GlNode.h"
#include "tulip/GlGraphInputData.h"
#include "tulip/Graph.h"

using namespace std;

// macro to get access to the undelying memory array of a vector
#if (__GNUC__ < 4 || __GNUC_MINOR__ < 2)
#define VECTOR_DATA(vec) &vec[0]
#else
#define VECTOR_DATA(vec) vec.data()
#endif


namespace tlp
{
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
	edgesModified(false)
{
	colorInterpolate=inputData->parameters->isEdgeColorInterpolate();
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
	linesRenderingIndexArray.clear();
	linesSelectedRenderingIndexArray.clear();
	points1PNodesRenderingIndexArray.clear();
	points1PNodesSelectedRenderingIndexArray.clear();
	points2PNodesRenderingIndexArray.clear();
	points2PNodesSelectedRenderingIndexArray.clear();
	points1PEdgesRenderingIndexArray.clear();
	points1PEdgesSelectedRenderingIndexArray.clear();

	linesRenderingIndexArray.reserve(graph->numberOfEdges()*2);
	linesSelectedRenderingIndexArray.reserve(graph->numberOfEdges()*2);
	points1PNodesRenderingIndexArray.reserve(graph->numberOfNodes());
	points1PNodesSelectedRenderingIndexArray.reserve(graph->numberOfNodes());
	points2PNodesRenderingIndexArray.reserve(graph->numberOfNodes());
	points2PNodesSelectedRenderingIndexArray.reserve(graph->numberOfNodes());

	points1PEdgesRenderingIndexArray.reserve(graph->numberOfEdges());
	points1PEdgesSelectedRenderingIndexArray.reserve(graph->numberOfEdges());

	if(!vectorLayoutSizeInit){
		linesCoordsArray.reserve(graph->numberOfEdges()*2);
		pointsCoordsArray.reserve(graph->numberOfEdges()+graph->numberOfNodes());

		vectorLayoutSizeInit=true;
	}

	if(!vectorColorSizeInit){
		linesColorsArray.reserve(graph->numberOfEdges()*2);
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
	if(linesSelectedRenderingIndexArray.size()!=0) {
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));
		glDrawElements(GL_LINES, linesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(linesSelectedRenderingIndexArray));
	}

	//============ Graph elements rendering ============================

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
	if(linesRenderingIndexArray.size()!=0){
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(linesColorsArray));
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));

		glDrawElements(GL_LINES, linesRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(linesRenderingIndexArray));
	}

	glPointSize(1);
	glLineWidth(1);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();

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
	if(toComputeAll || toComputeLayout){

		size_t lastIndex=linesCoordsArray.size();
		edge->getVertices(inputData,linesCoordsArray);
		size_t numberOfVertices=linesCoordsArray.size()-lastIndex;

		if(edgeToLineIndexVector.size()<edge->id+1)
			edgeToLineIndexVector.resize(edge->id+1);

		edgeToLineIndexVector[edge->id]=pair<unsigned int,unsigned int>(linesIndexArray.size(),linesIndexCountArray.size());

		if(edgeToPointIndexVector.size()<edge->id+1)
			edgeToPointIndexVector.resize(edge->id+1);

		vector<Coord> vertices;
		if(numberOfVertices!=0){
			linesIndexArray.push_back(lastIndex);
			for(size_t i=0;i<numberOfVertices;++i){
				vertices.push_back(linesCoordsArray[lastIndex+i]);
			}
			edge->getColors(inputData,vertices,linesColorsArray);
			linesIndexCountArray.push_back(numberOfVertices);

			pointsCoordsArray.push_back(linesCoordsArray[lastIndex]);
			pointsColorsArray.push_back(linesColorsArray[lastIndex]);

			edgeToPointIndexVector[edge->id]=pointsCoordsArray.size()-1;
		}else{
			linesIndexCountArray.push_back(0);
			edgeToPointIndexVector[edge->id]=-1;
		}
	}else{
		size_t lastIndex=linesColorsArray.size();

		pair<unsigned int,unsigned int> index=edgeToLineIndexVector[edge->id];
		GLsizei numberOfVertices=linesIndexCountArray[index.second];
		if(numberOfVertices!=0){
			edge->getColors(inputData,&linesCoordsArray[linesIndexArray[index.first]],numberOfVertices,linesColorsArray);
			pointsColorsArray.push_back(linesColorsArray[lastIndex]);
		}
	}
}

void GlVertexArrayManager::addNode(GlNode *node){
	if(toComputeAll || toComputeLayout){
		node->getPointAndColor(inputData,pointsCoordsArray,pointsColorsArray);

		if(nodeToPointIndexVector.size()<node->id+1)
			nodeToPointIndexVector.resize(node->id+1);
		nodeToPointIndexVector[node->id]=pointsCoordsArray.size()-1;
	}else{
		node->getColor(inputData,pointsColorsArray);
	}
}

void GlVertexArrayManager::activateLineEdgeDisplay(GlEdge *edge,bool selected){
	pair<unsigned int,unsigned int> index=edgeToLineIndexVector[edge->id];
	unsigned int numberOfVertices=linesIndexCountArray[index.second];
	if(numberOfVertices==0)
		return;

	unsigned int baseIndex=linesIndexArray[index.first];
	if(!selected){
		for(unsigned int i=0;i<numberOfVertices-1;++i){
			linesRenderingIndexArray.push_back(baseIndex+i);
			linesRenderingIndexArray.push_back(baseIndex+i+1);
		}
	}else{
		for(unsigned int i=0;i<numberOfVertices-1;++i){
			linesSelectedRenderingIndexArray.push_back(baseIndex+i);
			linesSelectedRenderingIndexArray.push_back(baseIndex+i+1);
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
	if(edgesModified || inputData->getElementColor()==property){
		setHaveToComputeColor(true);
		clearColorData();
		inputData->getElementColor()->removePropertyObserver(this);
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

	vectorLayoutSizeInit=false;
}

void GlVertexArrayManager::clearColorData() {
	toComputeColor=true;

	linesColorsArray.clear();
	pointsColorsArray.clear();

	vectorColorSizeInit=false;
}

void GlVertexArrayManager::clearData() {
	toComputeAll=true;

	linesCoordsArray.clear();
	linesColorsArray.clear();
	linesIndexArray.clear();
	linesIndexCountArray.clear();
	edgeToLineIndexVector.clear();

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
		colorObserverActivated=false;
	}
}
}
