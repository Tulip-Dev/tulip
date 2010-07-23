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
#include <iostream>
#include <cassert>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QHeaderView>

#include "InstallPluginDialog.h"

using namespace std;

namespace tlp {

  InstallPluginDialog::InstallPluginDialog(const std::vector<string> &namePluginsInstall,const std::vector<string> &namePluginsRemove,QWidget *parent)
    :QDialog(parent),processNumber(0){
    setupUi(this);

    exitDialog = new InstallExitDialog(this);
    cancelButton->setEnabled(true);
    okButton->setEnabled(false);
    connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelInstall()));

    titleLabel->setText("Requested operations");

    // install
    unsigned int index=0;
    if(namePluginsInstall.size()==0) {

    }else{
      installTableWidget->setColumnCount(2);
      for(vector<string>::const_iterator it=namePluginsInstall.begin();it!=namePluginsInstall.end();++it) {
        installTableWidget->insertRow(index);

        installTableWidget->setItem(index,0,new QTableWidgetItem((*it).c_str()));
        installTableWidget->setItem(index,1,new QTableWidgetItem("in process"));

        pluginsInstallIndex[*it]=index;
        index++;
        processNumber++;
      }
    }

    // remove
    index=0;
    if(namePluginsRemove.size()==0) {

    }else{
      removeTableWidget->setColumnCount(2);
      for(vector<string>::const_iterator it=namePluginsRemove.begin();it!=namePluginsRemove.end();++it) {
        removeTableWidget->insertRow(index);

        removeTableWidget->setItem(index,0,new QTableWidgetItem((*it).c_str()));
        removeTableWidget->setItem(index,1,new QTableWidgetItem("in process"));

        pluginsRemoveIndex[*it]=index;
        index++;
        processNumber++;
      }
    }
    totalProcess=processNumber;
    progressBar->setMaximum(totalProcess);

    installTableWidget->resizeColumnsToContents();
    installTableWidget->horizontalHeader()->setVisible(false);
    installTableWidget->verticalHeader()->setVisible(false);
    installTableWidget->setShowGrid(false);
    removeTableWidget->resizeColumnsToContents();
    removeTableWidget->horizontalHeader()->setVisible(false);
    removeTableWidget->verticalHeader()->setVisible(false);
    removeTableWidget->setShowGrid(false);
  }

  void InstallPluginDialog::installStart(const string&){
  }

  void InstallPluginDialog::installFinished(const string &name,bool loadCheckOK){
    if (!loadCheckOK) {
      installErrors += "The check of the '" + name + "' plugin loading failed,\n";
      installErrors += "the file has not be installed.";
    }
    //installPart(name,1.);
    processNumber--;
    progressBar->setValue(totalProcess-processNumber);
    if(processNumber==0)
      terminated();
  }

  void InstallPluginDialog::stopInstall(){

  }

  void InstallPluginDialog::addPlugin(bool install, const string &name){
    QTableWidgetItem *itemWidget=new QTableWidgetItem(name.c_str());

    unsigned int index;
    if(install){
      installTableWidget->setColumnCount(2);

      index=installTableWidget->rowCount();
      pluginsInstallIndex[name]=index+1;

      installTableWidget->insertRow(installTableWidget->rowCount());
      installTableWidget->setItem(installTableWidget->rowCount(),0,itemWidget);
      installTableWidget->setItem(installTableWidget->rowCount(),1,new QTableWidgetItem("in process"));
    }else{
      removeTableWidget->setColumnCount(2);

      index=removeTableWidget->rowCount();
      pluginsRemoveIndex[name]=index+1;

      removeTableWidget->insertRow(removeTableWidget->rowCount());
      removeTableWidget->setItem(removeTableWidget->rowCount(),0,itemWidget);
      removeTableWidget->setItem(removeTableWidget->rowCount(),1,new QTableWidgetItem("in process"));
    }
    processNumber++;
    totalProcess++;
    progressBar->setMaximum(totalProcess);

    installTableWidget->resizeColumnsToContents();
    removeTableWidget->resizeColumnsToContents();
  }

  void InstallPluginDialog::closeEvent(QCloseEvent*){
    /*if(terminate==false){
      e->ignore();
      cancelInstall();
    }
    else{
      reject();
    }*/
  }

  void InstallPluginDialog::terminated(){
    if(exitDialog->isVisible()){
      exitDialog->reject();
    }

    titleLabel->setText("Completed operations");
    /*QPalette pal;
    pal.setColor(labelTitle->foregroundRole(), Qt::blue);
    labelTitle->setAutoFillBackground(true);
    labelTitle->setPalette(pal);*/

    cancelButton->setEnabled(false);
    okButton->setEnabled(true);

    if (installErrors.size() > 0) {
      QMessageBox::critical(this,
			    "Errors when trying to load Tulip plugins",
			    installErrors.c_str());
    }
  }

  void InstallPluginDialog::installPart(const std::string &name, int currentPart,int partNumber) {

    map<string, unsigned int>::iterator it;
    it=pluginsInstallIndex.find(name);
    if(it!=pluginsInstallIndex.end()){
      unsigned int index=(*it).second;
      if(currentPart==partNumber && installTableWidget->item(index,1)->text()!="complete")
        installTableWidget->item(index,1)->setText("complete");
      return;
    }
    it=pluginsRemoveIndex.find(name);
    if(it!=pluginsRemoveIndex.end()){
      unsigned int index=(*it).second;
      if(currentPart==partNumber && removeTableWidget->item(index,1)->text()!="complete")
        removeTableWidget->item(index,1)->setText("complete");
      return;
    }
    assert(false);
  }



  void InstallPluginDialog::cancelInstall(){
    exitDialog->show();
  }

}
