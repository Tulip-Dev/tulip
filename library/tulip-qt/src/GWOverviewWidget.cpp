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
      float x = middle[0];
      float y = middle[1];
      float z = 0.0;
      _observedView->screenTo3DWorld(x, y , z);
      Camera cover  = _view->getRenderingParameters().getCamera();
      Camera cview  = _observedView->getRenderingParameters().getCamera();
      _view->worldTo2DScreen(x, y, z);
      //      cerr << "Square center: " << Coord(x, y, z) << endl;
      double dx, dy, dz;
      dx = (x - mouseClicX) * viewport[2] * cview.zoomFactor / (cover.zoomFactor * widgetWidth);
      dy = (y - (widgetHeight - mouseClicY)) * viewport[3] * cview.zoomFactor / (cover.zoomFactor * widgetHeight);
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
  float x[4],xv[4];
  float y[4],yv[4];
  float z[4],zv[4];
  _observedView->screenTo3DWorld(x[0],y[0],z[0]);

  Vector<int, 4> viewport = _observedView->getRenderingParameters().getViewport();
  int X,Y,W,H;
  X = viewport[0];
  Y = viewport[1];
  W = viewport[2];
  H = viewport[3];

  for (int i=0;i<4;++i) z[i]=0;
  x[0]=X; y[0]=Y;
  x[1]=X+W; y[1]=Y;
  x[2]=X+W; y[2]=Y+H;
  x[3]=X; y[3]=Y+H;
  for (int i=0;i<4;++i)
    _observedView->screenTo3DWorld(x[i],y[i],z[i]);


  viewport = _view->getRenderingParameters().getViewport();
  X = viewport[0];
  Y = viewport[1];
  W = viewport[2];
  H = viewport[3];

  for (int i=0;i<4;++i)
    zv[i]=0;
  xv[0]=X;   yv[0]=Y;
  xv[1]=X+W; yv[1]=Y;
  xv[2]=X+W; yv[2]=Y+H;
  xv[3]=X;   yv[3]=Y+H;
  for (int i=0;i<4;++i)
    _view->screenTo3DWorld(xv[i],yv[i],zv[i]);
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
  glVertex3d(xv[0],yv[0],zv[0]);
  glColor4ub(255,255,255,100);
  glVertex3d(x[0],y[0],z[0]);
  glVertex3d(x[1],y[1],z[1]);
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3d(xv[1],yv[1],zv[1]);

  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3d(xv[1],yv[1],zv[1]);
  glColor4ub(255,255,255,100);
  glVertex3d(x[1],y[1],z[1]);
  glVertex3d(x[2],y[2],z[2]);
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3d(xv[2],yv[2],zv[2]);

  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3d(xv[2],yv[2],zv[2]);
  glColor4ub(255,255,255,100);
  glVertex3d(x[2],y[2],z[2]);
  glVertex3d(x[3],y[3],z[3]);
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3d(xv[3],yv[3],zv[3]);

  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3d(xv[3],yv[3],zv[3]);
  glColor4ub(255,255,255,100);
  glVertex3d(x[3],y[3],z[3]);
  glVertex3d(x[0],y[0],z[0]);
  glColor4ub(RVAL,GVAL,BVAL,200);
  glVertex3d(xv[0],yv[0],zv[0]);

  glEnd();
  glDisable(GL_BLEND);
  glColor3ub(0,0,0);
  glLineWidth(1);
  glBegin(GL_LINE_LOOP);
  for (int i=0;i<4;++i)
    glVertex3d(x[i],y[i],z[i]);
  glEnd();
  glLineWidth(1);
  glEnable(GL_CULL_FACE);
  glLineStipple(2, 0xAAAA);
  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  for (int i=0;i<4;++i) {
    glVertex3d(xv[i],yv[i],zv[i]);
    glVertex3d(x[i],y[i],z[i]);
  }
  glEnd();
  glPopAttrib();
}
//=============================================================================
RectPosition::RectPosition(GlGraphWidget *view, GlGraph *observedView) : 
  _observedView(observedView), _view(view) {
}
//=============================================================================
