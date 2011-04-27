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
  
QWidget *ControllerViewsTools::noInteractorConfigurationWidget=0;

  void ControllerViewsTools::createView(const string &name,Graph *,DataSet,QWidget *parent,string *createdViewName, View **createdView, QWidget **createdWidget){
    string verifiedName=name;
    View *newView=ViewPluginsManager::getInst().createView(name);

    // if we can not create a view with given name : create a Node Link Diagram Component 
    if(!newView){
      verifiedName=mainViewName;
      newView=ViewPluginsManager::getInst().createView(mainViewName);
    }

    // Get interactors for this view and add them to view
    list<string> interactorsNameList;
    if(newView->getRealViewName()=="")
    	interactorsNameList=InteractorManager::getInst().getSortedCompatibleInteractors(verifiedName);
    else
    	interactorsNameList=InteractorManager::getInst().getSortedCompatibleInteractors(newView->getRealViewName());

    list<Interactor *> interactorsList;
    for(list<string>::iterator it=interactorsNameList.begin();it!=interactorsNameList.end();++it){
      interactorsList.push_back(InteractorManager::getInst().getInteractor(*it));
    }
    newView->setInteractors(interactorsList);

    QWidget *widget;
    widget=newView->construct(parent);
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
    QWidget *noInteractorConfigurationWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(noInteractorConfigurationWidget);
    QLabel *label = new QLabel(noInteractorConfigurationWidget);
    label->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(label, 0, 0, 1, 1);
    label->setText("No interactor configuration");
    return noInteractorConfigurationWidget;
  }
  
}

