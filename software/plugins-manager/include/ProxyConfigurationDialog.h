//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 26/01/2009
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_PROXYCONFIGURATIONDIALOG_H
#define Tulip_PROXYCONFIGURATIONDIALOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtNetwork/QNetworkProxy>

#include "ProxyConfigurationDialogData.h"

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE ProxyConfigurationDialog : public QDialog, public Ui::ProxyConfigurationDialogData {

    Q_OBJECT

  public:

    ProxyConfigurationDialog(QWidget* parent);
    void saveProxy();

  protected slots:
    void proxyStateChange(int);
    void useUsernamePasswordStateChange(int);


  };

}

#endif // Tulip_PROXYCONFIGURATIONDIALOG_H
