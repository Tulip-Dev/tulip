//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 Last modification : 04/02/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_SERVERSOPTIONDIALOG_H_
#define _Tulip_SERVERSOPTIONDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QListWidget>
#include "ModifyServerDialog.h"
#include "MultiServerManager.h"

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE ServersOptionDialog : public QDialog
  {
    Q_OBJECT
    
  protected :
    
    MultiServerManager* serverManager;
    ModifyServerDialog* modifDialog;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* propertiesButton;
    QBoxLayout* boxLayout;
    QBoxLayout* subBoxLayout;
    QListWidget* serversList;
    QWidget* buttonsWidget;
    
    void showPropertiesDialog(int i = -1);
    virtual void modifyPopup();
    void updateList();
    virtual void closeEvent(QCloseEvent* event);
    
  public : 
    ServersOptionDialog(MultiServerManager* msm,QWidget *parent=0);
    virtual ~ServersOptionDialog();
				  
  public slots :
    void removeServer();
    virtual void addServer();
    void serverProperties();
    void serverNameReceived(MultiServerManager*,std::string,std::string);
  };

}
#endif
