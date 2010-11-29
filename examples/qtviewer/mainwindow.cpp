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
