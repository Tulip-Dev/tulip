/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
