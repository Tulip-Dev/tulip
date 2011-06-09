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
#include "tulip3/Controller.h"

#include <QtGui/QWorkspace>

#include "ui_Tulip.h"

using namespace std;

//====================================================
tlp::Controller* tlp::Controller::currentController=0;

namespace tlp {
  MainWindowFacade::MainWindowFacade()
  :mainWindow(NULL),menuBar(NULL),toolBar(NULL),interactorsToolBar(NULL),workspace(NULL),statusBar(NULL) {
  }
  MainWindowFacade::MainWindowFacade(QMainWindow *mainWindow,QToolBar *toolBar,QToolBar *interactorsToolBar,QWorkspace *workspace)
		:mainWindow(mainWindow),menuBar(mainWindow->menuBar()),toolBar(toolBar),interactorsToolBar(interactorsToolBar),workspace(workspace),statusBar(mainWindow->statusBar()){
  }



  Controller::Controller(tlp::PerspectiveContext &c): Perspective(c) {
  }

  void Controller::construct() {
    _buildUi();
    setData(tlp::newGraph());
  }

  void Controller::construct(tlp::TulipProject *project) {
    _buildUi();
  }

  void Controller::attachMainWindow(MainWindowFacade facade) {
    mainWindowFacade=facade;
  }

  void Controller::_buildUi() {
    Ui::TulipData *ui = new Ui::TulipData;
    ui->setupUi(_mainWindow);
    _mainWindow->show();

    QGridLayout *gridLayout = new QGridLayout(ui->tab);
    QWorkspace *workspace = new QWorkspace(ui->tab);
    workspace->setBackground(QBrush(QPixmap(QString::fromUtf8(":/background_logo.png"))));
    gridLayout->addWidget(workspace, 0, 0, 1, 1);

    connect(ui->fileOpenAction,SIGNAL(triggered()),this,SIGNAL(showOpenProjectWindow()));
    connect(ui->fileSaveAction,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->fileSaveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(ui->helpDocumentationAction,SIGNAL(triggered()),this,SIGNAL(showTulipAboutPage()));
    connect(ui->helpDocumentationAction,SIGNAL(triggered()),this,SIGNAL(showTulipAboutPage()));
    connect(ui->pluginsAction,SIGNAL(triggered()),this,SIGNAL(showTulipPluginsCenter()));
    connect(ui->helpAboutAction,SIGNAL(triggered()),this,SIGNAL(showTulipAboutPage()));
    attachMainWindow(MainWindowFacade(_mainWindow,ui->toolBar,ui->graphToolBar,workspace));
  }

  void Controller::save() {

  }

  void Controller::saveAs() {

  }

}
