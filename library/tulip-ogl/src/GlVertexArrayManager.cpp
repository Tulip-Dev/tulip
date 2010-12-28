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

#include <iostream>

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
	vectorIndexSizeInit(false)
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
	points1PRenderingIndexArray.clear();
	points1PSelectedRenderingIndexArray.clear();
	points2PRenderingIndexArray.clear();
	points2PSelectedRenderingIndexArray.clear();

	linesRenderingIndexArray.reserve(graph->numberOfEdges()*2);
	linesSelectedRenderingIndexArray.reserve(graph->numberOfEdges()*2);
	points1PRenderingIndexArray.reserve(graph->numberOfNodes()+graph->numberOfEdges());
	points1PSelectedRenderingIndexArray.reserve(graph->numberOfNodes()+graph->numberOfEdges());
	points2PRenderingIndexArray.reserve(graph->numberOfNodes()+graph->numberOfEdges());
	points2PSelectedRenderingIndexArray.reserve(graph->numberOfNodes()+graph->numberOfEdges());

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


	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedEdgesStencil(), 0xFFFF);
	Color selectionColor=inputData->parameters->getSelectionColor();
	glColor4ubv(((const GLubyte *)(&selectionColor)));
	glLineWidth(4);

	glPointSize(1);
	if(points1PSelectedRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glDrawElements(GL_POINTS, points1PSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points1PSelectedRenderingIndexArray));
	}

	glPointSize(2);
	if(points2PSelectedRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glDrawElements(GL_POINTS, points2PSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points2PSelectedRenderingIndexArray));
	}

	glPointSize(1);

	if(linesSelectedRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));
		glDrawElements(GL_LINES, linesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(linesSelectedRenderingIndexArray));
	}

	glEnableClientState(GL_COLOR_ARRAY);

	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_LEQUAL, inputData->parameters->getEdgesStencil(), 0xFFFF);
	glLineWidth(1);

	glPointSize(1);
	if(points1PRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
		glDrawElements(GL_POINTS, points1PRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points1PRenderingIndexArray));
	}
	glPointSize(2);
	if(points2PRenderingIndexArray.size()!=0){
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
		glDrawElements(GL_POINTS, points2PRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(points2PRenderingIndexArray));
	}
	glPointSize(1);

	if(linesRenderingIndexArray.size()!=0){
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(linesColorsArray));
		glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));

		glDrawElements(GL_LINES, linesRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(linesRenderingIndexArray));
	}

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
		points1PRenderingIndexArray.push_back(index);
	}else{
		points1PSelectedRenderingIndexArray.push_back(index);
	}
}

void GlVertexArrayManager::activatePointNodeDisplay(GlNode *node,bool onePixel,bool selected){
	unsigned int index=nodeToPointIndexVector[node->id];
	if(index==(unsigned int)(-1))
		return;

	if(!selected){
		if(onePixel)
			points1PRenderingIndexArray.push_back(index);
		else
			points2PRenderingIndexArray.push_back(index);
	}else{
		if(onePixel)
			points1PSelectedRenderingIndexArray.push_back(index);
		else
			points2PSelectedRenderingIndexArray.push_back(index);
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
	if(graph->getProperty(inputData->getElementLayoutPropName())==property || graph->getProperty(inputData->getElementSizePropName())==property){
		setHaveToComputeLayout(true);
		clearLayoutData();
		graph->getProperty(inputData->getElementLayoutPropName())->removePropertyObserver(this);
		layoutObserverActivated=false;
	}
	if(graph->getProperty(inputData->getElementColorPropName())==property){
		setHaveToComputeColor(true);
		clearColorData();
		graph->getProperty(inputData->getElementColorPropName())->removePropertyObserver(this);
		colorObserverActivated=false;
	}
}

void GlVertexArrayManager::beforeSetAllNodeValue(PropertyInterface *property) {
	propertyValueChanged(property);
}

void GlVertexArrayManager::beforeSetAllEdgeValue(PropertyInterface *property) {
	propertyValueChanged(property);
}

void GlVertexArrayManager::beforeSetNodeValue(PropertyInterface *property, const node){
	propertyValueChanged(property);
}

void GlVertexArrayManager::beforeSetEdgeValue(PropertyInterface *property, const edge){
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
		graph->getProperty(inputData->getElementLayoutPropName())->addPropertyObserver(this);
		graph->getProperty(inputData->getElementSizePropName())->addPropertyObserver(this);
		layoutObserverActivated=true;
	}
	if(!colorObserverActivated){
		graph->getProperty(inputData->getElementColorPropName())->addPropertyObserver(this);
		colorObserverActivated=true;
	}
}

void GlVertexArrayManager::clearObservers() {

	if(graphObserverActivated){
		graph->removeGraphObserver(this);
		graphObserverActivated=false;
	}
	if(layoutObserverActivated){
		graph->getProperty(inputData->getElementLayoutPropName())->removePropertyObserver(this);
		graph->getProperty(inputData->getElementSizePropName())->removePropertyObserver(this);
		layoutObserverActivated=false;
	}
	if(colorObserverActivated){
		graph->getProperty(inputData->getElementColorPropName())->removePropertyObserver(this);
		colorObserverActivated=false;
	}
}
}
