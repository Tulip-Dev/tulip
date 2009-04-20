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
#include <tulip/GlMainWidget.h>
#include <tulip/DrawingTools.h>
#include <tulip/ForEach.h>

#include <tulip/MouseEdgeBendEditor.h>

#include <QtGui/qevent.h>

using namespace tlp;
using namespace std;

//========================================================================================
MouseEdgeBendEditor::MouseEdgeBendEditor()
  :glMainWidget(NULL){
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
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    initProxies(glMainWidget);
    bool hasSelection = computeBendsCircles(glMainWidget);

    editPosition[0] = qMouseEv->x();
    editPosition[1] = qMouseEv->y();
    editPosition[2] = 0;

    //  cerr << "edit pos:" << editPosition << endl;

    switch(qMouseEv->buttons()) {
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
      if (qMouseEv->modifiers() & Qt::ShiftModifier){//vérifier que le lieu du clic est sur l'arête
        operation = NEW_OP;
        //int newX = qMouseEv->x();
        //int newY = qMouseEv->y();
        //cout << "C R E A T E   C A L L : " << endl;
        //cout << "===================================" << endl;
        mMouseCreate(editPosition[0], editPosition[1], glMainWidget);
      } else {
        bool circleSelected =
          glMainWidget->selectGlEntities((int)editPosition[0] - 3, (int)editPosition[1] - 3, 6, 6, select, glMainWidget->getScene()->getSelectionLayer());
        if (circleSelected) {
          theCircle=circleString.findKey((GlSimpleEntity*)(select[0]));
          //(&circles[i])->fcolor(0) = Color(40,255,40,200);
          //(&circles[i])->ocolor(0) = Color(20,128,20,200);
          if (qMouseEv->modifiers() &
#if defined(__APPLE__)
              Qt::AltModifier
#else
              Qt::ControlModifier
#endif
          ){
            operation = DELETE_OP;
            mMouseDelete();
          }
          else
          {
            operation = TRANSLATE_OP;
            glMainWidget->setCursor(QCursor(Qt::SizeAllCursor));
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
      glMainWidget->setCursor(QCursor(Qt::ArrowCursor));
      break;
    default: return false;
    }
    glMainWidget->redraw();
    return true;
  }
  if (e->type() == QEvent::MouseButtonRelease &&
      ((QMouseEvent *) e)->button() == Qt::LeftButton &&
      operation != NONE_OP) {
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    stopEdition();
    glMainWidget->setCursor(QCursor(Qt::ArrowCursor));
    glMainWidget->redraw();
    return true;
  }
  if  (e->type() == QEvent::MouseMove &&
      ((QMouseEvent *) e)->buttons() == Qt::LeftButton &&
      operation != NONE_OP) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    int newX = qMouseEv->x();
    int newY = qMouseEv->y();
    //int i=0;
    //while(&circles[i] != select[0].second) i++;
    switch (operation) {
    case TRANSLATE_OP:
      mMouseTranslate(newX, newY, glMainWidget);
      return true;
    case NONE_OP:
      cerr << "[Error] : " <<__FUNCTION__ << " should not be call" << endl;
    default:
      return false;
    }
  }
  return false;
}
//========================================================================================
bool MouseEdgeBendEditor::compute(GlMainWidget *glMainWidget) {
  if (computeBendsCircles(glMainWidget)) {
    glMainWidget->getScene()->getSelectionLayer()->addGlEntity(&circleString,"EdgeBendEditorComposite");
    this->glMainWidget=glMainWidget;
    return true;
  }
  return false;
}
//========================================================================================
bool MouseEdgeBendEditor::draw(GlMainWidget *glMainWidget) {
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
  // allow to undo
  _graph->push();
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
void MouseEdgeBendEditor::initProxies(GlMainWidget *glMainWidget) {
  _graph     = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  _layout    = _graph->getProperty<LayoutProperty>("viewLayout");
  _selection = _graph->getProperty<BooleanProperty>("viewSelection");
  _rotation  = _graph->getProperty<DoubleProperty>("viewRotation");
  _sizes     = _graph->getProperty<SizeProperty>("viewSize");
}
//========================================================================================
//Does the point p belong to the segment [u,v]?
bool MouseEdgeBendEditor::belong(Coord u, Coord v, Coord p, GlMainWidget *glMainWidget)
{
  int W = glMainWidget->width();
  int H = glMainWidget->height();
  Coord m=glMainWidget->getScene()->getLayer("Main")->getCamera()->worldTo2DScreen(u);
  m[0] = W - m[0];
  m[1] = H - m[1];
  Coord n=glMainWidget->getScene()->getLayer("Main")->getCamera()->worldTo2DScreen(v);
  n[0] = W - n[0];
  n[1] = H - n[1];
  double mnDist = m.dist(n);
  double mpDist = m.dist(p);
  double pnDist = p.dist(n);
  return ((mpDist + pnDist) - mnDist)/mnDist < 1E-3;
  /*int W = glMainWidget->width();
  int H = glMainWidget->height();
  Coord m=glMainWidget->worldTo2DScreen(u);
  //m[0] = (double)W - m[0];
  m[1] = (double)H - m[1];
  m[2] = 0;
  cout << PointType::toString(m) << endl;
  Coord n=glMainWidget->worldTo2DScreen(v);
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
void MouseEdgeBendEditor::mMouseTranslate(double newX, double newY, GlMainWidget *glMainWidget) {
  // cerr << __PRETTY_FUNCTION__ << endl;
initProxies(glMainWidget);

  Coord v0(0,0,0);
  Coord v1((double)(editPosition[0] - newX), -(double)(editPosition[1] - newY),0);
  v0 = glMainWidget->getScene()->getLayer("Main")->getCamera()->screenTo3DWorld(v0);
  v1 = glMainWidget->getScene()->getLayer("Main")->getCamera()->screenTo3DWorld(v1);
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
  // allow to undo
  _graph->push();
  _layout->setEdgeValue(mEdge, coordinates);
  Observable::unholdObservers();
}
//========================================================================================
void MouseEdgeBendEditor::mMouseCreate(double x, double y, GlMainWidget *glMainWidget) {
  Coord screenClick(glMainWidget->width() - (double) x, (double) y, 0);
  //cout << PointType::toString(screenClick) << endl;
  Coord worldLocation= glMainWidget->getScene()->getLayer("Main")->getCamera()->screenTo3DWorld(screenClick);
  if(coordinates.empty())
    coordinates.push_back(worldLocation);
  else{
    Coord first=coordinates[0];
    Coord last=coordinates[coordinates.size() - 1];
    bool firstSeg=belong(start, first, screenClick, glMainWidget);
    bool lastSeg=belong(end, last, screenClick, glMainWidget);
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
	  midSeg=belong(first, last, screenClick, glMainWidget);
	  if(midSeg){
	    coordinates.insert(CoordIt, worldLocation);
	    break;
	  }
	  CoordIt++;
	}
      //delete CoordIt;
    }
  }
  //cout << "C R E A T E : " << endl;
  //cout << "===================================" << endl;
  Observable::holdObservers();
  // allow to undo
  _graph->push();
  _layout->setEdgeValue(mEdge, coordinates);
  Observable::unholdObservers();
}
//========================================================================================
bool MouseEdgeBendEditor::computeBendsCircles(GlMainWidget *glMainWidget) {
  initProxies(glMainWidget);

  Iterator<edge> *itE;
  edge ite;
  bool hasSelection=false;

  //Verify if we have only one selected edge
  //  if we have more than one selected : deselect all edges
  itE =_graph->getEdges();
  while (itE->hasNext()) {
    ite=itE->next();
    if(_selection->getEdgeValue(ite)){
      if(hasSelection){
        _selection->setAllEdgeValue(false);
        hasSelection=false;
        break;
      }else{
        hasSelection=true;
      }
    }
  }

  Coord tmp;
  coordinates.clear();
  circles.clear();
  select.clear();
  circleString.reset(false);
  //int W = glMainWidget->width();
  //int H = glMainWidget->height();
  itE =_graph->getEdges();
  while (itE->hasNext()) {
    ite=itE->next();
    if (_selection->getEdgeValue(ite)) {
      mEdge=ite;
      coordinates=_layout->getEdgeValue(ite);
      start=_layout->getNodeValue(_graph->source(mEdge));
      end=_layout->getNodeValue(_graph->target(mEdge));
      vector<Coord>::iterator CoordIt=coordinates.begin();
      while(CoordIt!=coordinates.end()) {
        tmp=Coord(CoordIt->getX(), CoordIt->getY(), CoordIt->getZ());
        tmp=glMainWidget->getScene()->getLayer("Main")->getCamera()->worldTo2DScreen(tmp);
        //tmp[1] = (double)H - tmp[1];
        //tmp[0] = (double)W - tmp[0];
        basicCircle.set(tmp, 5, 0.);
        circles.push_back(basicCircle);
        CoordIt++;
      }
    }
  }
  delete itE;
  for(unsigned int i=0;i<circles.size();i++)
    circleString.addGlEntity(&circles[i], IntegerType::toString(i));
  return hasSelection;
}
//========================================================================================
