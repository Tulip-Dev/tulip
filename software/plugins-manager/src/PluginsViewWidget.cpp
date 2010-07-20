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
#include "PluginsViewWidget.h"

#include <iostream>

#include <QtGui/QHeaderView>
#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

#include "AuthorizationInstallDependencies.h"
#include "ChooseServerDialog.h"
#include "PluginsInfoWidget.h"

using namespace std;

namespace tlp {

  void PluginsViewWidget::init(QWidget * parent){
    setParent(parent);
    setColumnCount(2);
    QStringList ls;
    ls.push_back(QString("Name                                                      "));
    ls.push_back(QString("Installed Version"));
    setHeaderLabels(ls);
    header()->setResizeMode(QHeaderView::ResizeToContents);
    pluginsDirIsWritable = UpdatePlugin::isInstallDirWritable();
    if (!pluginsDirIsWritable)
      QMessageBox::warning(parent, "Unable to manage plugins", QString("The plugins installation directory : \n")+QString(PluginInfo::pluginsDirName.c_str())+QString("\n is not writable,\nyou are not allowed to install/remove plugins."));
    //header()->setStretchLastSection(false);
  }

  PluginsViewWidget::PluginsViewWidget(MultiServerManager* msm, QWidget * parent):listIsChanging(false){
    init(parent);
    _msm = msm;
    lastVersion = false;
    compatibleVersion = false;
    notInstalledVersion = false;

    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(getPluginInfoSlot()));
    connect(this,SIGNAL(itemChanged(QTreeWidgetItem*, int)),this,SLOT(changed(QTreeWidgetItem*)));
    connect(_msm,SIGNAL(nameReceived(MultiServerManager*,std::string,std::string)),this,SLOT(serverNameReceived(MultiServerManager*,std::string,std::string)));

  }

  PluginsViewWidget::~PluginsViewWidget(){
  }

  void PluginsViewWidget::changeList(){
    listIsChanging=true;
    int serverPosition = (_msm->getListPosition())[SERVER_POSITION_IN_V];
    int versionPosition = (_msm->getListPosition())[VERSION_POSITION_IN_V];

    CompletePluginsList list;
    _msm->getPluginsList(list);

    QTreeWidgetItem *root;

    //Hide all
    if(topLevelItemCount()!=0){
      root = topLevelItem(0);
      hideChild(root);
    }

    //clear();
    if(topLevelItemCount()==0){
      root = new QTreeWidgetItem(this);
      root->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      root->setText(0, "Plugins List");
      insertTopLevelItem(0, root);
    }

    for(CompletePluginsList::iterator it=list.begin();it!=list.end();++it){
      QTreeWidgetItem *parent=root;
      vector<string> *subList=&((*it).second);
      const PluginInfo *pluginInfo=(*it).first;

      for(int i=0;i<(int)(subList->size());i++){
	string text;
	if(i!=serverPosition){
	  text = (*subList)[i];
	}else{
	  text = _msm->getName((*subList)[i]);
	}

	QTreeWidgetItem *twi=findChildrenWithText(parent,text);
	if(!twi) {
	  if(i!=versionPosition){
	    twi = new QTreeWidgetItem(0);
	    twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	  }else{
	    twi = new QTreeWidgetItem(1);
	    setItemCheckability(pluginInfo,true,twi);
	  }
	  twi->setText(0, text.c_str());

	  parent->addChild(twi);

	}else{
	  if(i==versionPosition){
	    if(twi->isHidden()){
          setItemCheckability(pluginInfo,true,twi);
        }else{
	      setItemCheckability(pluginInfo,false,twi);
	    }
	  }
	  if(twi->isHidden()) {
	    twi->setHidden(false);
	  }
	}

	/*if(distPluginInfo->linuxVersion)
	twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);
      else
      twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);*/

	if(i==versionPosition){
	  setPluginDisplayInTree(pluginInfo,twi);
	}

	parent=twi;
      }
    }

    applyFilter(root);

    //Remove Hidden
    removeHiddenChild(root);

    root->setHidden(false);

    root->setExpanded(true);
    listIsChanging=false;
  }

  void PluginsViewWidget::hideChild(QTreeWidgetItem *parent) {
    QTreeWidgetItem *child;
    int childNumber=parent->childCount();
    for(int i=0;i<childNumber;i++) {
      child=parent->child(i);
      child->setHidden(true);
      hideChild(child);
    }
  }

  void PluginsViewWidget::removeHiddenChild(QTreeWidgetItem *parent) {
    QTreeWidgetItem *child;
    int childNumber=parent->childCount();
    for(int i=0;i<childNumber;i++) {
      child=parent->child(i);
      removeHiddenChild(child);
    }
    bool toHidden=true;
    if(childNumber==0){
      toHidden=false;
    }else{
      for(int i=0;i<childNumber;i++) {
	child=parent->child(i);
	if(!child->isHidden()) {
	  toHidden=false;
	}
      }
    }
    if(toHidden) {
      parent->setHidden(true);
    }
  }

  QTreeWidgetItem *PluginsViewWidget::findChildrenWithText(QTreeWidgetItem *parent,const string &text) {
    QTreeWidgetItem *child;
    int childNumber=parent->childCount();
    for(int i=0;i<childNumber;i++) {
      child=parent->child(i);
      if(child->text(0).toStdString()==text)
	return child;
    }
    return NULL;
  }

  void PluginsViewWidget::setItemCheckability(const PluginInfo *pluginInfo,bool created, QTreeWidgetItem *twi) {
    if (!pluginsDirIsWritable) {
      twi->setFlags(twi->flags() & (!Qt::ItemIsUserCheckable));
      return;
    }
    if(!pluginInfo->local && ((twi->flags() & Qt::ItemIsUserCheckable) == Qt::ItemIsUserCheckable)) {
      bool havePlugin=false;
#if defined(__APPLE__)
#if defined(MACPPC)
      havePlugin=((DistPluginInfo*)pluginInfo)->macVersion_ppc;
#else
      havePlugin=((DistPluginInfo*)pluginInfo)->macVersion_intel;
#endif
#elif defined(_WIN32)
      havePlugin=((DistPluginInfo*)pluginInfo)->windowsVersion;
#elif defined(I64)
      havePlugin=((DistPluginInfo*)pluginInfo)->linuxVersion_i64;
#else
      havePlugin=((DistPluginInfo*)pluginInfo)->linuxVersion;
#endif

      if(created) {
        if(havePlugin) {
          twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);
        }else{
          twi->setFlags(twi->flags() & (!Qt::ItemIsUserCheckable));
        }
      }else{
        if(havePlugin) {
          twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);
        }
      }
    }else{
      if(pluginInfo->local){
        //if(((LocalPluginInfo*)pluginInfo)->isInstalledInHome()){
        //twi->setFlags(twi->flags() & (!Qt::ItemIsUserCheckable));
        twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);
        /*}else{
          twi->setFlags(twi->flags() & (!Qt::ItemIsUserCheckable));
          twi->setFlags(twi->flags() | Qt::ItemIsSelectable);
        }*/
      }else{
        twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);
      }
    }
  }

  void PluginsViewWidget::applyFilter(QTreeWidgetItem * tree){
    // DEBUT DE FILTRAGE

    QTreeWidgetItemIterator it(tree);
    while (*it) {
      if ((*it)->type() == 1){ //We are on a version Number

        //Filter only compatibles versions
        if(compatibleVersion){
          if(!isCompatible((*it)->text(0).toStdString())){
            (*it)->setHidden(true);  //if the version is not compatible hide it

            bool removeParent = true;
            for (int i=0 ; i < (*it)->parent()->childCount() ; ++i){
              if (!((*it)->parent()->child(i)->isHidden())){
                removeParent = false;
              }
            }
            if(removeParent){
              (*it)->parent()->setHidden(true);
            }
          }
        }else{
          if(!isCompatible((*it)->text(0).toStdString())){
            (*it)->setFlags((*it)->flags()&(!Qt::ItemIsUserCheckable));
          }
        }

        //Filter only compatibles versions
        /*if(compatibleVersion){
	  if(!isCompatible((*it)->text(0).toStdString())){
	    (*it)->setHidden(true);  //if the version is not compatible hide it
	  }
	  }*/

        // Filter only last version
        if(lastVersion){
          string version = "0.0.0 0.0";
          for (int i=0 ; i < (*it)->parent()->childCount() ; ++i){
            if (isMoreRecent((*it)->parent()->child(i)->text(0).toStdString(), version)){
              version = (*it)->parent()->child(i)->text(0).toStdString();
            }
          }
          for (int i=0 ; i < (*it)->parent()->childCount() ; ++i){
            if((*it)->parent()->child(i)->text(0).toStdString() != version){
              (*it)->parent()->child(i)->setHidden(true);
            }
          }
        }

        // Filter not installed
        if(notInstalledVersion) {
          if((*it)->checkState(0)!=Qt::Unchecked) {
            (*it)->setHidden(true);
          }
        }

        bool removeParent = true;
        for (int i=0 ; i < (*it)->parent()->childCount() ; ++i){
          if (!((*it)->parent()->child(i)->isHidden())){
            removeParent = false;
          }
        }
        if(removeParent){
          (*it)->parent()->setHidden(true);
        }

      }
      ++it;
    }
  };


  bool PluginsViewWidget::isCompatible(const string & version){
    QStringList list = QString(version.c_str()).split(' ');
    QString tulipCompatibilityNumber(TULIP_RELEASE);
    tulipCompatibilityNumber = tulipCompatibilityNumber.left(tulipCompatibilityNumber.lastIndexOf("."));
    return (list[0].startsWith(tulipCompatibilityNumber));
  };

  bool PluginsViewWidget::isMoreRecent(const string & version1, const string & version2){
    QStringList list1 = QString(version1.c_str()).split(' ');
    QStringList list2 = QString(version2.c_str()).split(' ');
    QStringList subList1 = list1[0].split(".");
    QStringList subList2 = list2[0].split(".");

    for(int i=0;i<subList1.size();i++){
      if(subList1[i].compare(subList2[i]) != 0){
	if(subList1[i].compare(subList2[i]) > 0){
	  return true;
	}else{
	  return false;
	}
      }
    }

    subList1 = list1[1].split(".");
    subList2 = list2[1].split(".");

    for(int i=0;i<subList1.size();i++){
      if(subList1[i].compare(subList2[i]) != 0){
	if(subList1[i].compare(subList2[i]) > 0){
	  return true;
	}else{
	  return false;
	}
      }
    }

    return false;
  }


  bool PluginsViewWidget::setPluginDisplayInTree(const PluginInfo *pluginInfo,QTreeWidgetItem *twi){

    twi->setText(1, QString(""));
    if(pluginInfo->local) {
      if(!((LocalPluginInfo*)pluginInfo)->isInstalledInHome()){
        twi->setFlags(twi->flags() & (!Qt::ItemIsUserCheckable));
        twi->setFlags(twi->flags() | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
      }else{
        twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);
        twi->setCheckState(0,Qt::Checked);
      }
      //twi->setCheckState(0,Qt::Checked);
      twi->setText(1, QString("=Installed="));
      twi->setTextColor(1, QColor(0,255,0));
      return true;
    }else{
      DistPluginInfo *distPluginInfo=(DistPluginInfo*)pluginInfo;
      QString localPluginVersion;
      if(distPluginInfo->localVersion!="")
        localPluginVersion=QString(distPluginInfo->localVersion.c_str()).split(" ")[1];
      QString distPluginVersion;
      if(distPluginInfo->version!="")
        distPluginVersion=QString(distPluginInfo->version.c_str()).split(" ")[1];
      if(localPluginVersion==distPluginVersion) {
        twi->setCheckState(0,Qt::Checked);
        twi->setText(1, QString("=Installed="));
        twi->setTextColor(1, QColor(0,255,0));
        return true;
      }else{
        if(distPluginInfo->localVersion!=""){
          twi->setText(1, QString(distPluginInfo->localVersion.c_str()));
          twi->setTextColor(1, QColor(0,0,255));
        }
        twi->setCheckState(0,Qt::Unchecked);
        return false;
      }
    }
  }

  void  PluginsViewWidget::applyChange(){

    if(pluginsToInstall.size()!=0 || pluginsToRemove.size()!=0){
      connect(&updatePlugin,SIGNAL(pluginInstalled()),this,SLOT(pluginInstalledSlot()));
      connect(&updatePlugin,SIGNAL(pluginUninstalled()),this,SLOT(pluginUninstalledSlot()));
      if(updatePlugin.pluginsCheckAndUpdate(_msm,pluginsToInstall,pluginsToRemove,this)!=0){
        pluginsToInstall.clear();
        pluginsToRemove.clear();
      }
    }else{
      QMessageBox::warning(this, tr("Apply change"),
                         tr("No plugin to install/remove"),
                         QMessageBox::Ok);
    }
  }

  void PluginsViewWidget::pluginInstalledSlot(){
    changeList();
    emit pluginInstalled();
  }

  void PluginsViewWidget::pluginUninstalledSlot() {
    changeList();
  }

  void PluginsViewWidget::restore(){
    pluginsToInstall.clear();
    pluginsToRemove.clear();
    changeList();
  }

  bool PluginsViewWidget::isAVersionItem(QTreeWidgetItem *item){

    int versionPosition = (_msm->getListPosition())[VERSION_POSITION_IN_V];
    if(versionPosition==3){
      if(item->childCount()==0){
	return true;
      }else return false;
    }
    else if(versionPosition==2){
      if( item->childCount()>0 && item->child(0)->childCount()==0 ){
	return true;
      }
      else return false;
    }
    return false;
  }

  void PluginsViewWidget::setLastVersion(bool version){
    lastVersion = version;
  }

  void PluginsViewWidget::setCompatiblesVersion(bool version){
    compatibleVersion = version;
  }

  void PluginsViewWidget::setNotinstalledVersion(bool version){
    notInstalledVersion = version;
  }

  void PluginsViewWidget::getPluginInfoSlot(){

    QList<QTreeWidgetItem*> tmpList=selectedItems();

    int namePosition = (_msm->getListPosition())[NAME_POSITION_IN_V];
    int typePosition = (_msm->getListPosition())[TYPE_POSITION_IN_V];
    int versionPosition = (_msm->getListPosition())[VERSION_POSITION_IN_V];

    if(!tmpList.empty()){
      QTreeWidgetItem* ti = tmpList.first();
      if(isAVersionItem(ti)){
        const PluginInfo *actualPlugin;
        actualPlugin=_msm->getPluginInformation(getNthParent(ti,(versionPosition-namePosition))->text(0).toStdString(),getNthParent(ti,(versionPosition-typePosition))->text(0).toStdString(),ti->text(0).toStdString());
        if(!PluginsInfoWidget::haveInfo(actualPlugin)) {
          vector<const PluginInfo *> resultList;
          _msm->getPluginsInformation(actualPlugin->name,actualPlugin->type,resultList);
          for(vector<const PluginInfo *>::const_iterator it=resultList.begin();it!=resultList.end();++it) {
            emit pluginInfoSignal(*it);
          }
        }else{
          emit pluginInfoSignal(actualPlugin);
        }
      }
    }
  }



  void PluginsViewWidget::updateCheckBox(QTreeWidgetItem *it,string name,string type,string version,Qt::CheckState state, int namePosition, int typePosition){

    if( it->parent()!=NULL ){
      if( state==Qt::Checked ){
        if(  (namePosition>typePosition && it->text(0).toStdString().compare(name)== 0 && it->parent()->text(0).toStdString().compare(type)== 0)
          || (namePosition<typePosition && it->text(0).toStdString().compare(type)== 0 && it->parent()->text(0).toStdString().compare(name)== 0) ){
          for(int i=0;i<it->childCount();i++){
            if( (it->child(i)->text(0).toStdString().compare(version) != 0) && (it->child(i)->checkState(0)==Qt::Checked) ){
              it->child(i)->setCheckState(0,Qt::Unchecked);
            }
            if( (it->child(i)->text(0).toStdString().compare(version) == 0) && (it->child(i)->checkState(0)==Qt::Unchecked) ){
              it->child(i)->setCheckState(0,Qt::Checked);
            }
          }
        }
      }
      else{
        if( (namePosition>typePosition && it->text(0).toStdString().compare(name)== 0 && it->parent()->text(0).toStdString().compare(type)== 0)
          || (namePosition<typePosition && it->text(0).toStdString().compare(type)== 0 && it->parent()->text(0).toStdString().compare(name)== 0) ){
          for(int i=0;i<it->childCount();i++){
            if( (it->child(i)->text(0).toStdString().compare(version) == 0) && (it->child(i)->checkState(0)==Qt::Checked) ){
              it->child(i)->setCheckState(0,Qt::Unchecked);
            }
          }
        }
      }
    }
    for(int i=0;i<it->childCount();i++){
      updateCheckBox(it->child(i),name,type,version,state,namePosition,typePosition);
    }
  }


  void PluginsViewWidget::updateToInstallList(QTreeWidgetItem *it){

    int namePosition = (_msm->getListPosition())[NAME_POSITION_IN_V];
    int typePosition = (_msm->getListPosition())[TYPE_POSITION_IN_V];
    int versionPosition = (_msm->getListPosition())[VERSION_POSITION_IN_V];
    int serverPosition = (_msm->getListPosition())[SERVER_POSITION_IN_V];

    string pluginName = getNthParent(it->parent(),(versionPosition-namePosition)-1)->text(0).toStdString();
    string pluginType = getNthParent(it->parent(),(versionPosition-typePosition)-1)->text(0).toStdString();

    string pluginVersion = it->text(0).toStdString();
    string pluginServer;

    bool operationCancelled = false;

    if(serverPosition>versionPosition){ //Server is not defined
      if(it->checkState(0)==Qt::Checked){
	if(it->childCount() == 1){
	  pluginServer = it->child(0)->text(0).toStdString();
	}else{
	  QStringList * serverList = new QStringList();
	  for (int i = 0; i < it->childCount(); i++)
	    serverList->append(it->child(i)->text(0));

	  //    serverList->append("serverTest1");
	  //serverList->append("serverTest1");
	  ChooseServerDialog * chooseServer= new ChooseServerDialog(serverList);
	    chooseServer->exec();
	    if (chooseServer->result() != QDialog::Rejected){
	      chooseServer->getSelectedServer(pluginServer);
	    }else{
	      operationCancelled = true;
	    }
	}

      }else{
	pluginServer = it->child(0)->text(0).toStdString();
      }

    }else{
      pluginServer = getNthParent(it->parent(),(versionPosition-serverPosition)-1)->text(0).toStdString();
    }

    if(operationCancelled){
      it->setCheckState(0,Qt::Unchecked);
    }else{

      if(it->checkState(0)==Qt::Checked){
	PluginMatchNameTypeAndVersionPred pred(pluginName,pluginType,pluginVersion);
	set<LocalPluginInfo,PluginCmp>::iterator iter2=find_if(pluginsToRemove.begin(),pluginsToRemove.end(),pred);
	if(iter2 != pluginsToRemove.end()){
	  pluginsToRemove.erase(iter2);
	}else{
	  //adding the element in the list of plugins to Install
	  set<DistPluginInfo,PluginCmp>::iterator iter=find_if(pluginsToInstall.begin(),pluginsToInstall.end(),pred);
	  if(iter == pluginsToInstall.end()){
	    const PluginInfo *actualPlugin=_msm->getPluginInformation(pluginName,pluginType,pluginVersion,pluginServer);
	    pluginsToInstall.insert(*(DistPluginInfo*)actualPlugin);
	  }
	}
      }else{
	PluginMatchNameTypeAndVersionPred pred(pluginName,pluginType,pluginVersion);
	set<DistPluginInfo,PluginCmp>::iterator iter2=find_if(pluginsToInstall.begin(),pluginsToInstall.end(),pred);
	if(iter2 != pluginsToInstall.end()){
	  pluginsToInstall.erase(iter2);
	}else{
	  //Adding the element in the list of plugin to remove
	  set<LocalPluginInfo,PluginCmp>::iterator iter=find_if(pluginsToRemove.begin(),pluginsToRemove.end(),pred);
	  if(iter == pluginsToRemove.end()){
	    const PluginInfo *actualPlugin=_msm->getPluginInformation(pluginName,pluginType,pluginVersion,pluginServer);
	    const PluginInfo *localPlugin=_msm->getLocalPlugin(actualPlugin);
	    if (localPlugin)
	      pluginsToRemove.insert(*(LocalPluginInfo*)localPlugin);
	  }
	}
      }
    }

  }

  void PluginsViewWidget::changed(QTreeWidgetItem *it){
    if(listIsChanging)
      return;

    if(isAVersionItem(it)){
      int namePosition = (_msm->getListPosition())[NAME_POSITION_IN_V];
      int typePosition = (_msm->getListPosition())[TYPE_POSITION_IN_V];

      updateToInstallList(it);

      string namePlugin;
      string typePlugin;
      string version = it->text(0).toStdString();
      if(namePosition>typePosition){
	namePlugin = it->parent()->text(0).toStdString();
	typePlugin = it->parent()->parent()->text(0).toStdString();
      }
      else{
	typePlugin = it->parent()->text(0).toStdString();
	namePlugin = it->parent()->parent()->text(0).toStdString();
      }
      updateCheckBox(it->treeWidget()->itemAt(0,0),namePlugin,typePlugin,version,it->checkState(0),namePosition, typePosition );
    }
  }




  QTreeWidgetItem *PluginsViewWidget::getNthParent(QTreeWidgetItem *twi,int n){
    for(int i=0;i<n;i++){
      twi=twi->parent();
    }
    return twi;
  }


  void PluginsViewWidget::serverNameReceived(MultiServerManager*, string, string){
    changeList();
  }

}
