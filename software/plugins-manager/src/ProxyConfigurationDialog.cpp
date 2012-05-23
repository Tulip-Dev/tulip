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
#include "ProxyConfigurationDialog.h"

#include <iostream>

#include <QtCore/QSettings>
#include <QtGui/QIntValidator>

#include <tulip/TlpNetworkAccess.h>

using namespace std;

namespace tlp {

ProxyConfigurationDialog::ProxyConfigurationDialog(QWidget* parent) : QDialog(parent) {
  setupUi(this);
  connect(enableProxy,SIGNAL(stateChanged(int)),this,SLOT(proxyStateChange(int)));
  connect(useUsernamePassword,SIGNAL(stateChanged(int)),this,SLOT(useUsernamePasswordStateChange(int)));

  NetworkProxyConfiguration npc = getNetworkProxyConfiguration();

  if(npc.proxyEnabled)
    enableProxy->setCheckState(Qt::Checked);

  if(npc.authenticationEnabled)
    useUsernamePassword->setCheckState(Qt::Checked);

  hostEdit->setText(npc.address);
  portEdit->setText(QString::number(npc.port));
  portEdit->setValidator(new QIntValidator(0,65535,portEdit));
  usernameEdit->setText(npc.login);
  passwordEdit->setText(npc.passwd);
}

void ProxyConfigurationDialog::saveProxy() {
  NetworkProxyConfiguration npc;
  npc.proxyEnabled = enableProxy->isChecked();
  npc.address = hostEdit->text();
  npc.port = portEdit->text().toUInt();
  npc.authenticationEnabled = useUsernamePassword->isChecked();
  npc.login = usernameEdit->text();
  npc.passwd = passwordEdit->text();

  setNetworkProxyConfiguration(npc);
}

void ProxyConfigurationDialog::proxyStateChange(int state) {
  bool enable = (state == Qt::Checked);

  hostEdit->setEnabled(enable);
  portEdit->setEnabled(enable);
  useUsernamePassword->setEnabled(enable);
}

void ProxyConfigurationDialog::useUsernamePasswordStateChange(int state) {
  bool enable = (state == Qt::Checked);

  usernameEdit->setEnabled(enable);
  passwordEdit->setEnabled(enable);
}

}
