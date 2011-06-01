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
#include "ChooseControllerDialog.h"

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
  connect(listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
  
  listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ChooseControllerDialog::setControllersAvailable(const vector<string> &controllersName){
  for(vector<string>::const_iterator it=controllersName.begin();it!=controllersName.end();++it){
    QListWidgetItem *item=new QListWidgetItem((*it).c_str());
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    listWidget->addItem(item);
    if((*it)==defaultControllerName) {
      QFont f = item->font();
      f.setWeight(QFont::Bold);
      item->setFont(f);
      listWidget->setItemSelected(item, true);
      listWidget->setCurrentItem(item);
    }
  }
  //Perform default selection if no item are selected.
  if(listWidget->selectedItems().isEmpty() && listWidget->count()!=0){
      listWidget->item(0)->setSelected(true);
  }
}

void ChooseControllerDialog::setDefaultCheckedControllerName(const string &controllerName){
  defaultControllerName=controllerName;
}

void ChooseControllerDialog::setDefaultControllerName(const string &controllerName){
  savedControllerLabel->setText(controllerName.c_str());
}

string ChooseControllerDialog::getCheckedControllerName(){
  assert(!listWidget->selectedItems().isEmpty());
  return listWidget->selectedItems().front()->text().toStdString();
}

void ChooseControllerDialog::accept() {
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");
  settings.setValue("autoLoadController",askAgain->isChecked());
  settings.endGroup();
  QDialog::accept();
}
