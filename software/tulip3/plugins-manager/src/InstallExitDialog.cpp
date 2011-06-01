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
#include <QtGui/QBoxLayout>
#include <QtGui/QCloseEvent>

#include "InstallExitDialog.h"
#include "InstallPluginDialog.h"

namespace tlp {

  InstallExitDialog::InstallExitDialog(InstallPluginDialog* parent){
    dialogParent = parent;
    
    setModal(true);
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(20);
    vbox->setSpacing(20);
    QHBoxLayout *hbox = new QHBoxLayout();
    label = new QLabel("Are you sure to want stopping the installation?",this);
    QFont font;
    font.setBold(true);
    //setFont(font);
    label->setFont(font);
    QLabel *note = new QLabel("That will remove the current plugin installation.",this);
    yes = new QPushButton("Yes",this);
    no = new QPushButton("No",this);
    connect(yes,SIGNAL(clicked()),this,SLOT(stopped()));
    connect(no,SIGNAL(clicked()),this,SLOT(reject()));
    
    vbox->addWidget(label);
    vbox->addWidget(note);
    hbox->addWidget(yes);
    hbox->addWidget(no);
    vbox->addLayout(hbox);
    setLayout(vbox);
     
  }

  //Stop the current installation
  void InstallExitDialog::stopped(){
    reject();
    dialogParent->stopInstall();
    dialogParent->reject();
  }
  
  
  void InstallExitDialog::closeEvent(QCloseEvent *e){
    e->ignore();
  }

}
