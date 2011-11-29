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
#include <tulip/GlGraphLowDetailsRenderer.h>

#include <iostream>

#include <tulip/ForEach.h>
#include <tulip/GraphProperty.h>

#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlLODSceneVisitor.h>
#include <tulip/GlScene.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlVertexArrayVisitor.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/OcclusionTest.h>

using namespace std;

namespace tlp {

GlGraphLowDetailsRenderer::GlGraphLowDetailsRenderer(Graph* graph, GlGraphInputData &inputData, GlGraphRenderingParameters &parameters):GlGraphRenderer(graph,inputData,parameters),buildVBO(true) {
  fakeScene = new GlScene;
  fakeScene->addLayer(new GlLayer("fakeLayer"));
  addObservers();
}

GlGraphLowDetailsRenderer::~GlGraphLowDetailsRenderer() {
  delete fakeScene;
  removeObservers();
}

void GlGraphLowDetailsRenderer::visitGraph(GlSceneVisitor *visitor, bool visitHiddenEntities) {
  Graph *graph=inputData.getGraph();

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

void GlGraphLowDetailsRenderer::visitNodes(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities) {
  if(parameters.isDisplayNodes() || parameters.isDisplayMetaNodes() || visitHiddenEntities) {
    visitor->reserveMemoryForNodes(graph->numberOfNodes());
    GlNode glNode(0);
    bool isMetaNode;

    Iterator<node>* nodesIterator = graph->getNodes();

    while (nodesIterator->hasNext()) {
      node n=nodesIterator->next();
      isMetaNode = inputData.getGraph()->isMetaNode(n);

      if((parameters.isDisplayNodes() && !isMetaNode) || (parameters.isDisplayMetaNodes() && isMetaNode) || visitHiddenEntities) {
        glNode.id=n.id;
        glNode.acceptVisitor(visitor);
      }
    }

    delete nodesIterator;
  }
}

void GlGraphLowDetailsRenderer::visitEdges(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities) {
  if(parameters.isDisplayEdges() || visitHiddenEntities) {
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
//====================================================
void GlGraphLowDetailsRenderer::initEdgesArray() {
  Graph *graph=inputData.getGraph();
  LayoutProperty *layout=graph->getProperty<LayoutProperty>(inputData.getElementLayoutPropName());
  ColorProperty *color=graph->getProperty<ColorProperty>(inputData.getElementColorPropName());

  size_t nbEdges = graph->numberOfEdges();
  size_t nbBends = 0;
  {
    edge e;
    forEach(e, graph->getEdges()) {
      nbBends += layout->getEdgeValue(e).size();
    }
  }
  points.resize(nbEdges*2 + nbBends); //todo: should be #|V| !!!
  indices.resize(nbEdges*2 + nbBends * 2);
  colors.resize(nbEdges*2 + nbBends);
  //cout << "nb lines = " << indices.size()/2 << endl;
  size_t i_point  = 0;
  size_t i_indices = 0;
  size_t i_col = 0;
  edge e;
  forEach(e, graph->getEdges()) {
    Color a = color->getNodeValue(graph->source(e));
    Color b = color->getNodeValue(graph->target(e));
    Vec4f ca, cb;

    for (size_t i=0; i < 4; ++i) {
      ca[i] = a[i];
      cb[i] = b[i];
    }

    indices[i_indices++] = i_point;
    colors[i_col++] = a;
    points[i_point][0] = layout->getNodeValue(graph->source(e))[0];
    points[i_point++][1] = layout->getNodeValue(graph->source(e))[1];

    vector<Coord> bends = layout->getEdgeValue(e);

    for (size_t j=0; j < bends.size(); ++j) {
      Vec4f tmp((ca - cb));
      tmp *= 1./(bends.size() + 2);
      tmp *= j+1;
      tmp += ca;
      colors[i_col++] = Color((int)tmp[0], (int)tmp[1], (int)tmp[2], (int)tmp[3]);
      indices[i_indices++] = i_point;
      indices[i_indices++] = i_point;
      points[i_point][0] = bends[j][0];
      points[i_point++][1] = bends[j][1];
    }

    indices[i_indices++] = i_point;
    colors[i_col++] = b;
    points[i_point][0] = layout->getNodeValue(graph->target(e))[0];
    points[i_point++][1] = layout->getNodeValue(graph->target(e))[1];

  }
}
//====================================================
void GlGraphLowDetailsRenderer::initNodesArray() {
  Graph *graph=inputData.getGraph();
  LayoutProperty *layout=graph->getProperty<LayoutProperty>(inputData.getElementLayoutPropName());
  ColorProperty *color=graph->getProperty<ColorProperty>(inputData.getElementColorPropName());
  SizeProperty *size=graph->getProperty<SizeProperty>(inputData.getElementSizePropName());

  size_t nbNodes= graph->numberOfNodes();
  quad_points.resize(nbNodes * 4);
  quad_indices.resize(nbNodes * 4);
  quad_colors.resize(nbNodes * 4);
  // i % x  i%3
  float tab1[4] = { -1,     1,  1, -1};
  float tab2[4] = { -1,     -1,  1, 1};



  size_t i_point = 0;
  size_t i_col = 0;
  size_t i_indices = 0;

  node n;
  forEach(n, graph->getNodes()) {
    Coord p ( layout->getNodeValue(n));
    Size  s ( size->getNodeValue(n) / 2.f);
    Color c ( color->getNodeValue(n));

    for (int i =0; i<4; ++i) {
      Vec3f a = p;
      a[0] += s[0] * tab1[i];//s[0] * i%2 ;  // 0 1 1 0
      a[1] += s[1] * tab2[i];//s[1] * i/2 %2;  // 0 0 1 1

      quad_colors[i_col++] = c;
      quad_indices[i_indices++] = i_point;
      quad_points[i_point][0] = a[0];
      quad_points[i_point++][1] = a[1];
    }
  }
}
//===================================================================
void GlGraphLowDetailsRenderer::draw(float ,Camera*) {

  if (buildVBO) {
    initEdgesArray();
    initNodesArray();
    buildVBO = false;
  }

  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(2, GL_FLOAT, 2*sizeof(GLfloat), &points[0]);
  glColorPointer(4, GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), &colors[0]);
  size_t cur = 0;

  while(cur< indices.size()) {
    if (indices.size() - cur > 64000) {
      glDrawElements(GL_LINES, 64000, GL_UNSIGNED_INT, &indices[cur]);
    }
    else
      glDrawElements(GL_LINES, indices.size() - cur, GL_UNSIGNED_INT, &indices[cur]);

    cur += 64000;
  }

  glDisable(GL_BLEND);

  glVertexPointer(2, GL_FLOAT, 2*sizeof(GLfloat), &quad_points[0]);
  glColorPointer(4, GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), &quad_colors[0]);
  cur = 0;

  while(cur< quad_indices.size()) {
    if (quad_indices.size() - cur > 64000) {
      glDrawElements(GL_QUADS, 64000, GL_UNSIGNED_INT, &quad_indices[cur]);
    }
    else
      glDrawElements(GL_QUADS, quad_indices.size() - cur, GL_UNSIGNED_INT, &quad_indices[cur]);

    cur += 64000;
  }

  //glDrawElements(GL_QUADS, quad_indices.size(), GL_UNSIGNED_INT, &quad_indices[0]);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void GlGraphLowDetailsRenderer::addObservers() {
  observedGraph = inputData.getGraph();
  observedGraph->addListener(this);
  observedLayoutProperty=observedGraph->getProperty<LayoutProperty>(inputData.getElementLayoutPropName());
  observedLayoutProperty->addPropertyObserver(this);
  observedSizeProperty=observedGraph->getProperty<SizeProperty>(inputData.getElementSizePropName());
  observedSizeProperty->addPropertyObserver(this);
  observedSelectionProperty=observedGraph->getProperty<BooleanProperty>(inputData.getElementSelectedPropName());
  observedSelectionProperty->addPropertyObserver(this);
  observedColorProperty=observedGraph->getProperty<ColorProperty>(inputData.getElementColorPropName());
  observedColorProperty->addPropertyObserver(this);
}

void GlGraphLowDetailsRenderer::removeObservers() {
  observedGraph->removeListener(this);
  observedLayoutProperty->removePropertyObserver(this);
  observedSizeProperty->removePropertyObserver(this);
  observedSelectionProperty->removePropertyObserver(this);
  observedColorProperty->removePropertyObserver(this);
}

void GlGraphLowDetailsRenderer::updateObservers() {
  removeObservers();
  addObservers();
}

void GlGraphLowDetailsRenderer::treatEvent(const Event &ev) {
  if (typeid(ev) == typeid(GraphEvent)) {
    const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&ev);

    switch(graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
    case GraphEvent::TLP_ADD_EDGE:
    case GraphEvent::TLP_DEL_NODE:
    case GraphEvent::TLP_DEL_EDGE:
      buildVBO=true;
      break;

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY: {
      const std::string name = graphEvent->getPropertyName();

      if(name == inputData.getElementLayoutPropName()
         || name == inputData.getElementSizePropName()
         || name == inputData.getElementColorPropName()
         || name == inputData.getElementSelectedPropName()) {
        buildVBO=true;
        updateObservers();
      }

      break;
    }

    default:
      break;
    }
  }
  else if(typeid(ev) == typeid(PropertyEvent)) {
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&ev);

    switch(propertyEvent->getType()) {
    case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:
      buildVBO=true;
      break;

    default:
      break;
    }
  }
  else if (ev.type()==Event::TLP_DELETE) {

    if (dynamic_cast<tlp::Graph*>(ev.sender())) {
      removeObservers();
    }
  }
}

}
