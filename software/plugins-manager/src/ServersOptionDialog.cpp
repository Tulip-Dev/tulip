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
#include "ServersOptionDialog.h"

#include <QtCore/QSettings>
#include <QtGui/QMessageBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

using namespace std;

namespace tlp {

  ServersOptionDialog::ServersOptionDialog(MultiServerManager* msm,QWidget *parent):QDialog(parent),serverManager(msm){
 
    setWindowTitle("Servers Manager");
  
    // Compute servers list
    serversList = new QListWidget(this);
    updateList();
    
    // Edit dialog
    modifDialog = new ModifyServerDialog(this);
    
    // Buttons
    addButton = new QPushButton("Add Server",this);
    addButton->setObjectName("add");
    removeButton = new QPushButton("Remove",this);
    removeButton->setObjectName("remove");
    propertiesButton = new QPushButton("Properties",this);
    exitButton = new QPushButton("Close",this);

    // Buttons widget
    buttonsWidget = new QWidget(this);
    buttonsWidget->setObjectName("buttonWidget");
    subBoxLayout = new QVBoxLayout(buttonsWidget);
    subBoxLayout->addWidget(addButton);
    subBoxLayout->addWidget(removeButton);
    subBoxLayout->addWidget(propertiesButton);
    subBoxLayout->insertStretch(3);
    subBoxLayout->addWidget(exitButton);
    
    // Big widget
    boxLayout = new QHBoxLayout(this);
    boxLayout->addWidget(serversList);  
    boxLayout->addWidget(buttonsWidget);
    
    //Signal connection
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeServer()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addServer()));
    connect(propertiesButton, SIGNAL(clicked()), this, SLOT(serverProperties()));
    connect(msm,SIGNAL(nameReceived(MultiServerManager*,std::string,std::string)),this,SLOT(serverNameReceived(MultiServerManager*,std::string,std::string)));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));

  }

  ServersOptionDialog::~ServersOptionDialog(){
  }

  void ServersOptionDialog::updateList(){
    serversList->clear();
    vector<string> serversName;
    serverManager->getNames(serversName);
    QStringList strList;
    for(vector<string>::iterator it=serversName.begin();it!=serversName.end();++it) {
      strList.append((*it).c_str());
    }
    serversList->insertItems(0, strList);
  }

  void ServersOptionDialog::removeServer(){
  
    if(serversList->count() <= 0) return;

    int nbRemovedBefore = 0;
    for(int i=0; i<serversList->count(); ++i){
      if(serversList->isItemSelected(serversList->item(i))){
	serverManager->removeServer(i - nbRemovedBefore);
	nbRemovedBefore ++;
      }
    }
 
    updateList();
  }

  void ServersOptionDialog::modifyPopup(){
    if(modifDialog->isVisible()){
      modifDialog->raise();
    }else { 
      modifDialog->setVisible(true);
      modifDialog->exec();
    }
  }
   

  void ServersOptionDialog::addServer(){
    modifDialog->setAddr("");
    modifyPopup();
    serverManager->addServer(modifDialog->getAddr());
    updateList();
  }

  void ServersOptionDialog::serverProperties(){

    for(int i = 0; i < serversList->count() ; ++i) // finds the first selected element
      {
	if(serversList->isItemSelected(serversList->item(i)))
	  {
	    vector<string> addrList;
	    serverManager->getAddrs(addrList);
	    string oldServerAddr = addrList[i];
	    modifDialog->setAddr(oldServerAddr);
	    modifyPopup();
	    if(modifDialog->getAddr()!=oldServerAddr){
	      serverManager->modifyServer(i,"erreur",modifDialog->getAddr());
	    }
	  
	    updateList();
	    return;
	  }
      }
  }

  void ServersOptionDialog::closeEvent(QCloseEvent*){
    modifDialog->done(true);

    vector<string> serversAddrs;
    serverManager->getAddrs(serversAddrs);

    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    settings.setValue("serverNumber_v2",(unsigned int)(serversAddrs.size()));
    int i=0;
    for(vector<string>::iterator it=serversAddrs.begin();it!=serversAddrs.end();++it) {
      settings.setValue("server_v2_"+QString::number(i),(*it).c_str());
      i++;
    }
    settings.endGroup();
  }

  void ServersOptionDialog::serverNameReceived(MultiServerManager*,string,string){
    updateList();
  }

}
