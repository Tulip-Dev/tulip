#include <QtGui/qcolor.h>
#include <QtGui/qslider.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcolordialog.h>
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
  GWOverviewWidget::GWOverviewWidget(QWidget* parent) : QWidget(parent),_initialCamera(NULL){
  setupUi(this);
  _observedView = 0;
  _glDraw = 0;
  _view = new GlMainWidget(frame);
  _view->setViewLabel(false);
  GlLayer* layer=new GlLayer("Main");
  //GlMainComposite* graphComposite=new GlMainComposite();
  //layer->addGlEntity(graphComposite,"graphComposite");
  //_view->getScene()->setGlMainComposite(graphComposite);
  _view->getScene()->addLayer(layer);
  QGridLayout* gridLayout = new QGridLayout(frame);
  gridLayout->setMargin(0);
  gridLayout->setSpacing(0);
  gridLayout->addWidget( _view, 0, 0, 1, 1);
  _view->installEventFilter(this);
  _glDraw = new RectPosition(_view, 0);
  _view->addForegroundEntity(_glDraw);
  //_view->addGlAugmentedDisplay(_glDraw, "Overview");
  //paramDialog = new RenderingParametersDialog(this);
}
//=============================================================================
GWOverviewWidget::~GWOverviewWidget() {
  if (_observedView != 0) {
    disconnect(this, 0, 0, 0);
  }
  // no need to explicitely delete _view because it is a children
  // of frame8, and so it will be deleted further
  //delete _view;
  delete _glDraw;
  //delete paramDialog;
}
//=============================================================================
GlMainWidget *GWOverviewWidget::getObservedView() {
  return _observedView;
}
//=============================================================================
bool GWOverviewWidget::eventFilter(QObject *obj, QEvent *e) {
  if ( obj->inherits("tlp::GlMainWidget") &&
       ((e->type() == QEvent::MouseButtonPress) ||
	(e->type() == QEvent::MouseMove))) {
    if (_observedView == 0) return false;
    QMouseEvent *me = (QMouseEvent *) e;
    if (me->buttons()==Qt::LeftButton) {
      GlMainWidget *glw = (GlMainWidget *) obj;
      assert(glw == _view);
      double mouseClicX = me->x();
      double mouseClicY = me->y();
      double widgetWidth = _view->width();
      double widgetHeight = _view->height();
      Vector<int, 4> viewport = _observedView->getScene()->getViewport();
      Coord upperLeftCorner(viewport[0], viewport[1],0);
      Coord lowerRightCorner(viewport[0] + viewport[2],
			     viewport[1] + viewport[3],
			     0);
      Coord middle = (upperLeftCorner + lowerRightCorner) / 2.0;
      middle[2] = 0.;
      middle = _observedView->getScene()->getCamera()->screenTo3DWorld(middle);
      Camera cover  = *_view->getScene()->getCamera();
      Camera cview  = *_observedView->getScene()->getCamera();
      middle = _view->getScene()->getCamera()->worldTo2DScreen(middle);
      //      cerr << "Square center: " << Coord(x, y, z) << endl;
      float dx, dy, dz;
      dx = (middle[0] - mouseClicX) * viewport[2] * cview.getZoomFactor() / (cover.getZoomFactor() * widgetWidth);
      dy = (middle[1] - (widgetHeight - mouseClicY)) * viewport[3] * cview.getZoomFactor() / (cover.getZoomFactor() * widgetHeight);
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
  assert( glG == _observedView);
  if (isVisible()) {
  	if (_observedView != 0) {
  		if(_initialCamera && !graphChanged) {
  			Camera *currentCamera=_observedView->getScene()->getCamera();
  			if((currentCamera->getUp()==_initialCamera->getUp())) {
  				if((currentCamera->getCenter()-currentCamera->getEyes())==(_initialCamera->getCenter()-_initialCamera->getEyes())) {
  					_view->redraw();
  					return;
  				}
  			}
      }
      _view->getScene()->centerScene();
      _initialCamera = _view->getScene()->getCamera();
      Camera cam = *_observedView->getScene()->getCamera();
      cam.setScene(_initialCamera->getScene());
      cam.setZoomFactor(0.5);
      cam.setEyes(cam.getEyes() - (cam.getCenter() - _initialCamera->getCenter()));
      cam.setCenter(cam.getCenter() - (cam.getCenter() - _initialCamera->getCenter()));
      _view->getScene()->setCamera(&cam);
  	}
    _view->draw();
  }
}
//=============================================================================
  void GWOverviewWidget::setObservedView(GlMainWidget *glWidget,GlSimpleEntity *observedEntity){
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << glWidget << endl << flush;
#endif
  if (_observedView != 0) {
    disconnect(_observedView, SIGNAL(graphRedrawn(GlMainWidget *,bool)),
	       this, SLOT(draw(GlMainWidget *,bool)));
    disconnect(_observedView, SIGNAL(destroyed(QObject *)),
	       this, SLOT(observedViewDestroyed(QObject *)));
    //_observedView->getScene()->getSelectionLayout()->clear();
    _observedView = 0;
  }
  if (glWidget)
#if defined(__APPLE__)
    _view->setToolTip(QString("Click+Alt show rendering parameters"));
#else
  _view->setToolTip(QString("Click Left+Ctrl to show rendering parameters"));
#endif
  else
    _view->setToolTip(QString());
  _observedView = glWidget;
  _glDraw->setObservedView(_observedView);

  if (_observedView != 0) {
    _view->getScene()->getLayer("Main")->deleteGlEntity("entity");
    _view->getScene()->getLayer("Main")->addGlEntity(observedEntity,"entity");

    GlGraphComposite *p_subclass = dynamic_cast<GlGraphComposite *>( observedEntity );
    if(p_subclass)
      _view->getScene()->addGlGraphCompositeInfo(_view->getScene()->getGraphLayer(),p_subclass);
    _view->getScene()->centerScene();
    _view->getScene()->setBackgroundColor(_observedView->getScene()->getBackgroundColor() );

    if(isVisible())
      _glDraw->draw(_view);

    //_observedView->getScene()->setRenderingParameters(_view->getScene()->getRenderingParameters());

    //_observedView->getScene()->setViewport(_view->getScene()->getViewport());
    //syncFromView();
    connect(_observedView, SIGNAL(graphRedrawn(GlMainWidget *,bool)),
	   this, SLOT(draw(GlMainWidget *,bool)));
    connect(_observedView, SIGNAL(destroyed(QObject *)),
	    this, SLOT(observedViewDestroyed(QObject *)));
  } else {
    _view->getScene()->addGlGraphCompositeInfo(0,0);
    _view->getScene()->getLayer("Main")->deleteGlEntity("entity");
  }
}
//=============================================================================
void GWOverviewWidget::observedViewDestroyed(QObject *glWidget) {
  assert(_observedView == glWidget);
  _observedView = 0;
  _glDraw->setObservedView(0);
  _view->getScene()->getLayer("Main")->deleteGlEntity("entity");
  _view->getScene()->addGlGraphCompositeInfo(0,0);
  draw(0);
}
//=============================================================================
/*void GWOverviewWidget::setBackgroundColor(QColor tmp) {
  if (tmp.isValid()) {
    QPalette palette;
    palette.setColor(QPalette::Button, tmp);
    int h,s,v;
    tmp.getHsv(&h, &s, &v);
    if (v < 128)
      palette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    else
      palette.setColor(QPalette::ButtonText, QColor(0, 0, 0));
    paramDialog->background->setPalette(palette);
  }
  }*/
//=============================================================================
/*void GWOverviewWidget::backColor() {
  setBackgroundColor(QColorDialog::getColor(paramDialog->background->palette().color(QPalette::Button), this));
  updateView();
  }*/
//=============================================================================
/*void GWOverviewWidget::syncFromView() {
  if (_observedView!=0) {
    _synchronizing = true;
    GlGraphRenderingParameters param = _observedView->getScene()->getGlGraphComposite()->getRenderingParameters();
    paramDialog->arrows->setChecked( param.isViewArrow());
    paramDialog->colorInterpolation->setChecked( param.isEdgeColorInterpolate());
    paramDialog->sizeInterpolation->setChecked( param.isEdgeSizeInterpolate());
    paramDialog->ordering->setChecked( param.isElementOrdered());
    paramDialog->orthogonal->setChecked(_observedView->getScene()->isViewOrtho());
    paramDialog->edge3D->setChecked( param.isEdge3D());
    Color tmp = _observedView->getScene()->getBackgroundColor();
    setBackgroundColor(QColor(tmp[0],tmp[1],tmp[2]));
    paramDialog->fonts->setCurrentIndex(param.getFontsType());
    paramDialog->density->setValue(param.getLabelsBorder());
    //((RenderingParametersDialog *) paramDialog)->updateEdgeOptions();

    GlGraphRenderingParameters paramView = _view->getScene()->getGlGraphComposite()->getRenderingParameters();
    //paramView.setViewOrtho( param.isViewOrtho() );
    paramView.setEdgeSizeInterpolate( param.isEdgeSizeInterpolate() );
    paramView.setEdgeColorInterpolate( param.isEdgeColorInterpolate() );
    paramView.setTexturePath( param.getTexturePath() );
    //paramView.setViewNodeLabel(false);
    //paramView.setViewEdgeLabel(false);
    //paramView.setElementOrdered(false);
    _view->getScene()->getGlGraphComposite()->setRenderingParameters(paramView);
    _view->getScene()->setViewOrtho(_observedView->getScene()->isViewOrtho());

    _synchronizing=false;
  }
  }*/
//=============================================================================
  void GWOverviewWidget::updateView() {
    if (_observedView!=0) {
      draw(_observedView);
    }
  }
/*void GWOverviewWidget::updateView() {
  if (_observedView!=0 && !_synchronizing) {
    GlGraphRenderingParameters paramObservedViev = _observedView->getScene()->getGlGraphComposite()->getRenderingParameters();

    paramObservedViev.setViewArrow(paramDialog->arrows->isChecked());
    paramObservedViev.setEdgeColorInterpolate(paramDialog->colorInterpolation->isChecked());
    paramObservedViev.setEdgeSizeInterpolate(paramDialog->sizeInterpolation->isChecked());
    paramObservedViev.setElementOrdered(paramDialog->ordering->isChecked());
    _observedView->getScene()->setViewOrtho(paramDialog->orthogonal->isChecked());
    paramObservedViev.setEdge3D(paramDialog->edge3D->isChecked());
    paramObservedViev.setFontsType(paramDialog->fonts->currentIndex());
    QColor tmp = paramDialog->background->palette().color(QPalette::Button);
    _observedView->getScene()->setBackgroundColor(Color(tmp.red(),tmp.green(),tmp.blue()));
    paramObservedViev.setLabelsBorder(paramDialog->density->value());
    _observedView->getScene()->getGlGraphComposite()->setRenderingParameters(paramObservedViev);

    GlGraphRenderingParameters paramView = _view->getScene()->getGlGraphComposite()->getRenderingParameters();
    _view->getScene()->setBackgroundColor(_observedView->getScene()->getBackgroundColor() );
    paramView.setEdgeColorInterpolate( paramObservedViev.isEdgeColorInterpolate() );
    _view->getScene()->setViewOrtho(_observedView->getScene()->isViewOrtho());
    paramView.setEdgeSizeInterpolate(paramObservedViev.isEdgeSizeInterpolate() );
    //paramView.setViewNodeLabel(false);
    //paramView.setViewEdgeLabel(false);
    //paramView.setElementOrdered(false);
    _view->getScene()->getGlGraphComposite()->setRenderingParameters(paramView);

    _observedView->draw();
  }
  }*/
//=============================================================================
void RectPosition::draw(GlMainWidget *widget) {
  //assert (_view == target);
  if(_observedView == 0) {
    return ;
  }

  _observedView->makeCurrent();
  Vector<Coord, 4> points;
  Vector<int,   4> viewport = _observedView->getScene()->getViewport();
  points[0] = Coord(viewport[0]              , viewport[1], 0);
  points[1] = Coord(viewport[0] + viewport[2], viewport[1], 0);
  points[2] = Coord(viewport[0] + viewport[2], viewport[1] + viewport[3], 0.0);
  points[3] = Coord(viewport[0]              , viewport[1] + viewport[3], 0.0);
  for (int i=0;i<4;++i)
    points[i] = _observedView->getScene()->getCamera()->screenTo3DWorld(points[i]);

  _view->makeCurrent();
  viewport = _view->getScene()->getViewport();
  Vector<Coord, 4> points2;
  points2[0] = Coord(viewport[0]              , viewport[1], 0);
  points2[1] = Coord(viewport[0] + viewport[2], viewport[1], 0);
  points2[2] = Coord(viewport[0] + viewport[2], viewport[1] + viewport[3], 0.0);
  points2[3] = Coord(viewport[0]              , viewport[1] + viewport[3], 0.0);
  for (int i=0;i<4;++i)
    points2[i] = _view->getScene()->getCamera()->screenTo3DWorld(points2[i]);

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

  //#define RVAL 59
  //#define GVAL 90
  //#define BVAL 233

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
  //  glEnable(GL_CULL_FACE);
  glLineStipple(2, 0xAAAA);
  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  for (int i=0;i<4;++i) {
    glVertex3fv((float *)&points2[i]);
    glVertex3fv((float *)&points[i]);
  }
  glEnd();

  /*boundingBox=BoundingBox();
  for(int i=0;i<4;++i) {
    boundingBox.check(points2[i]);
    }*/

  glPopAttrib();
}
//=============================================================================
RectPosition::RectPosition(GlMainWidget *view, GlMainWidget *observedView) :
  _observedView(observedView), _view(view) {
  //setCheckByBoundingBoxVisitor(false);
  //boundingBox=BoundingBox(Coord(-1,-1,-1),Coord(1,1,1));
}
//=============================================================================

}
