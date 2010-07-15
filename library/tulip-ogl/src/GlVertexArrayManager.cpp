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

namespace tlp
{
  GlVertexArrayManager::GlVertexArrayManager(GlGraphInputData *inputData):inputData(inputData),graph(inputData->getGraph()),isBegin(false),toCompute(true){
    colorInterpolate=inputData->parameters->isEdgeColorInterpolate();
  }

  GlVertexArrayManager::~GlVertexArrayManager(){
    clearObservers();
    clearData();
  }

  bool GlVertexArrayManager::haveToCompute(){
    if(toCompute)
      return true;
    if(inputData->parameters->isEdgeColorInterpolate()!=colorInterpolate){
      colorInterpolate=inputData->parameters->isEdgeColorInterpolate();
      return true;
    }

    return false;
  }

  void GlVertexArrayManager::beginRendering() {
    graph=inputData->getGraph();

    isBegin=true;
    linesRenderingIndexArray.clear();
    linesRenderingIndexCountArray.clear();
    linesSelectedRenderingIndexArray.clear();
    linesSelectedRenderingIndexCountArray.clear();
  }

  void GlVertexArrayManager::endRendering() {
    if(!isBegin)
      return;
    isBegin=false;

    glDisable(GL_LIGHTING);

    glEnableClientState(GL_VERTEX_ARRAY);


    glDisable(GL_DEPTH_TEST);
    glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedEdgesStencil(), 0xFFFF);
    Color selectionColor=inputData->parameters->getSelectionColor();
    glColor4ubv(((const GLubyte *)(&selectionColor)));
    glLineWidth(4);

    if(linesSelectedRenderingIndexArray.size()!=0){
      glVertexPointer(3, GL_FLOAT, 0, &linesCoordsArray.data()[0]);
      glMultiDrawElements(GL_LINE_STRIP, linesSelectedRenderingIndexCountArray.data(), GL_UNSIGNED_INT, (const void **)(linesSelectedRenderingIndexArray.data()),linesSelectedRenderingIndexArray.size());
    }

    glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_DEPTH_TEST);
    glStencilFunc(GL_LEQUAL, inputData->parameters->getEdgesStencil(), 0xFFFF);
    glLineWidth(1);

    if(linesRenderingIndexArray.size()!=0){
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, &linesColorsArray.data()[0]);
      glVertexPointer(3, GL_FLOAT, 0, &linesCoordsArray.data()[0]);

      glMultiDrawElements(GL_LINE_STRIP, linesRenderingIndexCountArray.data(), GL_UNSIGNED_INT, (const void **)(linesRenderingIndexArray.data()),linesRenderingIndexArray.size());
    }

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  void GlVertexArrayManager::addEdge(GlEdge *edge){
    size_t lastIndex=linesCoordsArray.size();
    edge->getVertices(inputData,linesCoordsArray);
    size_t numberOfVertices=linesCoordsArray.size()-lastIndex;
    vector<Coord> vertices;
    if(numberOfVertices!=0){
      linesIndexArray.push_back(new GLint[numberOfVertices]);
      GLint *index=linesIndexArray.back();
      for(size_t i=0;i<numberOfVertices;++i){
        vertices.push_back(linesCoordsArray[lastIndex+i]);
        index[i]=lastIndex+i;
      }
      edge->getColors(inputData,vertices,linesColorsArray);
      linesIndexCountArray.push_back(numberOfVertices);
    }else{
      linesIndexArray.push_back(NULL);
      linesIndexCountArray.push_back(0);
    }
    edgeToLineIndexHashMap[edge->id]=linesIndexArray.size()-1;
  }

  void GlVertexArrayManager::activateLineEdgeDisplay(GlEdge *edge,bool selected){
    size_t index=edgeToLineIndexHashMap[edge->id];
    if(!selected){
      linesRenderingIndexArray.push_back(linesIndexArray[index]);
      linesRenderingIndexCountArray.push_back(linesIndexCountArray[index]);
    }else{
      linesSelectedRenderingIndexArray.push_back(linesIndexArray[index]);
      linesSelectedRenderingIndexCountArray.push_back(linesIndexCountArray[index]);
    }
  }

  void GlVertexArrayManager::beforeSetNodeValue(PropertyInterface*, const node){
    clearData();
    clearObservers();
  }

  void GlVertexArrayManager::destroy(PropertyInterface*){
    clearData();
    clearObservers();
  }

  void GlVertexArrayManager::clearData() {
    toCompute=true;
    linesCoordsArray.clear();
    linesColorsArray.clear();
    for(vector<GLint*>::iterator it=linesIndexArray.begin();it!=linesIndexArray.end();++it)
      delete[] (*it);
    linesIndexArray.clear();
    linesIndexCountArray.clear();
    edgeToLineIndexHashMap.clear();
  }

  void GlVertexArrayManager::initObservers() {
    graph->getProperty(inputData->getElementLayoutPropName())->addPropertyObserver(this);
    graph->getProperty(inputData->getElementColorPropName())->addPropertyObserver(this);
  }

  void GlVertexArrayManager::clearObservers() {
    graph->getProperty(inputData->getElementLayoutPropName())->removePropertyObserver(this);
    graph->getProperty(inputData->getElementColorPropName())->removePropertyObserver(this);
  }
}
