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
