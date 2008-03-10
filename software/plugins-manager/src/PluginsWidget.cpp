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

  void PluginsWidget::createWidget(QWidget *parent){
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    QHBoxLayout* boxLayout = new QHBoxLayout(mainLayout);

    connect(serverManager, SIGNAL(newPluginList()), this, SLOT(updatePluginsTree()));

    pluginsList = new PluginsViewWidget(serverManager,this);
    boxLayout->addWidget(pluginsList);
    
    pluginsInfo = new PluginsInfoWidget(this);
    boxLayout->addWidget(pluginsInfo);
    connect(pluginsList, SIGNAL(pluginInfoSignal(const PluginInfo*)), this, SLOT(clickOnPluginSlot(const PluginInfo *)));

    updatePluginsTree();

    serverDialog=new ServersOptionDialog(serverManager,parent);
  }
  
  PluginsWidget::PluginsWidget(vector<LocalPluginInfo> &plugins,QWidget *parent):QDialog(parent){
    serverManager = new MultiServerManager(plugins);
   
    createWidget(parent);
  }

  PluginsWidget::PluginsWidget(MultiServerManager *msm,QWidget *parent):QDialog(parent){
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

  void PluginsWidget::modifyTreeView(int viewNumber, bool lastVersion, bool compatiblesVersion){
    serverManager->modifyTreeView(viewNumber);
    pluginsList->setLastVersion(lastVersion);
    pluginsList->setCompatiblesVersion(compatiblesVersion);
    pluginsList->changeList();
  }
  
  void PluginsWidget::serverViewSlot(){
    modifyTreeView(VIEW_BY_SERVER, true, true);
  }

  void PluginsWidget::groupViewSlot(){
    modifyTreeView(VIEW_BY_TYPE, true, true);
  }

  void PluginsWidget::pluginViewSlot(){
    modifyTreeView(VIEW_BY_NAME, true, true);
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
