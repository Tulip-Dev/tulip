//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_INSTALLEXITDIALOG_H_
#define _Tulip_INSTALLEXITDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

#include <tulip/tulipconf.h>

namespace tlp {
  class InstallPluginDialog;
  
  class TLP_PLUGINSMANAGER_SCOPE InstallExitDialog : public QDialog{
    Q_OBJECT
    
  public:
    InstallExitDialog(InstallPluginDialog* parent);
    
  private:
    InstallPluginDialog* dialogParent;
    QLabel* label;
    QPushButton* yes;
    QPushButton* no;
    void closeEvent(QCloseEvent* e);
				   
  private slots:
    void stopped();
 
  };

}

#endif
