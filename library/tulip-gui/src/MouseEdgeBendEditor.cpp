/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/GlMainWidget.h>
#include <tulip/MouseEdgeBendEditor.h>

#include <QtGui/qevent.h>

using namespace tlp;
using namespace std;

//========================================================================================
MouseEdgeBendEditor::MouseEdgeBendEditor()
  :glMainWidget(NULL),layer(NULL),targetTriangle(Coord(0,0,0),Size(1,1,1)),circleString(NULL){
  operation = NONE_OP;
  _copyLayout = 0;
  _copySizes = 0;
  _copyRotation = 0;

  basicCircle.resizePoints(30); //circle
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(255, 102, 255, 200));
  basicCircle.setOutlineColor(Color(128, 20, 20, 200));
  basicCircle.setStencil(0);
}
//========================================================================================
MouseEdgeBendEditor::~MouseEdgeBendEditor(){
  if(layer)
    glMainWidget->getScene()->removeLayer(layer,true);
}
//========================================================================================
bool MouseEdgeBendEditor::eventFilter(QObject *widget, QEvent *e) {

  QMouseEvent * qMouseEv = (QMouseEvent *) e;
  if(qMouseEv == NULL)
    return false;

  // Double click to create a new control point
  if(e->type() == QEvent::MouseButtonDblClick &&  qMouseEv->button() == Qt::LeftButton && haveSelection(glMainWidget)) {
    operation = NEW_OP;
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
        operation = NONE_OP;
        return false;
      }

      bool entityIsSelected = glMainWidget->selectGlEntities((int)editPosition[0] - 3, (int)editPosition[1] - 3, 6, 6, select, layer);

      if(!entityIsSelected){
        // We have click outside an entity
        operation = NONE_OP;
        return false;
      }

      if (entityIsSelected) {
        selectedEntity=circleString->findKey((GlSimpleEntity*)(select[0]));
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
      }
      break;
    }

    case Qt::MidButton :{
      undoEdition();
      break;
    }
    default:{
      return false;
    }
    }

    glMainWidget->redraw();
    return true;
  }

  if (e->type() == QEvent::MouseButtonRelease &&
      qMouseEv->button() == Qt::LeftButton &&
      operation != NONE_OP) {
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;

    if(selectedEntity=="targetTriangle"){
      node tmpNode;
      edge tmpEdge;
      ElementType type;
      if (glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge) && type == NODE){
        glMainWidget->getGraph()->setEnds(mEdge,glMainWidget->getGraph()->ends(mEdge).first,tmpNode);
      }
    }else if(selectedEntity=="sourceCircle"){
      node tmpNode;
      edge tmpEdge;
      ElementType type;
      if (glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge) && type == NODE){
        glMainWidget->getGraph()->setEnds(mEdge,tmpNode,glMainWidget->getGraph()->ends(mEdge).second);
      }
    }

    stopEdition();
    glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));
    glMainWidget->redraw();
    return true;
  }
  if  (e->type() == QEvent::MouseMove) {
    if(qMouseEv->buttons() == Qt::LeftButton &&
      operation != NONE_OP) {
      GlMainWidget *glMainWidget = (GlMainWidget *) widget;
      switch (operation) {
      case TRANSLATE_OP:
        mMouseTranslate(qMouseEv->x(), qMouseEv->y(), glMainWidget);
        return true;
      default:
        return false;
      }
    }
    else if(qMouseEv->buttons() == Qt::NoButton){
      node tmpNode;
      edge tmpEdge;
      ElementType type;
      GlMainWidget *g = (GlMainWidget *) widget;
      if (g->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge) && type == EDGE) {
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
    if(operation == NONE_OP)
      glMainWidget->setCursor(QCursor(Qt::PointingHandCursor));

    if(!layer){
      layer=new GlLayer("edgeBendEditorLayer",true);
      layer->setCamera(Camera(glMainWidget->getScene(),false));
      glMainWidget->getScene()->insertLayerAfter(layer,"Main");
      if(!circleString)
        circleString = new GlComposite(false);
      layer->addGlEntity(circleString,"selectionComposite");
    }
    this->glMainWidget=glMainWidget;
    return true;
  }else{
    glMainWidget->setCursor(QCursor(Qt::CrossCursor));
  }
  return false;
}
//========================================================================================
bool MouseEdgeBendEditor::draw(GlMainWidget*) {
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
  if (operation == NONE_OP) return;
  operation = NONE_OP;
  delete _copyLayout;   _copyLayout = 0;
  delete _copySizes;    _copySizes = 0;
  delete _copyRotation; _copyRotation = 0;
  selectedEntity="";
  computeSrcTgtEntities(glMainWidget);
  glMainWidget->draw(false);
}
//========================================================================================
void MouseEdgeBendEditor::initProxies(GlMainWidget *glMainWidget) {
  GlGraphInputData *inputData=glMainWidget->getScene()->getGlGraphComposite()->getInputData();
  _graph     = inputData->getGraph();
  _layout    = _graph->getProperty<LayoutProperty>(inputData->getElementLayoutPropName());
  _selection = _graph->getProperty<BooleanProperty>(inputData->getElementSelectedPropName());
  _rotation  = _graph->getProperty<DoubleProperty>(inputData->getElementRotationPropName());
  _sizes     = _graph->getProperty<SizeProperty>(inputData->getElementSizePropName());
  _shape     = _graph->getProperty<IntegerProperty>(inputData->getElementShapePropName());
  if(_graph->existProperty("viewPolygonCoords"))
    _coordsVectorProperty=_graph->getProperty<CoordVectorProperty>("viewPolygonCoords");
  else
    _coordsVectorProperty=NULL;
}
//========================================================================================
//Does the point p belong to the segment [u,v]?
bool MouseEdgeBendEditor::belong(Coord u, Coord v, Coord p, GlMainWidget *glMainWidget)
{
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
  if(selectedEntity=="targetTriangle"){
    targetTriangle.translate(Coord(-(double)(editPosition[0] - newX), (double)(editPosition[1] - newY),0));
    glMainWidget->draw(false);
  }else if(selectedEntity=="sourceCircle"){
    sourceCircle.translate(Coord(-(double)(editPosition[0] - newX), (double)(editPosition[1] - newY),0));
    glMainWidget->draw(false);
  }else{
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
void MouseEdgeBendEditor::mMouseDelete()
{
  if(selectedEntity!="targetTriangle" && selectedEntity!="sourceCircle"){
    int i;
    IntegerType::fromString(i, selectedEntity);
    vector<Coord>::iterator CoordIt=coordinates.begin();
    vector<tlp::GlCircle>::iterator circleIt=circles.begin();
    int tmp=0;
    while(tmp!=i){
      tmp++;CoordIt++;circleIt++;
    }
    if(!edgeSelected && coordinates.size() <= 3)
      return;
    coordinates.erase(CoordIt);
    circles.erase(circleIt);
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
  else{
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
    if(!firstSeg && !lastSeg && !firstLastSeg){
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
    if(_selection->getEdgeValue(ite)){
      if(hasSelection){
        _selection->setAllEdgeValue(false);
        _selection->setAllNodeValue(false);
        hasSelection=false;
        multipleSelection=true;
        break;
      }else{
        mEdge=ite;
        edgeSelected=true;
        hasSelection=true;
      }
    }
  }
  delete itE;
  if(!multipleSelection){
    itN=_graph->getNodes();
    while(itN->hasNext()){
      itn=itN->next();
      if(_selection->getNodeValue(itn)){
        if(hasSelection){
          _selection->setAllEdgeValue(false);
          _selection->setAllNodeValue(false);
          hasSelection=false;
          break;
        }else{
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
void MouseEdgeBendEditor::computeSrcTgtEntities(GlMainWidget *glMainWidget){
  float endAngle=0.;
  if(coordinates.empty()){
    endAngle=atan((start[1]-end[1])/(start[0]-end[0]));
    if(start[0]-end[0]>0)
      endAngle+=M_PI;
  }else{
    endAngle=atan((end[1]-coordinates[coordinates.size()-1][1])/(end[0]-coordinates[coordinates.size()-1][0]));
    if(end[0]-coordinates[coordinates.size()-1][0]>0)
      endAngle+=M_PI;
  }

  if(selectedEntity!="targetTriangle"){
    Coord tmp(glMainWidget->getScene()->getLayer("Main")->getCamera().worldTo2DScreen(end));
    targetTriangle=GlTriangle(tmp,Size(7,7,0),Color(255, 102, 255, 200),Color(128, 20, 20, 200));
    targetTriangle.setStartAngle(M_PI+endAngle);
    targetTriangle.setStencil(0);
  }

  if(selectedEntity!="sourceCircle"){
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

  if(!hasSelection)
    return false;

  if(edgeSelected){
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
      CoordIt++;
    }
  }else{
    int complexPolygonGlyphId=GlyphManager::getInst().glyphId("2D - Complex Polygon");
    if(_shape->getNodeValue(mNode)==complexPolygonGlyphId && complexPolygonGlyphId!=0){
      if(_coordsVectorProperty){
        vector<Coord> baseCoordinates=_coordsVectorProperty->getNodeValue(mNode);
        vector<Coord> coordinatesWithRotation;

        Coord min,max;
        min=baseCoordinates[0];
        max=baseCoordinates[0];
        for(vector<Coord>::const_iterator it=baseCoordinates.begin();it!=baseCoordinates.end();++it){
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
        for(vector<Coord>::const_iterator it=baseCoordinates.begin();it!=baseCoordinates.end();++it){
          Coord tmp(Coord((((*it)[0]-min[0])/(max[0]-min[0]))*nodeSize[0]-nodeSize[0]/2.,(((*it)[1]-min[1])/(max[1]-min[1]))*nodeSize[1]-nodeSize[1]/2.,0));
          coordinatesWithRotation.push_back(_layout->getNodeValue(mNode)+Coord(tmp[0]*cos(rotation)-tmp[1]*sin(rotation),tmp[0]*sin(rotation)+tmp[1]*cos(rotation),0));
          coordinates.push_back(_layout->getNodeValue(mNode)+tmp);
        }

        vector<Coord>::iterator coordIt=coordinatesWithRotation.begin();
        while(coordIt!=coordinatesWithRotation.end()){
          Coord tmp(glMainWidget->getScene()->getLayer("Main")->getCamera().worldTo2DScreen(*coordIt));
          basicCircle.set(tmp, 5, 0.);
          circles.push_back(basicCircle);
          coordIt++;
        }
      }
    }
  }



  for(unsigned int i=0;i<circles.size();i++)
    circleString->addGlEntity(&circles[i], IntegerType::toString(i));
  return hasSelection;
}
//========================================================================================
