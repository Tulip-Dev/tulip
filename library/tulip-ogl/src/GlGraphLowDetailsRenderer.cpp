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
#include <tulip/GlGraphLowDetailsRenderer.h>
#include <tulip/ForEach.h>
#include <tulip/GraphProperty.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlLODSceneVisitor.h>
#include <tulip/GlScene.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlVertexArrayVisitor.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/OcclusionTest.h>
#include <tulip/OpenGlConfigManager.h>

using namespace std;

namespace tlp {

GlGraphLowDetailsRenderer::GlGraphLowDetailsRenderer(const GlGraphInputData *inputData):GlGraphRenderer(inputData),fakeScene(new GlScene),buildVBO(true) {
  fakeScene->createLayer("fakeLayer");
  addObservers();
}

GlGraphLowDetailsRenderer::~GlGraphLowDetailsRenderer() {
  delete fakeScene;
  removeObservers();
}
//====================================================
void GlGraphLowDetailsRenderer::initEdgesArray() {
  Graph *graph=inputData->getGraph();
  LayoutProperty *layout=inputData->getElementLayout();
  ColorProperty *color=inputData->getElementColor();

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
  //tlp::debug() << "nb lines = " << indices.size()/2 << endl;
  size_t i_point  = 0;
  size_t i_indices = 0;
  size_t i_col = 0;
  edge e;
  forEach(e, graph->getEdges()) {
    const pair<node, node> &ends = graph->ends(e);
    Color a = color->getEdgeValue(e);
    Color b = color->getEdgeValue(e);
    Vec4f ca, cb;

    for (size_t i=0; i < 4; ++i) {
      ca[i] = a[i];
      cb[i] = b[i];
    }

    indices[i_indices++] = i_point;
    colors[i_col++] = a;
    points[i_point][0] = layout->getNodeValue(ends.first)[0];
    points[i_point++][1] = layout->getNodeValue(ends.first)[1];

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
    points[i_point][0] = layout->getNodeValue(ends.second)[0];
    points[i_point++][1] = layout->getNodeValue(ends.second)[1];

  }
}
//====================================================
void GlGraphLowDetailsRenderer::initNodesArray() {
  Graph *graph=inputData->getGraph();
  LayoutProperty *layout=inputData->getElementLayout();
  ColorProperty *color=inputData->getElementColor();
  SizeProperty *size=inputData->getElementSize();

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

  if (!inputData->renderingParameters()->isAntialiased()) {
    OpenGlConfigManager::getInst().desactivateAntiAliasing();
  }

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

  OpenGlConfigManager::getInst().activateAntiAliasing();

}

void GlGraphLowDetailsRenderer::addObservers() {
  observedGraph = inputData->getGraph();
  observedGraph->addListener(this);
  observedLayoutProperty=inputData->getElementLayout();
  observedLayoutProperty->addListener(this);
  observedSizeProperty=inputData->getElementSize();
  observedSizeProperty->addListener(this);
  observedSelectionProperty=inputData->getElementSelected();
  observedSelectionProperty->addListener(this);
  observedColorProperty=inputData->getElementColor();
  observedColorProperty->addListener(this);
}

void GlGraphLowDetailsRenderer::removeObservers() {
  observedGraph->removeListener(this);
  observedLayoutProperty->removeListener(this);
  observedSizeProperty->removeListener(this);
  observedSelectionProperty->removeListener(this);
  observedColorProperty->removeListener(this);
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
      const PropertyInterface *property=inputData->getGraph()->getProperty(graphEvent->getPropertyName());

      if(property == inputData->getElementLayout()
          || property == inputData->getElementSize()
          || property == inputData->getElementColor()
          || property == inputData->getElementSelected()) {
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
