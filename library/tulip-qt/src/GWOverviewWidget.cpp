#include <qcolor.h>
#include <qslider.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qframe.h>
#include <qevent.h>
#include <qtooltip.h>

#include <tulip/GlSimpleEntity.h>

#include "tulip/GWOverviewWidget.h"
#include "tulip/GlGraphWidget.h"
#include "tulip/RenderingParametersDialogData.h"
#include "tulip/GlTools.h"


using namespace std;
using namespace tlp;

struct RectPosition : public tlp::GlSimpleEntity {
  void draw(float lod);
  RectPosition(GlGraphWidget *, GlGraphWidget *);
  void setObservedView(GlGraphWidget *glG) {
    _observedView = glG;
  }
private :
  GlGraphWidget * _observedView;
  GlGraphWidget * _view;
};

class RenderingParametersDialog : public RenderingParametersDialogData {
  GWOverviewWidget *overview;
public:
  RenderingParametersDialog(GWOverviewWidget* parent) : RenderingParametersDialogData(parent->parentWidget()) {
    overview = parent;
  }

  void windowActivationChange(bool oldActive) {
    if (!oldActive)
      buttonClose->setFocus();
  }

  void updateEdgeOptions() {
    bool visible = edgeVisible->isChecked();
    arrows->setEnabled(visible);
    edge3D->setEnabled(visible);
    colorInterpolation->setEnabled(visible);
    sizeInterpolation->setEnabled(visible);
  }

  void updateView() {
    updateEdgeOptions();
    overview->updateView();
  }

  void backColor() {
    overview->backColor();
  }
};

//=============================================================================
GWOverviewWidget::GWOverviewWidget(QWidget* parent,  
				   const char* name, bool modal, Qt::WFlags fl )
  : GWOverviewWidgetData( parent, name, (Qt::WFlags) (fl | Qt::Widget) ) {
  _observedView = 0;
  _glDraw = 0;
  _view = new GlGraphWidget( frame8, "view" );
  _view->setViewLabel(false);
  GlLayer* layer=new GlLayer("Main");
  //GlGraphComposite* graphComposite=new GlGraphComposite();
  //layer->addGlEntity(graphComposite,"graphComposite");
  //_view->getScene()->setGlGraphComposite(graphComposite);
  _view->getScene()->addLayer(layer);
  _view->setMinimumSize( QSize( 128, 128 ) );
  _view->setMaximumSize( QSize( 2000, 2000 ) );
  gridLayout->addWidget( _view, 0, 0 );
  _view->installEventFilter(this);
  _glDraw = new RectPosition(_view, 0);
  layer->addGlEntity(_glDraw,"RectPosition");
  //_view->addGlAugmentedDisplay(_glDraw, "Overview");
  paramDialog = new RenderingParametersDialog(this);
}
//=============================================================================
void GWOverviewWidget::showRenderingParametersDialog() {
  paramDialog->exec();
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
  delete paramDialog;
}
//=============================================================================
bool GWOverviewWidget::eventFilter(QObject *obj, QEvent *e) {
  if ( obj->inherits("GlGraphWidget") &&
       ((e->type() == QEvent::MouseButtonPress) ||
	(e->type() == QEvent::MouseMove))) {
    if (_observedView == 0) return false;
    QMouseEvent *me = (QMouseEvent *) e;
    if (me->state()==LeftButton || me->button()==LeftButton) {
      if  (me->state() &
#if defined(__APPLE__)
	   Qt::AltButton
#else
	   Qt::ControlButton
#endif
	   ) {
	paramDialog->show();
	return true;
      }
      GlGraphWidget *glw = (GlGraphWidget *) obj;
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
    else {
      return false;
    }
  }
  return false;
}
//=============================================================================
void GWOverviewWidget::draw(GlGraphWidget *glG) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  assert( glG == _observedView);
  if (_observedView != 0) {
    _view->getScene()->centerScene();
    _initialCamera = _view->getScene()->getCamera();   
    Camera cam = *_observedView->getScene()->getCamera();
    cam.scene=_initialCamera->scene;
    cam.setZoomFactor(0.5);
    cam.setEyes(cam.getEyes() - (cam.getCenter() - _initialCamera->getCenter()));
    cam.setCenter(cam.getCenter() - (cam.getCenter() - _initialCamera->getCenter()));
    _view->getScene()->setCamera(&cam);
  }
  _view->draw();
}
//=============================================================================
void GWOverviewWidget::setObservedView(GlGraphWidget *glWidget){
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << glWidget << endl << flush;
#endif
  if (_observedView != 0) {
    disconnect(_observedView, SIGNAL(graphRedrawn(GlGraphWidget *)), 
	       this, SLOT(draw(GlGraphWidget *)));
    disconnect(_observedView, SIGNAL(destroyed(QObject *)), 
	       this, SLOT(observedViewDestroyed(QObject *)));
    //_observedView->getScene()->getSelectionLayout()->clear();
    _observedView = 0;
  }
  if (glWidget)
#if defined(__APPLE__)
    QToolTip::add(_view, "Click+Alt show rendering parameters");
#else
    QToolTip::add(_view, "Click Left+Ctrl to show rendering parameters");
#endif
  else
    QToolTip::remove(_view);
  _observedView = glWidget;
  _glDraw->setObservedView(_observedView);
  
  if (_observedView != 0) {
    _view->getScene()->getLayer("Main")->deleteGlEntity("graphComposite");
    _view->getScene()->getLayer("Main")->addGlEntity(_observedView->getScene()->getGlGraphComposite(),"graphComposite");
    _view->getScene()->addGlGraphCompositeInfo(_view->getScene()->getGraphLayer(),_observedView->getScene()->getGlGraphComposite());
    _view->getScene()->centerScene();
    //_observedView->getScene()->setRenderingParameters(_view->getScene()->getRenderingParameters());
    
    //_observedView->getScene()->setViewport(_view->getScene()->getViewport());
    syncFromView();
    connect(_observedView, SIGNAL(graphRedrawn(GlGraphWidget *)),
	   this, SLOT(draw(GlGraphWidget *)));
    connect(_observedView, SIGNAL(destroyed(QObject *)), 
	    this, SLOT(observedViewDestroyed(QObject *)));
  } else {
    _view->getScene()->addGlGraphCompositeInfo(0,0);
    _view->getScene()->getLayer("Main")->deleteGlEntity("graphComposite");
  }
}
//=============================================================================
void GWOverviewWidget::observedViewDestroyed(QObject *glWidget) { 	 
  assert(_observedView == glWidget); 	 
  _observedView = 0; 	 
  _glDraw->setObservedView(0); 	
  _view->getScene()->getLayer("Main")->deleteGlEntity("graphComposite");
  _view->getScene()->addGlGraphCompositeInfo(0,0);
  draw(0); 	 
} 	 
//=============================================================================
void GWOverviewWidget::setBackgroundColor(QColor tmp) {
  if (tmp.isValid()) {
    paramDialog->background->setPaletteBackgroundColor(tmp);
    int h,s,v;
    tmp.getHsv(&h, &s, &v);
    if (v < 128)
      paramDialog->background->setPaletteForegroundColor(QColor(255, 255, 255));
    else
      paramDialog->background->setPaletteForegroundColor(QColor(0, 0, 0));
  }
}
//=============================================================================
void GWOverviewWidget::backColor() {
  setBackgroundColor(QColorDialog::getColor(paramDialog->background->paletteBackgroundColor(),
					    this, tr("Choose background color")));
  updateView();
}
//=============================================================================
void GWOverviewWidget::syncFromView() {
  if (_observedView!=0) {
    _synchronizing = true;
    GlGraphRenderingParameters param = _observedView->getScene()->getGlGraphComposite()->getRenderingParameters();
    paramDialog->arrows->setChecked( param.isViewArrow());
    paramDialog->edgeVisible->setChecked( param.isDisplayEdges());
    paramDialog->elabels->setChecked( param.isViewEdgeLabel());
    paramDialog->nlabels->setChecked( param.isViewNodeLabel());
    paramDialog->colorInterpolation->setChecked( param.isEdgeColorInterpolate());
    paramDialog->sizeInterpolation->setChecked( param.isEdgeSizeInterpolate());
    paramDialog->ordering->setChecked( param.isElementOrdered());
    paramDialog->orthogonal->setChecked(_observedView->getScene()->isViewOrtho());
    paramDialog->mlabels->setChecked( param.isViewMetaLabel());
    paramDialog->edge3D->setChecked( param.isEdge3D());
    Color tmp = _observedView->getScene()->getBackgroundColor();
    setBackgroundColor(QColor(tmp[0],tmp[1],tmp[2]));
    paramDialog->fonts->setCurrentItem(param.getFontsType());
    paramDialog->density->setValue(param.getLabelsBorder());
    ((RenderingParametersDialog *) paramDialog)->updateEdgeOptions();

    GlGraphRenderingParameters paramView = _view->getScene()->getGlGraphComposite()->getRenderingParameters();
    //paramView.setViewOrtho( param.isViewOrtho() );
    paramView.setEdgeSizeInterpolate( param.isEdgeSizeInterpolate() );
    paramView.setEdgeColorInterpolate( param.isEdgeColorInterpolate() );
    paramView.setTexturePath( param.getTexturePath() );
    paramView.setViewNodeLabel(false);
    paramView.setViewEdgeLabel(false);
    paramView.setElementOrdered(false);
    _view->getScene()->getGlGraphComposite()->setRenderingParameters(paramView);
    _view->getScene()->setViewOrtho(_observedView->getScene()->isViewOrtho());

    _synchronizing=false;
  }
}
//=============================================================================
void GWOverviewWidget::updateView() {
  if (_observedView!=0 && !_synchronizing) {
    GlGraphRenderingParameters paramObservedViev = _observedView->getScene()->getGlGraphComposite()->getRenderingParameters();

    paramObservedViev.setViewArrow(paramDialog->arrows->isChecked());
    paramObservedViev.setDisplayEdges(paramDialog->edgeVisible->isChecked());
    paramObservedViev.setViewNodeLabel(paramDialog->nlabels->isChecked());
    paramObservedViev.setViewEdgeLabel(paramDialog->elabels->isChecked());
    paramObservedViev.setEdgeColorInterpolate(paramDialog->colorInterpolation->isChecked());
    paramObservedViev.setEdgeSizeInterpolate(paramDialog->sizeInterpolation->isChecked());
    paramObservedViev.setElementOrdered(paramDialog->ordering->isChecked());
    _observedView->getScene()->setViewOrtho(paramDialog->orthogonal->isChecked());
    paramObservedViev.setViewMetaLabel(paramDialog->mlabels->isChecked());
    paramObservedViev.setEdge3D(paramDialog->edge3D->isChecked());
    paramObservedViev.setFontsType(paramDialog->fonts->currentItem());
    QColor tmp = paramDialog->background->paletteBackgroundColor();
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
}
//=============================================================================
void RectPosition::draw(float lod) {
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

  glColor4ub(RVAL,GVAL,BVAL,ALPHA);
  glVertex3fv((float *)&points2[0]);
  //  glColor4ub(255,255,255,0);
  glVertex3fv((float *)&points[0]);
  glVertex3fv((float *)&points[1]);
  //  glColor4ub(RVAL,GVAL,BVAL,ALPHA);
  glVertex3fv((float *)&points2[1]);

  //  glColor4ub(RVAL,GVAL,BVAL,ALPHA);
  glVertex3fv((float *)&points2[1]);
  //  glColor4ub(255,255,255,0);
  glVertex3fv((float *)&points[1]);
  glVertex3fv((float *)&points[2]);
  //  glColor4ub(RVAL,GVAL,BVAL,ALPHA);
  glVertex3fv((float *)&points2[2]);
  
  //  glColor4ub(RVAL,GVAL,BVAL,ALPHA);
  glVertex3fv((float *)&points2[2]);
  //  glColor4ub(255,255,255,0);
  glVertex3fv((float *)&points[2]);
  glVertex3fv((float *)&points[3]);
  //  glColor4ub(RVAL,GVAL,BVAL,ALPHA);
  glVertex3fv((float *)&points2[3]);

  //  glColor4ub(RVAL,GVAL,BVAL,ALPHA);
  glVertex3fv((float *)&points2[3]);
  //  glColor4ub(255,255,255,0);
  glVertex3fv((float *)&points[3]);
  glVertex3fv((float *)&points[0]);
  //  glColor4ub(RVAL,GVAL,BVAL,ALPHA);
  glVertex3fv((float *)&points2[0]);

  glEnd();

  glDisable(GL_BLEND);
  glColor3ub(0,0,0);
  glLineWidth(1);
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

  glPopAttrib();
}
//=============================================================================
RectPosition::RectPosition(GlGraphWidget *view, GlGraphWidget *observedView) : 
  _observedView(observedView), _view(view) {
  boundingBox=BoundingBox(Coord(-1,-1,-1),Coord(1,1,1));
}
//=============================================================================
