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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainView.h>
#include <tulip/GlTools.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include <tulip/MouseEdgeBuilder.h>

using namespace std;
using namespace tlp;

MouseEdgeBuilder::MouseEdgeBuilder():started(false),graph(NULL),layoutProperty(NULL){}

MouseEdgeBuilder::~MouseEdgeBuilder(){
  if(graph)
    graph->removeGraphObserver(this);
}

bool MouseEdgeBuilder::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;

    ElementType type;
    node tmpNode;
    edge tmpEdge;
    Graph * _graph = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();

    LayoutProperty* mLayout = _graph->getProperty<LayoutProperty>(glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getElementLayoutPropName());
    if (qMouseEv->buttons()==Qt::LeftButton) {
      if (!started) {
        bool result=glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge);
        if (result && (type == NODE)) {
          started=true;
          initObserver(_graph);
          source=tmpNode;
          glMainWidget->setMouseTracking(true);
          curPos=startPos=mLayout->getNodeValue(source);
          return true;
        }
        return false;
      }
      else {
        bool result = glMainWidget->doSelect(qMouseEv->x(),qMouseEv->y(),type,tmpNode,tmpEdge);
        if (result && (type == NODE)) {
          Observable::holdObservers();
          started=false;
          clearObserver();
          glMainWidget->setMouseTracking(false);
          // allow to undo
          _graph->push();
          edge newEdge = _graph->addEdge(source, tmpNode);
          mLayout->setEdgeValue(newEdge, bends);
          //	  mColors->setEdgeValue(newEdge, ((Application *)qApp)->edgeColor);
          bends.clear();
          Observable::unholdObservers();
          NodeLinkDiagramComponent *nodeLinkView=(NodeLinkDiagramComponent *)view;
          nodeLinkView->elementSelectedSlot(newEdge.id, false);
        }
        else {
          Coord point((double) glMainWidget->width() - (double) qMouseEv->x(),
              (double) qMouseEv->y(),
              0);
          bends.push_back(glMainWidget->getScene()->getCamera()->screenTo3DWorld(point));
          glMainWidget->draw();
        }
      }
      return true;
    }
    if (qMouseEv->buttons()==Qt::MidButton) {
      bends.clear();
      glMainWidget->setMouseTracking(false);
      started=false;
      clearObserver();
      glMainWidget->draw();
      return true;
    }
  }
  if  (e->type() == QEvent::MouseMove) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (!started) return false;

    Coord point((double) glMainWidget->width() - (double) qMouseEv->x(),
        (double) qMouseEv->y(),
        0);
    point = glMainWidget->getScene()->getCamera()->screenTo3DWorld(point);
    curPos.set(point[0], point[1], point[2]);
    glMainWidget->draw();
    return true;
  }

  return false;
}

bool MouseEdgeBuilder::draw(GlMainWidget *glMainWidget) {
  if (!started) return false;
  glStencilFunc(GL_LEQUAL,0,0xFFFF);
  glMainWidget->getScene()->getCamera()->initGl();
  glDisable(GL_LIGHTING);
  vector<Coord>::iterator lCoordIt=bends.begin();
  setColor(Color(255,0,0,255));
  glBegin(GL_LINE_STRIP);{
    glVertex3f(startPos.getX(),startPos.getY(),startPos.getZ());
    while(lCoordIt!=bends.end()) {
      glVertex3f(lCoordIt->getX(),lCoordIt->getY(),lCoordIt->getZ());
      ++lCoordIt;
    }
    glVertex3f(curPos.getX(),curPos.getY(),curPos.getZ());
  }glEnd();
  return true;
}

void MouseEdgeBuilder::initObserver(Graph *newGraph){
  newGraph->addGraphObserver(this);
  graph=newGraph;
  layoutProperty=newGraph->getProperty<LayoutProperty>("viewLayout");
  layoutProperty->addPropertyObserver(this);
}

void MouseEdgeBuilder::clearObserver(){
  if(graph)
    graph->removeGraphObserver(this);
  graph=NULL;
  if(layoutProperty)
    layoutProperty->removePropertyObserver(this);
  layoutProperty=NULL;
}

void MouseEdgeBuilder::delNode(Graph *g,const node n){
  if(n==source){
    GlMainWidget *glMainWidget=((GlMainView*)view)->getGlMainWidget();
    bends.clear();
    glMainWidget->setMouseTracking(false);
    started=false;
    clearObserver();
  }
}

void MouseEdgeBuilder::destroy(Graph *g){
  if(graph=g)
    graph=NULL;
}

void MouseEdgeBuilder::afterSetNodeValue(PropertyInterface *property, const node n){
  if(n==source && property==layoutProperty){
    startPos=layoutProperty->getNodeValue(source);
  }
}

void MouseEdgeBuilder::destroy(PropertyInterface *property){
  if(property==layoutProperty)
    layoutProperty=NULL;
}
