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
#include "tulip/AugmentedDisplayDialog.h"

#include <tulip/Graph.h>

using namespace std;

namespace tlp {

  AugmentedDisplayDialog::AugmentedDisplayDialog(QWidget* parent,Graph *graph,const string &viewType) : QDialog(parent) {
    setupUi(this);

    connect(removeButton,SIGNAL(clicked()),this,SLOT(removeClicked()));

    vector<string> displayedNames;
    DataSet viewDataSet;
    if(graph->attributeExist(viewType)){
      graph->getAttribute(viewType,viewDataSet);

      Iterator< std::pair<std::string, DataType*> > *infoDataSetIt=viewDataSet.getValues();
      while(infoDataSetIt->hasNext()) {
        pair<string, DataType*> infoData;
        infoData = infoDataSetIt->next();

        displayedNames.push_back(infoData.first);
      }
    }

    if(!displayedNames.empty()){
      removeButton->setEnabled(true);

      for(vector<string>::iterator it=displayedNames.begin();it!=displayedNames.end();++it){
        listWidget->addItem((*it).c_str());
      }
    }else{
      removeButton->setEnabled(false);
    }
  }

  void AugmentedDisplayDialog::getRemovedList(std::vector<std::string> &toRemove) {
    for(vector<string>::iterator it=removedList.begin();it!=removedList.end();++it){
      toRemove.push_back(*it);
    }
  }

  void AugmentedDisplayDialog::removeClicked() {
    QList<QListWidgetItem *> selectedItems=listWidget->selectedItems();

    for(QList<QListWidgetItem *>::iterator it=selectedItems.begin();it!=selectedItems.end();++it){
      removedList.push_back((*it)->text().toStdString());
      delete listWidget->takeItem(listWidget->row(*it));
    }
  }
}
