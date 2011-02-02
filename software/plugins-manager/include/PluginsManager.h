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

#ifndef _Tulip_PLUGINSMANAGERMAINWINDOW_H
#define _Tulip_PLUGINSMANAGERMAINWINDOW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtGui/QMainWindow>

#include "PluginsWidget.h"

class QAbstractButton;
class QDialogButtonBox;
class QActionGroup;
class QVBoxLayout;

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE PluginsManagerMainWindow : public QDialog{
    Q_OBJECT

  protected:

    void createActions(QWidget *parent);
    void createMenus();

    QMenuBar* menuBar;
    QDialogButtonBox* buttons;
    PluginsWidget* pluginsWidget;
    QAction* lastPluginsAct;
    QAction* compatiblesPluginsAct;
    QAction* notinstalledPluginsAct;
    int currentView;
    
    PluginsViewWidget* pluginsList;
    PluginsInfoWidget* pluginsInfo;
    MultiServerManager* serverManager;
    ServersOptionDialog* serverDialog;
  public:
    void createWidget();
    PluginsManagerMainWindow(MultiServerManager *msm,QWidget *parent=0);

  signals:
    void closeSignal();

  private slots:
    void serverView();
    void groupView();
    void pluginView();
    void showPluginsModeChanged();
    void servers();
    void proxy();
    void clicked (QAbstractButton*);
  };
}

#endif
