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
