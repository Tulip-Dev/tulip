//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#define SPACE_BETWEEN_AXIS 100
#define AXIS_HEIGHT 200

#include <tulip/GlSphere.h>
#include <tulip/GlBox.h>
#include <tulip/GlPolygon.h>
#include <tulip/BooleanProperty.h>
//#include <tulip/Qt3ForTulip.h>

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QMenu>
#include <Qt/qapplication.h>

#include <vector>

#include "ParallelCoordinatesWidget.h"
#include "ParallelTools.h"
#include "ViewConfigDialog.h"

using namespace std;

namespace tlp {

  ParallelCoordinatesWidget::ParallelCoordinatesWidget(QWidget *parent) : GlMainWidget(parent), viewLayer(NULL), 
									  parallelCoordsView(NULL),vcd(NULL)  {
  
  glGraphComposite = new GlGraphComposite(tlp::newGraph());
  GlGraphRenderingParameters param = glGraphComposite->getRenderingParameters();
  DataSet glGraphData;
  dataSet = new DataSet();
  if (dataSet->get<DataSet>("displaying", glGraphData)) {
    param.setParameters(glGraphData);
    glGraphComposite->setRenderingParameters(param);
  }
  mainLayer = new GlLayer("Main");
  mainLayer->addGlEntity(glGraphComposite, "graph");

  getScene()->addLayer(mainLayer);
  getScene()->addGlGraphCompositeInfo(mainLayer, glGraphComposite);
  
  navigator = new MouseNKeysNavigator();
  pushInteractor(navigator);
  installEventFilter(this);
  
}

ParallelCoordinatesWidget::~ParallelCoordinatesWidget() {
  if (parallelCoordsView != NULL) {
    delete parallelCoordsView;
    clearObservers();
  }

  if (viewLayer != NULL) {
    delete viewLayer;
  }

  delete mainLayer;
  delete dataSet;
  delete glGraphComposite;
  delete navigator;
}

void ParallelCoordinatesWidget::initObservers() {
  
  if (graphProxy==0) return;
  /*for (unsigned int i=0; i<NB_VIEWED_PROPERTIES; ++i) {
    if (graphProxy->existProperty(viewed_properties[i]))
      graphProxy->getProperty(viewed_properties[i])->addObserver(this);
      }*/

  graphProxy->pushObserver(this);

  list<string> props = graphProxy->getAllProperties();
  list<string>::iterator it;
  for (it = props.begin() ; it != props.end() ; ++it) {
    if (graphProxy->existProperty(*it)) {
      graphProxy->getProperty(*it)->addObserver(this);
    }
  }
  
}

void ParallelCoordinatesWidget::clearObservers() {
  
  if (graphProxy == 0) return;
  /*for (unsigned int i=0; i<NB_VIEWED_PROPERTIES; ++i) {
    if (graphProxy->existProperty(viewed_properties[i])) {
      graphProxy->getProperty(viewed_properties[i])->removeObserver(this);
    }
    }*/
  
  graphProxy->popObserver(this);
  
  list<string> props = graphProxy->getAllProperties();
  list<string>::iterator it;
  for (it = props.begin() ; it != props.end() ; ++it) {
    if (graphProxy->existProperty(*it)) {
      graphProxy->getProperty(*it)->removeObserver(this);
    }
  }
  
}

  void ParallelCoordinatesWidget::setData(Graph *graph,string *in) {
  this->graph=graph;
  if (viewLayer == NULL) {
    viewLayer = new GlLayer("Parallel Coordinates View");
    getScene()->addLayer(viewLayer);
  }

  graphProxy = new ParallelCoordinatesGraphProxy(graph);

  if(!vcd) {
    vcd=new ViewConfigDialog(graphProxy, this);
    vcd->setModal(true);
  }
  //vcd.exec();
  
  graphProxy->setSelectedProperties(vcd->getSelectedProperties());
  graphProxy->setDataLocation(vcd->getDataLocation());
  
  createView(vcd->getViewType());

  getScene()->centerScene();
  draw();
}

  Graph* ParallelCoordinatesWidget::getGraph() {
    return graph;
  }


void ParallelCoordinatesWidget::createView(viewType vType) {

  if (parallelCoordsView != NULL) {
    viewLayer->deleteGlEntity(parallelCoordsView);
    delete parallelCoordsView;
    parallelCoordsView = NULL;
    clearObservers();
  }

  unsigned int nbAxis = graphProxy->getNumberOfSelectedProperties();
  
  parallelCoordsView = new ParallelCoordinatesView(graphProxy, Coord(10,10), 
						   (nbAxis - 1) * SPACE_BETWEEN_AXIS, 
						   AXIS_HEIGHT,
						   vType);
  initObservers();
  viewLayer->addGlEntity(parallelCoordsView, "Parallel Coordinates");
}

  void ParallelCoordinatesWidget::showConfigDialog() {
    vcd->exec();
    setData(graph);
  }

void ParallelCoordinatesWidget::delNode(Graph *g, const node n) {
  parallelCoordsView->deleteDataRepresentation(n.id);
  draw();  
}

void ParallelCoordinatesWidget::delEdge(Graph *g,const edge e) {
  parallelCoordsView->deleteDataRepresentation(e.id);
  draw();
}

void ParallelCoordinatesWidget::update(set<Observable *>::iterator begin ,set<Observable *>::iterator end) {
  if (parallelCoordsView != NULL) {
    Observable::holdObservers();
    clearObservers();
    parallelCoordsView->redrawView();
    draw();
    Observable::unholdObservers();
    initObservers();
  }
}

void ParallelCoordinatesWidget::swapAxis(string axisName1, string axisName2){
  parallelCoordsView->swapAxis(axisName1, axisName2);
  draw();
}

void ParallelCoordinatesWidget::setViewType(viewType vType) {
  if (parallelCoordsView != NULL) {
    parallelCoordsView->setViewType(vType);
  }
}

bool ParallelCoordinatesWidget::eventFilter(QObject *obj, QEvent *e) {
  
  if (obj == this && graphProxy != NULL &&
      (e->type() == QEvent::MouseButtonRelease)) {
    
    popInteractor();
    
    QMouseEvent *me = (QMouseEvent *) e;
    if (me->button()==Qt::RightButton) {
     
      vector<GlEntity *> selectedEntities;
      bool result;
      result = selectGlEntities(me->x(), me->y(), selectedEntities, viewLayer);
      
      if (result) {
	
	vector<GlEntity *>::iterator it;
	int selectedData = -1;
	for (it = selectedEntities.begin() ; it != selectedEntities.end() ; ++it) {
	  GlEntity *entity = *it;
	  selectedData = parallelCoordsView->getDataIdFromGlEntity(entity);
	  if (selectedData != -1)
	    break;
	}
	
	if (selectedData != -1) {
	
	  /*bool isNode = graphProxy->getDataLocation() == NODES;
	  int itemId = selectedData;
	  QMenu contextMenu("dd",this);
	  stringstream sstr;
	  sstr << (isNode ? "Node " : "Edge ") << itemId;
	  contextMenu.setItemEnabled(contextMenu.insertItem(tr(sstr.str().c_str())), false);
	  contextMenu.insertSeparator();
	  contextMenu.insertItem(tr("Add to/Remove from selection"));
	  int selectId = contextMenu.insertItem(tr("Select"));
	  int deleteId = contextMenu.insertItem(tr("Delete"));
	  contextMenu.insertSeparator();
	  contextMenu.insertSeparator();
	  int propId = contextMenu.insertItem(tr("Properties"));
	  
	  int menuId = contextMenu.exec(me->globalPos(), 2);
	  if (menuId == -1)
	    return true;
      
	  Observable::holdObservers();
      
	  if (menuId == deleteId) { // Delete
	    // delete graph item
	    if (isNode)
	      graphProxy->delNode(node(itemId));
	    else
	      graphProxy->delEdge(edge(itemId));
	  } else if (menuId == propId) { // Properties
	    ShowEltPropertyEvent showEltPropsEvent(itemId, isNode);
	    QApplication::sendEvent(this, &showEltPropsEvent);
	  } else {
	    BooleanProperty *elementSelected = graphProxy->getLocalProperty<BooleanProperty>("viewSelection");
	    if (menuId == selectId) { // Select
	      // empty selection
	      elementSelected->setAllNodeValue(false);
	      elementSelected->setAllEdgeValue(false);
	      graphProxy->setDataSelected(itemId, true);
	    } else {
	      // selection add/remove graph item
	      graphProxy->setDataSelected(itemId, !graphProxy->isDataSelected(itemId));
					  
	    }
	  }
	  
	  Observable::unholdObservers();*/
	  
	  pushInteractor(navigator);

	  return true;
	}
	
      } else {
	
	/*Q3PopupMenu contextMenu(this,"dd");
	int centerViewId = contextMenu.insertItem(tr("Center View"));
	int viewConfigId = contextMenu.insertItem(tr("View Configuration"));
	
	int menuId = contextMenu.exec(me->globalPos(), 2);
	
	if (menuId == centerViewId) {
	  getScene()->centerScene();
	  draw();
	} else if (menuId == viewConfigId) {
	  ViewConfigDialog vcd(graphProxy, this);
	  vcd.setModal(true);
	  vcd.setViewType(parallelCoordsView->getViewType());
	  vcd.setDataLocation(graphProxy->getDataLocation());
	  vcd.exec();
	  
	  graphProxy->setSelectedProperties(vcd.getSelectedProperties());
	  graphProxy->setDataLocation(vcd.getDataLocation());
	  createView(vcd.getViewType());
	  draw();
	  } */

      }
    }
    pushInteractor(navigator);
      
    return true;
  }
  
    
  return false;
}

}
