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

#ifndef _Tulip_SERVERSOPTIONDIALOG_H_
#define _Tulip_SERVERSOPTIONDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QListWidget>
#include "ModifyServerDialog.h"
#include "MultiServerManager.h"

namespace tlp {

  class ServersOptionDialog : public QDialog
  {
    Q_OBJECT
    
  protected :
    
    MultiServerManager* serverManager;
    ModifyServerDialog* modifDialog;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* propertiesButton;
    QPushButton* exitButton;
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
