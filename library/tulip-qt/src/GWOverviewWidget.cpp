#include <qcolor.h>
#include <qslider.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qframe.h>
#include <qevent.h>

#include "tulip/GWOverviewWidget.h"
#include "tulip/GlGraphWidget.h"

using namespace std;
using namespace tlp;

struct RectPosition : public tlp::GlAugmentedDisplay {
  void draw(tlp::GlGraph *);
  RectPosition(GlGraphWidget *, tlp::GlGraph *);
  void setObservedView(GlGraph *glG) {
    _observedView = glG;
  }
private :
  tlp::GlGraph  * _observedView;
  GlGraphWidget * _view;
};

//=============================================================================
GWOverviewWidget::GWOverviewWidget(QWidget* parent,  
				   const char* name, bool modal, Qt::WFlags fl )
#if (QT_REL == 3)
  : GWOverviewWidgetData( parent, name, modal )
#else
  : GWOverviewWidgetData( parent, name, (Qt::WFlags) (fl | Qt::Widget) ) 
#endif
{
  _observedView = 0;
  _glDraw = 0;
  _extendedViewIsVisible = false;
  
  extView->hide();
  _view = new GlGraphWidget( frame8, "view" );
  _view->setMinimumSize( QSize( 128, 128 ) );
  _view->setMaximumSize( QSize( 2000, 2000 ) );
#if (QT_REL == 3)
  frame8Layout->addWidget( _view, 0, 0 );
#else
  gridLayout->addWidget( _view, 0, 0 );
#endif
  _view->installEventFilter(this);
  _glDraw = new RectPosition(_view, 0);
  _view->addGlAugmentedDisplay(_glDraw, "Overview");

  showParameters(false);
}
//=============================================================================
void GWOverviewWidget::showParameters(bool visible) {
  if (visible) {
    parameterBasic->show();
    if (_extendedViewIsVisible)
      extView->show();
  }
  else {
    parameterBasic->hide();
    extView->hide();
  }
}
//=============================================================================
GWOverviewWidget::~GWOverviewWidget() {
  if (_observedView != 0) _observedView->removeObserver(this);
  delete _view;
  delete _glDraw;
}
//=============================================================================
bool GWOverviewWidget::eventFilter(QObject *obj, QEvent *e) {
  if (_observedView == 0) return false;
  if ( obj->inherits("GlGraphWidget") && ( (e->type() == QEvent::MouseButtonPress) || (e->type() == QEvent::MouseMove))  ) {
    QMouseEvent *me = (QMouseEvent *) e;
    GlGraphWidget *glw = (GlGraphWidget *) obj;
    assert(glw == _view);
    if (me->state()==LeftButton || me->button()==LeftButton) {
      double mouseClicX = me->x();
      double mouseClicY = me->y();
      double widgetWidth = _view->width();
      double widgetHeight = _view->height();
      Vector<int, 4> viewport = _observedView->getRenderingParameters().getViewport();
      Coord upperLeftCorner(viewport[0], viewport[1],0);
      Coord lowerRigihtCorner(viewport[0] + viewport[2], 
			      viewport[1] + viewport[3], 
			      0);
      Coord middle = (upperLeftCorner + lowerRigihtCorner) / 2.0;
      middle[2] = 0.;
      middle = _observedView->screenTo3DWorld(middle);
      Camera cover  = _view->getRenderingParameters().getCamera();
      Camera cview  = _observedView->getRenderingParameters().getCamera();
      middle = _view->worldTo2DScreen(middle);
      //      cerr << "Square center: " << Coord(x, y, z) << endl;
      float dx, dy, dz;
      dx = (middle[0] - mouseClicX) * viewport[2] * cview.zoomFactor / (cover.zoomFactor * widgetWidth);
      dy = (middle[1] - (widgetHeight - mouseClicY)) * viewport[3] * cview.zoomFactor / (cover.zoomFactor * widgetHeight);
      dz = 0;
      //      cerr << "Translation : " << Coord(dx, dy, dz) << endl;
      _observedView->translateCamera((int) dx, (int) dy, 0);
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
void GWOverviewWidget::draw(GlGraph *glG) {
  assert( glG == _observedView);
  if (_observedView != 0) {
    _view->centerScene();
    _initialCamera = _view->getRenderingParameters().getCamera();   
    Camera cam = _observedView->getRenderingParameters().getCamera();
    cam.zoomFactor = 0.5;
    cam.eyes -= (cam.center - _initialCamera.center);
    cam.center -= (cam.center - _initialCamera.center);
    GlGraphRenderingParameters newParam = _view->getRenderingParameters();
    newParam.setCamera(cam);
    _view->setRenderingParameters(newParam);
  }
  _view->draw();
}
//=============================================================================
void GWOverviewWidget::setObservedView(GlGraph *glWidget){
  cerr << __PRETTY_FUNCTION__ << glWidget << endl << flush;
  if (_observedView != 0)
    _observedView->removeObserver(this);
  _observedView = glWidget;
  _glDraw->setObservedView(glWidget);
  
  if (glWidget != 0) {

    GlGraphRenderingParameters newParam = _observedView->getRenderingParameters();
    newParam.setViewport(_view->getRenderingParameters().getViewport());
    _view->setRenderingParameters(newParam);
    syncFromView();
    //    _view->centerScene();
    //    _initialCamera = _view->getRenderingParameters().getCamera();
    _observedView->addObserver(this);
  }
}
//=============================================================================
void GWOverviewWidget::destroy(GlGraph *glGraph) {
  assert(_observedView == glGraph);
  _observedView = 0;
  _glDraw->setObservedView(0);
  GlGraphRenderingParameters param = _view->getRenderingParameters();
  param.setGraph(0);
  _view->setRenderingParameters(param);
  draw(0);
}
//=============================================================================
void GWOverviewWidget::backColor() {
  QColor tmp = QColorDialog::getColor(background->paletteBackgroundColor(), this, tr("Choose background color") ) ;
  if (tmp.isValid()) {
    background->setPaletteBackgroundColor(tmp);
    updateView();
  }
}
//=============================================================================
void GWOverviewWidget::showExtendedParameters(bool b) {
  if (!b)
    extView->hide();
  else
    extView->show();
  _extendedViewIsVisible = b;
}
//=============================================================================
void GWOverviewWidget::syncFromView() {
  if (_observedView!=0) {
    _synchronizing = true;
    GlGraphRenderingParameters param = _observedView->getRenderingParameters();
    arrows->setChecked( param.isViewArrow());
    edges->setChecked( param.isDisplayEdges());
    labels->setChecked( param.isViewNodeLabel());
    interpolation->setChecked( param.isEdgeColorInterpolate());
    interpolationEdge->setChecked( param.isEdgeSizeInterpolate());
    strahler->setChecked( param.isElementOrdered());
    orthogonal->setChecked( param.isViewOrtho());
    metaLabel->setChecked( param.isViewMetaLabel());
    edge3D->setChecked( param.isEdge3D());
    Color tmp = param.getBackgroundColor();
    background->setPaletteBackgroundColor(QColor(tmp[0],tmp[1],tmp[2]));
    fonts->setCurrentItem(param.getFontsType());
    density->setValue(param.getLabelsBorder());

    GlGraphRenderingParameters paramView = _view->getRenderingParameters();
    paramView.setViewOrtho( param.isViewOrtho() );
    paramView.setEdgeSizeInterpolate( param.isEdgeSizeInterpolate() );
    paramView.setEdgeColorInterpolate( param.isEdgeColorInterpolate() );
    paramView.setTexturePath( param.getTexturePath() );
    paramView.setViewNodeLabel(false);
    paramView.setViewEdgeLabel(false);
    paramView.setElementOrdered(false);
    _view->setRenderingParameters(paramView);

    _synchronizing=false;
  }
}
//=============================================================================
void GWOverviewWidget::updateView() {
  if (_observedView!=0 && !_synchronizing) {
    GlGraphRenderingParameters paramObservedViev = _observedView->getRenderingParameters();

    paramObservedViev.setViewArrow(arrows->isChecked());
    paramObservedViev.setDisplayEdges(edges->isChecked());
    paramObservedViev.setViewNodeLabel(labels->isChecked());
    paramObservedViev.setEdgeColorInterpolate(interpolation->isChecked());
    paramObservedViev.setEdgeSizeInterpolate(interpolationEdge->isChecked());
    paramObservedViev.setElementOrdered(strahler->isChecked());
    paramObservedViev.setViewOrtho(orthogonal->isChecked());
    paramObservedViev.setViewMetaLabel(metaLabel->isChecked());
    paramObservedViev.setEdge3D(edge3D->isChecked());
    paramObservedViev.setFontsType(fonts->currentItem());
    QColor tmp = background->paletteBackgroundColor();
    paramObservedViev.setBackgroundColor(Color(tmp.red(),tmp.green(),tmp.blue()));
    paramObservedViev.setLabelsBorder(density->value());
    _observedView->setRenderingParameters(paramObservedViev);

    GlGraphRenderingParameters paramView = _view->getRenderingParameters();
    paramView.setBackgroundColor( paramObservedViev.getBackgroundColor() );
    paramView.setEdgeColorInterpolate( paramObservedViev.isEdgeColorInterpolate() );
    paramView.setViewOrtho(paramObservedViev.isViewOrtho());
    paramView.setEdgeSizeInterpolate(paramObservedViev.isEdgeSizeInterpolate() );
    paramView.setViewNodeLabel(false);
    paramView.setViewEdgeLabel(false);
    paramView.setElementOrdered(false);
    _view->setRenderingParameters(paramView);
    
    _observedView->draw();
  }
}
//=============================================================================
void RectPosition::draw(GlGraph *target) {
  assert (_view == target);
  if(_observedView == 0) {
    return ;
  }
  Vector<Coord, 4> points;
  Vector<int,   4> viewport = _observedView->getRenderingParameters().getViewport();
  points[0] = Coord(viewport[0]              , viewport[1], 0);
  points[1] = Coord(viewport[0] + viewport[2], viewport[1], 0);
  points[2] = Coord(viewport[0] + viewport[2], viewport[1] + viewport[3], 0.0);
  points[3] = Coord(viewport[0]              , viewport[1] + viewport[3], 0.0);
  for (int i=0;i<4;++i)
    points[i] = _observedView->screenTo3DWorld(points[i]);
  

  viewport = _view->getRenderingParameters().getViewport();
  Vector<Coord, 4> points2;
  points2[0] = Coord(viewport[0]              , viewport[1], 0);
  points2[1] = Coord(viewport[0] + viewport[2], viewport[1], 0);
  points2[2] = Coord(viewport[0] + viewport[2], viewport[1] + viewport[3], 0.0);
  points2[3] = Coord(viewport[0]              , viewport[1] + viewport[3], 0.0);
  for (int i=0;i<4;++i)
    points2[i] = _view->screenTo3DWorld(points2[i]);
  
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
  float col[4]={0.2,0.8,0.8,0.2};
  glColor4fv(col);
  glBegin(GL_QUADS);
  /*
  for (int i=0;i<4;++i)
    glVertex3d(x[i],y[i],z[i]);
  */
#define RVAL 59
#define GVAL 90
#define BVAL 233
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3fv((float *)&points2[0]);
  glColor4ub(255,255,255,100);
  glVertex3fv((float *)&points[0]);
  glVertex3fv((float *)&points[1]);
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3fv((float *)&points2[1]);

  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3fv((float *)&points2[1]);
  glColor4ub(255,255,255,100);
  glVertex3fv((float *)&points[1]);
  glVertex3fv((float *)&points[2]);
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3fv((float *)&points2[2]);
  
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3fv((float *)&points2[2]);
  glColor4ub(255,255,255,100);
  glVertex3fv((float *)&points[2]);
  glVertex3fv((float *)&points[3]);
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3fv((float *)&points2[3]);

  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3fv((float *)&points2[3]);
  glColor4ub(255,255,255,100);
  glVertex3fv((float *)&points[3]);
  glVertex3fv((float *)&points[0]);
  glColor4ub(RVAL,GVAL,BVAL,200);
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
  glEnable(GL_CULL_FACE);
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
RectPosition::RectPosition(GlGraphWidget *view, GlGraph *observedView) : 
  _observedView(observedView), _view(view) {
}
//=============================================================================
