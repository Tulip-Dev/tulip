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
#include <QtGui/qcolor.h>
#include <QtGui/qslider.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qframe.h>
#include <QtGui/qevent.h>
#include <QtGui/qtooltip.h>
#include <QtGui/QMenu>

#include <tulip/GlSimpleEntity.h>

#include "tulip/GWOverviewWidget.h"
#include "tulip/GlMainWidget.h"
#include "tulip/GlTools.h"


using namespace std;

namespace tlp {

struct RectPosition : public ForegroundEntity {
  void draw(GlMainWidget *widget);
  RectPosition(GlMainWidget *, GlMainWidget *);
  void setObservedView(GlMainWidget *glG) {
    _observedView = glG;
  }

private :
  GlMainWidget * _observedView;
  GlMainWidget * _view;
};

//=============================================================================
  GWOverviewWidget::GWOverviewWidget(QWidget* parent,bool drawIfNotVisible) : QWidget(parent),_observedView(NULL),_initialCamera(NULL),drawIfNotVisible(drawIfNotVisible){
  setupUi(this);

  // Setup the internal view of the overview
  //  This view is a GlMainWidget with only one layer : "Main"
  _view = new GlMainWidget(frame);
  _view->setViewLabel(false);
  GlLayer* layer=new GlLayer("Main");
  layer->getComposite()->setDeleteComponentsInDestructor(false);
  _view->getScene()->addLayer(layer);
  // Add the visible rectangle on the overview
  _glDraw = new RectPosition(_view, 0);
  _view->addForegroundEntity(_glDraw);

  // Add the view to the overview widget
  QGridLayout* gridLayout = new QGridLayout(frame);
  gridLayout->setMargin(0);
  gridLayout->setSpacing(0);
  gridLayout->addWidget( _view, 0, 0, 1, 1);
  _view->installEventFilter(this);
}
//=============================================================================
GWOverviewWidget::~GWOverviewWidget() {
  if (_observedView) {
    disconnect(this, 0, 0, 0);
  }
}
//=============================================================================
GlMainWidget *GWOverviewWidget::getObservedView() {
  return _observedView;
}
//=============================================================================
GlMainWidget *GWOverviewWidget::getView() {
  return _view;
}
//=============================================================================
bool GWOverviewWidget::eventFilter(QObject *obj, QEvent *e) {
  if ( obj->inherits("tlp::GlMainWidget") &&
       ((e->type() == QEvent::MouseButtonPress) ||
	(e->type() == QEvent::MouseMove))) {
    if (_observedView == 0) return false;
    QMouseEvent *me = (QMouseEvent *) e;
    if (me->buttons()==Qt::LeftButton) {
      assert(((GlMainWidget *) obj) == _view);
      double mouseClicX = me->x();
      double mouseClicY = me->y();
      double widgetWidth = _view->width();
      double widgetHeight = _view->height();
      Vector<int, 4> viewport = _observedView->getScene()->getViewport();
      viewport[0] = viewport[1] = 0;
      Coord upperLeftCorner(viewport[0], viewport[1],0);
      Coord lowerRightCorner(viewport[0] + viewport[2],
			     viewport[1] + viewport[3],
			     0);
      Coord middle = (upperLeftCorner + lowerRightCorner) / 2.f;
      middle[2] = 0.;
      middle = _observedView->getScene()->getCamera().screenTo3DWorld(middle);
      Camera cover  = _view->getScene()->getCamera();
      Camera cview  = _observedView->getScene()->getCamera();
      middle = _view->getScene()->getCamera().worldTo2DScreen(middle);
      //      cerr << "Square center: " << Coord(x, y, z) << endl;
      float dx, dy, dz;
      int resultViewport;
      if(viewport[2]<viewport[3])
        resultViewport=viewport[2];
      else
        resultViewport=viewport[3];
      dx = (middle[0] - mouseClicX) * resultViewport * cview.getZoomFactor() / (cover.getZoomFactor() * widgetWidth);
      dy = (middle[1] - (widgetHeight - mouseClicY)) * resultViewport * cview.getZoomFactor() / (cover.getZoomFactor() * widgetHeight);
      dz = 0;
      //      cerr << "Translation : " << Coord(dx, dy, dz) << endl;
      _observedView->getScene()->translateCamera((int) dx, (int) dy, 0);
      _observedView->draw();
      return true;
    }
    else if (me->buttons()==Qt::RightButton) {
      QMenu contextMenu(this);
      QAction *hide=contextMenu.addAction("Hide");
      QAction* menuAction=contextMenu.exec(me->globalPos());
      if(menuAction==hide) {
	emit hideOverview(true);
      }
      return true;
    }else{
      return false;
    }
  }
  return false;
}
//=============================================================================
  void GWOverviewWidget::draw(GlMainWidget *glG,bool graphChanged) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  (void) glG;
  assert( glG == _observedView);

  if (isVisible() || drawIfNotVisible) {

    if (_observedView != 0) {
      // If we have an observed view
      if(_initialCamera && !graphChanged) {
        // Check if the camera changed. If no, only redraw overview
        Camera currentCamera=_observedView->getScene()->getCamera();
        if((currentCamera.getUp()==_initialCamera->getUp())) {
          if((currentCamera.getCenter()-currentCamera.getEyes())==(_initialCamera->getCenter()-_initialCamera->getEyes())) {
  					_view->redraw();
  					return;
  				}
  			}
      }

      // Draw the overview
      _view->getScene()->centerScene();

      // Init camera and background color
      _initialCamera = &_view->getScene()->getCamera();
      Camera cam = _observedView->getScene()->getCamera();
      cam.setScene(_initialCamera->getScene());
      cam.setZoomFactor(1);
      cam.setEyes(cam.getEyes() - (cam.getCenter() - _initialCamera->getCenter()));
      cam.setCenter(cam.getCenter() - (cam.getCenter() - _initialCamera->getCenter()));
      cam.setSceneRadius(_initialCamera->getSceneRadius());
      _view->getScene()->setCamera(cam);
      _initialCamera = &_view->getScene()->getCamera();
      _view->getScene()->setBackgroundColor(_observedView->getScene()->getBackgroundColor() );

      // The overview use the observed view inputData but we don't want to see inside metanodes in overview, so :
      //  - Store old MetaNodeRenderer
      //  - Put a new default MetaNodeRenderer
      //  - Put the old MetaNodeRenderer after draw
      GlMetaNodeRenderer *oldMetaNodeRenderer = NULL;
      if(_view->getScene()->getGlGraphComposite()){
        oldMetaNodeRenderer=_view->getScene()->getGlGraphComposite()->getInputData()->getMetaNodeRenderer();
        _view->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(&metaNodeRenderer,false);
      }
      _view->draw();
      if(_view->getScene()->getGlGraphComposite())
        _view->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(oldMetaNodeRenderer,false);

    }else{
      // We don't have any observed view : only draw the view
      _view->draw();
    }
  }

}
//=============================================================================
  void GWOverviewWidget::setObservedView(GlMainWidget *glWidget,GlSimpleEntity *observedEntity){
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << glWidget << endl << flush;
#endif
  if (_observedView != 0) {
    // Signal deconnection
    disconnect(_observedView, SIGNAL(graphRedrawn(GlMainWidget *,bool)),
	       this, SLOT(draw(GlMainWidget *,bool)));
    disconnect(_observedView, SIGNAL(destroyed(QObject *)),
	       this, SLOT(observedViewDestroyed(QObject *)));
    _observedView = 0;
  }

  // ToolTip activation if we have an observed view
  if (glWidget)
    _view->setToolTip(QString("Click Left to center the view on the selected point"));
  else
    _view->setToolTip(QString());

  _observedView = glWidget;
  _glDraw->setObservedView(_observedView);

  if (_observedView != 0) {
    // If we have an observed view : initialise the "Main" layer
    _view->getScene()->getLayer("Main")->deleteGlEntity("overviewEntity");
    _view->getScene()->getLayer("Main")->addGlEntity(observedEntity,"overviewEntity");

    GlGraphComposite *p_subclass = dynamic_cast<GlGraphComposite *>( observedEntity );
    if(p_subclass)
      _view->getScene()->addGlGraphCompositeInfo(_view->getScene()->getGraphLayer(),p_subclass);

    // Connect signals
    connect(_observedView, SIGNAL(graphRedrawn(GlMainWidget *,bool)),
	   this, SLOT(draw(GlMainWidget *,bool)));
    connect(_observedView, SIGNAL(destroyed(QObject *)),
	    this, SLOT(observedViewDestroyed(QObject *)));
  } else {
    // If we don't have any observed view : clear "Main" layer
    _view->getScene()->addGlGraphCompositeInfo(0,0);
    _view->getScene()->getLayer("Main")->deleteGlEntity("overviewEntity");
  }
}
//=============================================================================
void GWOverviewWidget::observedViewDestroyed(QObject *glWidget) {
  assert(_observedView == glWidget);
  (void) glWidget;
  _observedView = 0;
  _glDraw->setObservedView(0);
  _view->getScene()->getLayer("Main")->deleteGlEntity("overviewEntity");
  _view->getScene()->addGlGraphCompositeInfo(0,0);
  draw(0);
}
//=============================================================================
void GWOverviewWidget::updateView() {
  if (_observedView!=0) {
    draw(_observedView);
  }
}
//=============================================================================
void GWOverviewWidget::show(){
  updateView();
}
//=============================================================================
void RectPosition::draw(GlMainWidget*) {
  //assert (_view == target);
  if(_observedView == 0) {
    return ;
  }

  //_observedView->makeCurrent();
  Array<Coord, 4> points;
  Vector<int,   4> viewport = _observedView->getScene()->getViewport();
  viewport[0] = viewport[1] = 0;
  points[0] = Coord(viewport[0]              , viewport[1], 0);
  points[1] = Coord(viewport[0] + viewport[2], viewport[1], 0);
  points[2] = Coord(viewport[0] + viewport[2], viewport[1] + viewport[3], 0.0);
  points[3] = Coord(viewport[0]              , viewport[1] + viewport[3], 0.0);
  for (int i=0;i<4;++i)
    points[i] = _observedView->getScene()->getCamera().screenTo3DWorld(points[i]);

  //_view->makeCurrent();
  viewport = _view->getScene()->getViewport();
  viewport[0] = viewport[1] = 0;
  Array<Coord, 4> points2;
  points2[0] = Coord(viewport[0]              , viewport[1], 0);
  points2[1] = Coord(viewport[0] + viewport[2], viewport[1], 0);
  points2[2] = Coord(viewport[0] + viewport[2], viewport[1] + viewport[3], 0.0);
  points2[3] = Coord(viewport[0]              , viewport[1] + viewport[3], 0.0);
  for (int i=0;i<4;++i)
    points2[i] = _view->getScene()->getCamera().screenTo3DWorld(points2[i]);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_COLOR_MATERIAL);
  setMaterial(Color(255,255,255,0));

  glBegin(GL_QUADS);

#define RVAL  180
#define GVAL  180
#define BVAL  180
#define ALPHA 120

  setColor(Color(RVAL,GVAL,BVAL,ALPHA));
  glVertex3fv((float *)&points2[0]);
  glVertex3fv((float *)&points[0]);
  glVertex3fv((float *)&points[1]);
  glVertex3fv((float *)&points2[1]);

  glVertex3fv((float *)&points2[1]);
  glVertex3fv((float *)&points[1]);
  glVertex3fv((float *)&points[2]);
  glVertex3fv((float *)&points2[2]);

  glVertex3fv((float *)&points2[2]);
  glVertex3fv((float *)&points[2]);
  glVertex3fv((float *)&points[3]);
  glVertex3fv((float *)&points2[3]);

  glVertex3fv((float *)&points2[3]);
  glVertex3fv((float *)&points[3]);
  glVertex3fv((float *)&points[0]);
  glVertex3fv((float *)&points2[0]);

  glEnd();

  glDisable(GL_BLEND);
  setColor(Color(0,0,0,255));
  glLineWidth(1);
  glStencilFunc(GL_LEQUAL,0x0001,0xFFFF);
  glBegin(GL_LINE_LOOP);
  for (int i=0;i<4;++i)
    glVertex3fv((float *)&points[i]);
  glEnd();
  glLineWidth(1);
  glLineStipple(2, 0xAAAA);
  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  for (int i=0;i<4;++i) {
    glVertex3fv((float *)&points2[i]);
    glVertex3fv((float *)&points[i]);
  }
  glEnd();

  glPopAttrib();
}
//=============================================================================
RectPosition::RectPosition(GlMainWidget *view, GlMainWidget *observedView) :
  _observedView(observedView), _view(view) {
}
//=============================================================================

}
