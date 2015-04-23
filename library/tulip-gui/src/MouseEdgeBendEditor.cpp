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

#include <tulip/GlMainWidget.h>
#include <tulip/MouseEdgeBendEditor.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlyphManager.h>
#include <tulip/Camera.h>

using namespace tlp;
using namespace std;

//========================================================================================
MouseEdgeBendEditor::MouseEdgeBendEditor()
  :glMainWidget(NULL),layer(NULL),edgeEntity(NULL),targetTriangle(Coord(0,0,0),Size(1,1,1)),circleString(NULL),mouseButtonPressOnEdge(false) {
  _operation = NONE_OP;

  basicCircle.resizePoints(30); //circle
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(255, 102, 255, 200));
  basicCircle.setOutlineColor(Color(128, 20, 20, 200));
  basicCircle.setStencil(0);
}
//========================================================================================
MouseEdgeBendEditor::~MouseEdgeBendEditor() {
}
//========================================================================================
void MouseEdgeBendEditor::clear() {
  if (glMainWidget != NULL) {
    glMainWidget->getScene()->removeLayer(layer,false);

    delete layer;
    layer=NULL;
    circleString=NULL;

    glMainWidget->getScene()->getGraphLayer()->deleteGlEntity("edgeEntity");
    delete edgeEntity;
    edgeEntity=NULL;

    glMainWidget->setCursor(QCursor());
  }
}
//========================================================================================

void MouseEdgeBendEditor::stopEdition() {
  _operation = NONE_OP;
}

//========================================================================================
bool MouseEdgeBendEditor::eventFilter(QObject *widget, QEvent *e) {

  QMouseEvent * qMouseEv = (QMouseEvent *) e;

  if(qMouseEv == NULL)
    return false;

  // Double click to create a new control point
  if(e->type() == QEvent::MouseButtonDblClick &&  qMouseEv->button() == Qt::LeftButton && haveSelection(glMainWidget)) {
    _operation = NEW_OP;
    mMouseCreate(qMouseEv->x(), qMouseEv->y(), glMainWidget);
    return true;
  }

  if (e->type() == QEvent::MouseButtonPress) {
    if(!glMainWidget)
      glMainWidget = (GlMainWidget *) widget;

    initProxies(glMainWidget);
    bool hasSelection = haveSelection(glMainWidget);

    editPosition[0] = qMouseEv->x();
    editPosition[1] = qMouseEv->y();
    editPosition[2] = 0;

    switch(qMouseEv->buttons()) {
    case Qt::LeftButton : {

      if (!hasSelection) {
        // event occurs outside the selection rectangle
        // so from now we delegate the job to a MouseEdgeSelector object
        // which should intercept the event
        _operation = NONE_OP;
      }
      else {

        bool entityIsSelected = glMainWidget->pickGlEntities((int)editPosition[0] - 3, (int)editPosition[1] - 3, 6, 6, select, layer);

        if(!entityIsSelected) {
          // We have click outside an entity
          _operation = NONE_OP;
        }
        else {
          selectedEntity=circleString->findKey(select[0].getSimpleEntity());

          if (qMouseEv->modifiers() &
#if defined(__APPLE__)
              Qt::AltModifier
#else
              Qt::ControlModifier
#endif
             ) {
            _operation = DELETE_OP;
            mMouseDelete();
          }
          else {
            _graph->push();
            _operation = TRANSLATE_OP;
            glMainWidget->setCursor(QCursor(Qt::SizeAllCursor));
            mode = COORD;
          }

          return true;
        }
      }

      break;
    }

    default: {
      return false;
    }
    }

    glMainWidget->redraw();
  }

  if (e->type() == QEvent::MouseButtonRelease &&
      qMouseEv->button() == Qt::LeftButton &&
      _operation != NONE_OP) {
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;

    if(selectedEntity=="targetTriangle") {
      SelectedEntity selectedEntity;

      if (glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) && selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        glMainWidget->getScene()->getGlGraphComposite()->getGraph()->setEnds(mEdge,glMainWidget->getScene()->getGlGraphComposite()->getGraph()->ends(mEdge).first,node(selectedEntity.getComplexEntityId()));
      }
    }
    else if(selectedEntity=="sourceCircle") {
      SelectedEntity selectedEntity;

      if (glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) && selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        glMainWidget->getScene()->getGlGraphComposite()->getGraph()->setEnds(mEdge,node(selectedEntity.getComplexEntityId()),glMainWidget->getScene()->getGlGraphComposite()->getGraph()->ends(mEdge).second);
      }
    }

    selectedEntity="";

    _operation = NONE_OP;
    glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));
    glMainWidget->redraw();
    return true;
  }

  if(e->type() == QEvent::MouseButtonPress) {
    vector<SelectedEntity> selectedEntities;
    glMainWidget->pickGlEntities(qMouseEv->x(), qMouseEv->y(), selectedEntities);

    if(!selectedEntities.empty())
      if(selectedEntities[0].getEntityType()==SelectedEntity::SIMPLE_ENTITY_SELECTED)
        if(selectedEntities[0].getSimpleEntity()==edgeEntity) {
          mouseButtonPressOnEdge=true;
          return true;
        }
  }

  if(e->type() == QEvent::MouseButtonRelease) {
    vector<SelectedEntity> selectedEntities;
    glMainWidget->pickGlEntities(qMouseEv->x(), qMouseEv->y(), selectedEntities);

    if(!selectedEntities.empty())
      if(selectedEntities[0].getEntityType()==SelectedEntity::SIMPLE_ENTITY_SELECTED)
        if(selectedEntities[0].getSimpleEntity()==edgeEntity && mouseButtonPressOnEdge) {
          mouseButtonPressOnEdge=false;
          return true;
        }

    mouseButtonPressOnEdge=false;
  }

  if  (e->type() == QEvent::MouseMove) {
    if(qMouseEv->buttons() == Qt::LeftButton &&
        _operation != NONE_OP) {
      GlMainWidget *glMainWidget = (GlMainWidget *) widget;

      switch (_operation) {
      case TRANSLATE_OP:
        mMouseTranslate(qMouseEv->x(), qMouseEv->y(), glMainWidget);
        return true;

      default:
        return false;
      }
    }
    else if(qMouseEv->buttons() == Qt::NoButton) {
      SelectedEntity selectedEntity;
      GlMainWidget *g = (GlMainWidget *) widget;

      if (g->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) && selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        g->setCursor(Qt::CrossCursor);
      }
      else {
        g->setCursor(Qt::ArrowCursor);
      }

      return false;
    }
  }

  return false;
}
//========================================================================================
bool MouseEdgeBendEditor::compute(GlMainWidget *glMainWidget) {
  if (computeBendsCircles(glMainWidget)) {
    if(_operation == NONE_OP)
      glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));

    if(!layer) {
      layer=new GlLayer("edgeBendEditorLayer",true);
      layer->setCamera(new Camera(glMainWidget->getScene(),false));

      if(!circleString)
        circleString = new GlComposite(false);

      layer->addGlEntity(circleString,"selectionComposite");
    }

    bool layerInScene=false;
    const vector<pair<std::string, GlLayer*> > &layersList=glMainWidget->getScene()->getLayersList();

    for(vector<pair<std::string, GlLayer*> >::const_iterator it=layersList.begin(); it!=layersList.end(); ++it) {
      if((*it).second==layer) {
        layerInScene=true;
        break;
      }
    }

    if(!layerInScene)
      glMainWidget->getScene()->addExistingLayerAfter(layer,"Main");

    this->glMainWidget=glMainWidget;
    return true;
  }
  else {
    glMainWidget->setCursor(QCursor(Qt::CrossCursor));
  }

  return false;
}
//========================================================================================
bool MouseEdgeBendEditor::draw(GlMainWidget*) {
  return true;
}
//========================================================================================
void MouseEdgeBendEditor::initProxies(GlMainWidget *glMainWidget) {
  GlGraphInputData *inputData=glMainWidget->getScene()->getGlGraphComposite()->getInputData();
  _graph     = inputData->getGraph();
  _layout    = inputData->getElementLayout();
  _selection = inputData->getElementSelected();
  _rotation  = inputData->getElementRotation();
  _sizes     = inputData->getElementSize();
  _shape     = inputData->getElementShape();

  if(_graph->existProperty("viewPolygonCoords"))
    _coordsVectorProperty=_graph->getProperty<CoordVectorProperty>("viewPolygonCoords");
  else
    _coordsVectorProperty=NULL;
}
//========================================================================================
//Does the point p belong to the segment [u,v]?
bool MouseEdgeBendEditor::belong(Coord u, Coord v, Coord p, GlMainWidget *glMainWidget) {
  int W = glMainWidget->width();
  int H = glMainWidget->height();
  Coord m=glMainWidget->getScene()->getLayer("Main")->getCamera().worldTo2DScreen(u);
  m[0] = W - m[0];
  m[1] = H - m[1];
  Coord n=glMainWidget->getScene()->getLayer("Main")->getCamera().worldTo2DScreen(v);
  n[0] = W - n[0];
  n[1] = H - n[1];
  double mnDist = m.dist(n);
  double mpDist = m.dist(p);
  double pnDist = p.dist(n);
  return ((mpDist + pnDist) - mnDist)/mnDist < 1E-3;
}
//========================================================================================
void MouseEdgeBendEditor::mMouseTranslate(double newX, double newY, GlMainWidget *glMainWidget) {
  initProxies(glMainWidget);

  Coord v0(0,0,0);
  Coord v1((double)(editPosition[0] - newX), -(double)(editPosition[1] - newY),0);
  v0 = glMainWidget->getScene()->getLayer("Main")->getCamera().screenTo3DWorld(v0);
  v1 = glMainWidget->getScene()->getLayer("Main")->getCamera().screenTo3DWorld(v1);
  v1 -= v0;

  if(selectedEntity=="targetTriangle") {
    targetTriangle.translate(Coord(-(double)(editPosition[0] - newX), (double)(editPosition[1] - newY),0));
    glMainWidget->draw(false);
  }
  else if(selectedEntity=="sourceCircle") {
    sourceCircle.translate(Coord(-(double)(editPosition[0] - newX), (double)(editPosition[1] - newY),0));
    glMainWidget->draw(false);
  }
  else {
    int i;
    IntegerType::fromString(i, selectedEntity);
    coordinates[i]+=v1;
    Observable::holdObservers();

    if(edgeSelected)
      _layout->setEdgeValue(mEdge, coordinates);
    else
      _coordsVectorProperty->setNodeValue(mNode,coordinates);

    Observable::unholdObservers();
  }

  editPosition[0]  = newX;
  editPosition[1]  = newY;
}
//========================================================================================
void MouseEdgeBendEditor::mMouseDelete() {
  if(selectedEntity!="targetTriangle" && selectedEntity!="sourceCircle") {
    int i;
    IntegerType::fromString(i, selectedEntity);
    vector<Coord>::iterator CoordIt=coordinates.begin();
    vector<tlp::GlCircle>::iterator circleIt=circles.begin();
    int tmp=0;

    while(tmp!=i) {
      ++tmp;
      ++CoordIt;
      ++circleIt;
    }

    if(!edgeSelected && coordinates.size() <= 3)
      return;

    coordinates.erase(CoordIt);
    circles.erase(circleIt);
    edgeEntity->setCoordinates(start,end,coordinates);
    Observable::holdObservers();
    // allow to undo
    _graph->push();

    if(edgeSelected)
      _layout->setEdgeValue(mEdge, coordinates);
    else
      _coordsVectorProperty->setNodeValue(mNode, coordinates);

    Observable::unholdObservers();
  }
}
//========================================================================================
void MouseEdgeBendEditor::mMouseCreate(double x, double y, GlMainWidget *glMainWidget) {
  Coord screenClick(glMainWidget->width() - (double) x, (double) y, 0);
  Coord worldLocation= glMainWidget->getScene()->getLayer("Main")->getCamera().screenTo3DWorld(screenClick);

  if(coordinates.empty())
    coordinates.push_back(worldLocation);
  else {
    Coord first=coordinates[0];
    Coord last=coordinates[coordinates.size() - 1];
    bool firstSeg=belong(start, first, screenClick, glMainWidget);
    bool lastSeg=belong(end, last, screenClick, glMainWidget);
    bool firstLastSeg=false;

    if(!edgeSelected)
      firstLastSeg=belong(first,last,screenClick,glMainWidget);

    if(firstSeg)
      coordinates.insert(coordinates.begin(),worldLocation);

    if(lastSeg || firstLastSeg)
      coordinates.push_back(worldLocation);

    if(!firstSeg && !lastSeg && !firstLastSeg) {
      vector<Coord>::iterator CoordIt=coordinates.begin();
      last=Coord(CoordIt->getX(), CoordIt->getY(), CoordIt->getZ());
      ++CoordIt;

      while(CoordIt!=coordinates.end()) {
        first=last;
        last=Coord(CoordIt->getX(), CoordIt->getY(), CoordIt->getZ());
        bool midSeg=belong(first, last, screenClick, glMainWidget);

        if(midSeg) {
          coordinates.insert(CoordIt, worldLocation);
          break;
        }

        ++CoordIt;
      }
    }
  }

  Observable::holdObservers();
  // allow to undo
  _graph->push();

  if(edgeSelected)
    _layout->setEdgeValue(mEdge, coordinates);
  else {
    if(_coordsVectorProperty)
      _coordsVectorProperty->setNodeValue(mNode,coordinates);
  }

  Observable::unholdObservers();
}
//========================================================================================
bool MouseEdgeBendEditor::haveSelection(GlMainWidget *glMainWidget) {
  initProxies(glMainWidget);

  Iterator<edge> *itE;
  Iterator<node> *itN;
  edge ite;
  node itn;
  bool hasSelection=false;
  bool multipleSelection=false;

  itE =_graph->getEdges();

  while (itE->hasNext()) {
    ite=itE->next();

    if(_selection->getEdgeValue(ite)) {
      if(hasSelection) {
        hasSelection=false;
        multipleSelection=true;
        break;
      }
      else {
        mEdge=ite;
        edgeSelected=true;
        hasSelection=true;
      }
    }
  }

  delete itE;

  if(!multipleSelection) {
    itN=_graph->getNodes();

    while(itN->hasNext()) {
      itn=itN->next();

      if(_selection->getNodeValue(itn)) {
        if(hasSelection) {
          hasSelection=false;
          break;
        }
        else {
          mNode=itn;
          edgeSelected=false;
          hasSelection=true;
        }
      }
    }

    delete itN;
  }

  return hasSelection;
}
//========================================================================================
void MouseEdgeBendEditor::computeSrcTgtEntities(GlMainWidget *glMainWidget) {
  float endAngle=0.;

  if(coordinates.empty()) {
    endAngle=atan((start[1]-end[1])/(start[0]-end[0]));

    if(start[0]-end[0]>0)
      endAngle+=float(M_PI);
  }
  else {
    endAngle=atan((end[1]-coordinates[coordinates.size()-1][1])/(end[0]-coordinates[coordinates.size()-1][0]));

    if(end[0]-coordinates[coordinates.size()-1][0]>0)
      endAngle+=float(M_PI);
  }

  if(selectedEntity!="targetTriangle") {
    Coord tmp(glMainWidget->getScene()->getLayer("Main")->getCamera().worldTo2DScreen(end));
    targetTriangle=GlTriangle(tmp,Size(7,7,0),Color(255, 102, 255, 200),Color(128, 20, 20, 200));
    targetTriangle.setStartAngle(M_PI+endAngle);
    targetTriangle.setStencil(0);
  }

  if(selectedEntity!="sourceCircle") {
    Coord tmp(glMainWidget->getScene()->getLayer("Main")->getCamera().worldTo2DScreen(start));
    sourceCircle=GlCircle(tmp,6,Color(128, 20, 20, 200),Color(255, 102, 255, 200),true,true);
    sourceCircle.setStencil(0);
  }
}
//========================================================================================
bool MouseEdgeBendEditor::computeBendsCircles(GlMainWidget *glMainWidget) {
  initProxies(glMainWidget);

  Coord tmp;
  coordinates.clear();
  circles.clear();
  select.clear();

  if(circleString)
    circleString->reset(false);
  else
    circleString=new GlComposite(false);

  bool hasSelection=haveSelection(glMainWidget);

  if(!hasSelection) {
    glMainWidget->getScene()->getGraphLayer()->deleteGlEntity("edgeEntity");
    return false;
  }

  if(edgeSelected) {
    coordinates=_layout->getEdgeValue(mEdge);
    start=_layout->getNodeValue(_graph->source(mEdge));
    end=_layout->getNodeValue(_graph->target(mEdge));

    computeSrcTgtEntities(glMainWidget);
    circleString->addGlEntity(&targetTriangle,"targetTriangle");
    circleString->addGlEntity(&sourceCircle,"sourceCircle");

    // Bends circles
    vector<Coord>::iterator CoordIt=coordinates.begin();

    while(CoordIt!=coordinates.end()) {
      tmp=Coord(CoordIt->getX(), CoordIt->getY(), CoordIt->getZ());
      tmp=glMainWidget->getScene()->getLayer("Main")->getCamera().worldTo2DScreen(tmp);
      basicCircle.set(tmp, 5, 0.);
      circles.push_back(basicCircle);
      ++CoordIt;
    }

    if(!edgeEntity)
      edgeEntity=new EdgeEntity;

    edgeEntity->setCoordinates(start,end,coordinates);
    glMainWidget->getScene()->getGraphLayer()->addGlEntity(edgeEntity,"edgeEntity");
  }
  else {
    int complexPolygonGlyphId = GlyphManager::getInst().glyphId("2D - Complex Polygon");

    if(_shape->getNodeValue(mNode)==complexPolygonGlyphId && complexPolygonGlyphId!=0) {
      if(_coordsVectorProperty) {
        vector<Coord> baseCoordinates=_coordsVectorProperty->getNodeValue(mNode);
        vector<Coord> coordinatesWithRotation;

        Coord min,max;
        min=baseCoordinates[0];
        max=baseCoordinates[0];

        for(vector<Coord>::const_iterator it=baseCoordinates.begin(); it!=baseCoordinates.end(); ++it) {
          if((*it)[0]<min[0])
            min[0]=(*it)[0];

          if((*it)[0]>max[0])
            max[0]=(*it)[0];

          if((*it)[1]<min[1])
            min[1]=(*it)[1];

          if((*it)[1]>max[1])
            max[1]=(*it)[1];
        }

        Size nodeSize=_sizes->getNodeValue(mNode);
        double rotation=_rotation->getNodeValue(mNode)*M_PI/180;

        for(vector<Coord>::const_iterator it=baseCoordinates.begin(); it!=baseCoordinates.end(); ++it) {
          Coord tmp(Coord((((*it)[0]-min[0])/(max[0]-min[0]))*nodeSize[0]-nodeSize[0]/2.,(((*it)[1]-min[1])/(max[1]-min[1]))*nodeSize[1]-nodeSize[1]/2.,0));
          coordinatesWithRotation.push_back(_layout->getNodeValue(mNode)+Coord(tmp[0]*cos(rotation)-tmp[1]*sin(rotation),tmp[0]*sin(rotation)+tmp[1]*cos(rotation),0));
          coordinates.push_back(_layout->getNodeValue(mNode)+tmp);
        }

        vector<Coord>::iterator coordIt=coordinatesWithRotation.begin();

        while(coordIt!=coordinatesWithRotation.end()) {
          Coord tmp(glMainWidget->getScene()->getLayer("Main")->getCamera().worldTo2DScreen(*coordIt));
          basicCircle.set(tmp, 5, 0.);
          circles.push_back(basicCircle);
          ++coordIt;
        }
      }
    }
  }



  for(unsigned int i=0; i<circles.size(); ++i)
    circleString->addGlEntity(&circles[i], IntegerType::toString(i));

  return hasSelection;
}
//========================================================================================
