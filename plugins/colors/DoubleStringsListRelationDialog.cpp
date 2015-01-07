/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include "ui_DoubleStringsListRelationDialog.h"

#include <tulip/TlpQtTools.h>

#include <QScrollBar>

using namespace std;

namespace tlp {

DoubleStringsListRelationDialog::DoubleStringsListRelationDialog(const std::vector<std::string> &firstValues,const std::vector<Color> &secondValues,QWidget *parent) : QDialog(parent),_ui(new Ui::DoubleStringsListRelationDialogData) {
  _ui->setupUi(this);

  setWindowTitle("Associate colors to values");

  for(vector<string>::const_iterator it=firstValues.begin(); it!=firstValues.end(); ++it) {
    _ui->firstListWidget->addItem((*it).c_str());
  }

  for(vector<Color>::const_iterator it=secondValues.begin(); it!=secondValues.end(); ++it) {
    QListWidgetItem *item=new QListWidgetItem;
    item->setBackground(QBrush(QColor((*it)[0],(*it)[1],(*it)[2],(*it)[3])));
    _ui->secondListWidget->addItem(item);
  }

  connect(_ui->upButton,SIGNAL(clicked()),this,SLOT(upButtonClicked()));
  connect(_ui->downButton,SIGNAL(clicked()),this,SLOT(downButtonClicked()));
  connect(_ui->upButtonColor,SIGNAL(clicked()),this,SLOT(upButtonColorClicked()));
  connect(_ui->downButtonColor,SIGNAL(clicked()),this,SLOT(downButtonColorClicked()));
  connect(_ui->okButton,SIGNAL(clicked()),this,SLOT(accept()));
  connect(_ui->cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
  connect(((QAbstractSlider*)(_ui->firstListWidget->verticalScrollBar())),SIGNAL(valueChanged(int)),this,SLOT(scrollBarValueChanged(int)));
  connect(((QAbstractSlider*)(_ui->secondListWidget->verticalScrollBar())),SIGNAL(valueChanged(int)),this,SLOT(scrollBarValueChanged(int)));
}

DoubleStringsListRelationDialog::~DoubleStringsListRelationDialog() {
  delete _ui;
}

void DoubleStringsListRelationDialog::getResult(std::vector<std::pair<std::string,Color> > &result) {
  for(int i=0; (i<_ui->firstListWidget->count()) &&
      (i < _ui->secondListWidget->count()) ; ++i) {
    QColor color=_ui->secondListWidget->item(i)->background().color();
    result.push_back(pair<string,Color>(_ui->firstListWidget->item(i)->text().toStdString(), QColorToColor(color)));
  }
}

void DoubleStringsListRelationDialog::upButtonClicked() {
  int currentRow=_ui->firstListWidget->currentRow();

  if(currentRow==0)
    return;

  QListWidgetItem *item=_ui->firstListWidget->takeItem(currentRow);
  _ui->firstListWidget->insertItem(currentRow-1,item);
  _ui->firstListWidget->setCurrentItem(item);
}

void DoubleStringsListRelationDialog::downButtonClicked() {
  int currentRow=_ui->firstListWidget->currentRow();

  if(currentRow==_ui->firstListWidget->count()+1)
    return;

  QListWidgetItem *item=_ui->firstListWidget->takeItem(currentRow);
  _ui->firstListWidget->insertItem(currentRow+1,item);
  _ui->firstListWidget->setCurrentItem(item);
}

void DoubleStringsListRelationDialog::upButtonColorClicked() {
  int currentRow=_ui->secondListWidget->currentRow();

  if(currentRow==0)
    return;

  QListWidgetItem *item=_ui->secondListWidget->takeItem(currentRow);
  _ui->secondListWidget->insertItem(currentRow-1,item);
  _ui->secondListWidget->setCurrentItem(item);
}

void DoubleStringsListRelationDialog::downButtonColorClicked() {
  int currentRow=_ui->secondListWidget->currentRow();

  if(currentRow==_ui->secondListWidget->count()+1)
    return;

  QListWidgetItem *item=_ui->secondListWidget->takeItem(currentRow);
  _ui->secondListWidget->insertItem(currentRow+1,item);
  _ui->secondListWidget->setCurrentItem(item);
}

void DoubleStringsListRelationDialog::scrollBarValueChanged(int value) {
  if(_ui->firstListWidget->verticalScrollBar()->value()!=value)
    _ui->firstListWidget->verticalScrollBar()->setSliderPosition(value);

  if(_ui->secondListWidget->verticalScrollBar()->value()!=value)
    _ui->secondListWidget->verticalScrollBar()->setSliderPosition(value);
}

}
