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
#include "PluginsWidget.h"

#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <iostream>

#include "PluginsViewWidget.h"
#include "PluginsInfoWidget.h"
#include "ServersOptionDialog.h"


//#include "../XML_Plugin_Info/LocalPluginLoaderXML.h"
//#include "../ServerManager/PluginInfo.h"

using namespace std;

namespace tlp {

  void PluginsWidget::createWidget(QWidget *parent) {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    QHBoxLayout* boxLayout = new QHBoxLayout();
    boxLayout->setParent(mainLayout);
    mainLayout->addItem(boxLayout);

    connect(serverManager, SIGNAL(newPluginList()), this, SLOT(updatePluginsTree()));

    pluginsList = new PluginsViewWidget(serverManager,this);
    boxLayout->addWidget(pluginsList);
    
    pluginsInfo = new PluginsInfoWidget(serverManager,this);
    boxLayout->addWidget(pluginsInfo);
    connect(pluginsList, SIGNAL(pluginInfoSignal(const PluginInfo*)), this, SLOT(clickOnPluginSlot(const PluginInfo *)));

    updatePluginsTree();

    serverDialog=new ServersOptionDialog(serverManager,parent);
  }
  
  PluginsWidget::PluginsWidget(vector<LocalPluginInfo> &plugins,QWidget *parent):QWidget(parent){
    serverManager = new MultiServerManager(plugins);
   
    createWidget(parent);
  }

  PluginsWidget::PluginsWidget(MultiServerManager *msm,QWidget *parent):QWidget(parent){
    serverManager = msm;

    createWidget(parent);
  }

  void PluginsWidget::addServer(const string &serverAddr) {
    serverManager->addServer(serverAddr);
  }

  PluginsWidget::~PluginsWidget(){
    delete serverManager;
  }


  void PluginsWidget::updatePluginsTree(){
    pluginsList->changeList();
    repaint();
    emit pluginsViewIsUpdated();
  }


  void PluginsWidget::serverPopup(){
    if(serverDialog->isVisible()){
      serverDialog->raise();
    }
    else {
      serverDialog->setVisible(true);
      serverDialog->exec();
    }
    updatePluginsTree();
  }

  void PluginsWidget::modifyTreeView(int viewNumber, bool lastVersion, bool compatiblesVersion, bool notinstalledVersion){
    serverManager->modifyTreeView(viewNumber);
    pluginsList->setLastVersion(lastVersion);
    pluginsList->setCompatiblesVersion(compatiblesVersion);
    pluginsList->setNotinstalledVersion(notinstalledVersion);
    pluginsList->changeList();
  }
  
  void PluginsWidget::serverViewSlot(){
    modifyTreeView(VIEW_BY_SERVER, true, true, true);
  }

  void PluginsWidget::groupViewSlot(){
    modifyTreeView(VIEW_BY_TYPE, true, true, true);
  }

  void PluginsWidget::pluginViewSlot(){
    modifyTreeView(VIEW_BY_NAME, true, true, true);
  }

  void PluginsWidget::clickOnPluginSlot(const PluginInfo *pi){
    string addr;
    serverManager->getAddr(pi->server,addr);
    pluginsInfo->setPluginInfo(pi,addr);
  }
  
  void PluginsWidget::applyChangeSlot(){
    pluginsList->applyChange();  
  }
  
  void PluginsWidget::restoreSlot(){
    pluginsList->restore();
    updatePluginsTree();  
  }

  
}
