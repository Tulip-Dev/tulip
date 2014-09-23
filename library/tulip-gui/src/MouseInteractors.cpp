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

#include <QEvent>
#include <QWheelEvent>
#include <QPinchGesture>
#include <QPanGesture>

#include <tulip/GlNode.h>
#include <tulip/GlMainWidget.h>
#include <tulip/View.h>
#include <tulip/Observable.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainView.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/DrawingTools.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/MouseInteractors.h>

#include <iostream>

using namespace tlp;
using namespace std;

//===============================================================
bool MousePanNZoomNavigator::eventFilter(QObject *widget, QEvent *e) {
// according to Qt's doc, this constant has been defined by wheel mouse vendors
// we need it to interpret the value of QWheelEvent->delta()
#define WHEEL_DELTA 120
  GlMainWidget *g = static_cast<GlMainWidget *>(widget);

  if (e->type() == QEvent::Wheel) {
    QWheelEvent *we = static_cast<QWheelEvent*>(e);

    if (we->orientation() == Qt::Vertical && we->modifiers() == Qt::NoModifier) {
      g->getScene()->zoomXY(we->delta() / WHEEL_DELTA, we->x(), we->y());
      g->draw(false);
      return true;
    }
  }

  if(e->type() == QEvent::Gesture) {
    QGestureEvent* gesture = static_cast<QGestureEvent*>(e);
    QPointF center;
    //swipe events and pan events are never fired, known Qt bug
    /*if(gesture->gesture(Qt::SwipeGesture)) {
      QSwipeGesture* swipe = (QSwipeGesture*)gesture->gesture(Qt::SwipeGesture);
      int x = cos(swipe->swipeAngle()) * swipe->property("velocity").toFloat();
      int y = sin(swipe->swipeAngle()) * swipe->property("velocity").toFloat();
      g->getScene()->translateCamera(x, y, 0);
    }*/

    if(gesture->gesture(Qt::PinchGesture)) {
      QPinchGesture* pinch = static_cast<QPinchGesture*>(gesture->gesture(Qt::PinchGesture));
      Camera& camera = g->getScene()->getGraphCamera();

      //store the camera scale factor when starting the gesture
      if(pinch->state() == Qt::GestureStarted) {
        cameraScaleFactor = camera.getZoomFactor();
        isGesturing = true;
      }

      if(pinch->changeFlags() & QPinchGesture::ScaleFactorChanged) {
        //only set the zoom factor if two events in a row were in the same direction (zoom in or out) to smooth a bit the effect.
        if((pinch->lastScaleFactor() > 1 && pinch->scaleFactor() > 1) || (pinch->lastScaleFactor() <= 1 && pinch->scaleFactor() <= 1)) {
          camera.setZoomFactor(cameraScaleFactor * pinch->totalScaleFactor());
        }
      }

      if(pinch->changeFlags() & QPinchGesture::RotationAngleChanged) {
        /*//backup the current camera center
              Coord oldCenter = camera.getCenter();
              Coord oldEye = camera.getEyes();
        //sets the camera center to the center of the pich gesture
              Coord rotationCenter(g->mapFromGlobal(pinch->centerPoint().toPoint()).x(), g->mapFromGlobal(pinch->centerPoint().toPoint()).y(), oldCenter.getZ());
              Coord rotationEye=camera.getEyes()+(rotationCenter-oldCenter);
              camera.setCenter(rotationCenter);
              camera.setEyes(rotationEye);*/
        //rotates the camera
        camera.rotate((pinch->rotationAngle() - pinch->lastRotationAngle())/180*M_PI, 0, 0, 1);
        /*
        //restore old camera center and eyes
              camera.setCenter(oldCenter);
              camera.setEyes(oldEye); */
      }

      if(pinch->state() == Qt::GestureFinished) {
        isGesturing = false;
      }

      if(gesture->gesture(Qt::PanGesture)) {
        QPanGesture* pan = static_cast<QPanGesture*>(gesture->gesture(Qt::PanGesture));

        if(pan->state() == Qt::GestureStarted) {
          isGesturing = true;
        }

        if(pan->state() == Qt::GestureFinished) {
          isGesturing = false;
        }

        center = pan->delta();
        g->getScene()->translateCamera(pan->delta().x(), -pan->delta().y(), 0);
      }
    }

    g->draw(false);
    return true;
  }

  return false;
}

//===============================================================
bool MouseElementDeleter::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);

  if(qMouseEv != NULL) {
    SelectedEntity selectedEntity;
    GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);

    if(e->type() == QEvent::MouseMove) {
      if (glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity)) {
        glMainWidget->setCursor(QCursor(QPixmap(":/tulip/gui/icons/i_del.png")));
      }
      else {
        glMainWidget->setCursor(Qt::ArrowCursor);
      }

      return false;
    }
    else if (e->type() == QEvent::MouseButtonPress && qMouseEv->button()==Qt::LeftButton) {
      if (glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity)) {
        Observable::holdObservers();
        Graph* graph = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
        // allow to undo
        graph->push();

        switch(selectedEntity.getEntityType()) {
        case SelectedEntity::NODE_SELECTED:
          graph->delNode(node(selectedEntity.getComplexEntityId()));
          break;

        case SelectedEntity::EDGE_SELECTED:
          graph->delEdge(edge(selectedEntity.getComplexEntityId()));
          break;

        default :
          break;
        }

        glMainWidget->redraw();
        Observable::unholdObservers();
        return true;
      }
    }
  }

  return false;
}

void MouseElementDeleter::clear() {
  GlMainView *glMainView=static_cast<GlMainView*>(view());
  glMainView->getGlMainWidget()->setCursor(QCursor());
}
//===============================================================
class MouseRotXRotY:public InteractorComponent {
public:
  MouseRotXRotY() : x(INT_MAX), y(INT_MAX) {}
  ~MouseRotXRotY() {}
  int x,y;
  bool eventFilter(QObject *, QEvent *);
};

bool MouseRotXRotY::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);
    x = qMouseEv->x();
    y = qMouseEv->y();
    return true;
  }

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);
    GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);
    int deltaX,deltaY;
    deltaX=qMouseEv->x()-x;
    deltaY=qMouseEv->y()-y;

    if (abs(deltaX)>abs(deltaY))
      deltaY=0;
    else
      deltaX=0;

    if (deltaY!=0) glMainWidget->getScene()->rotateScene(deltaY,0,0);

    if (deltaX!=0) glMainWidget->getScene()->rotateScene(0,deltaX,0);

    x=qMouseEv->x();
    y=qMouseEv->y();
    glMainWidget->draw(false);
    return true;
  }

  return false;
}
//===============================================================
class MouseZoomRotZ:public InteractorComponent {
public:
  MouseZoomRotZ(): x(INT_MAX), y(INT_MAX), inRotation(false), inZoom(false) {}
  ~MouseZoomRotZ() {}
  int x,y;
  bool inRotation, inZoom;
  bool eventFilter(QObject *, QEvent *);
};

bool MouseZoomRotZ::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);
    x = qMouseEv->x();
    y = qMouseEv->y();
    inRotation=false;
    inZoom=false;
    return true;
  }

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);
    GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);
    int deltaX,deltaY;

    if(!inRotation && !inZoom) {
      deltaX = qMouseEv->x() - x;
      deltaY= qMouseEv->y() - y;

      if (deltaY && abs(deltaX) >= 3 * abs(deltaY)) {
        inRotation=true;
        inZoom=false;
      }
      else if (deltaX && abs(deltaY) >=  3 * abs(deltaX)) {
        inZoom=true;
        inRotation=false;
      }
      else {

      }

      x = qMouseEv->x();
      y = qMouseEv->y();
    }

    if (inZoom) {
      // Zoom
      deltaY = qMouseEv->y() - y;
      glMainWidget->getScene()->zoom(-deltaY/2);
      y = qMouseEv->y();
    }

    if(inRotation) {
      // Rotation
      deltaX = qMouseEv->x() - x;
      glMainWidget->getScene()->rotateScene(0,0,deltaX);
      x = qMouseEv->x();
    }

    glMainWidget->draw(false);
    return true;
  }

  return false;
}
//===============================================================
class MouseMove:public InteractorComponent {
public:
  int x,y;
  MouseMove() : x(INT_MAX), y(INT_MAX) {}
  ~MouseMove() {}
  bool eventFilter(QObject *, QEvent *);
};

bool MouseMove::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);
    x = qMouseEv->x();
    y = qMouseEv->y();
    return true;
  }

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);
    GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);

    if (qMouseEv->x() != x)
      glMainWidget->getScene()->translateCamera(qMouseEv->x()-x,0,0);

    if (qMouseEv->y() != y)
      glMainWidget->getScene()->translateCamera(0,y-qMouseEv->y(),0);

    x = qMouseEv->x();
    y = qMouseEv->y();
    glMainWidget->draw(false);
    return true;
  }

  return false;
}


// animation during meta node interaction
class MyQtGlSceneZoomAndPanAnimator : public tlp::QtGlSceneZoomAndPanAnimator {
public :

  MyQtGlSceneZoomAndPanAnimator(tlp::GlMainWidget *glWidget,tlp::View *view, const tlp::BoundingBox &boundingBox,tlp::Graph *graph,tlp::node n,const float &color):tlp::QtGlSceneZoomAndPanAnimator(glWidget,boundingBox),view(view),graph(graph),n(n),alphaEnd(color) {
    tlp::ColorProperty *colorProp=graph->getProperty<tlp::ColorProperty>("viewColor");
    alphaBegin=colorProp->getNodeValue(n)[3];
  }

protected:

  virtual void zoomAndPanAnimStepSlot(int animationStep);

protected :

  tlp::View *view;
  tlp::Graph *graph;
  tlp::node n;
  float alphaEnd;
  float alphaBegin;

};


void MyQtGlSceneZoomAndPanAnimator::zoomAndPanAnimStepSlot(int animationStep) {
  int nbAnimationSteps = animationDurationMsec / 40 + 1;
  float decAlpha=(alphaEnd-alphaBegin)/nbAnimationSteps;
  ColorProperty *colorProp=graph->getProperty<ColorProperty>("viewColor");
  Color color=colorProp->getNodeValue(n);
  color[3]=alphaBegin+decAlpha*animationStep;
  colorProp->setNodeValue(n,color);
  QtGlSceneZoomAndPanAnimator::zoomAndPanAnimationStep(animationStep);
  view->draw();
}
//===============================================================
bool MouseNKeysNavigator::eventFilter(QObject *widget, QEvent *e) {
  if(isGesturing) {
    return MousePanNZoomNavigator::eventFilter(widget, e);
  }

  if(currentSpecInteractorComponent) {
    if(currentSpecInteractorComponent->eventFilter(widget,e))
      return true;
  }

  GlMainWidget *glmainwidget = static_cast<GlMainWidget *>(widget);
  QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

  if (e->type() == QEvent::MouseButtonDblClick &&
      qMouseEv->button() == Qt::LeftButton) {
    Graph *graph=glmainwidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();

    if (qMouseEv->modifiers()!=Qt::ControlModifier) {
      vector<SelectedEntity> tmpNodes;
      vector<SelectedEntity> tmpEdges;
      glmainwidget->pickNodesEdges(qMouseEv->x()-1,qMouseEv->y()-1,3,3,tmpNodes,tmpEdges);
      node metaNode;
      bool find=false;

      for(unsigned int i=0; i<tmpNodes.size(); ++i) {
        if(graph->isMetaNode(node(tmpNodes[i].getComplexEntityId()))) {
          metaNode=node(tmpNodes[i].getComplexEntityId());
          find=true;
          break;
        }
      }

      if (find) {
        Graph *metaGraph=graph->getNodeMetaInfo(metaNode);

        if (metaGraph && nldc) {
          graphHierarchy.push_back(graph);
          nodeHierarchy.push_back(metaNode);
          cameraHierarchy.push_back(nldc->goInsideItem(metaNode));
        }
      }
      else  //no double click on a metanode. Do not block event.
        return false;

      return true;
    }
    else {
      if(!graphHierarchy.empty() && nldc) {
        Graph * oldGraph=graphHierarchy.back();
        graphHierarchy.pop_back();
        Camera camera=cameraHierarchy.back();
        cameraHierarchy.pop_back();
        node n=nodeHierarchy.back();
        nodeHierarchy.pop_back();

        Observable::holdObservers();

        ColorProperty *colorProp=oldGraph->getProperty<ColorProperty>("viewColor");
        float alphaOrigin=colorProp->getNodeValue(n)[3];
        Color color=colorProp->getNodeValue(n);
        color[3]=0;
        colorProp->setNodeValue(n,color);

        Observable::unholdObservers();

        nldc->requestChangeGraph(oldGraph);
        glmainwidget->getScene()->getLayer("Main")->getCamera().setCenter(camera.getCenter());
        glmainwidget->getScene()->getLayer("Main")->getCamera().setEyes(camera.getEyes());
        glmainwidget->getScene()->getLayer("Main")->getCamera().setSceneRadius(camera.getSceneRadius());
        glmainwidget->getScene()->getLayer("Main")->getCamera().setUp(camera.getUp());
        glmainwidget->getScene()->getLayer("Main")->getCamera().setZoomFactor(camera.getZoomFactor());
        glmainwidget->draw(false);

        GlBoundingBoxSceneVisitor *visitor;
        visitor=new GlBoundingBoxSceneVisitor(glmainwidget->getScene()->getGlGraphComposite()->getInputData());
        glmainwidget->getScene()->getLayer("Main")->acceptVisitor(visitor);
        BoundingBox boundingBox=visitor->getBoundingBox();

        MyQtGlSceneZoomAndPanAnimator navigator(glmainwidget,nldc,boundingBox,oldGraph,n,alphaOrigin);
        navigator.animateZoomAndPan();

        return true;
      }
    }
  }

  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);

    if (qMouseEv->buttons() == Qt::LeftButton) {
      oldCursor=glmainwidget->cursor();
      InteractorComponent *currentMouse;
      // give focus so we can catch key events
      glmainwidget->setFocus();

      if (qMouseEv->modifiers() &
#if defined(__APPLE__)
          Qt::AltModifier
#else
          Qt::ControlModifier
#endif
         )
        currentMouse = new MouseZoomRotZ();
      else if (qMouseEv->modifiers() & Qt::ShiftModifier)
        currentMouse = new MouseRotXRotY();
      else {
        currentMouse = new MouseMove();
        glmainwidget->setCursor(QCursor(Qt::ClosedHandCursor));
      }

      bool result = currentMouse->eventFilter(widget, e);

      currentSpecInteractorComponent=currentMouse;

      //currentMouseID = abstractView->pushInteractor(currentMouse);
      return result;
    }

    return false;
  }

  if (e->type() == QEvent::MouseButtonRelease) {
    glmainwidget->setCursor(oldCursor);
    delete currentSpecInteractorComponent;
    currentSpecInteractorComponent=NULL;
    return true;
  }

  if (e->type() == QEvent::KeyPress) {
    QKeyEvent *ke = static_cast<QKeyEvent*>(e);
    int delta = (ke->isAutoRepeat() ? 3 : 1);

    switch(ke->key()) {
    case Qt::Key_Left:
      glmainwidget->getScene()->translateCamera(delta * 2,0,0);
      break;

    case Qt::Key_Right:
      glmainwidget->getScene()->translateCamera(-1 * delta * 2,0,0);
      break;

    case Qt::Key_Up:
      glmainwidget->getScene()->translateCamera(0,-1 * delta * 2,0);
      break;

    case Qt::Key_Down:
      glmainwidget->getScene()->translateCamera(0,delta * 2,0);
      break;

    case Qt::Key_PageUp:
      glmainwidget->getScene()->zoom(delta);
      break;

    case Qt::Key_PageDown:
      glmainwidget->getScene()->zoom(-1 * delta);
      break;

    case Qt::Key_Home:
      glmainwidget->getScene()->translateCamera(0,0,-1 * delta * 2);
      break;

    case Qt::Key_End:
      glmainwidget->getScene()->translateCamera(0,0,delta * 2);
      break;

    case Qt::Key_Insert:
      glmainwidget->getScene()->rotateScene(0,0,-1 * delta * 2);
      break;

    case Qt::Key_Delete :
      glmainwidget->getScene()->rotateScene(0,0,delta * 2);
      break;

    default:
      return false;
    }

    glmainwidget->draw();
    return true;
  }

  if (e->type() == QEvent::KeyRelease) {
    QKeyEvent *ke = static_cast<QKeyEvent*>(e);

    switch(ke->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
    case Qt::Key_Home:
    case Qt::Key_End:
    case Qt::Key_Insert:
    case Qt::Key_Delete:
      break;

    default:
      return false;
    }

    return true;
  }

  return MousePanNZoomNavigator::eventFilter(widget, e);
}

void MouseNKeysNavigator::viewChanged(View *view) {
  nldc=static_cast<NodeLinkDiagramComponent*>(view);
}

void MouseNKeysNavigator::clear() {}

