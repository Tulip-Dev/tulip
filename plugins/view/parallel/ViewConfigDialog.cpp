//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#include "ViewConfigDialog.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>


namespace tlp {

  template <typename T>
  bool listContains(list<T> &l, T &elt) {
    typename list<T>::iterator it;
    for (it = l.begin() ; it != l.end() ; ++it) {
      if (*it == elt)
	return true;
    }
    return false;
  }

  ViewConfigDialog::ViewConfigDialog(ParallelCoordinatesGraphProxy *data, QWidget *parent) : QDialog(parent) {


    items = new ItemsDialogList(&hashDataBase,this);
    choices = new ItemsDialogList(&hashDataBase,this);
 
    QHBoxLayout *valide = new QHBoxLayout;
    QVBoxLayout *valide2 = new QVBoxLayout;
    QVBoxLayout *buttons = new QVBoxLayout;
    QVBoxLayout *buttons2 = new QVBoxLayout;
    QHBoxLayout *rbLayout = new QHBoxLayout();
    QHBoxLayout *rbLayout2 = new QHBoxLayout();

    nodesButton = new QRadioButton(tr("nodes"));
    nodesButton->setChecked(true);
    edgesButton = new QRadioButton(tr("edges"));
    rbLayout->addWidget(nodesButton);
    rbLayout->addWidget(edgesButton);
  
    QGroupBox *gBox = new QGroupBox(tr("Pick Data Location : "));
    gBox->setAlignment(Qt::AlignLeft);
    gBox->setLayout(rbLayout);
  

    QVBoxLayout *determination2 =  new QVBoxLayout;
  
    determination2->addWidget(new QLabel("Available variates", this));
    determination2->addWidget(items);
    
    valide2->addWidget(new QLabel("Variates to display ", this));
    valide2->addWidget(choices);
    valide->addLayout(valide2);
    QHBoxLayout *globalSearch = new QHBoxLayout;
    globalSearch->addLayout(determination2);
    globalSearch->addLayout(buttons);
    globalSearch->addLayout(valide);
    globalSearch->addLayout(buttons2);
    QVBoxLayout *global = new QVBoxLayout();
    QHBoxLayout *globalButtons = new QHBoxLayout;
    QPushButton *ok = new QPushButton(QIcon(":/tick.png"),"");
    QPushButton *cancel = new QPushButton(QIcon(":/cross.png"),"");

    view3D = new QCheckBox("3D View");
    view3D->setChecked(true);

    splineView = new QCheckBox("Use curves (only in 2D View)");

    rbLayout2->addWidget(view3D);
    rbLayout2->addWidget(splineView);

    globalButtons->addWidget(ok);
    globalButtons->addWidget(cancel);
    global->addWidget(gBox);
    global->addStretch(1);
    global->addLayout(globalSearch);
    global->addLayout(rbLayout2);
    global->addLayout(globalButtons);
    setLayout(global);

    QPushButton* add = new QPushButton(QIcon(":/right.png"),"");
    QPushButton* remove = new QPushButton(QIcon(":/left.png"),"");
    QPushButton* up = new QPushButton(QIcon(":/up.png"),"");
    QPushButton *down = new QPushButton(QIcon(":/down.png"),"");
    up->setMaximumSize(30,40);
    down->setMaximumSize(30,40);
    buttons->addWidget(add);
    buttons->addWidget(remove);
    buttons2->addWidget(up);
    buttons2->addWidget(down);

    list<string> properties = data->getAllProperties();
    list<string> selectedProperties = data->getSelectedProperties();
    list<string>::iterator it;
    string propertyName;

    for (it = properties.begin() ; it != properties.end() ; ++it) {
      propertyName = *it;
      if (propertyName.find("view") == string::npos) {
	if (!listContains(selectedProperties, propertyName)) {
	  items->addItem(QString(propertyName.c_str()));
	} else {
	  choices->addItem(QString(propertyName.c_str()));
	  res.push_back(propertyName);
	}
      }
    }

    connect(ok,SIGNAL(clicked()),this,SLOT(pushOK())); 
    connect(cancel,SIGNAL(clicked()),this,SLOT(close())); 
 
    connect(add,SIGNAL(clicked()),this,SLOT(pressButtonAdd()));
    connect(remove,SIGNAL(clicked()),this,SLOT(pressButtonRem())); 

    connect(up,SIGNAL(clicked()),this,SLOT(pressButtonUp()));
    connect(down,SIGNAL(clicked()),this,SLOT(pressButtonDown())); 

    connect(view3D, SIGNAL(clicked()), this, SLOT(viewCheckboxClicked()));
    connect(splineView, SIGNAL(clicked()), this, SLOT(splineCheckboxClicked()));

  }

  void ViewConfigDialog::setData(DataSet dataSet,ParallelCoordinatesGraphProxy *data) {
    bool b;
    dataSet.get("nodesButton",b);
    nodesButton->setChecked(b);
    edgesButton->setChecked(!b);
    dataSet.get("view3D",b);
    view3D->setChecked(b);
    splineView->setChecked(!b);
  }
  
  DataSet ViewConfigDialog::getData() {
    DataSet data;
    data.set<bool>("nodesButton",nodesButton);
    data.set<bool>("view3D",view3D);
    return data;
  }

  std::list<std::string>& ViewConfigDialog::getSelectedProperties() {
    return res;
  }

  void ViewConfigDialog::pressButtonAdd(){
 
    if(items->currentItem()!=NULL){
      choices->addItemList(items->currentItem()->text());
      items->deleteItemList(items->currentItem());
    }
  }
 
  void ViewConfigDialog::pressButtonRem(){

    if(choices->currentItem()!=NULL){
      items->addItemList(choices->currentItem()->text());
      choices->deleteItemList(choices->currentItem());
    }
  }

  void ViewConfigDialog::pushOK(){
    res.clear();
    for(int i = 0; i < choices->count(); ++i) {
      res.push_back(choices->item(i)->text().toStdString());
    }
    close();
  }

  viewType ViewConfigDialog::getViewType() {
    if (view3D->isChecked()) {
      return VIEW_3D;
    } else if (splineView->isChecked()) {
      return VIEW_2D_SPLINE;
    } else {
      return VIEW_2D;
    }
  }

  void ViewConfigDialog::setViewType(viewType vType) {
    if (vType == VIEW_3D) {
      view3D->setChecked(true);
    } else if (vType == VIEW_2D_SPLINE) {
      view3D->setChecked(false);
      splineView->setChecked(true);
    } else {
      view3D->setChecked(false);
    }
  }

  void ViewConfigDialog::pressButtonUp() {
    if (choices->count() > 0) {
      int row = choices->currentRow();
      if ( row > 0) {
	QString s = choices->currentItem()->text();
	QString s2 = choices->item(row - 1)->text();
	choices->deleteItemList(choices->item(row - 1));
	choices->deleteItemList(choices->item(row - 1));
	choices->insertItem(row - 1, s2);
	choices->insertItem(row - 1, s);
	choices->setCurrentRow(row - 1);
      }
    }
  }

  void ViewConfigDialog::pressButtonDown() {
  
    if (choices->count() > 0) {
      int row = choices->currentRow();
      if ( row < (choices->count() - 1)) {
	QString s = choices->currentItem()->text();
	QString s2 = choices->item(row + 1)->text();
	choices->deleteItemList(choices->item(row));
	choices->deleteItemList(choices->item(row));
	choices->insertItem(row, s);
	choices->insertItem(row, s2);
	choices->setCurrentRow(row + 1);
      }
    } 
  }

  DataLocation ViewConfigDialog::getDataLocation() {
    if (nodesButton->isChecked()) {
      return NODES;
    } else {
      return EDGES;
    }
  }

  void ViewConfigDialog::setDataLocation(DataLocation location) {
    if (location == NODES) {
      edgesButton->setChecked(false);
      nodesButton->setChecked(true);
    } else {
      edgesButton->setChecked(true);
      nodesButton->setChecked(false);
    }
  }

  void ViewConfigDialog::viewCheckboxClicked() {
    if (view3D->isChecked()) {
      splineView->setChecked(false);
    }
  }

  void ViewConfigDialog::splineCheckboxClicked() {
    if (splineView->isChecked()) {
      view3D->setChecked(false);
    }
  }

}
