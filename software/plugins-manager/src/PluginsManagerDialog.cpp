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
#include "PluginsManagerDialog.h"
#include "UpdatePlugin.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>

#include "PluginsManagerMainWindow.h"

using namespace std;

namespace tlp {

  void PluginsManagerDialog::createWidget(PluginsManagerMainWindow *pluginsManager) {
    connect(pluginsManager, SIGNAL(closeSignal()), this, SLOT(close()));
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(pluginsManager);
    QDialog::setLayout(layout);
  }

  PluginsManagerDialog::PluginsManagerDialog(std::vector<LocalPluginInfo> &plugins,QWidget *parent):QDialog(parent){
    PluginsManagerMainWindow *pluginsManager=new PluginsManagerMainWindow(plugins,parent);
    createWidget(pluginsManager);
  }

  PluginsManagerDialog::PluginsManagerDialog(MultiServerManager *msm,QWidget *parent):QDialog(parent){
    PluginsManagerMainWindow *pluginsManager=new PluginsManagerMainWindow(msm,parent);
    createWidget(pluginsManager);
  }

  bool PluginsManagerDialog::pluginUpdatesPending() {
    return UpdatePlugin::pluginUpdatesPending();
  }
}
