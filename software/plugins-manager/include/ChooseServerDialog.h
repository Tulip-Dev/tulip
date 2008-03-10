//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_CHOOSESERVERDIALOG_H_
#define _Tulip_CHOOSESERVERDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QStringList>
#include <QtGui/QListWidget>

#include <tulip/tulipconf.h>

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE ChooseServerDialog : public QDialog{
    Q_OBJECT
    
    public:
    ChooseServerDialog(QStringList * serverNames);
    void getSelectedServer(std::string &server);

  private:
    
    QLabel* label;
    QPushButton* ok;
    QPushButton* cancel;
    QListWidget * serverList;
    std::string selectedServer;
    std::vector<std::string> names;
    void closeEvent(QCloseEvent *e);
  
  
  private slots:
    
    void cancelSelection();
    void selectServer();
  
  };

}
#endif
