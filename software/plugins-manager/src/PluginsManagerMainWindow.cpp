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
#include "PluginsManagerMainWindow.h"

#include <QtCore/QSettings>
#include <QtGui/QVBoxLayout>
#include <QtGui/QStatusBar>
#include <QtGui/QMenuBar>
#include <QtGui/QActionGroup>
#include <QtGui/QMessageBox>
#include <iostream>

#include "ProxyConfigurationDialog.h"

using namespace std;

namespace tlp {

  void PluginsManagerMainWindow::createWidget(QWidget *parent){
    w = new QWidget(this);
    setCentralWidget(w);
#if QT_MINOR_REL >= 6
    menuBar()->setNativeMenuBar(false);
#endif

    vbox = new QVBoxLayout();
    vbox->addWidget(widg);
    w->setLayout(vbox);

    createActions(parent);
    createMenus();
    statusBar()->showMessage("");

    setWindowTitle("Plugins Manager");
    setMinimumSize(800, 600);

    serverViewAct->setChecked(true);
    serverView();
  }

  PluginsManagerMainWindow::PluginsManagerMainWindow(std::vector<LocalPluginInfo> &plugins,QWidget *parent):QMainWindow(parent){
    //Read setting

    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    int serverNumber=settings.value("serverNumber_v2",0).toInt();
    vector<string> serversAddr;
    for(int i=0;i<serverNumber;i++) {
      serversAddr.push_back(settings.value("server_v2_"+QString::number(i),"").toString().toStdString());
    }
    settings.endGroup();

    widg=new PluginsWidget(plugins,this);

    for(vector<string>::iterator it=serversAddr.begin();it!=serversAddr.end();++it)
      widg->addServer(*it);

    createWidget(parent);
  }

  PluginsManagerMainWindow::PluginsManagerMainWindow(MultiServerManager *msm,QWidget *parent):QMainWindow(parent){
    widg=new PluginsWidget(msm,this);
    createWidget(parent);
  }

  void PluginsManagerMainWindow::createActions(QWidget *parent){

    exitAct = new QAction(tr("E&xit"), vbox);
    exitAct->setShortcut(tr("Ctrl+X"));
    exitAct->setStatusTip(tr("Exit the window"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    serverViewAct = new QAction(tr("Sort by S&erver"), vbox);
    serverViewAct->setShortcut(tr("Ctrl+E"));
    serverViewAct->setStatusTip(tr("Display plugins by server"));
    serverViewAct->setCheckable(true);
    connect(serverViewAct, SIGNAL(triggered()), this, SLOT(serverView()));

    groupViewAct = new QAction(tr("Sort by Gro&up"), vbox);
    groupViewAct->setShortcut(tr("Ctrl+U"));
    groupViewAct->setStatusTip(tr("Display plugins by group"));
    groupViewAct->setCheckable(true);
    groupViewAct->setChecked(true);
    connect(groupViewAct, SIGNAL(triggered()), this, SLOT(groupView()));

    pluginViewAct = new QAction(tr("Sort by Plug&in"), vbox);
    pluginViewAct->setShortcut(tr("Ctrl+P"));
    pluginViewAct->setStatusTip(tr("Display plugins by name"));
    pluginViewAct->setCheckable(true);
    connect(pluginViewAct, SIGNAL(triggered()), this, SLOT(pluginView()));

    sortActionGroup = new QActionGroup(vbox);
    sortActionGroup->addAction(serverViewAct);
    sortActionGroup->addAction(groupViewAct);
    sortActionGroup->addAction(pluginViewAct);
    sortActionGroup->setExclusive(true);


    lastPluginsAct = new QAction(tr("Show only &latest plugins"), vbox);
    lastPluginsAct->setShortcut(tr("Ctrl+L"));
    lastPluginsAct->setStatusTip(tr("Enable/Disable the display of the more recent plugins"));
    lastPluginsAct->setCheckable(true);
    connect(lastPluginsAct, SIGNAL(triggered()), this, SLOT(showLatestPlugins()));

    compatiblesPluginsAct = new QAction(tr("Show only &compatibles plugins"), vbox);
    compatiblesPluginsAct->setShortcut(tr("Ctrl+C"));
    compatiblesPluginsAct->setStatusTip(tr("Enable/Disable the display of compatibles plugins"));
    compatiblesPluginsAct->setCheckable(true);
    connect(compatiblesPluginsAct, SIGNAL(triggered()), this, SLOT(showCompatiblesPlugins()));

    notinstalledPluginsAct = new QAction(tr("Show not installed plugins"), vbox);
    notinstalledPluginsAct->setShortcut(tr("Ctrl+C"));
    notinstalledPluginsAct->setStatusTip(tr("Enable/Disable the display of not installed plugins"));
    notinstalledPluginsAct->setCheckable(true);
    connect(notinstalledPluginsAct, SIGNAL(triggered()), this, SLOT(showNotinstalledPlugins()));

    applyAct = new QAction(tr("&Apply Change"), vbox);
    applyAct->setShortcut(tr("Ctrl+A"));
    applyAct->setStatusTip(tr("Apply the change"));
    connect(applyAct, SIGNAL(triggered()), this, SLOT(applyChange()));

    restoreAct = new QAction(tr("&Restore"), vbox);
    restoreAct->setShortcut(tr("Ctrl+R"));
    restoreAct->setStatusTip(tr("Restore"));
    connect(restoreAct, SIGNAL(triggered()), this, SLOT(restore()));

    serverAct = new QAction(tr("&Servers"), vbox);
    serverAct->setShortcut(tr("Ctrl+S"));
    serverAct->setStatusTip(tr("Open the Servers configuration windows"));
    connect(serverAct, SIGNAL(triggered()), this, SLOT(servers()));

    proxyAct = new QAction(tr("&Http proxy"), vbox);
    proxyAct->setStatusTip(tr("Open the http proxy configuration windows"));
    connect(proxyAct, SIGNAL(triggered()), this, SLOT(proxy()));
  }

  void PluginsManagerMainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(applyAct);
    fileMenu->addAction(restoreAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    configureMenu = menuBar()->addMenu(tr("&Configure"));
    configureMenu->addAction(serverAct);
    configureMenu->addAction(proxyAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(serverViewAct);
    viewMenu->addAction(groupViewAct);
    viewMenu->addAction(pluginViewAct);

    viewMenu->addSeparator();
    viewMenu->addAction(lastPluginsAct);
    viewMenu->addAction(compatiblesPluginsAct);
    viewMenu->addAction(notinstalledPluginsAct);
  }

  void PluginsManagerMainWindow::serverView(){
    currentView = VIEW_BY_SERVER;
    widg->modifyTreeView(currentView, lastPluginsAct->isChecked(), compatiblesPluginsAct->isChecked(),notinstalledPluginsAct->isChecked());
  }

  void PluginsManagerMainWindow::groupView(){
    currentView = VIEW_BY_TYPE;
    widg->modifyTreeView(currentView, lastPluginsAct->isChecked(), compatiblesPluginsAct->isChecked(),notinstalledPluginsAct->isChecked());
  }

  void PluginsManagerMainWindow::pluginView(){
    currentView = VIEW_BY_NAME;
    widg->modifyTreeView(currentView, lastPluginsAct->isChecked(), compatiblesPluginsAct->isChecked(),notinstalledPluginsAct->isChecked());
  }

  void PluginsManagerMainWindow::applyChange(){
    widg->applyChangeSlot();
  }

  void PluginsManagerMainWindow::restore(){
    widg->restoreSlot();
  }

  void PluginsManagerMainWindow::servers(){
    widg->serverPopup();
  }

  void PluginsManagerMainWindow::proxy(){
    ProxyConfigurationDialog proxyDialog(this);
    int result=proxyDialog.exec();
    if(result==QDialog::Accepted){
      proxyDialog.saveProxy();
      QMessageBox::warning(this,
              tr("Http proxy configuration"),
              tr("To finish http proxy configuration \nTulip must be restart."),
              QMessageBox::Ok | QMessageBox::Default);
    }
  }

  void PluginsManagerMainWindow::showCompatiblesPlugins(){
    widg->modifyTreeView(currentView,lastPluginsAct->isChecked(), compatiblesPluginsAct->isChecked(),notinstalledPluginsAct->isChecked());
  }

  void PluginsManagerMainWindow::showLatestPlugins(){
    widg->modifyTreeView(currentView,lastPluginsAct->isChecked(), compatiblesPluginsAct->isChecked(),notinstalledPluginsAct->isChecked());
  }

  void PluginsManagerMainWindow::showNotinstalledPlugins(){
    widg->modifyTreeView(currentView,lastPluginsAct->isChecked(), compatiblesPluginsAct->isChecked(),notinstalledPluginsAct->isChecked());
  }

  void PluginsManagerMainWindow::close() {
    emit closeSignal();
  }

}
