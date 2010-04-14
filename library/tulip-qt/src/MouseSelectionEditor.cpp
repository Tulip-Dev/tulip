#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/DrawingTools.h>
#include <tulip/ForEach.h>

#include <tulip/MouseSelectionEditor.h>

#include <QtGui/qevent.h>

#define EPSILON 1.0
#define EPSILON_SCREEN 50
#define EPSILON_STRETCH_MIN 1 - 1.0e-01
#define EPSILON_STRETCH_MAX 1 + 1.0e-01

using namespace tlp;
using namespace std;

//========================================================================================
MouseSelectionEditor::MouseSelectionEditor():glMainWidget(NULL),layer(NULL),composite(NULL){
  operation = NONE;

  //composite.addGlEntity(&centerRect, "CenterRectangle");
  //composite.addGlEntity(&_controls[0], "left");
  _controls[0].resizePoints(3); //triangle
  _controls[0].setStencil(0);
  //composite.addGlEntity(&_controls[1], "top-left");
  _controls[1].resizePoints(4); //square
  _controls[1].setStencil(0);
  //composite.addGlEntity(&_controls[2], "top");
  _controls[2].resizePoints(3); //triangle
  _controls[2].setStencil(0);
  //composite.addGlEntity(&_controls[3], "top-right");
  _controls[3].resizePoints(30); //circle
  _controls[3].setStencil(0);
  //composite.addGlEntity(&_controls[4], "right");
  _controls[4].resizePoints(3); //triangle
  _controls[4].setStencil(0);
  //composite.addGlEntity(&_controls[5], "bottom-right");
  _controls[5].resizePoints(4); //square
  _controls[5].setStencil(0);
  //composite.addGlEntity(&_controls[6], "bottom");
  _controls[6].resizePoints(3); //triangle
  _controls[6].setStencil(0);
  //composite.addGlEntity(&_controls[7], "bottom-left");
  _controls[7].resizePoints(30); //circle
  _controls[7].setStencil(0);

  centerRect.setStencil(0);


  //widget->getScene()->getLayout()->addGlEntity(&composite);

  /*centerRect.setRenderState(GlAD_ZEnable, false);
  centerRect.setRenderState(GlAD_Culling, false);
  centerRect.setRenderState(GlAD_Wireframe, false);
  centerRect.setRenderState(GlAD_Solid, true);
  centerRect.setRenderState(GlAD_AlphaBlending, true);
  centerRect.setRenderState(GlAD_Lighting, false);*/

  Color hudColor(128,128,128,128);
  centerRect.setFillMode(true);
  centerRect.setOutlineMode(true);
  centerRect.fcolor(0) =  hudColor;
  centerRect.fcolor(1) =  hudColor;
  centerRect.fcolor(2) =  hudColor;
  centerRect.fcolor(3) =  hudColor;

  for(unsigned int i=0; i < 8; ++i) {
    _controls[i].setFillMode(true);
    _controls[i].setOutlineMode(true);
    _controls[i].fcolor(0) = Color(255,40,40,200);
    _controls[i].ocolor(0) = Color(128,20,20,200);

    /*_controls[i].setRenderState(GlAD_ZEnable, false);
    _controls[i].setRenderState(GlAD_Culling, false);
    _controls[i].setRenderState(GlAD_AlphaBlending, true);
    _controls[i].setRenderState(GlAD_Wireframe, false);
    _controls[i].setRenderState(GlAD_Solid, true);*/
  }
}
//========================================================================================
MouseSelectionEditor::~MouseSelectionEditor(){
  if(layer){
    glMainWidget->getScene()->removeLayer(layer,true);
    layer=NULL;
  }
}
//========================================================================================
bool MouseSelectionEditor::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    initProxies(glMainWidget);
    computeFFD(glMainWidget);

    int W, H;
    W = glMainWidget->width();
    H = glMainWidget->height();
    editCenter = centerRect.getCenter();
    editCenter[2] = 0;
    editCenter[1]= H- editCenter[1];
    //editCenter[0] = W - editCenter[0];
    editPosition[0] = qMouseEv->x();
    editPosition[1] = qMouseEv->y();
    editPosition[2] = 0;
    editLayoutCenter = _layoutCenter;

    //  cerr << "edit pos:" << editPosition << endl;

    vector < GlEntity *> select;
    switch(qMouseEv->buttons()) {
    case Qt::LeftButton : {
      // first ensure that something is selected
      bool hasSelection = false;
      node no;
      forEach(no, _selection->getNodesEqualTo(true, _graph)) {
        hasSelection = true;
        breakForEach;
      }
      if (!hasSelection) {
        edge ed;
        forEach(ed, _selection->getEdgesEqualTo(true, _graph)) {
          hasSelection = true;
          breakForEach;
        }
      }
      if (!hasSelection ||
          (!glMainWidget->selectGlEntities((int)editPosition[0]-3, (int)editPosition[1]-3,
              6, 6, select,layer) &&
              !centerRect.inRect((double) qMouseEv->x(),
                  (double)qMouseEv->y()))) {
        // event occurs outside the selection rectangle
        // so from now we delegate the job to a MouseSelector object
        // which should intercept the event
        operation = NONE;
        return false;
      }

      int shapeId=-1;

      for (unsigned int i = 0; (i < select.size()) && (shapeId==-1); ++i) {
        for(int j=0 ; j<8;++j) {
          if(select[i]==&_controls[j]){
            shapeId=i;
          }
        }
      }
      if (shapeId != -1) {
        ((GlCircle *)select[shapeId])->fcolor(0) = Color(40,255,40,200);
        ((GlCircle *)select[shapeId])->ocolor(0) = Color(20,128,20,200);
        //left <-> right anchor
        // stretch_x
        if (select[shapeId] == &_controls[0] || select[shapeId] == &_controls[4]) {
          operation = STRETCH_X;
          glMainWidget->setCursor(QCursor(Qt::SizeHorCursor));
        }else
          //top <-> bottom anchor
          // stretch_y
          if (select[shapeId] == &_controls[2] || select[shapeId] == &_controls[6]) {
            operation = STRETCH_Y;
            glMainWidget->setCursor(QCursor(Qt::SizeVerCursor));
          }
          else
            //Corner anchor bottom-right top-left
            // rotate
            if (select[shapeId] == &_controls[3] || select[shapeId] == &_controls[7]) {
              glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));
              operation = ROTATE_Z;
            }
            else
              //Corner anchor top-right bottom-left
              //stretch_xy
              if (select[shapeId] == &_controls[1] || select[shapeId] == &_controls[5]) {
                operation = STRETCH_XY;
                glMainWidget->setCursor(QCursor(Qt::SizeFDiagCursor));

              }
      }
      else {
        if (qMouseEv->modifiers() &
#if defined(__APPLE__)
            Qt::AltModifier
#else
            Qt::ControlModifier
#endif
        ) {
          operation = ROTATE_XY;
          glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));
        }
        else {
          operation = TRANSLATE;
          glMainWidget->setCursor(QCursor(Qt::SizeAllCursor));
        }
      }

      mode = COORD_AND_SIZE;
      if (qMouseEv->modifiers() & Qt::ShiftModifier)
        mode = COORD;
      if (qMouseEv->modifiers() &
#if defined(__APPLE__)
          Qt::AltModifier
#else
          Qt::ControlModifier
#endif
      )
        mode = SIZE;
      initEdition();
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
      operation != NONE) {
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    stopEdition();
    for(unsigned int i=0; i < 8; ++i) { //restore colors
      _controls[i].fcolor(0) = Color(255,40,40,200);
      _controls[i].ocolor(0) = Color(128,20,20,200);
    }
    glMainWidget->setCursor(QCursor(Qt::ArrowCursor));
    glMainWidget->draw();
    return true;
  }
  if  (e->type() == QEvent::MouseMove &&
      ((QMouseEvent *) e)->buttons() & Qt::LeftButton &&
      operation != NONE) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    int newX = qMouseEv->x();
    int newY = qMouseEv->y();

    switch (operation) {
    case STRETCH_X:
    case STRETCH_Y:
    case STRETCH_XY:
      mMouseStretchAxis(newX, newY, glMainWidget);
      return true;
    case ROTATE_Z:
    case ROTATE_XY:
      mMouseRotate(newX, newY, glMainWidget);
      return true;
    case TRANSLATE:
      mMouseTranslate(newX, newY, glMainWidget);
      return true;
    case NONE:
      cerr << "[Error] : " <<__FUNCTION__ << " should not be call" << endl;
      break;
    }
  }
  return false;
}
//========================================================================================
bool MouseSelectionEditor::compute(GlMainWidget *glMainWidget) {
  if (computeFFD(glMainWidget)) {
    if(!layer){
      layer=new GlLayer("selectionEditorLayer",true);
      layer->setCamera(Camera(glMainWidget->getScene(),false));
      glMainWidget->getScene()->insertLayerAfter(layer,"Main");
      composite = new GlComposite(false);
      layer->addGlEntity(composite,"selectionComposite");
    }
    composite->addGlEntity(&centerRect, "CenterRectangle");
    composite->addGlEntity(&_controls[0], "left");
    composite->addGlEntity(&_controls[1], "top-left");
    composite->addGlEntity(&_controls[2], "top");
    composite->addGlEntity(&_controls[3], "top-right");
    composite->addGlEntity(&_controls[4], "right");
    composite->addGlEntity(&_controls[5], "bottom-right");
    composite->addGlEntity(&_controls[6], "bottom");
    composite->addGlEntity(&_controls[7], "bottom-left");
    this->glMainWidget=glMainWidget;
    return true;
  }else{
    if(layer){
      glMainWidget->getScene()->removeLayer(layer,true);
      layer=NULL;
    }
    return false;
  }
}
//========================================================================================
bool MouseSelectionEditor::draw(GlMainWidget *glMainWidget) {
  //return compute(glMainWidget);
  return true;
}
//========================================================================================
void MouseSelectionEditor::initEdition() {
  _graph->push();
}
//========================================================================================
void MouseSelectionEditor::undoEdition() {
  if (operation == NONE) return;
  _graph->pop();
  operation = NONE;
}
//========================================================================================
void MouseSelectionEditor::stopEdition() {
  //cerr << __PRETTY_FUNCTION__ << endl;
  if(layer){
    glMainWidget->getScene()->removeLayer(layer,true);
    layer=NULL;
  }

  operation = NONE;
}
//========================================================================================
void MouseSelectionEditor::initProxies(GlMainWidget *glMainWidget) {
  GlGraphInputData *inputData=glMainWidget->getScene()->getGlGraphComposite()->getInputData();
  _graph     = inputData->getGraph();
  _layout    = inputData->getLayoutProperty();
  _selection = _graph->getProperty<BooleanProperty>(inputData->getElementSelectedPropName());
  _rotation  = _graph->getProperty<DoubleProperty>(inputData->getElementRotationPropName());
  _sizes     = _graph->getProperty<SizeProperty>(inputData->getElementSizePropName());
}
//========================================================================================
void MouseSelectionEditor::mMouseTranslate(double newX, double newY, GlMainWidget *glMainWidget) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  Observable::holdObservers();
  initProxies(glMainWidget);
  Coord v0(0,0,0);
  Coord v1((double)(editPosition[0] - newX), -(double)(editPosition[1] - newY),0);
  v0 = glMainWidget->getScene()->getCamera()->screenTo3DWorld(v0);
  v1 = glMainWidget->getScene()->getCamera()->screenTo3DWorld(v1);
  v1 -= v0;
  Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
  Iterator<edge> *itE = _selection->getEdgesEqualTo(true, _graph);
  _layout->translate(v1, itN, itE);
  delete itN;
  delete itE;
  editPosition[0]  = newX;
  editPosition[1]  = newY;
  Observable::unholdObservers();
}
//========================================================================================
void MouseSelectionEditor::mMouseStretchAxis(double newX, double newY, GlMainWidget* glMainWidget) {
  //  cerr << __PRETTY_FUNCTION__ << "/op=" << operation << ", mod:" << mode << endl;
  Coord curPos(newX, newY, 0);
  Coord stretch(1,1,1);
  //  cerr << "cur : << " << curPos << " center : " << editCenter << endl;
  if (operation == STRETCH_X || operation == STRETCH_XY) {
    stretch[0] =  (curPos[0] - editCenter[0]) / (editPosition[0] - editCenter[0]);
  }
  if (operation == STRETCH_Y || operation == STRETCH_XY) {
    stretch[1] = (curPos[1] - editCenter[1]) / (editPosition[1] - editCenter[1]);
  }
  //  cerr << "stretch : << "<< stretch << endl;

  Observable::holdObservers();
  _graph->pop();
  _graph->push();
  //stretch layout
  if (mode == COORD_AND_SIZE || mode == COORD) {
    Coord center(editLayoutCenter);
    center *= -1.;
    //move the center to the origin in order to be able to scale
    Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
    Iterator<edge> *itE = _selection->getEdgesEqualTo(true, _graph);
    _layout->translate(center, itN, itE);
    delete itN; delete itE;
    //scale the drawing
    itN = _selection->getNodesEqualTo(true, _graph);
    itE = _selection->getEdgesEqualTo(true, _graph);
    _layout->scale(stretch, itN, itE);
    delete itN; delete itE;
    //replace the center of the graph at its originale position
    center *= -1.;
    itN = _selection->getNodesEqualTo(true, _graph);
    itE = _selection->getEdgesEqualTo(true, _graph);
    _layout->translate(center, itN, itE);
    delete itN; delete itE;
  }
  //stretch size
  if (mode == COORD_AND_SIZE || mode == SIZE) {
    Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
    Iterator<edge> *itE = _selection->getEdgesEqualTo(true, _graph);
    _sizes->scale(stretch, itN, itE);
    delete itN; delete itE;
  }
  Observable::unholdObservers();
}
//========================================================================================
void MouseSelectionEditor::mMouseRotate(double newX, double newY, GlMainWidget *glMainWidget) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (operation == ROTATE_Z) {
    Coord curPos(newX, newY, 0);

    Coord vCS = editPosition - editCenter;
    vCS /= vCS.norm();
    Coord vCP =  curPos - editCenter;
    vCP /= vCP.norm();

    float sign = (vCS ^ vCP)[2];
    sign /= fabs(sign);
    double cosalpha = vCS.dotProduct(vCP);
    double deltaAngle = sign * acos(cosalpha);

    Observable::holdObservers();

    initProxies(glMainWidget);
    _graph->pop();
    _graph->push();

    double degAngle = (deltaAngle * 180.0 / M_PI);
    //rotate layout
    if (mode == COORD_AND_SIZE || mode == COORD) {
      Coord center(editLayoutCenter);
      center *= -1.;
      Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
      Iterator<edge> *itE = _selection->getEdgesEqualTo(true, _graph);
      _layout->translate(center, itN, itE);
      delete itN; delete itE;
      itN = _selection->getNodesEqualTo(true, _graph);
      itE = _selection->getEdgesEqualTo(true, _graph);
      _layout->rotateZ(-degAngle, itN, itE);
      delete itN; delete itE;
      center *= -1.;
      itN = _selection->getNodesEqualTo(true, _graph);
      itE = _selection->getEdgesEqualTo(true, _graph);
      _layout->translate(center, itN, itE);
      delete itN; delete itE;
    }
    if (mode == COORD_AND_SIZE || mode == SIZE) {
      Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
      while(itN->hasNext()) {
  node n = itN->next();
  double rotation = _rotation->getNodeValue(n);
  _rotation->setNodeValue(n, rotation - degAngle);
      } delete itN;
    }

    Observable::unholdObservers();
  } else {
    double initDelta, delta, cosa;
    double xAngle = 0, yAngle = 0;
    double nbPI = 0;

    delta = abs(newX - editPosition[0]);
    if (delta > abs(newY - editPosition[1])) {
      initDelta = abs(editCenter[0] - editPosition[0]);
      nbPI = floor(delta / (2. * initDelta));
      delta -= nbPI * 2. * initDelta;
      cosa = (initDelta - delta)/initDelta;

      yAngle = (acos(cosa) + (nbPI * M_PI)) * 180.0 / M_PI;
    } else {
      delta = abs(newY - editPosition[1]);
      initDelta = abs(editCenter[1] - editPosition[1]);
      nbPI = floor(delta / (2. * initDelta));
      delta -= nbPI * 2. * initDelta;
      cosa = (initDelta - delta)/initDelta;

      xAngle = (acos(cosa) + (nbPI * M_PI)) * 180.0 / M_PI;
    }

    Observable::holdObservers();

    initProxies(glMainWidget);
    _graph->pop();
    _graph->push();

    Coord center(editLayoutCenter);
    center *= -1.;
    Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
    Iterator<edge> *itE = _selection->getEdgesEqualTo(true, _graph);
    _layout->translate(center, itN, itE);
    delete itN; delete itE;
    itN = _selection->getNodesEqualTo(true, _graph);
    itE = _selection->getEdgesEqualTo(true, _graph);
    if (yAngle > xAngle)
      _layout->rotateY(yAngle, itN, itE);
    else
      _layout->rotateX(xAngle, itN, itE);
    delete itN; delete itE;
    center *= -1.;
    itN = _selection->getNodesEqualTo(true, _graph);
    itE = _selection->getEdgesEqualTo(true, _graph);
    _layout->translate(center, itN, itE);
    delete itN; delete itE;

    Observable::unholdObservers();
  }
}
//========================================================================================
Coord minCoord(const Coord &v1, const Coord &v2) {
  Coord result;
  for (unsigned int i =0; i<3; ++i)
    result[i] = std::min(v1[i], v2[i]);
  return result;
}
Coord maxCoord(const Coord &v1, const Coord &v2) {
  Coord result;
  for (unsigned int i =0; i<3; ++i)
    result[i] = std::max(v1[i], v2[i]);
  return result;
}
//========================================================================================
bool MouseSelectionEditor::computeFFD(GlMainWidget *glMainWidget) {
  if (!glMainWidget->getScene()->getGlGraphComposite() || !glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph())
      return false;
  // We calculate the bounding box for the selection :
  initProxies(glMainWidget);
  pair<Coord, Coord> boundingBox = tlp::computeBoundingBox(_graph, _layout, _sizes, _rotation, _selection);

  if (boundingBox.first[0] == -FLT_MAX)
    return false;
  Coord min2D, max2D;
  _layoutCenter = (boundingBox.first + boundingBox.second) / 2.f;

  //project the 8 points of the cube to obtain the bounding square on the 2D screen
  Coord bbsize = (boundingBox.first - boundingBox.second);
  //v1
  Coord tmp(boundingBox.second);
  tmp = glMainWidget->getScene()->getCamera()->worldTo2DScreen(tmp);
  min2D = tmp;
  max2D = tmp;
  //v2, v3, V4
  for (unsigned int i=0; i<3; ++i) {
    tmp = boundingBox.second;
    tmp[i] += bbsize[i];
    tmp = glMainWidget->getScene()->getCamera()->worldTo2DScreen(tmp);
    min2D = minCoord(tmp, min2D);
    max2D = maxCoord(tmp, max2D);
  }
  //v4
  tmp = boundingBox.second;
  tmp[0] += bbsize[0];
  tmp[1] += bbsize[1];
  tmp = glMainWidget->getScene()->getCamera()->worldTo2DScreen(tmp);
  min2D = minCoord(tmp, min2D);
  max2D = maxCoord(tmp, max2D);
  //v6
  tmp = boundingBox.second;
  tmp[0] += bbsize[0];
  tmp[2] += bbsize[2];
  tmp = glMainWidget->getScene()->getCamera()->worldTo2DScreen(tmp);
  min2D = minCoord(tmp, min2D);
  max2D = maxCoord(tmp, max2D);
  //v7
  tmp = boundingBox.second;
  tmp[1] += bbsize[1];
  tmp[2] += bbsize[2];
  tmp = glMainWidget->getScene()->getCamera()->worldTo2DScreen(tmp);
  min2D = minCoord(tmp, min2D);
  max2D = maxCoord(tmp, max2D);
  //v8
  tmp = boundingBox.second;
  tmp += bbsize;
  tmp = glMainWidget->getScene()->getCamera()->worldTo2DScreen(tmp);
  min2D = minCoord(tmp, min2D);
  max2D = maxCoord(tmp, max2D);

  ffdCenter = (boundingBox.first + boundingBox.second) / 2.f;

  Coord tmpCenter = glMainWidget->getScene()->getCamera()->worldTo2DScreen(ffdCenter);

  //  cerr << tmpCenter << endl;

  //tmpCenter[0] = (double)glMainWidget->width() - tmpCenter[0];
  //tmpCenter[1] = (double)glMainWidget->height() - tmpCenter[1];

  //  tmpCenter[1] = tmpCenter[1];

  int x = int(max2D[0] - min2D[0]) / 2 + 1; // (+1) because selection use glLineWidth=3 thus
  int y = int(max2D[1] - min2D[1]) / 2 + 1; //the rectangle can be too small.

  if (x < 20) x = 18;
  if (y < 20) y = 18;

  Coord positions[8];

  // we keep the z coordinate of the ffdCenter
  // to ensure a correct position of our controls (see GlHudPolygon.cpp)
  positions[0] = Coord( x,  0, ffdCenter[2]) + tmpCenter; // left
  positions[1] = Coord( x, -y, ffdCenter[2]) + tmpCenter; // Top left
  positions[2] = Coord( 0, -y, ffdCenter[2]) + tmpCenter; // Top
  positions[3] = Coord(-x, -y, ffdCenter[2]) + tmpCenter; // Top r
  positions[4] = Coord(-x,  0, ffdCenter[2]) + tmpCenter; // r
  positions[5] = Coord(-x,  y, ffdCenter[2]) + tmpCenter; // Bottom r
  positions[6] = Coord( 0,  y, ffdCenter[2]) + tmpCenter; // Bottom
  positions[7] = Coord( x,  y, ffdCenter[2]) + tmpCenter; // Bottom l

  for(int i=0;i<8;i++){
    positions[i][2]=0;
  }

  //Parameters of the rectangle that shows the selected area.
  centerRect.setTopLeftPos(positions[1]);
  centerRect.setBottomRightPos(positions[5]);

  _controls[0].set(positions[0], 7, 0.0); //t
  _controls[1].set(positions[1], 6, M_PI/4.); //c
  _controls[2].set(positions[2], 7, -M_PI/2.); //t
  _controls[3].set(positions[3], 5, 0.);//s
  _controls[4].set(positions[4], 7, M_PI);//t
  _controls[5].set(positions[5], 6, M_PI/4.);//c
  _controls[6].set(positions[6], 7, M_PI/2.);//
  _controls[7].set(positions[7], 5, 0.);//s

  return true;
}
//========================================================================================
