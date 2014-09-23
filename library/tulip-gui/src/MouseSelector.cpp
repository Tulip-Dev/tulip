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

#include <QMouseEvent>

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlTools.h>
#include <tulip/MouseSelector.h>
#include <tulip/GlGraphComposite.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace std;
using namespace tlp;

//==================================================================
MouseSelector::MouseSelector(Qt::MouseButton button,
                             Qt::KeyboardModifier modifier,
                             SelectionMode mode):
  mButton(button), kModifier(modifier), x(0),y(0),w(0),h(0),
  started(false),graph(NULL),_mode(mode) {
}
//==================================================================
bool MouseSelector::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);
  GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);
  Graph *g = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();

  if (e->type() == QEvent::MouseButtonPress) {

    if (qMouseEv->buttons()== mButton &&
        (kModifier == Qt::NoModifier ||
         qMouseEv->modifiers() & kModifier)) {
      if (!started) {
        x = qMouseEv->x();
        y = qMouseEv->y();
        w = 0;
        h = 0;
        started = true;
        graph=g;
        mousePressModifier=qMouseEv->modifiers();
      }
      else {
        if (g!=graph) {
          graph = NULL;
          started = false;
          return false;
        }
      }

      return true;
    }

    if (qMouseEv->buttons()==Qt::MidButton) {
      started = false;
      glMainWidget->redraw();
      return true;
    }
  }

  if  (e->type() == QEvent::MouseMove &&
       ((qMouseEv->buttons() & mButton) &&
        (kModifier == Qt::NoModifier ||
         qMouseEv->modifiers() & kModifier))) {

    if (g!=graph) {
      graph=NULL;
      started=false;
    }

    if (started) {
      int clampedX=qMouseEv->x();
      int clampedY=qMouseEv->y();

      if(clampedX<0)
        clampedX=0;

      if(clampedY<0)
        clampedY=0;

      if(clampedX>glMainWidget->width())
        clampedX=glMainWidget->width();

      if(clampedY>glMainWidget->height())
        clampedY=glMainWidget->height();

      w = clampedX - x;
      h = clampedY - y;
      glMainWidget->redraw();
      return true;
    }

    return false;
  }

  if  (e->type() == QEvent::MouseButtonRelease) {

    if (g!=graph) {
      graph=NULL;
      started=false;
      return false;
    }

    if (started) {
      Observable::holdObservers();
      BooleanProperty* selection=glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getElementSelected();
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
          if(selection->getNodeDefaultValue()==true || selection->getEdgeDefaultValue()==true) {
            graph->push();
            selection->setAllNodeValue(false);
            selection->setAllEdgeValue(false);
            needPush=false;
          }

          Iterator<node>* itn = selection->getNonDefaultValuatedNodes();

          if (itn->hasNext()) {
            if(needPush) {
              graph->push();
              needPush = false;
            }

            delete itn;
            selection->setAllNodeValue(false);
          }
          else
            delete itn;

          Iterator<edge>* ite = selection->getNonDefaultValuatedEdges();

          if (ite->hasNext()) {
            if (needPush) {
              graph->push();
              needPush = false;
            }

            delete ite;
            selection->setAllEdgeValue(false);
          }
          else
            delete ite;
        }
      }
      else {
        boolVal = true;
      }

      if ((w==0) && (h==0)) {
        SelectedEntity selectedEntity;
        bool result = glMainWidget->pickNodesEdges(x, y, selectedEntity);

        if (result) {
          switch(selectedEntity.getEntityType()) {
          case SelectedEntity::NODE_SELECTED:

            if(_mode == EdgesAndNodes || _mode == NodesOnly) {
              result = selection->getNodeValue(node(selectedEntity.getComplexEntityId()));

              if (revertSelection || boolVal != result) {
                if (needPush) {
                  graph->push();
                  needPush = false;
                }

                selection->setNodeValue(node(selectedEntity.getComplexEntityId()), !result);
              }
            }

            break;

          case SelectedEntity::EDGE_SELECTED:

            if(_mode == EdgesAndNodes || _mode == EdgesOnly) {
              result = selection->getEdgeValue(edge(selectedEntity.getComplexEntityId()));

              if (revertSelection || boolVal != result) {
                if (needPush) {
                  graph->push();
                  needPush = false;
                }

                selection->setEdgeValue(edge(selectedEntity.getComplexEntityId()), !result);
              }
            }

            break;

          default:
            break;
          }
        }
      }
      else {
        vector<SelectedEntity> tmpSetNode;
        vector<SelectedEntity> tmpSetEdge;

        if (w < 0) {
          w *= -1;
          x -= w;
        }

        if (h<0) {
          h *= -1;
          y -= h;
        }

        glMainWidget->pickNodesEdges(x, y, w, h, tmpSetNode, tmpSetEdge);

        if (needPush)
          graph->push();

        vector<SelectedEntity>::const_iterator it;

        if(_mode == EdgesAndNodes || _mode == NodesOnly) {
          for (it=tmpSetNode.begin(); it!=tmpSetNode.end(); ++it) {
            selection->setNodeValue(node((*it).getComplexEntityId()),
                                    revertSelection ?
                                    !selection->getNodeValue(node((*it).getComplexEntityId()))
                                    : boolVal);
          }
        }

        if(_mode == EdgesAndNodes || _mode == EdgesOnly) {
          for (it=tmpSetEdge.begin(); it!=tmpSetEdge.end(); ++it) {
            selection->setEdgeValue(edge((*it).getComplexEntityId()),
                                    revertSelection ?
                                    !selection->getEdgeValue(edge((*it).getComplexEntityId()))
                                    : boolVal);
          }
        }
      }

      started = false;
      Observable::unholdObservers();
      glMainWidget->redraw();
      return true;
    }
  }

  return false;
}
//==================================================================
bool MouseSelector::draw(GlMainWidget *glMainWidget) {
  if (!started) return false;

  if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
    graph = NULL;
    started = false;
  }

  float yy = glMainWidget->height() - y;
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity ();
  glOrtho(0.0, (GLdouble) glMainWidget->width(), 0.0, (GLdouble) glMainWidget->height(), -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
  float col[4]= {0,0,0,0.2f};

  if (mousePressModifier ==
#if defined(__APPLE__)
      Qt::AltModifier
#else
      Qt::ControlModifier
#endif
     ) {
    col[0]=1.;
    col[1]=0.8f;
    col[2]=1.;
  }
  else if(mousePressModifier == Qt::ShiftModifier) {
    col[0]=1.;
    col[1]=.7f;
    col[2]=.7f;
  }
  else {
    col[0]=0.8f;
    col[1]=0.8f;
    col[2]=0.7f;
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
