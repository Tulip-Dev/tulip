#include "ServersOptionDialog.h"

#include <QtGui/QMessageBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include "PluginsWidget.h"
#include <iostream>

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

    // Buttons widget
    buttonsWidget = new QWidget(this);
    buttonsWidget->setObjectName("buttonWidget");
    subBoxLayout = new QVBoxLayout(buttonsWidget);
    subBoxLayout->addWidget(addButton);
    subBoxLayout->addWidget(removeButton);
    subBoxLayout->addWidget(propertiesButton);
    
    // Big widget
    boxLayout = new QHBoxLayout(this);
    boxLayout->addWidget(serversList);  
    boxLayout->addWidget(buttonsWidget);
    
    //Signal connection
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeServer()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addServer()));
    connect(propertiesButton, SIGNAL(clicked()), this, SLOT(serverProperties()));
    connect(msm,SIGNAL(nameReceived(MultiServerManager*,std::string,std::string)),this,SLOT(serverNameReceived(MultiServerManager*,std::string,std::string)));

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

  void ServersOptionDialog::closeEvent(QCloseEvent* event){
    modifDialog->done(true);

    vector<string> serversAddrs;
    serverManager->getAddrs(serversAddrs);

    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    settings.setValue("serverNumber",(unsigned int)(serversAddrs.size()));
    int i=0;
    for(vector<string>::iterator it=serversAddrs.begin();it!=serversAddrs.end();++it) {
      settings.setValue("server"+QString::number(i),(*it).c_str());
      i++;
    }
    settings.endGroup();
  }

  void ServersOptionDialog::serverNameReceived(MultiServerManager* msm,string addr,string name){
    updateList();
  }

}
