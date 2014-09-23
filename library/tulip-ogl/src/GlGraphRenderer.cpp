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
#include <tulip/GlGraphRenderer.h>
#include <tulip/ForEach.h>
#include <tulip/GraphProperty.h>
#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlLODSceneVisitor.h>
#include <tulip/GlScene.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlVertexArrayVisitor.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>

using namespace std;

namespace tlp {

GlGraphRenderer::GlGraphRenderer(const GlGraphInputData *inputData):inputData(inputData),graphModified(true),selectionDrawActivate(false),selectionIdMap(NULL),selectionCurrentId(NULL) {
}

void GlGraphRenderer::visitGraph(GlSceneVisitor *visitor, bool visitHiddenEntities) {
  Graph *graph=inputData->getGraph();

  if(!graph)
    return;

  if(visitor->isThreadSafe()) {
#ifdef HAVE_OMP
    #pragma omp parallel
#endif
    {
#ifdef HAVE_OMP
      #pragma omp sections nowait
#endif
      {
        visitNodes(graph,visitor,visitHiddenEntities);
      }
#ifdef HAVE_OMP
      #pragma omp sections nowait
#endif
      {
        visitEdges(graph,visitor,visitHiddenEntities);
      }
    }
  }
  else {
    visitNodes(graph,visitor,visitHiddenEntities);
    visitEdges(graph,visitor,visitHiddenEntities);
  }
}

void GlGraphRenderer::visitNodes(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities) {
  if(inputData->parameters->isDisplayNodes()|| inputData->parameters->isViewNodeLabel() || inputData->parameters->isViewMetaLabel() || visitHiddenEntities) {
    visitor->reserveMemoryForNodes(graph->numberOfNodes());
    GlNode glNode(0);

    Iterator<node>* nodesIterator = graph->getNodes();

    while (nodesIterator->hasNext()) {
      node n=nodesIterator->next();
      glNode.id=n.id;
      glNode.acceptVisitor(visitor);
    }

    delete nodesIterator;
  }
}

void GlGraphRenderer::visitEdges(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities) {
  if(inputData->parameters->isDisplayEdges() || inputData->parameters->isViewEdgeLabel() || inputData->parameters->isViewMetaLabel() || visitHiddenEntities) {
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
}
