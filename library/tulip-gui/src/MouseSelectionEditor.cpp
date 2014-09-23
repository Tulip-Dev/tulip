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

#include <QMouseEvent>

#include <tulip/MouseSelectionEditor.h>
#include <tulip/Camera.h>
#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/DrawingTools.h>
#include <tulip/ForEach.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/TlpQtTools.h>

#include <cmath>
#include <climits>

#define EPSILON 1.0
#define EPSILON_SCREEN 50
#define EPSILON_STRETCH_MIN 1 - 1.0e-01
#define EPSILON_STRETCH_MAX 1 + 1.0e-01

using namespace tlp;
using namespace std;

const unsigned int arrowWithLineSize=8;
const Coord arrowWithLine[] = {Coord(0,3,0),Coord(-5,-5,0),Coord(5,-5,0),Coord(0,3,0),Coord(5,3,0),Coord(5,5,0),Coord(-5,5,0),Coord(-5,3,0)};
const unsigned int twoArrowWithLineSize=10;
const Coord twoArrowWithLine[] = {Coord(0,0,0),Coord(5,-5,0),Coord(-5,-5,0),Coord(0,0,0),Coord(-5,0,0),Coord(5,0,0),Coord(0,0,0),Coord(5,5,0),Coord(-5,5,0),Coord(0,0,0)};

//========================================================================================
MouseSelectionEditor::MouseSelectionEditor():glMainWidget(NULL),layer(NULL),composite(NULL) {
  operation = NONE;

  _controls[0].resizePoints(3); //triangle
  _controls[0].setStencil(0);
  _controls[1].resizePoints(4); //square
  _controls[1].setStencil(0);
  _controls[2].resizePoints(3); //triangle
  _controls[2].setStencil(0);
  _controls[3].resizePoints(30); //circle
  _controls[3].setStencil(0);
  _controls[4].resizePoints(3); //triangle
  _controls[4].setStencil(0);
  _controls[5].resizePoints(4); //square
  _controls[5].setStencil(0);
  _controls[6].resizePoints(3); //triangle
  _controls[6].setStencil(0);
  _controls[7].resizePoints(30); //circle
  _controls[7].setStencil(0);

  centerRect.setStencil(0);
  advRect.setStencil(0);

  Color hudColor(128,128,128,128);
  centerRect.setFillMode(true);
  centerRect.setOutlineMode(true);
  centerRect.setFillColor(hudColor);
  hudColor=Color(128,128,128,64);
  advRect.setFillMode(true);
  advRect.setOutlineMode(false);
  advRect.setFillColor(hudColor);

  for(unsigned int i=0; i < 8; ++i) {
    _controls[i].setFillMode(true);
    _controls[i].setOutlineMode(true);
    _controls[i].setFillColor(Color(255,40,40,200));
    _controls[i].setOutlineColor(Color(128,20,20,200));
  }
}
//========================================================================================
MouseSelectionEditor::~MouseSelectionEditor() {

}
//========================================================================================
void MouseSelectionEditor::clear() {
  if (glMainWidget != NULL) {
    glMainWidget->getScene()->removeLayer(layer,false);
    delete layer;
    layer=NULL;

    glMainWidget->setCursor(QCursor());
  }
}
//========================================================================================
void MouseSelectionEditor::getOperation(GlEntity *select) {
  //left <-> right anchor
  // stretch_x
  if (select == &_controls[0] || select == &_controls[4]) {
    operation = STRETCH_X;
    glMainWidget->setCursor(QCursor(Qt::SizeHorCursor));
    return;
  }

  //top <-> bottom anchor
  // stretch_y
  if (select == &_controls[2] || select == &_controls[6]) {
    operation = STRETCH_Y;
    glMainWidget->setCursor(QCursor(Qt::SizeVerCursor));
    return;
  }

  //Corner anchor bottom-right top-left
  // rotate
  if (select == &_controls[3] || select == &_controls[7]) {
    glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));
    operation = ROTATE_Z;
    return;
  }

  //Corner anchor top-right bottom-left
  //stretch_xy
  if (select == &_controls[1] || select == &_controls[5]) {
    operation = STRETCH_XY;
    glMainWidget->setCursor(QCursor(Qt::SizeFDiagCursor));
    return;
  }

  if( select == &_advControls[0]) {
    operation = ALIGN_TOP;
    return;
  }

  if( select == &_advControls[1]) {
    operation = ALIGN_BOTTOM;
    return;
  }

  if( select == &_advControls[2]) {
    operation = ALIGN_LEFT;
    return;
  }

  if( select == &_advControls[3]) {
    operation = ALIGN_RIGHT;
    return;
  }

  if( select == &_advControls[4]) {
    operation = ALIGN_HORIZONTALLY;
    return;
  }

  if( select == &_advControls[5]) {
    operation = ALIGN_VERTICALLY;
    return;
  }
}
//========================================================================================
bool MouseSelectionEditor::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);
  GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);

  if (e->type() == QEvent::MouseButtonPress) {

    initProxies(glMainWidget);
    computeFFD(glMainWidget);

    int H;
    H = glMainWidget->height();
    editCenter = centerRect.getCenter();
    editCenter[2] = 0;
    editCenter[1]= H- editCenter[1];
    //editCenter[0] = W - editCenter[0];
    editPosition[0] = qMouseEv->x();
    editPosition[1] = qMouseEv->y();
    editPosition[2] = 0;
    editLayoutCenter = _layoutCenter;

    vector <SelectedEntity> select;

    switch(qMouseEv->buttons()) {
    case Qt::LeftButton : {
      // first ensure that something is selected
      bool hasSelection = false;
      node no;
      forEach(no, _selection->getNodesEqualTo(true, _graph)) {
        hasSelection = true;
        break;
      }

      if (!hasSelection) {
        edge ed;
        forEach(ed, _selection->getEdgesEqualTo(true, _graph)) {
          hasSelection = true;
          break;
        }
      }

      if (!hasSelection ||
          (!glMainWidget->pickGlEntities((int)editPosition[0]-3, (int)editPosition[1]-3,
                                         6, 6, select,layer))) {
        // event occurs outside the selection rectangle
        // so from now we delegate the job to a MouseSelector object
        // which should intercept the event
        operation = NONE;
        glMainWidget->setCursor(QCursor(Qt::CrossCursor));

        return false;
      }

      glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));

      int shapeId=-1;
      bool advShape=false;

      for (unsigned int i = 0; (i < select.size()) && (shapeId==-1); ++i) {
        for(int j=0 ; j<8; ++j) {
          if(select[i].getSimpleEntity()==&_controls[j]) {
            shapeId=i;
          }
        }

        for(int j=0 ; j<6; ++j) {
          if(select[i].getSimpleEntity()==&_advControls[j]) {
            advShape=true;
            shapeId=i;
          }
        }
      }

      if (shapeId != -1) {
        if(!advShape) {
          ((GlCircle *)select[shapeId].getSimpleEntity())->setFillColor(Color(40,255,40,200));
          ((GlCircle *)select[shapeId].getSimpleEntity())->setOutlineColor(Color(20,128,20,200));
        }

        getOperation(select[shapeId].getSimpleEntity());

        switch (operation) {
        case ALIGN_TOP:
        case ALIGN_BOTTOM:
        case ALIGN_LEFT:
        case ALIGN_RIGHT:
        case ALIGN_VERTICALLY:
        case ALIGN_HORIZONTALLY:
          mAlign(operation,glMainWidget);
          return true;

        case ROTATE_Z:
        case ROTATE_XY:
        case NONE:
        case STRETCH_X:
        case STRETCH_Y:
        case STRETCH_XY:
        case TRANSLATE:
        default:
          break;
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

    default:
      return false;
    }

    glMainWidget->redraw();
    return true;
  }

  if (e->type() == QEvent::MouseButtonRelease &&
      qMouseEv->button() == Qt::LeftButton &&
      operation != NONE) {
    stopEdition();

    //restore colors
    for(unsigned int i=0; i < 8; ++i) {
      _controls[i].setFillColor(Color(255,40,40,200));
      _controls[i].setOutlineColor(Color(128,20,20,200));
    }

    glMainWidget->setCursor(QCursor(Qt::ArrowCursor));
    glMainWidget->draw();
    return true;
  }

  if(e->type() == QEvent::KeyPress) {
    // first ensure that something is selected
    bool hasSelection = false;
    node no;
    forEach(no, _selection->getNodesEqualTo(true, _graph)) {
      hasSelection = true;
      break;
    }

    if (!hasSelection) {
      edge ed;
      forEach(ed, _selection->getEdgesEqualTo(true, _graph)) {
        hasSelection = true;
        break;
      }
    }

    if(hasSelection) {
      switch(((QKeyEvent*)e)->key()) {
      case Qt::Key_Left:
        mMouseTranslate(editPosition[0]-1, editPosition[1], glMainWidget);
        break;

      case Qt::Key_Right:
        mMouseTranslate(editPosition[0]+1, editPosition[1], glMainWidget);
        break;

      case Qt::Key_Up:
        mMouseTranslate(editPosition[0], editPosition[1]-1, glMainWidget);
        break;

      case Qt::Key_Down:
        mMouseTranslate(editPosition[0], editPosition[1]+1, glMainWidget);
        break;
      }
    }
  }

  if  (e->type() == QEvent::MouseMove && qMouseEv->buttons() & Qt::LeftButton && operation != NONE) {
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
    case ALIGN_TOP:
    case ALIGN_BOTTOM:
    case ALIGN_LEFT:
    case ALIGN_RIGHT:
    case ALIGN_VERTICALLY:
    case ALIGN_HORIZONTALLY:
    default:
      qWarning() << "[Error] : " <<__FUNCTION__ << " should not have been called" << endl;
      break;
    }
  }

  return false;
}
//========================================================================================
bool MouseSelectionEditor::compute(GlMainWidget *glMainWidget) {
  if (computeFFD(glMainWidget)) {
    if(!layer) {
      layer=new GlLayer("selectionEditorLayer",true);
      layer->setCamera(new Camera(glMainWidget->getScene(),false));
      composite = new GlComposite(false);
      layer->addGlEntity(composite,"selectionComposite");
    }

    bool layerInScene=false;
    const vector<pair<std::string, GlLayer*> >& layersList=glMainWidget->getScene()->getLayersList();

    for(vector<pair<std::string, GlLayer*> >::const_iterator it=layersList.begin(); it!=layersList.end(); ++it) {
      if((*it).second==layer) {
        layerInScene=true;
        break;
      }
    }

    if(!layerInScene)
      glMainWidget->getScene()->addExistingLayerAfter(layer,"Main");

    composite->addGlEntity(&centerRect, "CenterRectangle");
    composite->addGlEntity(&_controls[0], "left");
    composite->addGlEntity(&_controls[1], "top-left");
    composite->addGlEntity(&_controls[2], "top");
    composite->addGlEntity(&_controls[3], "top-right");
    composite->addGlEntity(&_controls[4], "right");
    composite->addGlEntity(&_controls[5], "bottom-right");
    composite->addGlEntity(&_controls[6], "bottom");
    composite->addGlEntity(&_controls[7], "bottom-left");

    Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
    int moreThanOneNode=0;

    while(itN->hasNext()) {
      if(moreThanOneNode>=2)
        break;

      moreThanOneNode++;
      itN->next();
    }

    delete itN;

    if(moreThanOneNode>=2) {
      composite->addGlEntity(&advRect, "AdvRectangle");

      composite->addGlEntity(&_advControls[0], "center-top");
      composite->addGlEntity(&_advControls[1], "center-bottom");
      composite->addGlEntity(&_advControls[2], "center-right");
      composite->addGlEntity(&_advControls[3], "center-left");
      composite->addGlEntity(&_advControls[4], "center-horizontally");
      composite->addGlEntity(&_advControls[5], "center-vertically");
    }
    else {
      composite->deleteGlEntity("AdvRectangle");

      composite->deleteGlEntity("center-top");
      composite->deleteGlEntity("center-bottom");
      composite->deleteGlEntity("center-right");
      composite->deleteGlEntity("center-left");
      composite->deleteGlEntity("center-horizontally");
      composite->deleteGlEntity("center-vertically");
    }

    this->glMainWidget=glMainWidget;
    return true;
  }
  else {
    if(layer) {
      glMainWidget->getScene()->removeLayer(layer,true);
      layer=NULL;
    }

    return false;
  }
}
//========================================================================================
bool MouseSelectionEditor::draw(GlMainWidget *) {
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
  //qWarning() << __PRETTY_FUNCTION__ << endl;
  if(layer) {
    glMainWidget->getScene()->removeLayer(layer,true);
    layer=NULL;
  }

  operation = NONE;
}
//========================================================================================
void MouseSelectionEditor::initProxies(GlMainWidget *glMainWidget) {
  GlGraphInputData *inputData=glMainWidget->getScene()->getGlGraphComposite()->getInputData();
  _graph     = inputData->getGraph();
  _layout    = inputData->getElementLayout();
  _selection = inputData->getElementSelected();
  _rotation  = inputData->getElementRotation();
  _sizes     = inputData->getElementSize();
}
//========================================================================================
void MouseSelectionEditor::mMouseTranslate(double newX, double newY, GlMainWidget *glMainWidget) {
  //  qWarning() << __PRETTY_FUNCTION__ << endl;
  Observable::holdObservers();
  initProxies(glMainWidget);
  Coord v0(0,0,0);
  Coord v1((double)(editPosition[0] - newX), -(double)(editPosition[1] - newY),0);
  v0 = glMainWidget->getScene()->getGraphCamera().screenTo3DWorld(v0);
  v1 = glMainWidget->getScene()->getGraphCamera().screenTo3DWorld(v1);
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
void MouseSelectionEditor::mMouseStretchAxis(double newX, double newY, GlMainWidget* ) {
  //  qWarning() << __PRETTY_FUNCTION__ << "/op=" << operation << ", mod:" << mode << endl;
  Coord curPos(newX, newY, 0);
  Coord stretch(1,1,1);

  //  qWarning() << "cur : << " << curPos << " center : " << editCenter << endl;
  if (operation == STRETCH_X || operation == STRETCH_XY) {
    stretch[0] =  (curPos[0] - editCenter[0]) / (editPosition[0] - editCenter[0]);
  }

  if (operation == STRETCH_Y || operation == STRETCH_XY) {
    stretch[1] = (curPos[1] - editCenter[1]) / (editPosition[1] - editCenter[1]);
  }

  //  qWarning() << "stretch : << "<< stretch << endl;

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
    delete itN;
    delete itE;
    //scale the drawing
    itN = _selection->getNodesEqualTo(true, _graph);
    itE = _selection->getEdgesEqualTo(true, _graph);
    _layout->scale(stretch, itN, itE);
    delete itN;
    delete itE;
    //replace the center of the graph at its originale position
    center *= -1.;
    itN = _selection->getNodesEqualTo(true, _graph);
    itE = _selection->getEdgesEqualTo(true, _graph);
    _layout->translate(center, itN, itE);
    delete itN;
    delete itE;
  }

  //stretch size
  if (mode == COORD_AND_SIZE || mode == SIZE) {
    Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
    Iterator<edge> *itE = _selection->getEdgesEqualTo(true, _graph);
    stretch[0]=abs(stretch[0]);
    stretch[1]=abs(stretch[1]);
    stretch[2]=abs(stretch[1]);
    _sizes->scale(stretch, itN, itE);
    delete itN;
    delete itE;
  }

  Observable::unholdObservers();
}
//========================================================================================
void MouseSelectionEditor::mMouseRotate(double newX, double newY, GlMainWidget *glMainWidget) {
  //  qWarning() << __PRETTY_FUNCTION__ << endl;
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
      delete itN;
      delete itE;
      itN = _selection->getNodesEqualTo(true, _graph);
      itE = _selection->getEdgesEqualTo(true, _graph);
      _layout->rotateZ(-degAngle, itN, itE);
      delete itN;
      delete itE;
      center *= -1.;
      itN = _selection->getNodesEqualTo(true, _graph);
      itE = _selection->getEdgesEqualTo(true, _graph);
      _layout->translate(center, itN, itE);
      delete itN;
      delete itE;
    }

    if (mode == COORD_AND_SIZE || mode == SIZE) {
      Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);

      while(itN->hasNext()) {
        node n = itN->next();
        double rotation = _rotation->getNodeValue(n);
        _rotation->setNodeValue(n, rotation - degAngle);
      }

      delete itN;
    }

    Observable::unholdObservers();
  }
  else {
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
    }
    else {
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
    delete itN;
    delete itE;
    itN = _selection->getNodesEqualTo(true, _graph);
    itE = _selection->getEdgesEqualTo(true, _graph);

    if (yAngle > xAngle)
      _layout->rotateY(yAngle, itN, itE);
    else
      _layout->rotateX(xAngle, itN, itE);

    delete itN;
    delete itE;
    center *= -1.;
    itN = _selection->getNodesEqualTo(true, _graph);
    itE = _selection->getEdgesEqualTo(true, _graph);
    _layout->translate(center, itN, itE);
    delete itN;
    delete itE;

    Observable::unholdObservers();
  }
}
//========================================================================================
void MouseSelectionEditor::mAlign(EditOperation operation,GlMainWidget*) {
  Observable::holdObservers();
  _graph->push();

  Iterator<node> *itN = _selection->getNodesEqualTo(true, _graph);
  bool init=false;
  float min = -FLT_MAX, max = FLT_MAX;

  while (itN->hasNext()) {
    node n=itN->next();

    float valueMin = -FLT_MAX, valueMax = FLT_MAX;

    switch(operation) {
    case ALIGN_TOP:
      valueMax=_layout->getNodeValue(n)[1]+_sizes->getNodeValue(n)[1]/2.;
      break;

    case ALIGN_BOTTOM:
      valueMin=_layout->getNodeValue(n)[1]-_sizes->getNodeValue(n)[1]/2.;
      break;

    case ALIGN_HORIZONTALLY:
      valueMax=_layout->getNodeValue(n)[1]+_sizes->getNodeValue(n)[1]/2.;
      valueMin=_layout->getNodeValue(n)[1]-_sizes->getNodeValue(n)[1]/2.;
      break;

    case ALIGN_LEFT:
      valueMin=_layout->getNodeValue(n)[0]-_sizes->getNodeValue(n)[0]/2.;
      break;

    case ALIGN_RIGHT:
      valueMax=_layout->getNodeValue(n)[0]+_sizes->getNodeValue(n)[0]/2.;
      break;

    case ALIGN_VERTICALLY:
      valueMax=_layout->getNodeValue(n)[0]-_sizes->getNodeValue(n)[0]/2.;
      valueMin=_layout->getNodeValue(n)[0]+_sizes->getNodeValue(n)[0]/2.;
      break;

    case STRETCH_X:
    case STRETCH_Y:
    case STRETCH_XY:
    case ROTATE_Z:
    case ROTATE_XY:
    case TRANSLATE:
    case NONE:
    default:
      break;
    }

    if(!init) {
      max=valueMax;
      min=valueMin;
      init=true;
    }
    else {
      switch(operation) {
      case ALIGN_TOP:
      case ALIGN_RIGHT:

        if(valueMax>max)
          max=valueMax;

        break;

      case ALIGN_BOTTOM:
      case ALIGN_LEFT:

        if(valueMin<min)
          min=valueMin;

        break;

      case ALIGN_HORIZONTALLY:
      case ALIGN_VERTICALLY:

        if(valueMax>max) max=valueMax;

        if(valueMin<min) min=valueMin;

        break;

      case STRETCH_X:
      case STRETCH_Y:
      case STRETCH_XY:
      case ROTATE_Z:
      case ROTATE_XY:
      case TRANSLATE:
      case NONE:
      default:
        break;
      }
    }
  }

  itN = _selection->getNodesEqualTo(true, _graph);

  while (itN->hasNext()) {
    node n=itN->next();
    Coord old(_layout->getNodeValue(n));

    switch(operation) {
    case ALIGN_TOP:
      old[1]=max-_sizes->getNodeValue(n)[1]/2.;
      break;

    case ALIGN_BOTTOM:
      old[1]=min+_sizes->getNodeValue(n)[1]/2.;
      break;

    case ALIGN_HORIZONTALLY:
      old[1]=(max+min)/2;
      break;

    case ALIGN_LEFT:
      old[0]=min+_sizes->getNodeValue(n)[0]/2.;
      break;

    case ALIGN_RIGHT:
      old[0]=max-_sizes->getNodeValue(n)[0]/2.;
      break;

    case ALIGN_VERTICALLY:
      old[0]=(max+min)/2;
      break;

    case STRETCH_X:
    case STRETCH_Y:
    case STRETCH_XY:
    case ROTATE_Z:
    case ROTATE_XY:
    case TRANSLATE:
    case NONE:
    default:
      break;
    }

    _layout->setNodeValue(n,old);
  }

  Observable::unholdObservers();
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
//========================================================================================
bool MouseSelectionEditor::computeFFD(GlMainWidget *glMainWidget) {
  if (!glMainWidget->getScene()->getGlGraphComposite() || !glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph())
    return false;

  // We calculate the bounding box for the selection :
  initProxies(glMainWidget);
  BoundingBox boundingBox = tlp::computeBoundingBox(_graph, _layout, _sizes, _rotation, _selection);

  if (!boundingBox.isValid()) return false;

  if(operation==NONE)
    glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));

  Coord min2D, max2D;
  _layoutCenter = Coord(boundingBox.center());

  //project the 8 points of the cube to obtain the bounding square on the 2D screen
  Coord bbsize(boundingBox[1] - boundingBox[0]);
  //v1
  Coord tmp(boundingBox[0]);
  tmp = glMainWidget->getScene()->getGraphCamera().worldTo2DScreen(tmp);
  min2D = tmp;
  max2D = tmp;

  //v2, v3, V4
  for (unsigned int i=0; i<3; ++i) {
    tmp = Coord(boundingBox[0]);
    tmp[i] += bbsize[i];
    tmp = glMainWidget->getScene()->getGraphCamera().worldTo2DScreen(tmp);
    min2D = minCoord(tmp, min2D);
    max2D = maxCoord(tmp, max2D);
  }

  //v4
  tmp = Coord(boundingBox[0]);
  tmp[0] += bbsize[0];
  tmp[1] += bbsize[1];
  tmp = glMainWidget->getScene()->getGraphCamera().worldTo2DScreen(tmp);
  min2D = minCoord(tmp, min2D);
  max2D = maxCoord(tmp, max2D);
  //v6
  tmp = Coord(boundingBox[0]);
  tmp[0] += bbsize[0];
  tmp[2] += bbsize[2];
  tmp = glMainWidget->getScene()->getGraphCamera().worldTo2DScreen(tmp);
  min2D = minCoord(tmp, min2D);
  max2D = maxCoord(tmp, max2D);
  //v7
  tmp = Coord(boundingBox[0]);
  tmp[1] += bbsize[1];
  tmp[2] += bbsize[2];
  tmp = glMainWidget->getScene()->getGraphCamera().worldTo2DScreen(tmp);
  min2D = minCoord(tmp, min2D);
  max2D = maxCoord(tmp, max2D);
  //v8
  tmp = Coord(boundingBox[0]);
  tmp += bbsize;
  tmp = glMainWidget->getScene()->getGraphCamera().worldTo2DScreen(tmp);
  min2D = minCoord(tmp, min2D);
  max2D = maxCoord(tmp, max2D);

  ffdCenter = Coord(boundingBox.center());

  Coord tmpCenter = glMainWidget->getScene()->getGraphCamera().worldTo2DScreen(ffdCenter);

  //  qWarning() << tmpCenter << endl;

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

  for(int i=0; i<8; i++) {
    positions[i][2]=0;
  }

  //Parameters of the rectangle that shows the selected area.
  centerRect.setTopLeftPos(positions[1]);
  centerRect.setBottomRightPos(positions[5]);
  advRect.setTopLeftPos(positions[7]+Coord(-92,16,0));
  advRect.setBottomRightPos(positions[7]);

  _controls[0].set(positions[0], 7, 0.0); //t
  _controls[1].set(positions[1], 6, float(M_PI)/4.f); //c
  _controls[2].set(positions[2], 7, float(-M_PI)/2.f); //t
  _controls[3].set(positions[3], 5, 0.);//s
  _controls[4].set(positions[4], 7, float(M_PI));//t
  _controls[5].set(positions[5], 6, float(M_PI)/4.f);//c
  _controls[6].set(positions[6], 7, float(M_PI)/2.f);//
  _controls[7].set(positions[7], 5, 0.);//s

  vector<Coord> advControlVect;

  for(unsigned int i=0; i<arrowWithLineSize; ++i) {
    advControlVect.push_back(arrowWithLine[i]+positions[7]+Coord(-11,8,0));
  }

  _advControls[0] = GlComplexPolygon(advControlVect,Color(255,40,40,200),Color(128,20,20,200));
  advControlVect.clear();

  for(unsigned int i=0; i<arrowWithLineSize; ++i) {
    advControlVect.push_back(Coord(arrowWithLine[i][0],-arrowWithLine[i][1],0)+positions[7]+Coord(-25,8,0));
  }

  _advControls[1] = GlComplexPolygon(advControlVect,Color(255,40,40,200),Color(128,20,20,200));
  advControlVect.clear();

  for(unsigned int i=0; i<arrowWithLineSize; ++i) {
    advControlVect.push_back(Coord(-arrowWithLine[i][1],arrowWithLine[i][0],0)+positions[7]+Coord(-39,8,0));
  }

  _advControls[2] = GlComplexPolygon(advControlVect,Color(255,40,40,200),Color(128,20,20,200));
  advControlVect.clear();

  for(unsigned int i=0; i<arrowWithLineSize; ++i) {
    advControlVect.push_back(Coord(arrowWithLine[i][1],arrowWithLine[i][0],0)+positions[7]+Coord(-53,8,0));
  }

  _advControls[3] = GlComplexPolygon(advControlVect,Color(255,40,40,200),Color(128,20,20,200));
  advControlVect.clear();

  for(unsigned int i=0; i<twoArrowWithLineSize; ++i) {
    advControlVect.push_back(twoArrowWithLine[i]+positions[7]+Coord(-67,8,0));
  }

  _advControls[4] = GlComplexPolygon(advControlVect,Color(255,40,40,200),Color(128,20,20,200));
  advControlVect.clear();

  for(unsigned int i=0; i<twoArrowWithLineSize; ++i) {
    advControlVect.push_back(Coord(twoArrowWithLine[i][1],twoArrowWithLine[i][0],0)+positions[7]+Coord(-81,8,0));
  }

  _advControls[5] = GlComplexPolygon(advControlVect,Color(255,40,40,200),Color(128,20,20,200));
  advControlVect.clear();

  for(unsigned int i=0; i<6; ++i) {
    _advControls[i].setStencil(0);
  }

  return true;
}
//========================================================================================
