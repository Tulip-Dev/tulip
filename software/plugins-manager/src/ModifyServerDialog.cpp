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
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include "PluginsWidget.h"
#include "ModifyServerDialog.h"

#include <iostream>

using namespace std;

namespace tlp {

  ModifyServerDialog::ModifyServerDialog(QWidget *parent):QDialog(parent){
    
    labelAddr = new QLabel("Server url",this);
    textAddr = new QLineEdit(this); 
    valid = new QPushButton("Ok",this);
    cancel = new QPushButton("Cancel",this);
    
    setWindowTitle("Edit Server");
    
    // Big widget
    boxVLayout = new QVBoxLayout(this);
    boxHLayout1 = new QHBoxLayout();
    boxHLayout2 = new QHBoxLayout();
    
    boxHLayout1->addWidget(labelAddr);  
    boxHLayout1->addWidget(textAddr);
    
    boxHLayout2->addWidget(valid);  
    boxHLayout2->addWidget(cancel);
    
    
    boxVLayout->addLayout(boxHLayout1);
    boxVLayout->addLayout(boxHLayout2);
    
    connect(valid, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
  }
  
  ModifyServerDialog::~ModifyServerDialog(){
    
  }
  
  string ModifyServerDialog::getAddr() const {
    return textAddr->text().toStdString();
  }
  
  void ModifyServerDialog::setAddr(const string& s) {
    textAddr->setText(s.c_str());
  }

}

