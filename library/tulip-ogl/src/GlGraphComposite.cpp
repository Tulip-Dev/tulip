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
#include <tulip/GlGraphComposite.h>
#include <tulip/ForEach.h>
#include <tulip/GraphProperty.h>
#include <tulip/GlXMLTools.h>
#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlLODSceneVisitor.h>
#include <tulip/GlScene.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlVertexArrayVisitor.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/OcclusionTest.h>
#include <tulip/GlGraphHighDetailsRenderer.h>

using namespace std;

namespace tlp {

GlGraphComposite::GlGraphComposite(Graph* graph, GlGraphRenderer *graphRenderer):inputData(graph,&parameters),graphRenderer(graphRenderer),nodesModified(true) {
  if(graphRenderer==NULL) {
    this->graphRenderer=new GlGraphHighDetailsRenderer(&inputData);
  }

  if(!graph) {
    rootGraph=NULL;
  }
  else {
    rootGraph=graph->getRoot();
    graph->addListener(this);
    graph->getRoot()->getProperty<GraphProperty>("viewMetaGraph")->addListener(this);

    Iterator<node>* nodesIterator = graph->getNodes();

    while (nodesIterator->hasNext()) {
      node n=nodesIterator->next();

      if(graph->getNodeMetaInfo(n))
        metaNodes.insert(n);
    }

    delete nodesIterator;
  }
}

GlGraphComposite::GlGraphComposite(Graph* graph, GlScene *scene):inputData(graph,&parameters),nodesModified(true) {
  this->graphRenderer=new GlGraphHighDetailsRenderer(&inputData,scene);

  if(!graph) {
    rootGraph=NULL;
  }
  else {
    rootGraph=graph->getRoot();
    graph->addListener(this);
    graph->getRoot()->getProperty<GraphProperty>("viewMetaGraph")->addListener(this);

    Iterator<node>* nodesIterator = graph->getNodes();

    while (nodesIterator->hasNext()) {
      node n=nodesIterator->next();

      if(graph->getNodeMetaInfo(n))
        metaNodes.insert(n);
    }

    delete nodesIterator;
  }
}

GlGraphComposite::~GlGraphComposite() {
  delete graphRenderer;
}

void GlGraphComposite::acceptVisitor(GlSceneVisitor *visitor) {
  GlBoundingBoxSceneVisitor bbVisitor(&inputData);
  graphRenderer->visitGraph(&bbVisitor);
  boundingBox=bbVisitor.getBoundingBox();

  if(boundingBox.isValid())
    visitor->visit(this);
}

void GlGraphComposite::acceptVisitorOnGraph(GlSceneVisitor *visitor) {
  graphRenderer->visitGraph(visitor);
}

//===================================================================
void GlGraphComposite::draw(float lod,Camera* camera) {
  graphRenderer->draw(lod, camera);
}
//===================================================================
void GlGraphComposite::selectEntities(Camera *camera,RenderingEntitiesFlag type, int x, int y, int w, int h, vector<SelectedEntity> &selectedEntities) {
  graphRenderer->selectEntities(camera, type, x, y, w, h, selectedEntities);
}
//===================================================================
const GlGraphRenderingParameters& GlGraphComposite::getRenderingParameters() {
  return parameters;
}
//===================================================================
void GlGraphComposite::setRenderingParameters(const GlGraphRenderingParameters &parameter) {
  if(parameters.isElementOrdered() != parameter.isElementOrdered()) {
    parameters = parameter;
    graphRenderer->setGraphModified(true);
  }
  else {
    parameters = parameter;
  }
}
//===================================================================
GlGraphRenderingParameters* GlGraphComposite::getRenderingParametersPointer() {
  return &parameters;
}
//===================================================================
GlGraphInputData* GlGraphComposite::getInputData() {
  return &inputData;
}
//====================================================
void GlGraphComposite::getXML(string &outString) {
  GlXMLTools::createProperty(outString, "type", "GlGraphComposite","GlEntity");
}
//====================================================
void GlGraphComposite::setWithXML(const string &, unsigned int&) {
}

void GlGraphComposite::treatEvent(const Event& evt) {
  const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

  if (graphEvent) {
    switch(graphEvent->getType()) {

    case GraphEvent::TLP_ADD_NODE:
    case GraphEvent::TLP_DEL_NODE:
      nodesModified=true;
      graphRenderer->setGraphModified(true);
      break;

    case GraphEvent::TLP_ADD_EDGE:
    case GraphEvent::TLP_DEL_EDGE:
    case GraphEvent::TLP_REVERSE_EDGE:
    case GraphEvent::TLP_AFTER_SET_ENDS:
      graphRenderer->setGraphModified(true);
      break;

    default:
      break;
    }
  }
  else if(evt.type() == Event::TLP_DELETE) {
    Graph* g = dynamic_cast<Graph*>(evt.sender());

    if(g && inputData.getGraph() == g) {
      inputData.graph = NULL;
    }
  }
  else {
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&evt);

    if(propertyEvent && propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_NODE_VALUE) {
      nodesModified=true;
    }
  }
}

void GlGraphComposite::setRenderer(tlp::GlGraphRenderer* renderer) {
  delete graphRenderer;

  if(renderer == NULL) {
    graphRenderer = new GlGraphHighDetailsRenderer(&inputData);
  }
  else {
    graphRenderer = renderer;
  }

}

}


