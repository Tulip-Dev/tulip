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
#include "ParallelView.h"

#include <tulip/InteractorManager.h>

using namespace std;

namespace tlp {

  VIEWPLUGIN(ParallelCoordinatesViewCreator, "ParallelCoordinatesView", "Tulip Team", "16/04/2008", "parallel coordinates view", "1.0", 2);

  View* ParallelCoordinatesViewCreator::create(const std::string &pluginName,QWidget *parent){
    return new ParallelView(pluginName,parent);
  }

  ParallelView::ParallelView(const std::string &pluginName,QWidget *parent) : View(pluginName,parent){
    setupUi(this);

    mainWidget=new ParallelCoordinatesWidget(this);
    setCentralWidget(mainWidget);

    dialogMenu->addAction("Config");
    connect(dialogMenu, SIGNAL(triggered(QAction*)), SLOT(showDialog(QAction*)));

    constructInteractorsMap();
  }

  ParallelView::~ParallelView() {
    delete mainWidget;
  }

  void ParallelView::setData(Graph *graph,DataSet dataSet) {
    mainWidget->setData(graph,dataSet);
  }
  //==================================================
  DataSet ParallelView::getData() {
    return mainWidget->getData();
  }
  Graph* ParallelView::getGraph() {
    return mainWidget->getGraph();
  }

  //==================================================
  // Interactor functions
  //==================================================
  void ParallelView::constructInteractorsMap() {
    MutableContainer<Interactor *> interactors;
    InteractorManager::getInst().initInteractorList(interactors);
    interactorsMap["graphNavigate"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseNKeysNavigator")));
    interactorsMap["select"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["select"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseShowElementInfos")));
    interactorsMap["selection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["selection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseSelector")));
    interactorsMap["zoomBox"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["zoomBox"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseBoxZoomer")));
  }
  //==================================================
  void ParallelView::getInteractorsActionList(list<QAction*> &interactorsList) {
    interactorsList.push_back(new QAction(QIcon(":/i_navigation.png"),"graphNavigate",this));
    interactorsList.push_back(new QAction(QIcon(":/i_select.png"),"select",this));
    interactorsList.push_back(new QAction(QIcon(":/i_selection.png"),"selection",this));
    interactorsList.push_back(new QAction(QIcon(":/i_magic.png"),"magicSelection",this));
    interactorsList.push_back(new QAction(QIcon(":/i_zoom.png"),"zoomBox",this));
  }
  Iterator<Interactor *> *ParallelView::installInteractor(const string &name) {
    mainWidget->resetInteractors(interactorsMap[name]);
    return mainWidget->getInteractors();
  }

  void ParallelView::showDialog(QAction *action) {
    mainWidget->showConfigDialog();
  }

}
