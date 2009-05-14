#include "UpdatePluginsDialog.h"

#include <iostream>

#include "QtCore/QSettings"

using namespace std;

namespace tlp {

  UpdatePluginsDialog::UpdatePluginsDialog(const vector<DistPluginInfo*> &pluginsOutOfDate,QWidget* parent) : QDialog(parent) {
    setupUi(this);
    this->pluginsOutOfDate=&pluginsOutOfDate;

    set<DistPluginInfo,PluginTypeCmp> pluginsOutOfDateSorted;
    for(vector<DistPluginInfo*>::const_iterator it=pluginsOutOfDate.begin();it!=pluginsOutOfDate.end();++it){
      pluginsOutOfDateSorted.insert(*(*it));
    }

    string typeName="";
    for(set<DistPluginInfo>::iterator it=pluginsOutOfDateSorted.begin();it!=pluginsOutOfDateSorted.end();++it){
      if(typeName!=(*it).displayType){
        typeName=(*it).displayType;
        QListWidgetItem *item=new QListWidgetItem(QString("-- ")+typeName.c_str()+" --");
        item->setFlags(0);
        listWidget->addItem(item);
      }
      QListWidgetItem *item=new QListWidgetItem((*it).name.c_str());
      item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
      item->setCheckState(Qt::Checked);
      listWidget->addItem(item);
    }

    askAgainCheckBox->setEnabled(false);
    connect(listWidget,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(itemChangedSlot(QListWidgetItem *)));
    connect(okButton,SIGNAL(clicked()),this,SLOT(okSlot()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
  }

  void UpdatePluginsDialog::itemChangedSlot(QListWidgetItem *item){
    if(item->checkState()==Qt::Checked){
      if(pluginsToNoUpdate.find(item->text().toStdString())!=pluginsToNoUpdate.end()){
        pluginsToNoUpdate.erase(item->text().toStdString());
      }
      if(pluginsToNoUpdate.size()==0){
        askAgainCheckBox->setEnabled(false);
        askAgainCheckBox->setCheckState(Qt::Checked);
      }
    }else{
      if(pluginsToNoUpdate.find(item->text().toStdString())==pluginsToNoUpdate.end()){
        pluginsToNoUpdate.insert(item->text().toStdString());
      }
      if(pluginsToNoUpdate.size()!=0)
        askAgainCheckBox->setEnabled(true);
    }
  }

  void UpdatePluginsDialog::getPluginsToUpdate(set<DistPluginInfo,PluginCmp> &pluginsToUpdate){
    for(vector<DistPluginInfo *>::const_iterator it=pluginsOutOfDate->begin();it!=pluginsOutOfDate->end();++it){
      if(pluginsToNoUpdate.find((*it)->name)==pluginsToNoUpdate.end())
        pluginsToUpdate.insert(*(*it));
    }
  }

  void UpdatePluginsDialog::okSlot() {
    if(pluginsToNoUpdate.size()!=0 && askAgainCheckBox->checkState()==Qt::Unchecked){
      QSettings settings("TulipSoftware","Tulip");
      settings.beginGroup("UpdatePlugins");
      for(set<string>::iterator it=pluginsToNoUpdate.begin();it!=pluginsToNoUpdate.end();++it) {
        settings.setValue((*it).c_str(),true);
      }
      settings.endGroup();
    }
    accept();
  }
}
