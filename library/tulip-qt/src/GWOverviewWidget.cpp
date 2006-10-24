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
  tlp::GlGraph *observedView;
private :
  GlGraphWidget *view;
};

//=============================================================================
GWOverviewWidget::GWOverviewWidget(QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
#if (QT_REL == 3)
  : GWOverviewWidgetData( parent, name, modal ) {
#else
  : GWOverviewWidgetData( parent, name, (Qt::WFlags) (fl | Qt::Widget) ) {
#endif
		      observedView=0;
		      glDraw=0;
		      extView->hide();
		      view = new GlGraphWidget( frame8, "view" );
		      view->setMinimumSize( QSize( 128, 128 ) );
		      view->setMaximumSize( QSize( 2000, 2000 ) );
#if (QT_REL == 3)
		      frame8Layout->addWidget( view, 0, 0 );
#else
		      gridLayout->addWidget( view, 0, 0 );
#endif
		      view->installEventFilter(this);
		      extendedViewIsVisible = false;
		      showParameters(false);
}
//=============================================================================
void GWOverviewWidget::showParameters(bool visible) {
  if (visible) {
    parameterBasic->show();
    if (extendedViewIsVisible)
      extView->show();
  }
  else {
    parameterBasic->hide();
    extView->hide();
  }
}
  
//=============================================================================
GWOverviewWidget::~GWOverviewWidget() {}
//=============================================================================
bool GWOverviewWidget::eventFilter(QObject *obj, QEvent *e) {
  if (observedView == 0) return false;
  if ( obj->inherits("GlGraphWidget") && ( (e->type() == QEvent::MouseButtonPress) || (e->type() == QEvent::MouseMove))  ) {
    QMouseEvent *me = (QMouseEvent *) e;
    GlGraphWidget *glw = (GlGraphWidget *) obj;
    if (me->state()==LeftButton || me->button()==LeftButton) {
      double mouseClicX = me->x();
      double mouseClicY = me->y();
      double widgetWidth = glw->width();
      double widgetHeight = glw->height();
      GLint *vp = new GLint[4];  
      int X,Y,W,H;
      observedView->getWinParameters(&X,&Y,&W,&H,&vp);
      delete [] vp;
      Coord upperLeftCorner(X,Y,0);
      Coord lowerRigihtCorner(X+W, Y+H, 0);
      Coord middle = (upperLeftCorner + lowerRigihtCorner) / 2.0;
      float x = middle[0];
      float y = middle[1];
      float z = 0.0;
      observedView->screenTo3DWorld(x, y , z);
      Camera cover  = glw->getCamera();
      Camera cview  = observedView->getCamera();
      glw->worldTo2DScreen(x, y, z);
      //      cerr << "Square center: " << Coord(x, y, z) << endl;
      double dx, dy, dz;
      dx = (x - mouseClicX) * W * cview.zoomFactor / (cover.zoomFactor * widgetWidth);
      dy = (y - (widgetHeight - mouseClicY)) * H * cview.zoomFactor / (cover.zoomFactor * widgetHeight);
      dz = 0;
      //      cerr << "Translation : " << Coord(dx, dy, dz) << endl;
      observedView->translateCamera((int) dx, (int) dy, 0);
      observedView->draw();
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
  Camera cam = glG->getCamera();
  cam.zoomFactor = 0.5;
  cam.eyes -= (cam.center - initialCamera.center);
  cam.center -= (cam.center - initialCamera.center);
  view->setCamera(cam);
  view->draw();
}
//=============================================================================
void GWOverviewWidget::setObservedView(GlGraph *glV){
  //  cerr << __PRETTY_FUNCTION__ << endl << flush;
  if (observedView!=0)
    observedView->removeObserver(this);
  observedView=glV;
  if (glDraw==0) {
    glDraw = new RectPosition(view,observedView);
    view->addGlAugmentedDisplay(glDraw, "Overview");
  }
  glDraw->observedView=observedView;
  if (glV!=0) {
    // cerr << "." ;
    view->setGraph(glV->getGraph());
    syncFromView();
    view->centerScene();
    initialCamera = view->getCamera();
    observedView->addObserver(this);
  }
}
//=============================================================================
void GWOverviewWidget::destroy(GlGraph *glGraph) {
  observedView=0;
  glDraw->observedView = 0;
  view->setGraph(0);
  draw(glGraph);
}
//=============================================================================
void GWOverviewWidget::backColor() {
  QColor tmp=QColorDialog::getColor(background->paletteBackgroundColor(), this, tr("Choose background color") ) ;
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
  extendedViewIsVisible = b;
}
//=============================================================================
void GWOverviewWidget::syncFromView() {
  if (observedView!=0) {
    synchronizing=true;
    arrows->setChecked( observedView->isViewArrow());
    edges->setChecked( observedView->isDisplayEdges());
    labels->setChecked( observedView->isViewLabel());
    interpolation->setChecked( observedView->isEdgeColorInterpolate());
    view->setEdgeColorInterpolate( observedView->isEdgeColorInterpolate() );
    view->setTexturePath( observedView->getTexturePath() );
    interpolationEdge->setChecked( observedView->isEdgeSizeInterpolate());
    view->setEdgeSizeInterpolate( observedView->isEdgeSizeInterpolate() );
    strahler->setChecked( observedView->isViewStrahler());
    orthogonal->setChecked( observedView->isViewOrtho());
    view->setViewOrtho( observedView->isViewOrtho() );
    metaLabel->setChecked( observedView->isViewMetaLabel());
    edge3D->setChecked( observedView->isEdged3D());
    Color tmp=observedView->getBackgroundColor();
    background->setPaletteBackgroundColor(QColor(tmp[0],tmp[1],tmp[2]));
    fonts->setCurrentItem(observedView->fontsType());
    density->setValue(observedView->getLabelsBorder());
    synchronizing=false;
  }
}
//=============================================================================
void GWOverviewWidget::updateView() {
  if (observedView!=0 && !synchronizing) {
    observedView->setViewArrow(arrows->isChecked());
    observedView->setDisplayEdges(edges->isChecked());
    observedView->setViewLabel(labels->isChecked());

    observedView->setEdgeColorInterpolate(interpolation->isChecked());
    view->setEdgeColorInterpolate( observedView->isEdgeColorInterpolate() );

    observedView->setEdgeSizeInterpolate(interpolationEdge->isChecked());
    view->setEdgeSizeInterpolate( observedView->isEdgeSizeInterpolate() );

    observedView->setViewStrahler(strahler->isChecked());
    observedView->setViewOrtho(orthogonal->isChecked());
    view->setViewOrtho(orthogonal->isChecked());
    observedView->setViewMetaLabel(metaLabel->isChecked());
    observedView->setEdge3D(edge3D->isChecked());
    observedView->setFontsType(fonts->currentItem());
    QColor tmp = background->paletteBackgroundColor();
    observedView->setBackgroundColor(Color(tmp.red(),tmp.green(),tmp.blue()));
    observedView->setLabelsBorder(density->value());
    observedView->draw();
  }
}
//=============================================================================
void RectPosition::draw(GlGraph *target) {
  if(observedView == 0) {
    return ;
  }
  float x[4],xv[4];
  float y[4],yv[4];
  float z[4],zv[4];
  GLint *vp;
  int X,Y,W,H;
  vp = new GLint[4];  
  observedView->screenTo3DWorld(x[0],y[0],z[0]);
  observedView->getWinParameters(&X,&Y,&W,&H,&vp);
  delete [] vp;
  for (int i=0;i<4;++i)
    z[i]=0;
  x[0]=X; y[0]=Y;
  x[1]=X+W; y[1]=Y;
  x[2]=X+W; y[2]=Y+H;
  x[3]=X; y[3]=Y+H;
  for (int i=0;i<4;++i)
    observedView->screenTo3DWorld(x[i],y[i],z[i]);
  vp = new GLint[4];  
  view->getWinParameters(&X,&Y,&W,&H,&vp);
  delete [] vp;
  for (int i=0;i<4;++i)
    zv[i]=0;
  xv[0]=X;   yv[0]=Y;
  xv[1]=X+W; yv[1]=Y;
  xv[2]=X+W; yv[2]=Y+H;
  xv[3]=X;   yv[3]=Y+H;
  for (int i=0;i<4;++i)
    view->screenTo3DWorld(xv[i],yv[i],zv[i]);
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
  observedView(observedView), view(view) {
}
//=============================================================================
