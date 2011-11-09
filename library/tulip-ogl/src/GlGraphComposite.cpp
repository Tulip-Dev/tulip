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
#include <tulip/GlGraphComposite.h>

#include <iostream>

#include <tulip/ForEach.h>
#include <tulip/GraphProperty.h>

#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>

using namespace std;

namespace tlp {

GlGraphComposite::GlGraphComposite(Graph* graph):inputData(graph,&parameters),rootGraph(graph->getRoot()),haveToSort(true),nodesModified(true) {
  graph->addListener(this);
  graph->getRoot()->getProperty<GraphProperty>("viewMetaGraph")->addPropertyObserver(this);

  Iterator<node>* nodesIterator = graph->getNodes();

  while (nodesIterator->hasNext()) {
    node n=nodesIterator->next();

    if(graph->getNodeMetaInfo(n))
      metaNodes.insert(n);
  }

  delete nodesIterator;
}

void GlGraphComposite::acceptVisitorForNodes(Graph *graph,GlSceneVisitor *visitor) {
  if(isDisplayNodes() || isDisplayMetaNodes()) {
    visitor->reserveMemoryForNodes(graph->numberOfNodes());
    GlNode glNode(0);
    bool isMetaNode;

    Iterator<node>* nodesIterator = graph->getNodes();

    while (nodesIterator->hasNext()) {
      node n=nodesIterator->next();
      isMetaNode = inputData.getGraph()->isMetaNode(n);

      if((isDisplayNodes() && !isMetaNode) || (isDisplayMetaNodes() && isMetaNode)) {
        glNode.id=n.id;
        glNode.acceptVisitor(visitor);
      }
    }

    delete nodesIterator;
  }
}

void GlGraphComposite::acceptVisitorForEdges(Graph *graph,GlSceneVisitor *visitor) {
  if(isDisplayEdges()) {
    visitor->reserveMemoryForEdges(graph->numberOfEdges());

    GlEdge glEdge(0);
    Iterator<edge>* edgesIterator = graph->getEdges();

    while (edgesIterator->hasNext()) {
      glEdge.id=edgesIterator->next().id;
      glEdge.acceptVisitor(visitor);
    }

    delete edgesIterator;
  }
}

void GlGraphComposite::acceptVisitor(GlSceneVisitor *visitor) {
  Graph *graph=inputData.getGraph();

  if(!graph)
    return;

  // Check if the current graph are in the hierarchy
  assert((rootGraph==graph) || (rootGraph->isDescendantGraph(graph)));

  if(visitor->isThreadSafe()) {
#ifdef HAVE_OMP
    #pragma omp parallel
#endif
    {
#ifdef HAVE_OMP
      #pragma omp sections nowait
#endif
      {
        acceptVisitorForNodes(graph,visitor);
      }
#ifdef HAVE_OMP
      #pragma omp sections nowait
#endif
      {
        acceptVisitorForEdges(graph,visitor);
      }
    }
  }
  else {
    acceptVisitorForNodes(graph,visitor);
    acceptVisitorForEdges(graph,visitor);
  }
}
//===================================================================
const GlGraphRenderingParameters& GlGraphComposite::getRenderingParameters() {
  return parameters;
}
//===================================================================
void GlGraphComposite::setRenderingParameters(const GlGraphRenderingParameters &parameter) {
  if(parameters.isElementOrdered() != parameter.isElementOrdered()) {
    parameters = parameter;
    haveToSort=true;
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
      nodesModified=true;
      haveToSort=true;
      break;

    case GraphEvent::TLP_DEL_NODE:
      nodesModified=true;
      haveToSort=true;
      break;

    case GraphEvent::TLP_ADD_EDGE:
      haveToSort=true;
      break;

    case GraphEvent::TLP_DEL_EDGE:
      haveToSort=true;
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

}
