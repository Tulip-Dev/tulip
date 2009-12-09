#include "tulip/ControllerViewsTools.h"

#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

#include "tulip/ViewPluginsManager.h"
#include "tulip/InteractorManager.h"
#include "tulip/Interactor.h"

using namespace std;

namespace tlp {
  
  static const string mainViewName="Node Link Diagram view";
  
#ifdef _WIN32
#ifdef DLL_EXPORT
QWidget *ControllerViewsTools::noInteractorConfigurationWidget=0;
#endif
#else
QWidget *ControllerViewsTools::noInteractorConfigurationWidget=0;
#endif

  void ControllerViewsTools::createView(const string &name,Graph *graph,DataSet dataSet,QWidget *parent,string *createdViewName, View **createdView, QWidget **createdWidget){
    string verifiedName=name;
    View *newView=ViewPluginsManager::getInst().createView(name);

    // if we can not create a view with given name : create a Node Link Diagram Component 
    if(!newView){
      verifiedName=mainViewName;
      newView=ViewPluginsManager::getInst().createView(mainViewName);
    }

    // Get interactors for this view and add them to view
    multimap<int,string> interactorsNamesAndPriorityMap;
    if(newView->getRealViewName()=="")
      interactorsNamesAndPriorityMap=InteractorManager::getInst().getSortedCompatibleInteractors(verifiedName);
    else
      interactorsNamesAndPriorityMap=InteractorManager::getInst().getSortedCompatibleInteractors(newView->getRealViewName());

    list<Interactor *> interactorsList;
    for(multimap<int,string>::reverse_iterator it=interactorsNamesAndPriorityMap.rbegin();it!=interactorsNamesAndPriorityMap.rend();++it){
      interactorsList.push_back(InteractorManager::getInst().getInteractor((*it).second));
    }
    newView->setInteractors(interactorsList);

    QWidget *widget;
    widget=newView->construct(parent);
    newView->setData(graph,dataSet);
    widget->setObjectName(QString("ViewMainWidget p:")+QString::number((long)widget));
    widget->setAttribute(Qt::WA_DeleteOnClose,true);

    *createdViewName=verifiedName;
    *createdView=newView;
    *createdWidget=widget;
  }
  //**********************************************************************
  void ControllerViewsTools::createMainView(Graph *graph,DataSet dataSet,QWidget *parent,View **createdView, QWidget **createdWidget){
    string tmp;
    createView(mainViewName,graph,dataSet,parent,&tmp, createdView, createdWidget);
  }
  //**********************************************************************
  void ControllerViewsTools::installInteractors(View *view,QToolBar *toolBar) {
    toolBar->clear();
    
    list<Interactor *> interactorsList=view->getInteractors();
    list<QAction *> interactorsActionList;
    for(list<Interactor *>::iterator it=interactorsList.begin();it!=interactorsList.end();++it)
      interactorsActionList.push_back((*it)->getAction());
    
    for(list<QAction *>::iterator it=interactorsActionList.begin();it!=interactorsActionList.end();++it) {
      toolBar->addAction(*it);
    }
  }
  //**********************************************************************
  void ControllerViewsTools::changeInteractor(View *view,QToolBar *toolBar,QAction* action,QWidget **createdConfigurationWidget) {
    QList<QAction*> actions=toolBar->actions();
    for(QList<QAction*>::iterator it=actions.begin();it!=actions.end();++it) {
      (*it)->setChecked(false);
    }
    
    action->setCheckable(true);
    action->setChecked(true);
    
    InteractorAction *interactorAction=(InteractorAction *)action;
    view->setActiveInteractor(interactorAction->getInteractor());
    
    QWidget *interactorWidget=interactorAction->getInteractor()->getConfigurationWidget();
    QWidget *containerWidget=new QWidget();
    QGridLayout *gridLayout = new QGridLayout(containerWidget);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);
    if(interactorWidget){
      gridLayout->addWidget(interactorWidget,0,0);
    }else{
      gridLayout->addWidget(getNoInteractorConfigurationWidget(),0,0);
    }
    
    *createdConfigurationWidget=containerWidget;
  }
  //**********************************************************************
  QWidget *ControllerViewsTools::getNoInteractorConfigurationWidget(){
    if(!noInteractorConfigurationWidget){
      noInteractorConfigurationWidget = new QWidget();
      QGridLayout *gridLayout = new QGridLayout(noInteractorConfigurationWidget);
      QLabel *label = new QLabel(noInteractorConfigurationWidget);
      label->setAlignment(Qt::AlignCenter);
      gridLayout->addWidget(label, 0, 0, 1, 1);
      label->setText("No interactor configuration");
    }
    return noInteractorConfigurationWidget;
  }
  
}

