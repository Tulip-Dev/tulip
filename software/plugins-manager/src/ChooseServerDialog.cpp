#include <iostream>
#include <QtGui/QBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QCloseEvent>

#include "ChooseServerDialog.h"

using namespace std;

namespace tlp {

  ChooseServerDialog::ChooseServerDialog(QStringList * serverNames){
    
    setWindowTitle("Choose Your Server");
    setModal(true);
    
    QVBoxLayout* box = new QVBoxLayout(this);
    box->setMargin(50);
    box->setSpacing(25);
    
    serverList = new QListWidget(this);
    serverList->setSelectionMode(QAbstractItemView::SingleSelection);
    serverList->clear();
    serverList->insertItems(0, (*serverNames));  
    
    label = new QLabel("Please choose the server from which you want to download the plugin", this);
    QPalette pal;
    pal.setColor(label->foregroundRole(), Qt::blue);
    label->setAutoFillBackground(true);
    label->setPalette(pal);
    QFont font;
    font.setBold(true);
    font.setPixelSize(12); 
    label->setFont(font);
    cancel = new QPushButton("Cancel",this);
    ok = new QPushButton("Ok",this);
    
    connect(cancel,SIGNAL(clicked()),this,SLOT(cancelSelection()));
    connect(ok,SIGNAL(clicked()),this,SLOT(selectServer()));
    
    QHBoxLayout *boxButtons = new QHBoxLayout(this);
    boxButtons->addWidget(ok);
    boxButtons->addWidget(cancel);
    box->addWidget(label);
    box->addWidget(serverList);  
    
    box->addLayout(boxButtons);  
    
    setLayout(box);
  }
  
  
  void ChooseServerDialog::closeEvent(QCloseEvent*){
    reject();
  }
  
  void ChooseServerDialog::getSelectedServer(string &selected){
    selected=selectedServer;
  }
  
  void ChooseServerDialog::selectServer(){
    if (serverList->selectedItems().size() != 0){
      selectedServer = serverList->selectedItems().first()->text().toStdString();
      done(1);
    }else{
      reject();
    }
  }
  
  void ChooseServerDialog::cancelSelection(){
    reject();
  }

}
