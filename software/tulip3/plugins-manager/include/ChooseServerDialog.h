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

#ifndef _Tulip_CHOOSESERVERDIALOG_H_
#define _Tulip_CHOOSESERVERDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QStringList>
#include <QtGui/QListWidget>

#include <tulip/tulipconf.h>

namespace tlp {

  class ChooseServerDialog : public QDialog{
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
