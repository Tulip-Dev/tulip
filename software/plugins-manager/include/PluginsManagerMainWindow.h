//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_PLUGINSMANAGERMAINWINDOW_H
#define _Tulip_PLUGINSMANAGERMAINWINDOW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtGui/QMainWindow>

#include "PluginsWidget.h"

class QActionGroup;
class QVBoxLayout;

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE PluginsManagerMainWindow : public QMainWindow{
    Q_OBJECT
      
  protected:

    void createActions();
    void createMenus();

    QWidget *w;
    QVBoxLayout *vbox; 
    PluginsWidget* widg;
    QMenu* fileMenu;
    QMenu* configureMenu;
    QMenu* viewMenu;
    QAction* exitAct;
    QAction* pathAct;
    QAction* serverViewAct;
    QAction* groupViewAct;
    QAction* pluginViewAct;
    QAction* applyAct;
    QAction* restoreAct;
    QAction* serverAct;
    QAction* lastPluginsAct;
    QAction* compatiblesPluginsAct;
    QActionGroup* sortActionGroup;
    int currentView;
    
  public:
    
    void createWidget();
    PluginsManagerMainWindow(std::vector<LocalPluginInfo> &plugins,QWidget *parent=0);
    PluginsManagerMainWindow(MultiServerManager *msm,QWidget *parent=0);

    
  private slots:
    void serverView();
    void groupView();
    void pluginView();
    void showCompatiblesPlugins();
    void showLatestPlugins();
    void applyChange();
    void restore();
    void servers();  

  };

}

#endif
