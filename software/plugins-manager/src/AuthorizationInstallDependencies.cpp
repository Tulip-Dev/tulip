#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>

#include <iostream>

#include "AuthorizationInstallDependencies.h"

using namespace std;

namespace tlp {

  AuthorizationInstallDependencies::AuthorizationInstallDependencies(std::set<DistPluginInfo,PluginCmp> *depToInstall,std::set<DistPluginInfo,PluginCmp> *toInstall):depToInstall(depToInstall),toInstall(toInstall){
    install=true;

    QLabel* label;
    label=new QLabel("No installed dependencies :",this);

    box.addWidget(label);
    for(set<DistPluginInfo,PluginCmp>::iterator it=depToInstall->begin();it!=depToInstall->end();++it) {
      QLabel* depLabel = new QLabel((*it).name.c_str(),this);
      box.addWidget(depLabel);
    }
    
    
    QLabel* install;
    install = new QLabel("Install ?",this);
    subBox.addWidget(install);
  
    QPushButton* validButton = new QPushButton("Yes",this);
    QPushButton* cancelButton = new QPushButton("No",this);
    
    subBox.addWidget(validButton);
    subBox.addWidget(cancelButton);
    connect(validButton, SIGNAL(clicked()),this, SLOT(valid()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(annule()));
    box.addLayout(&subBox);
    setLayout(&box);
  }

  AuthorizationInstallDependencies::AuthorizationInstallDependencies(std::set<LocalPluginInfo,PluginCmp> *depToRemove,std::set<LocalPluginInfo,PluginCmp> *toRemove):depToRemove(depToRemove),toRemove(toRemove){
    install=false;

    QLabel* label;
    label=new QLabel("Dependancies to remove :",this);

    box.addWidget(label);
    for(set<LocalPluginInfo,PluginCmp>::iterator it=depToRemove->begin();it!=depToRemove->end();++it) {
      QLabel* depLabel = new QLabel((*it).name.c_str(),this);
      box.addWidget(depLabel);
    }
    
    
    QLabel* install;
    install = new QLabel("Remove ?",this);
    subBox.addWidget(install);
  
    QPushButton* validButton = new QPushButton("Yes",this);
    QPushButton* cancelButton = new QPushButton("No",this);
    
    subBox.addWidget(validButton);
    subBox.addWidget(cancelButton);
    connect(validButton, SIGNAL(clicked()),this, SLOT(valid()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(annule()));
    box.addLayout(&subBox);
    setLayout(&box);
  }
  
  void AuthorizationInstallDependencies::valid(){
    if(install) {
      for(set<DistPluginInfo,PluginCmp>::iterator it=depToInstall->begin();it!=depToInstall->end();++it)
	toInstall->insert(*it);
    }else{
      for(set<LocalPluginInfo,PluginCmp>::iterator it=depToRemove->begin();it!=depToRemove->end();++it)
	toRemove->insert(*it);
    }
    close();
  }
  
  void AuthorizationInstallDependencies::annule(){
    if(install)
      toInstall->clear();
    else
      toRemove->clear();
    close();
  }

}
