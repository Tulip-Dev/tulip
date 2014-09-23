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

#include <QCursor>
#include <QEvent>
#include <QThread>
#include <QApplication>
#include <QTimeLine>
#include <QMouseEvent>

#include <tulip/TulipPluginHeaders.h>
#include <tulip/MouseInteractors.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainView.h>
#include <tulip/ForEach.h>
#include <tulip/GlCircle.h>
#include <tulip/OcclusionTest.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlRect.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/Rectangle.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GlLayer.h>

#include "NeighborhoodHighlighterInteractor.h"
#include "../../utils/ViewNames.h"

#include <algorithm>
#include <sstream>

using namespace std;

namespace tlp {

class GraphCompositeDrawVisitor : public GlSceneVisitor {

public :

  GraphCompositeDrawVisitor(GlGraphInputData *inputData, Camera *camera) : inputData(inputData), camera(camera), drawLabelMode(false) {}

  void visit(GlNode *glNode) {
    if (!drawLabelMode) {
      glNode->draw(30, inputData, camera);
    }
    else {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glDisable(GL_LIGHTING);
      glDepthFunc(GL_ALWAYS );
      glDisable(GL_CULL_FACE);
      glDisable(GL_COLOR_MATERIAL);
      glNode->drawLabel(&occlusionTest, inputData);
      glPopAttrib();
    }
  }

  void visit(GlEdge *glEdge) {
    if (!drawLabelMode) {
      glEdge->draw(30, inputData, camera);
    }
    else {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glDisable(GL_LIGHTING);
      glDepthFunc(GL_ALWAYS );
      glDisable(GL_CULL_FACE);
      glDisable(GL_COLOR_MATERIAL);
      glEdge->drawLabel(&occlusionTest, inputData);
      glPopAttrib();
    }
  }

  void setDrawLabelMode(const bool drawLabel) {
    drawLabelMode = drawLabel;
  }

private :

  GlGraphInputData *inputData;
  Camera *camera;
  OcclusionTest occlusionTest;
  bool drawLabelMode;

};

class GraphLayoutMorphing : public AdditionalGlSceneAnimation {

public :
  virtual ~GraphLayoutMorphing() {}
  GraphLayoutMorphing(Graph *graph, LayoutProperty *srcLayout, LayoutProperty *destLayout, LayoutProperty *viewLayout) :
    graph(graph), srcLayout(srcLayout), destLayout(destLayout), viewLayout(viewLayout) {}

  void animationStep(int animationStep) {
    node n;
    forEach(n, graph->getNodes()) {
      const Coord &startPos = srcLayout->getNodeValue(n);
      const Coord &endPos = destLayout->getNodeValue(n);
      viewLayout->setNodeValue(n, startPos + (animationStep / (float) nbAnimationSteps) * (endPos - startPos));
    }

    edge e;
    forEach(e, graph->getEdges()) {
      const vector<Coord> &srcBends = srcLayout->getEdgeValue(e);
      const vector<Coord> &destBends = destLayout->getEdgeValue(e);
      vector<Coord> newBends;

      for (size_t i = 0 ; i < destBends.size() ; ++i) {
        newBends.push_back(srcBends[i] + (animationStep / (float) nbAnimationSteps) * (destBends[i] - srcBends[i]));
      }

      viewLayout->setEdgeValue(e, newBends);
    }
  }

private :

  Graph *graph;
  LayoutProperty *srcLayout, *destLayout, *viewLayout;
};

class NeighborNodesEdgeLengthOrdering : public binary_function<node, node, bool> {

public :

  NeighborNodesEdgeLengthOrdering(node centralNode, LayoutProperty *layout) : centralNode(centralNode), layout(layout) {}

  bool operator()(tlp::node n1, tlp::node n2) const {
    Coord centralNodeCoord = layout->getNodeValue(centralNode);
    Coord n1Coord = layout->getNodeValue(n1);
    Coord n2Coord = layout->getNodeValue(n2);
    float centralToN1Dist = centralNodeCoord.dist(n1Coord);
    float centralToN2Dist = centralNodeCoord.dist(n2Coord);
    return centralToN1Dist < centralToN2Dist;
  }

private :

  node centralNode;
  LayoutProperty *layout;
};

class MouseEventDiscardFilter : public QObject {

public :

  bool eventFilter(QObject*, QEvent *event) {
    if (event->type() == QEvent::MouseMove ||
        event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease) {
      return true;
    }

    return false;
  }

};

const unsigned char maxCircleAlphaValue = 200;

PLUGIN(NeighborhoodHighlighterInteractor)

NeighborhoodHighlighterInteractor::NeighborhoodHighlighterInteractor(const PluginContext *) : GLInteractorComposite(QIcon(":/i_neighborhood_highlighter.png"), "Highlight node neighborhood"), configWidget(NULL) {}

NeighborhoodHighlighterInteractor::~NeighborhoodHighlighterInteractor() {
  delete configWidget;
}

bool NeighborhoodHighlighterInteractor::isCompatible(const std::string &viewName) const {
  return ((viewName==NodeLinkDiagramComponent::viewName)
          ||(viewName==ViewName::HistogramViewName)
          ||(viewName==ViewName::MatrixViewName)
          ||(viewName==ViewName::ScatterPlot2DViewName)
         );
}

void NeighborhoodHighlighterInteractor::construct() {
  configWidget = new NeighborhoodHighlighterConfigWidget();

  push_back(new MousePanNZoomNavigator());
  NeighborhoodHighlighter *neighborhoodHighlighter = new NeighborhoodHighlighter();
  neighborhoodHighlighter->setConfigWidget(configWidget);
  push_back(neighborhoodHighlighter);
}

NeighborhoodHighlighter::NeighborhoodHighlighter() :
  originalGraph(NULL), originalGlGraphComposite(NULL),
  neighborhoodGraph(NULL), glNeighborhoodGraph(NULL), glNeighborhoodCamera(NULL),
  neighborhoodGraphLayout(NULL), neighborhoodGraphCircleLayout(NULL),
  neighborhoodGraphOriginalLayout(NULL),neighborhoodGraphColors(NULL),
  neighborhoodGraphBackupColors(NULL), centralNodeLocked(false),
  circleLayoutSet(false), glWidget(NULL), neighborhoodDist(1),
  configWidget(NULL), circleAlphaValue(maxCircleAlphaValue),
  startAlpha(0), endAlpha(255), nbAnimSteps(0) {
}

NeighborhoodHighlighter::NeighborhoodHighlighter(const NeighborhoodHighlighter &neighborhoodHighlighter) : neighborhoodGraph(NULL), glNeighborhoodGraph(NULL), glNeighborhoodCamera(NULL), neighborhoodGraphLayout(NULL), neighborhoodGraphCircleLayout(NULL),
  neighborhoodGraphOriginalLayout(NULL), neighborhoodGraphColors(NULL), neighborhoodGraphBackupColors(NULL),
  centralNodeLocked(false), circleLayoutSet(false), neighborhoodDist(1), circleAlphaValue(maxCircleAlphaValue) {
  configWidget = neighborhoodHighlighter.configWidget;
  connect(configWidget, SIGNAL(updateNeighborhoodGraph()), this, SLOT(updateNeighborhoodGraph()));
}

NeighborhoodHighlighter::~NeighborhoodHighlighter() {
  cleanupNeighborhoodGraph();
}

void NeighborhoodHighlighter::viewChanged(View *view) {
  originalGraph = NULL;

  if (view == NULL) {
    glWidget = NULL;
    return;
  }

  GlMainView *glView = (GlMainView *) view;
  glWidget = glView->getGlMainWidget();
}

bool NeighborhoodHighlighter::eventFilter(QObject*, QEvent *e) {
  if(originalGraph==NULL) {
    originalGraph = glWidget->getScene()->getGlGraphComposite()->getGraph();
    originalGlGraphComposite = glWidget->getScene()->getGlGraphComposite();

    if(!glWidget->hasMouseTracking()) {
      glWidget->setMouseTracking(true);
    }

    Iterator<string>* it = originalGraph->getProperties();

    while(it->hasNext()) {
      string propertyName = it->next();
      PropertyInterface* property = originalGraph->getProperty(propertyName);

      if(property->getTypename() == "double") {
        configWidget->setPropertyToUse(propertyName);
      }
    }

    delete it;
  }

  if(glWidget->getScene()->getGlGraphComposite()->getGraph() != originalGraph) {
    centralNodeLocked = false;
    circleLayoutSet = false;
    cleanupNeighborhoodGraph();
    originalGraph = glWidget->getScene()->getGlGraphComposite()->getGraph();
    originalGlGraphComposite = glWidget->getScene()->getGlGraphComposite();
  }

  SelectedEntity selectedEntity;

  if (e->type() == QEvent::Wheel && centralNodeLocked && !circleLayoutSet) {
    QWheelEvent *we = (QWheelEvent *)e;

    if (selectInAugmentedDisplayGraph(we->x(), we->y(), selectedEntity) && selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
      if (selectedEntity.getComplexEntityId() == neighborhoodGraphCentralNode.id) {
        int numDegrees = we->delta() / 8;
        int numSteps = numDegrees / 15;
        neighborhoodDist += numSteps;

        if (neighborhoodDist < 1) {
          neighborhoodDist = 1;
        }

        configWidget->setCurrentMaxDistanceForReachableNodes(neighborhoodDist);
        neighborhoodGraph->updateWithDistance(neighborhoodDist);
        updateNeighborhoodGraphLayoutAndColors();
        delete glNeighborhoodGraph;
        updateGlNeighborhoodGraph();
        glWidget->redraw();
        return true;
      }
    }

  }
  else if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = (QMouseEvent *) e;

    if (!centralNodeLocked) {
      node tmpNode = selectNodeInOriginalGraph(glWidget, qMouseEv->x(), qMouseEv->y());

      if (tmpNode != selectedNode) {
        buildNeighborhoodGraph(tmpNode, originalGraph);
        selectedNode = tmpNode;
      }
    }
    else {
      *neighborhoodGraphColors = *neighborhoodGraphBackupColors;

      if (selectInAugmentedDisplayGraph(qMouseEv->x(), qMouseEv->y(), selectedEntity) && selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        if (selectedEntity.getComplexEntityId() != neighborhoodGraphCentralNode.id) {
          neighborhoodGraphColors->setNodeValue(node(selectedEntity.getComplexEntityId()), Color(0,255,0));
        }
        else {
          neighborhoodGraphColors->setNodeValue(node(selectedEntity.getComplexEntityId()), Color(0,0,255));
        }
      }

      if(selectedEntity.getEntityType()==SelectedEntity::NODE_SELECTED)
        selectedNeighborNode = node(selectedEntity.getComplexEntityId());
      else
        selectedNeighborNode = node();
    }

    glWidget->redraw();
    return true;
  }
  else if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->buttons()==Qt::LeftButton) {
    if (neighborhoodGraphCentralNode.isValid() && !centralNodeLocked) {
      centralNodeLocked = true;
    }
    else if (neighborhoodGraphCentralNode.isValid() && centralNodeLocked &&
             !circleLayoutSet && selectedNeighborNode.isValid() &&
             selectedNeighborNode == neighborhoodGraphCentralNode && neighborhoodDist == 1) {
      computeNeighborhoodGraphCircleLayout();
      computeNeighborhoodGraphBoundingBoxes();
      GraphLayoutMorphing *layoutMorphing = new GraphLayoutMorphing(neighborhoodGraph, neighborhoodGraphOriginalLayout, neighborhoodGraphCircleLayout, neighborhoodGraphLayout);
      performZoomAndPan(neighborhoodGraphCircleLayoutBB, layoutMorphing);
      delete layoutMorphing;
      circleLayoutSet = true;
    }
    else if (neighborhoodGraphCentralNode.isValid() && circleLayoutSet && !selectedNeighborNode.isValid()) {
      circleLayoutSet = false;
      centralNodeLocked = false;
      neighborhoodGraphCentralNode = node();
      neighborhoodDist = 1;
      configWidget->setCurrentMaxDistanceForReachableNodes(neighborhoodDist);
      glWidget->redraw();
    }
    else if (selectedNeighborNode.isValid()) {
      if (circleLayoutSet) {
        if (selectedNeighborNode == neighborhoodGraphCentralNode) {
          GraphLayoutMorphing *layoutMorphing = new GraphLayoutMorphing(neighborhoodGraph, neighborhoodGraphCircleLayout, neighborhoodGraphOriginalLayout, neighborhoodGraphLayout);
          performZoomAndPan(neighborhoodGraphOriginalLayoutBB, layoutMorphing);
          delete layoutMorphing;
          circleLayoutSet = false;
        }
        else {
          node currentCentralNode = neighborhoodGraphCentralNode;
          buildNeighborhoodGraph(selectedNeighborNode, originalGraph);
          computeNeighborhoodGraphCircleLayout();
          computeNeighborhoodGraphBoundingBoxes();
          BoundingBox destBB;

          if (configWidget->bringAndGoAnimation1()) {
            destBB = neighborhoodGraphOriginalLayoutBB;
          }
          else {
            destBB = neighborhoodGraphCircleLayoutBB;
          }

          buildNeighborhoodGraph(currentCentralNode, originalGraph);
          computeNeighborhoodGraphCircleLayout();
          computeNeighborhoodGraphBoundingBoxes();
          *neighborhoodGraphLayout = *neighborhoodGraphCircleLayout;
          GraphLayoutMorphing *layoutMorphing = new GraphLayoutMorphing(neighborhoodGraph, neighborhoodGraphCircleLayout, neighborhoodGraphOriginalLayout, neighborhoodGraphLayout);
          performZoomAndPan(destBB, layoutMorphing);
          delete layoutMorphing;

          if (configWidget->bringAndGoAnimation1()) {
            morphCircleAlpha(maxCircleAlphaValue, 0);
          }

          buildNeighborhoodGraph(selectedNeighborNode, originalGraph);
          computeNeighborhoodGraphCircleLayout();
          computeNeighborhoodGraphBoundingBoxes();

          if (configWidget->bringAndGoAnimation1()) {
            morphCircleAlpha(0, maxCircleAlphaValue);
          }

          layoutMorphing = new GraphLayoutMorphing(neighborhoodGraph, neighborhoodGraphOriginalLayout, neighborhoodGraphCircleLayout, neighborhoodGraphLayout);
          performZoomAndPan(neighborhoodGraphCircleLayoutBB, layoutMorphing);
          delete layoutMorphing;
        }
      }
      else if (selectedNeighborNode != neighborhoodGraphCentralNode) {
        neighborhoodDist = 1;
        configWidget->setCurrentMaxDistanceForReachableNodes(neighborhoodDist);
        buildNeighborhoodGraph(selectedNeighborNode, originalGraph);
        glWidget->redraw();
      }
    }
    else {
      centralNodeLocked = false;
      circleLayoutSet = false;
      neighborhoodDist = 1;
      configWidget->setCurrentMaxDistanceForReachableNodes(neighborhoodDist);
      neighborhoodGraphCentralNode = node();
      glWidget->redraw();
    }

    return true;
  }

  return false;
}
node NeighborhoodHighlighter::selectNodeInOriginalGraph(GlMainWidget *glWidget, int x, int y) {
  node n;
  glWidget->makeCurrent();
  vector<SelectedEntity> selectedElements;
  glWidget->getScene()->selectEntities((RenderingEntitiesFlag)(RenderingNodes | RenderingWithoutRemove), x-1, y-1, 3, 3, NULL, selectedElements);

  if(!selectedElements.empty()) {
    n=node(selectedElements[0].getComplexEntityId());
  }

  return n;
}

bool NeighborhoodHighlighter::selectInAugmentedDisplayGraph(const int x, const int y, SelectedEntity &selectedEntity) {
  GlLayer *l = glWidget->getScene()->getLayer("Main");
  GlGraphComposite *graphComposite = (GlGraphComposite *) l->findGlEntity("graph");
  l->deleteGlEntity("graph");
  l->addGlEntity(glNeighborhoodGraph, "graph");
  bool ret = glWidget->pickNodesEdges(x, y, selectedEntity);
  l->deleteGlEntity("graph");
  l->addGlEntity(graphComposite, "graph");
  return ret;
}

void NeighborhoodHighlighter::performZoomAndPan(const BoundingBox &destBB, AdditionalGlSceneAnimation *additionalAnimation) {
  MouseEventDiscardFilter medf;
  glWidget->installEventFilter(&medf);
  QtGlSceneZoomAndPanAnimator sceneZoomAndPan(glWidget, destBB);

  if (additionalAnimation != NULL) {
    sceneZoomAndPan.setAdditionalGlSceneAnimation(additionalAnimation);
  }

  sceneZoomAndPan.animateZoomAndPan();
  glWidget->removeEventFilter(&medf);
}

void NeighborhoodHighlighter::cleanupNeighborhoodGraph() {
  delete glNeighborhoodGraph;
  glNeighborhoodGraph = NULL;
  delete neighborhoodGraph;
  neighborhoodGraph = NULL;
  delete neighborhoodGraphLayout;
  neighborhoodGraphLayout = NULL;
  delete neighborhoodGraphCircleLayout;
  neighborhoodGraphCircleLayout = NULL;
  delete neighborhoodGraphOriginalLayout;
  neighborhoodGraphOriginalLayout = NULL;
  delete neighborhoodGraphColors;
  neighborhoodGraphColors = NULL;
  delete neighborhoodGraphBackupColors;
  neighborhoodGraphBackupColors = NULL;
  delete glNeighborhoodCamera;
  glNeighborhoodCamera = NULL;
}

void NeighborhoodHighlighter::buildNeighborhoodGraph(node n, Graph *g) {

  neighborhoodGraphCentralNode = node();

  cleanupNeighborhoodGraph();

  if (n.isValid()) {

    neighborhoodGraphCentralNode = n;

    neighborhoodGraph = new NodeNeighborhoodView(g, n, configWidget->getNeighborsType(), neighborhoodDist, configWidget->computeReachableSubGraph(), configWidget->propertyToUse(), configWidget->numberOfNodesToBring());

    neighborhoodGraphLayout = new LayoutProperty(neighborhoodGraph);
    neighborhoodGraphCircleLayout = new LayoutProperty(neighborhoodGraph);
    neighborhoodGraphOriginalLayout = new LayoutProperty(neighborhoodGraph);
    neighborhoodGraphColors = new ColorProperty(neighborhoodGraph);
    neighborhoodGraphBackupColors = new ColorProperty(neighborhoodGraph);

    updateNeighborhoodGraphLayoutAndColors();
    updateGlNeighborhoodGraph();

    circleCenter = neighborhoodGraphLayout->getNodeValue(n);
  }
}

void NeighborhoodHighlighter::updateNeighborhoodGraphLayoutAndColors() {
  if(originalGlGraphComposite != NULL) {
    GlGraphInputData *originalInputData = originalGlGraphComposite->getInputData();
    LayoutProperty *origGraphLayout = originalInputData->getElementLayout();
    ColorProperty *origGraphColors = originalInputData->getElementColor();

    node n2;
    forEach(n2, neighborhoodGraph->getNodes()) {
      neighborhoodGraphOriginalLayout->setNodeValue(n2, origGraphLayout->getNodeValue(n2));
      neighborhoodGraphBackupColors->setNodeValue(n2, origGraphColors->getNodeValue(n2));
    }

    edge e;
    forEach(e, neighborhoodGraph->getEdges()) {
      neighborhoodGraphOriginalLayout->setEdgeValue(e, origGraphLayout->getEdgeValue(e));
      neighborhoodGraphBackupColors->setEdgeValue(e, origGraphColors->getEdgeValue(e));
    }

    *neighborhoodGraphLayout = *neighborhoodGraphOriginalLayout;
    *neighborhoodGraphColors = *neighborhoodGraphBackupColors;
  }
}

void NeighborhoodHighlighter::updateGlNeighborhoodGraph() {
  GlGraphInputData *originalInputData = originalGlGraphComposite->getInputData();
  glNeighborhoodGraph = new GlGraphComposite(neighborhoodGraph);
  GlGraphInputData *glNeighborhoodGraphInputData = glNeighborhoodGraph->getInputData();
  glNeighborhoodGraphInputData->setElementBorderColor(originalInputData->getElementBorderColor());
  glNeighborhoodGraphInputData->setElementBorderWidth(originalInputData->getElementBorderWidth());
  glNeighborhoodGraphInputData->setElementColor(neighborhoodGraphColors);
  glNeighborhoodGraphInputData->setElementLabel(originalInputData->getElementLabel());
  glNeighborhoodGraphInputData->setElementLabelColor(originalInputData->getElementLabelColor());
  glNeighborhoodGraphInputData->setElementLabelBorderColor(originalInputData->getElementLabelBorderColor());
  glNeighborhoodGraphInputData->setElementLabelPosition(originalInputData->getElementLabelPosition());
  glNeighborhoodGraphInputData->setElementLayout(neighborhoodGraphLayout);
  glNeighborhoodGraphInputData->setElementRotation(originalInputData->getElementRotation());
  glNeighborhoodGraphInputData->setElementSelected(originalInputData->getElementSelected());
  glNeighborhoodGraphInputData->setElementShape(originalInputData->getElementShape());
  glNeighborhoodGraphInputData->setElementSize(originalInputData->getElementSize());
  glNeighborhoodGraphInputData->setElementTexture(originalInputData->getElementTexture());
  glNeighborhoodGraphInputData->setElementFont(originalInputData->getElementFont());
  glNeighborhoodGraphInputData->setElementFontSize(originalInputData->getElementFontSize());
  glNeighborhoodGraphInputData->setElementSrcAnchorShape(originalInputData->getElementSrcAnchorShape());
  glNeighborhoodGraphInputData->setElementSrcAnchorSize(originalInputData->getElementSrcAnchorSize());
  glNeighborhoodGraphInputData->setElementTgtAnchorShape(originalInputData->getElementTgtAnchorShape());
  glNeighborhoodGraphInputData->setElementTgtAnchorSize(originalInputData->getElementTgtAnchorSize());
}

void NeighborhoodHighlighter::updateNeighborhoodGraph() {
  if (neighborhoodGraphCentralNode.isValid()) {
    buildNeighborhoodGraph(neighborhoodGraphCentralNode, originalGraph);
    glWidget->redraw();
  }
}

void NeighborhoodHighlighter::computeNeighborhoodGraphBoundingBoxes() {
  Coord centralNodeCoord = neighborhoodGraphOriginalLayout->getNodeValue(neighborhoodGraphCentralNode);
  float originalLayoutRadius = computeNeighborhoodGraphRadius(neighborhoodGraphOriginalLayout);
  float circleLayoutRadius = computeNeighborhoodGraphRadius(neighborhoodGraphCircleLayout);
  neighborhoodGraphOriginalLayoutBB[0] = centralNodeCoord - Coord(originalLayoutRadius, originalLayoutRadius);
  neighborhoodGraphOriginalLayoutBB[1] = centralNodeCoord + Coord(originalLayoutRadius, originalLayoutRadius);
  neighborhoodGraphCircleLayoutBB[0] = centralNodeCoord - Coord(circleLayoutRadius, circleLayoutRadius);
  neighborhoodGraphCircleLayoutBB[1] = centralNodeCoord + Coord(circleLayoutRadius, circleLayoutRadius);
}

bool boundingBoxesIntersect(const BoundingBox &bb1, const BoundingBox &bb2)  {
  return Rectangle<float>(bb1).intersect(Rectangle<float>(bb2));
}


void NeighborhoodHighlighter::computeNeighborhoodGraphCircleLayout() {

  Size centralNodeSize = originalGlGraphComposite->getInputData()->getElementSize()->getNodeValue(neighborhoodGraphCentralNode);
  Coord centralNodeCoord = neighborhoodGraphLayout->getNodeValue(neighborhoodGraphCentralNode);
  neighborhoodGraphCircleLayout->setNodeValue(neighborhoodGraphCentralNode, centralNodeCoord);

  vector<node> neighborsNodes;
  node n;
  forEach(n, neighborhoodGraph->getNodes()) {
    if (n != neighborhoodGraphCentralNode) {
      neighborsNodes.push_back(n);
    }
  }

  sort(neighborsNodes.begin(), neighborsNodes.end(), NeighborNodesEdgeLengthOrdering(neighborhoodGraphCentralNode, neighborhoodGraphLayout));

  BoundingBox centralNodeBB(Coord(centralNodeCoord.getX() - centralNodeSize.getW() / 2, centralNodeCoord.getY() - centralNodeSize.getH() / 2, 0),
                            Coord(centralNodeCoord.getX() + centralNodeSize.getW() / 2, centralNodeCoord.getY() + centralNodeSize.getH() / 2, 0));
  vector<BoundingBox> neighborhoodNodesNewLayoutBB;
  neighborhoodNodesNewLayoutBB.resize(neighborsNodes.size());

  for (size_t i = 0 ; i < neighborsNodes.size() ; ++i) {

    Size neighborNodeSize = originalGlGraphComposite->getInputData()->getElementSize()->getNodeValue(neighborsNodes[i]);
    Coord neighborNodeCoord = neighborhoodGraphLayout->getNodeValue(neighborsNodes[i]);
    float edgeLength = centralNodeCoord.dist(neighborNodeCoord);

    bool nodePosOk = false;
    unsigned int j = 1;

    while (!nodePosOk) {
      float ratio = ((centralNodeSize.getW() / 2) + j++ * neighborNodeSize.getW()) / edgeLength;
      Coord newNodeCoord = centralNodeCoord + ratio * (neighborNodeCoord - centralNodeCoord);
      neighborhoodNodesNewLayoutBB[i] = BoundingBox(Coord(newNodeCoord.getX() - neighborNodeSize.getW() / 2, newNodeCoord.getY() - neighborNodeSize.getH() / 2, 0),
                                        Coord(newNodeCoord.getX() + neighborNodeSize.getW() / 2, newNodeCoord.getY() + neighborNodeSize.getH() / 2, 0));
      nodePosOk = true;

      if (i > 0) {
        for (unsigned int k = 0 ; k < i ; ++k) {
          nodePosOk = nodePosOk && !boundingBoxesIntersect(neighborhoodNodesNewLayoutBB[i],neighborhoodNodesNewLayoutBB[k]) && !boundingBoxesIntersect(neighborhoodNodesNewLayoutBB[i], centralNodeBB);
        }
      }

      if (nodePosOk) {
        neighborhoodGraphCircleLayout->setNodeValue(neighborsNodes[i], newNodeCoord);
      }
    }
  }

  edge e;
  forEach(e, neighborhoodGraph->getEdges()) {
    const std::pair<node, node>& eEnds = neighborhoodGraph->ends(e);
    node srcNode = eEnds.first;
    node tgtNode = eEnds.second;
    Coord finalBendsCoord;

    if (srcNode != neighborhoodGraphCentralNode) {
      Coord srcNodeCoord = neighborhoodGraphCircleLayout->getNodeValue(srcNode);
      finalBendsCoord = circleCenter + (srcNodeCoord - circleCenter) / 2.f;
    }
    else {
      Coord tgtNodeCoord = neighborhoodGraphCircleLayout->getNodeValue(tgtNode);
      finalBendsCoord = circleCenter + (tgtNodeCoord - circleCenter) / 2.f;
    }

    vector<Coord> edgesBends = neighborhoodGraphLayout->getEdgeValue(e);

    for (size_t i = 0 ; i < edgesBends.size() ; ++i) {
      edgesBends[i] = finalBendsCoord;
    }

    neighborhoodGraphCircleLayout->setEdgeValue(e, edgesBends);
  }
}


void NeighborhoodHighlighter::morphCircleAlpha(unsigned char startA, unsigned endA, int nbAnimationSteps) {
  startAlpha = startA;
  endAlpha = endA;
  nbAnimSteps = nbAnimationSteps;
  QTimeLine timeLine(500);
  timeLine.setFrameRange(0, nbAnimSteps);
  connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(morphCircleAlphaAnimStep(int)));
  timeLine.start();

  while (timeLine.state() != QTimeLine::NotRunning) {
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
  }
}

void NeighborhoodHighlighter::morphCircleAlphaAnimStep(int animStep) {
  circleAlphaValue = (unsigned char) (startAlpha + animStep/(float)nbAnimSteps * (endAlpha - startAlpha));
  glWidget->redraw();
}

float NeighborhoodHighlighter::computeNeighborhoodGraphRadius(LayoutProperty *neighborhoodGraphLayoutProp) {
  float radius = 0;
  node n;
  Coord centralNodeCoord = neighborhoodGraphLayoutProp->getNodeValue(neighborhoodGraphCentralNode);
  forEach(n, neighborhoodGraph->getNodes()) {
    Coord nodeCoord = neighborhoodGraphLayoutProp->getNodeValue(n);
    Size nodeSize = originalGlGraphComposite->getInputData()->getElementSize()->getNodeValue(n);
    float dist = centralNodeCoord.dist(nodeCoord) + nodeSize.getW();

    if (dist > radius) {
      radius = dist;
    }
  }
  return radius;
}

bool NeighborhoodHighlighter::draw(GlMainWidget *glMainWidget) {

  if (neighborhoodGraphCentralNode.isValid() && glNeighborhoodGraph != NULL) {
    if(!glNeighborhoodCamera)
      glNeighborhoodCamera=new Camera(glMainWidget->getScene()->getLayer("Main")->getCamera());

    *glNeighborhoodCamera=glMainWidget->getScene()->getLayer("Main")->getCamera();
    glNeighborhoodCamera->initGl();

    glLineWidth(1.0);
    glPointSize(1.0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LEQUAL );
    glPolygonMode(GL_FRONT, GL_FILL);
    glColorMask(1, 1, 1, 1);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glIndexMask(UINT_MAX);
    glClearStencil(0xFFFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    GlCircle circle(circleCenter, computeNeighborhoodGraphRadius(neighborhoodGraphLayout), Color(0, 0 , 0, circleAlphaValue),
                    Color (255, 255, 255, circleAlphaValue), true, true, 0.0, 60);
    circle.draw(0,0);

    GlGraphRenderingParameters renderingParameters = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
    renderingParameters.setNodesStencil(1);
    renderingParameters.setNodesLabelStencil(1);
    renderingParameters.setDisplayEdges(configWidget->isdisplayEdgesCBChecked());
    glNeighborhoodGraph->setRenderingParameters(renderingParameters);
    glNeighborhoodGraph->draw(10,glNeighborhoodCamera);

  }

  return true;
}


}
