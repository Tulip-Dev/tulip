//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_MODIFYSERVERDIALOG_H_
#define _Tulip_MODIFYSERVERDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QBoxLayout>

#include <tulip/tulipconf.h>

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE ModifyServerDialog : public QDialog
  {
    Q_OBJECT
    
    protected :
    QLineEdit* textAddr;
    QLabel* labelAddr;
    QPushButton* valid;
    QPushButton* cancel;
    
    QBoxLayout* boxVLayout;
    QBoxLayout* boxHLayout1;
    QBoxLayout* boxHLayout2;
    
    
  public : 
    
    ModifyServerDialog(QWidget *parent=0);
    virtual ~ModifyServerDialog();
    
    std::string getAddr() const;
    void setAddr(const std::string& s);

  };

}
#endif //_MODIFYSERVERDIALOG_H_
