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
#include "mainwindow.h"

QtViewerMainWindow::~QtViewerMainWindow(){
	clearObservers();
	delete nodeLinkView;
	delete graph;
}

QtViewerMainWindow::QtViewerMainWindow(const string &filename) : QMainWindow(), graph(0), nodeLinkView(0) {
  nodeLinkView = new NodeLinkDiagramComponent();
  QWidget *widget=nodeLinkView->construct(this);
  setCentralWidget(widget);
  resize(800, 600);

  // Get interactors for this view and add them to view
  multimap<int,string> interactorsNamesAndPriorityMap;
  interactorsNamesAndPriorityMap=InteractorManager::getInst().getSortedCompatibleInteractors("Node Link Diagram view");
  
  list<Interactor *> interactorsList;
  for(multimap<int,string>::reverse_iterator it=interactorsNamesAndPriorityMap.rbegin();it!=interactorsNamesAndPriorityMap.rend();++it){
    interactorsList.push_back(InteractorManager::getInst().getInteractor((*it).second));
  }
  nodeLinkView->setInteractors(interactorsList);
  
  toolBar = new QToolBar(this);
  
  list<QAction *> interactorsActionList;
  for(list<Interactor *>::iterator it=interactorsList.begin();it!=interactorsList.end();++it)
    interactorsActionList.push_back((*it)->getAction());
    
  for(list<QAction *>::iterator it=interactorsActionList.begin();it!=interactorsActionList.end();++it) {
    toolBar->addAction(*it);
    connect(*it, SIGNAL(triggered()), this, SLOT(changeInteractor()));
  }
  addToolBar(toolBar);

  DataSet dataSet;
  dataSet.set("file::filename", filename);
  graph = tlp::importGraph("tlp", dataSet, NULL);
  if (graph) {
    openGraphOnGlMainWidget(graph,&dataSet,nodeLinkView->getGlMainWidget());
    initObservers();
  }
  nodeLinkView->init();
  
  
  QAction *firstInteractor = *(interactorsActionList.begin());
  firstInteractor->activate(QAction::Trigger);
}

void QtViewerMainWindow::changeInteractor() {
  QAction *action = (QAction*) sender();
  QList<QAction*> actions=toolBar->actions();
  for(QList<QAction*>::iterator it=actions.begin();it!=actions.end();++it) {
    (*it)->setChecked(false);
  }
    
  action->setCheckable(true);
  action->setChecked(true);
    
  InteractorAction *interactorAction=(InteractorAction *)action;
  nodeLinkView->setActiveInteractor(interactorAction->getInteractor());
}

void QtViewerMainWindow::initObservers() {
    if (graph) {
    graph->addObserver(this);
    Iterator<PropertyInterface*> *it = graph->getObjectProperties();
    while (it->hasNext()) {
      PropertyInterface* tmp = it->next();
      tmp->addObserver(this);
    } delete it;		
  }
}

void QtViewerMainWindow::clearObservers() {
   if (graph) {
	graph->removeObserver(this);
	Iterator<PropertyInterface*> *it =graph->getObjectProperties();
    	while (it->hasNext()) {
      		(it->next())->removeObserver(this);
    	} delete it;
   }
}

void QtViewerMainWindow::update ( ObserverIterator begin, ObserverIterator end) {
	nodeLinkView->draw();
}
