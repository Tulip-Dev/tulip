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

#include "DoubleStringsListRelationDialog.h"

#include <QtGui/QScrollBar>

using namespace std;

namespace tlp {

DoubleStringsListRelationDialog::DoubleStringsListRelationDialog(const vector<string> &firstValues,const vector<Color> &secondValues,QWidget *parent) : QDialog(parent) {
	setupUi(this);
  for(vector<string>::const_iterator it=firstValues.begin();it!=firstValues.end();++it){
    firstListWidget->addItem((*it).c_str());
  }
  for(vector<Color>::const_iterator it=secondValues.begin();it!=secondValues.end();++it){
    QListWidgetItem *item=new QListWidgetItem;
    item->setBackground(QBrush(QColor((*it)[0],(*it)[1],(*it)[2],(*it)[3])));
    secondListWidget->addItem(item);
  }

  connect(upButton,SIGNAL(clicked()),this,SLOT(upButtonClicked()));
  connect(downButton,SIGNAL(clicked()),this,SLOT(downButtonClicked()));
  connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
  connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
  connect(((QAbstractSlider*)(firstListWidget->verticalScrollBar())),SIGNAL(valueChanged(int)),this,SLOT(scrollBarValueChanged(int)));
  connect(((QAbstractSlider*)(secondListWidget->verticalScrollBar())),SIGNAL(valueChanged(int)),this,SLOT(scrollBarValueChanged(int)));
}

void DoubleStringsListRelationDialog::getResult(vector<pair<string,Color> > &result){
  for(int i=0;i<firstListWidget->count();++i){
    QColor color=secondListWidget->item(i)->background().color();
    result.push_back(pair<string,Color>(firstListWidget->item(i)->text().toStdString(),Color(color.red(),color.green(),color.blue(),color.alpha())));
  }
}

void DoubleStringsListRelationDialog::upButtonClicked(){
  int currentRow=firstListWidget->currentRow();
  if(currentRow==0)
    return;
  QListWidgetItem *item=firstListWidget->takeItem(currentRow);
  firstListWidget->insertItem(currentRow-1,item);
  firstListWidget->setCurrentItem(item);
}

void DoubleStringsListRelationDialog::downButtonClicked(){
  int currentRow=firstListWidget->currentRow();
  if(currentRow==firstListWidget->count()+1)
    return;
  QListWidgetItem *item=firstListWidget->takeItem(currentRow);
  firstListWidget->insertItem(currentRow+1,item);
  firstListWidget->setCurrentItem(item);
}

void DoubleStringsListRelationDialog::scrollBarValueChanged(int value){
  if(firstListWidget->verticalScrollBar()->value()!=value)
    firstListWidget->verticalScrollBar()->setSliderPosition(value);
  if(secondListWidget->verticalScrollBar()->value()!=value)
    secondListWidget->verticalScrollBar()->setSliderPosition(value);
}

}
