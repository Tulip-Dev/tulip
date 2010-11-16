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
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlTools.h>
#include <tulip/MouseSelector.h>

using namespace std;
using namespace tlp;

//==================================================================
MouseSelector::MouseSelector(Qt::MouseButton button,
			     Qt::KeyboardModifier modifier):
  mButton(button), kModifier(modifier), x(0),y(0),w(0),h(0),
  started(false),graph(0) {
}
//==================================================================
bool MouseSelector::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (qMouseEv->buttons()== mButton &&
        (kModifier == Qt::NoModifier ||
            ((QMouseEvent *) e)->modifiers() & kModifier)) {
      if (!started) {
        x = qMouseEv->x();
        y = qMouseEv->y();
        w = 0;
        h = 0;
        started = true;
        graph=glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
        mousePressModifier=qMouseEv->modifiers();
      }
      else {
        if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
          graph = 0;
          started = false;
          return false;
        }
      }
      return true;
    }
    if (qMouseEv->buttons()==Qt::MidButton){
      started = false;
      glMainWidget->redraw();
      return true;
    }
  }
  if  (e->type() == QEvent::MouseMove &&
      ((((QMouseEvent *) e)->buttons() & mButton) &&
          (kModifier == Qt::NoModifier ||
              ((QMouseEvent *) e)->modifiers() & kModifier))) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
      graph=0;
      started=false;
    }
    if (started){
      if ((qMouseEv->x()>0) && (qMouseEv->x()<glMainWidget->width()))
        w = qMouseEv->x() - x;
      if ((qMouseEv->y()>0) && (qMouseEv->y()<glMainWidget->height()))
        h = qMouseEv->y() - y;
      glMainWidget->redraw();
      return true;
    }
    return false;
  }
  if  (e->type() == QEvent::MouseButtonRelease) {
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
      graph=0;
      started=false;
      return false;
    }
    if (started) {
      Observable::holdObservers();
      BooleanProperty* selection=glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()->getProperty<BooleanProperty>(glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getElementSelectedPropName());
      bool revertSelection = false; // add to selection
      bool boolVal = true;
      bool needPush = true; // undo management
      if (mousePressModifier !=
#if defined(__APPLE__)
          Qt::AltModifier
#else
          Qt::ControlModifier
#endif
      ) {
        if (mousePressModifier == Qt::ShiftModifier && kModifier != Qt::ShiftModifier)
          boolVal = false;
        else {
          if(selection->getNodeDefaultValue()==true || selection->getEdgeDefaultValue()==true){
            graph->push();
            selection->setAllNodeValue(false);
            selection->setAllEdgeValue(false);
            needPush=false;
          }
          Iterator<node>* itn = selection->getNonDefaultValuatedNodes();
          if (itn->hasNext()) {
            if(needPush){
              graph->push();
              needPush = false;
            }
            delete itn;
            selection->setAllNodeValue(false);
          } else
            delete itn;
          Iterator<edge>* ite = selection->getNonDefaultValuatedEdges();
          if (ite->hasNext()) {
            if (needPush) {
              graph->push();
              needPush = false;
            }
            delete ite;
            selection->setAllEdgeValue(false);
          } else
            delete ite;
        }
      } else {
        boolVal = true;
      }
      if ((w==0) && (h==0)) {
        node tmpNode;
        edge tmpEdge;
        ElementType type;
        bool result = glMainWidget->doSelect(x, y, type, tmpNode, tmpEdge);
        if (result) {
          switch(type) {
          case NODE:
            result = selection->getNodeValue(tmpNode);
            if (revertSelection || boolVal != result) {
              if (needPush) {
                graph->push();
                needPush = false;
              }
              selection->setNodeValue(tmpNode, !result);
            }
            break;
          case EDGE:
            result = selection->getEdgeValue(tmpEdge);
            if (revertSelection || boolVal != result) {
              if (needPush) {
                graph->push();
                needPush = false;
              }
              selection->setEdgeValue(tmpEdge, !result);
            }
            break;
          }
        }
      } else {
        vector<node> tmpSetNode;
        vector<edge> tmpSetEdge;
        if (w < 0) {
          w *= -1;
          x -= w;
        }
        if (h<0) {
          h *= -1;
          y -= h;
        }
        glMainWidget->doSelect(x, y, w, h, tmpSetNode, tmpSetEdge);
        if (needPush)
          graph->push();
        vector<node>::const_iterator it;
        for (it=tmpSetNode.begin(); it!=tmpSetNode.end(); ++it) {
          selection->setNodeValue(*it,
              revertSelection ?
                  !selection->getNodeValue(*it)
                  : boolVal);
        }
        vector<edge>::const_iterator ite;
        for (ite=tmpSetEdge.begin(); ite!=tmpSetEdge.end(); ++ite) {
          selection->setEdgeValue(*ite,
              revertSelection ?
                  !selection->getEdgeValue(*ite)
                  : boolVal);
        }
      }
      started = false;
      glMainWidget->draw();
      Observable::unholdObservers();
      return true;
    }
  }
  return false;
}
//==================================================================
bool MouseSelector::draw(GlMainWidget *glMainWidget){
  if (!started) return false;
  if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
    graph = 0;
    started = false;
  }
  float yy = glMainWidget->height() - y;
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity ();
  gluOrtho2D (0.0, (GLdouble) glMainWidget->width(), 0.0, (GLdouble) glMainWidget->height());
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
  float col[4]={0,0,0,0.2};
  if (mousePressModifier ==
#if defined(__APPLE__)
            Qt::AltModifier
#else
            Qt::ControlModifier
#endif
        ){
    col[0]=1.;
    col[1]=0.8;
    col[2]=1.;
  }else if(mousePressModifier == Qt::ShiftModifier){
    col[0]=1.;
    col[1]=.7;
    col[2]=.7;
  }else{
    col[0]=0.8;
    col[1]=0.8;
    col[2]=0.7;
  }
  setColor(col);
  glBegin(GL_QUADS);
  glVertex2f(x, yy);
  glVertex2f(x+w, yy);
  glVertex2f(x+w, yy-h);
  glVertex2f(x, yy-h);
  glEnd();
  glDisable(GL_BLEND);
  glLineWidth(2);
  glLineStipple(2, 0xAAAA);
  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINE_LOOP);
  glVertex2f(x, yy);
  glVertex2f(x+w, yy);
  glVertex2f(x+w, yy-h);
  glVertex2f(x, yy-h);
  glEnd();
  glLineWidth(1);
  glPopMatrix();
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (GL_MODELVIEW);
  glPopAttrib();
  return true;
}
