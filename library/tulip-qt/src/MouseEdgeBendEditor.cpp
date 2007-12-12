#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include <string>
#include <sstream>
#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlGraphWidget.h>
#include <tulip/DrawingTools.h>
#include <tulip/ForEach.h>

#include <tulip/MouseEdgeBendEditor.h>

#if (QT_REL == 3)
#include <qevent.h>
// include below is for compilation purpose only
#include <qcursor.h>
#else
#include <QtGui/qevent.h>
#include "tulip/Qt3ForTulip.h"
#endif

/*#define EPSILON 1.0
#define EPSILON_SCREEN 50
#define EPSILON_STRETCH_MIN 1 - 1.0e-01
#define EPSILON_STRETCH_MAX 1 + 1.0e-01*/
#define EPSILON 10

using namespace tlp;
using namespace std;
//========================================================================================
MouseEdgeBendEditor::MouseEdgeBendEditor() 
  :glGraphWidget(NULL){
  operation = NONE_OP;
  _copyLayout = 0;
  _copySizes = 0;
  _copyRotation = 0;

  basicCircle.resizePoints(30); //circle
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.fcolor(0) = Color(255, 102, 255, 200);
  basicCircle.ocolor(0) = Color(128, 20, 20, 200);
    
  /*basicCircle.setRenderState(GlAD_ZEnable, false);
  basicCircle.setRenderState(GlAD_Culling, false);
  basicCircle.setRenderState(GlAD_AlphaBlending, true);
  basicCircle.setRenderState(GlAD_Wireframe, false);
  basicCircle.setRenderState(GlAD_Solid, true);*/
}
//========================================================================================
bool MouseEdgeBendEditor::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;
    initProxies(glGraphWidget);
    bool hasSelection = computeBendsCircles(glGraphWidget);
				
    editPosition[0] = qMouseEv->x();
    editPosition[1] = qMouseEv->y();
    editPosition[2] = 0;

    //  cerr << "edit pos:" << editPosition << endl;

    switch(qMouseEv->button()) {
    case Qt::LeftButton : {
      // first ensure that something is selected
      /* bool hasSelection = false;
      edge ed;
      forEach(ed, _selection->getEdgesEqualTo(true, _graph)) {
	hasSelection = true;
	breakForEach;
	}*/
      if (!hasSelection) {
	// event occurs outside the selection rectangle
	// so from now we delegate the job to a MouseEdgeSelector object
	// which should intercept the event
	operation = NONE_OP;
	return false;
      }
      if (qMouseEv->state() & Qt::ShiftButton){//vérifier que le lieu du clic est sur l'arête
	operation = NEW_OP;
	//int newX = qMouseEv->x();
	//int newY = qMouseEv->y();
	//cout << "C R E A T E   C A L L : " << endl;
	//cout << "===================================" << endl;
	mMouseCreate(editPosition[0], editPosition[1], glGraphWidget);
      } else {
	bool circleSelected =
	  glGraphWidget->selectGlEntities((int)editPosition[0] - 3, (int)editPosition[1] - 3, 6, 6, select, glGraphWidget->getScene()->getSelectionLayer());
	if (circleSelected) {
	  theCircle=circleString.findKey((GlSimpleEntity*)(select[0]));
	  //(&circles[i])->fcolor(0) = Color(40,255,40,200);
	  //(&circles[i])->ocolor(0) = Color(20,128,20,200);
	  if (qMouseEv->state() &
#if defined(__APPLE__)
	      Qt::AltButton
#else
	      Qt::ControlButton
#endif
	      ){
	    operation = DELETE_OP;
	    mMouseDelete();
	  }
	  else
	    {
	      operation = TRANSLATE_OP;
	      glGraphWidget->setCursor(QCursor(Qt::SizeAllCursor));
	      initEdition();
	      mode = COORD;
	    }
	} else{
	  operation = NONE_OP;
	  return false;
	}
      }
      break;
    }
    case Qt::MidButton :
      undoEdition();
      glGraphWidget->setCursor(QCursor(Qt::ArrowCursor));
      break;
    default: return false;
    }
    glGraphWidget->redraw();
    return true;
  }
  if (e->type() == QEvent::MouseButtonRelease && ((QMouseEvent *) e)->button() == Qt::LeftButton && operation != NONE_OP) {
    GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;
    stopEdition();
    glGraphWidget->setCursor(QCursor(Qt::ArrowCursor));
    glGraphWidget->redraw();
    return true;
  }
  if  (e->type() == QEvent::MouseMove && ((QMouseEvent *) e)->state() & Qt::LeftButton && operation != NONE_OP) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;
    int newX = qMouseEv->x();
    int newY = qMouseEv->y();
    //int i=0;
    //while(&circles[i] != select[0].second) i++;
    switch (operation) {
    case TRANSLATE_OP:
      mMouseTranslate(newX, newY, glGraphWidget);
      return true;
    case NONE_OP:
      cerr << "[Error] : " <<__FUNCTION__ << " should not be call" << endl;
      break;
    }
  }
  return false;
}
//========================================================================================
bool MouseEdgeBendEditor::compute(GlGraphWidget *glGraphWidget) {
  if (computeBendsCircles(glGraphWidget)) {
    glGraphWidget->getScene()->getSelectionLayer()->addGlEntity(&circleString,"EdgeBendEditorComposite");
    this->glGraphWidget=glGraphWidget;
  }
}
//========================================================================================
bool MouseEdgeBendEditor::draw(GlGraphWidget *glGraphWidget) {
  return true;
}
//========================================================================================
void MouseEdgeBendEditor::restoreInfo() {
  assert(_copyLayout != 0);
  assert(_copySizes != 0);
  assert(_copyRotation != 0);
  edge e;
  forEach(e, _selection->getEdgesEqualTo(true, _graph)) {
    _rotation->setEdgeValue(e, _copyRotation->getEdgeValue(e));
    _layout->setEdgeValue(e, _copyLayout->getEdgeValue(e));
    _sizes->setEdgeValue(e, _copySizes->getEdgeValue(e));
  }
}
//========================================================================================
void MouseEdgeBendEditor::saveInfo() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  assert(_copyLayout == 0);
  assert(_copySizes == 0);
  assert(_copyRotation == 0);
  _copyRotation = new DoubleProperty(_graph);
  _copyLayout   = new LayoutProperty(_graph);
  _copySizes    = new SizeProperty(_graph);
  edge e;
  forEach(e, _selection->getEdgesEqualTo(true, _graph)) {
    _copyRotation->setEdgeValue(e, _rotation->getEdgeValue(e));
    _copyLayout->setEdgeValue(e, _layout->getEdgeValue(e));
    _copySizes->setEdgeValue(e, _sizes->getEdgeValue(e));
  }
}
//========================================================================================
void MouseEdgeBendEditor::initEdition() {
  saveInfo();
}
//========================================================================================
void MouseEdgeBendEditor::undoEdition() {
  if (operation == NONE_OP) return;
  restoreInfo();
  operation = NONE_OP;
  delete _copyLayout;   _copyLayout = 0;
  delete _copySizes;    _copySizes = 0;
  delete _copyRotation; _copyRotation = 0;
}
//========================================================================================
void MouseEdgeBendEditor::stopEdition() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (operation == NONE_OP) return;
  operation = NONE_OP;
  delete _copyLayout;   _copyLayout = 0;
  delete _copySizes;    _copySizes = 0;
  delete _copyRotation; _copyRotation = 0;
}
//========================================================================================
void MouseEdgeBendEditor::initProxies(GlGraphWidget *glGraphWidget) {
  _graph     = glGraphWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  _layout    = _graph->getProperty<LayoutProperty>("viewLayout");
  _selection = _graph->getProperty<BooleanProperty>("viewSelection");
  _rotation  = _graph->getProperty<DoubleProperty>("viewRotation");
  _sizes     = _graph->getProperty<SizeProperty>("viewSize");
}
//========================================================================================
//Does the point p belong to the segment [u,v]? 
bool MouseEdgeBendEditor::belong(Coord u, Coord v, Coord p, GlGraphWidget *glGraphWidget)
{
  int W = glGraphWidget->width();
  int H = glGraphWidget->height();
  Coord m=glGraphWidget->getScene()->getLayer("Main")->getCamera()->worldTo2DScreen(u);
  m[0] = W - m[0];
  m[1] = H - m[1];
  Coord n=glGraphWidget->getScene()->getLayer("Main")->getCamera()->worldTo2DScreen(v);
  n[0] = W - n[0];
  n[1] = H - n[1];
  double mnDist = m.dist(n);
  double mpDist = m.dist(p);
  double pnDist = p.dist(n);
  return ((mpDist + pnDist) - mnDist)/mnDist < 1E-3;
  /*int W = glGraphWidget->width();
  int H = glGraphWidget->height();
  Coord m=glGraphWidget->worldTo2DScreen(u);
  //m[0] = (double)W - m[0];
  m[1] = (double)H - m[1];
  m[2] = 0;
  cout << PointType::toString(m) << endl;
  Coord n=glGraphWidget->worldTo2DScreen(v);
  //n[0] = (double)W - n[0];
  n[1] = (double)H - n[1];
  n[2] = 0;
  cout << PointType::toString(n) << endl;
  double lambda=(p[0]-m[0])/(n[0]-m[0]);
  double mu=(p[1]-u[1])/(v[1]-u[1]);
  double delta=abs(lambda-mu);
  //return(delta<=1.0e-01 && abs(mu)<=1);
  return(abs(mu)<=1); */
}
//========================================================================================
void MouseEdgeBendEditor::mMouseTranslate(double newX, double newY, GlGraphWidget *glGraphWidget) {
  // cerr << __PRETTY_FUNCTION__ << endl;
initProxies(glGraphWidget);
	
  Coord v0(0,0,0);
  Coord v1((double)(editPosition[0] - newX), -(double)(editPosition[1] - newY),0);
  v0 = glGraphWidget->getScene()->getLayer("Main")->getCamera()->screenTo3DWorld(v0);
  v1 = glGraphWidget->getScene()->getLayer("Main")->getCamera()->screenTo3DWorld(v1);
  v1 -= v0;
  //cout << "T R A N S L A T E : " << theCircle << endl;
  //cout << "===================================" << endl;
  int i;
  IntegerType::fromString(i, theCircle);
  coordinates[i]+=v1;
  Observable::holdObservers();
  _layout->setEdgeValue(mEdge, coordinates);
  Observable::unholdObservers();
  editPosition[0]  = newX;
  editPosition[1]  = newY;
}
//========================================================================================
void MouseEdgeBendEditor::mMouseDelete()
{
  int i;
  IntegerType::fromString(i, theCircle);
  vector<Coord>::iterator CoordIt=coordinates.begin();
  vector<tlp::GlCircle>::iterator circleIt=circles.begin();
  int tmp=0;
  while(tmp!=i){
    tmp++;CoordIt++;circleIt++;
  }
  //cout << "D E L E T E : " << theCircle << endl;
  //cout << "===================================" << endl;
  coordinates.erase(CoordIt);
  circles.erase(circleIt);
  Observable::holdObservers();
  _layout->setEdgeValue(mEdge, coordinates);
  Observable::unholdObservers();
}
//========================================================================================
void MouseEdgeBendEditor::mMouseCreate(double x, double y, GlGraphWidget *glGraphWidget) {
  Coord screenClick(glGraphWidget->width() - (double) x, (double) y, 0);
  //cout << PointType::toString(screenClick) << endl;
  Coord worldLocation= glGraphWidget->getScene()->getLayer("Main")->getCamera()->screenTo3DWorld(screenClick);
  if(coordinates.empty())
    coordinates.push_back(worldLocation);
  else{
    Coord first=coordinates[0];
    Coord last=coordinates[coordinates.size() - 1];
    bool firstSeg=belong(start, first, screenClick, glGraphWidget);
    bool lastSeg=belong(end, last, screenClick, glGraphWidget);
    if(firstSeg)
      coordinates.insert(coordinates.begin(),worldLocation);
    if(lastSeg)
      coordinates.push_back(worldLocation);
    if(!firstSeg && !lastSeg){
      bool midSeg;
      vector<Coord>::iterator CoordIt=coordinates.begin();
      last=Coord(CoordIt->getX(), CoordIt->getY(), CoordIt->getZ());
      CoordIt++;
      while(CoordIt!=coordinates.end())
	{
	  first=last;
	  last=Coord(CoordIt->getX(), CoordIt->getY(), CoordIt->getZ());
	  midSeg=belong(first, last, screenClick, glGraphWidget);
	  if(midSeg)
	    coordinates.insert(CoordIt, worldLocation);
	  CoordIt++;
	}
      //delete CoordIt;
    }
  }
  //cout << "C R E A T E : " << endl;
  //cout << "===================================" << endl;
  Observable::holdObservers();
  _layout->setEdgeValue(mEdge, coordinates);
  Observable::unholdObservers();
}
//========================================================================================
bool MouseEdgeBendEditor::computeBendsCircles(GlGraphWidget *glGraphWidget) {
  initProxies(glGraphWidget);
  Coord tmp;
  bool hasSelection=false;
  coordinates.clear();
  circles.clear();
  select.clear();
  edge ite;
  circleString.reset(false);
  int W = glGraphWidget->width();
  int H = glGraphWidget->height();
  Iterator<edge> *itE=_graph->getEdges();
  while (itE->hasNext()) {
    ite=itE->next();
    if (_selection->getEdgeValue(ite)) {
      mEdge=ite;
      hasSelection=true;
      coordinates=_layout->getEdgeValue(ite);
      start=_layout->getNodeValue(_graph->source(mEdge));
      end=_layout->getNodeValue(_graph->target(mEdge));
      vector<Coord>::iterator CoordIt=coordinates.begin();
      while(CoordIt!=coordinates.end()) {
	tmp=Coord(CoordIt->getX(), CoordIt->getY(), CoordIt->getZ());
	tmp=glGraphWidget->getScene()->getLayer("Main")->getCamera()->worldTo2DScreen(tmp);
	//tmp[1] = (double)H - tmp[1];
	//tmp[0] = (double)W - tmp[0];
	basicCircle.set(tmp, 5, 0.);
	circles.push_back(basicCircle);
	CoordIt++;
      }
      //delete CoordIt;
    }
  }
  delete itE;
  for(int i=0;i<circles.size();i++)
    circleString.addGlEntity(&circles[i], IntegerType::toString(i));
  return hasSelection;
}
//========================================================================================
