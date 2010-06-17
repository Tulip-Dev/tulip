/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "ChooseControllerDialog.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>

#include <QtGui/QPushButton>
#include <QtCore/QSettings>

#include <tulip/PreferenceManager.h>

using namespace std;
using namespace tlp;

ChooseControllerDialog::ChooseControllerDialog( QWidget* parent)
    : QDialog( parent) {
  setupUi(this);
  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  bool autoLoadController=preferenceManager.getAutoLoadController();
  askAgain->setChecked(autoLoadController);
}

void ChooseControllerDialog::setControllersAvailable(const vector<string> &controllersName){
  for(vector<string>::const_iterator it=controllersName.begin();it!=controllersName.end();++it){
    QListWidgetItem *item=new QListWidgetItem((*it).c_str());
    if((*it)!=defaultControllerName)
      item->setCheckState(Qt::Unchecked);
    else
      item->setCheckState(Qt::Checked);
    listWidget->addItem(item);
    connect(listWidget,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(itemClicked(QListWidgetItem *)));
  }
}

void ChooseControllerDialog::setDefaultCheckedControllerName(const string &controllerName){
  defaultControllerName=controllerName;
}

void ChooseControllerDialog::setDefaultControllerName(const string &controllerName){
  savedControllerLabel->setText(controllerName.c_str());
}

string ChooseControllerDialog::getCheckedControllerName(){
  vector<QListWidgetItem *> checkedItems;
  for(unsigned int i=0;i<listWidget->count();++i){
    QListWidgetItem *item=listWidget->item(i);
    if(item->checkState()==Qt::Checked)
      return item->text().toStdString();
  }
}

void ChooseControllerDialog::itemClicked(QListWidgetItem *item){
  vector<QListWidgetItem *> checkedItems;
  for(unsigned int i=0;i<listWidget->count();++i){
    QListWidgetItem *item=listWidget->item(i);
    if(item->checkState()==Qt::Checked)
      checkedItems.push_back(item);
  }

  if(checkedItems.size()==0)
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  else
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

  if(checkedItems.size()>1){
    for(vector<QListWidgetItem *>::iterator it=checkedItems.begin();it!=checkedItems.end();++it){
      if((*it)!=item)
        (*it)->setCheckState(Qt::Unchecked);
    }
  }
}

void ChooseControllerDialog::accept() {
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");
  settings.setValue("autoLoadController",askAgain->isChecked());
  settings.endGroup();
  QDialog::accept();
}
