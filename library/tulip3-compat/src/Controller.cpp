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
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <tulip/SizeProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/TulipProject.h>
#include "tulip3/QtProgress.h"
#include <QtCore/QDebug>

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
    tlp::Graph *g;
    DataSet controllerData;
    if (_externalFile.isEmpty()) {
      g = tlp::newGraph();
      initializeGraph(g);
    }
    else {
      QtProgress progress(_mainWindow, "Loading file");
      progress.show();
      DataSet importData;
      importData.set("file::filename",_externalFile.toStdString());
      g = tlp::importGraph("tlp",importData,&progress);

      if(importData.exist("controller")) {
        DataSet tmp;
        importData.get<DataSet>("controller", tmp);
        Iterator< std::pair<std::string, DataType*> > *it=tmp.getValues();
        std::pair<std::string, DataType*> p;
        p = it->next();
        controllerData=*((DataSet*)p.second->value);
      }
    }

    setData(g,controllerData);
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

  void Controller::saveAs(const QString &file) {
  }

  void Controller::initializeGraph(Graph *graph) {
    graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
    graph->getProperty<SizeProperty>("viewSize")->setAllEdgeValue(Size(0.125,0.125,0.5));
    graph->getProperty<ColorProperty>("viewColor")->setAllNodeValue(Color(255,0,0));
    graph->getProperty<ColorProperty>("viewColor")->setAllEdgeValue(Color(0,0,0));
    graph->getProperty<IntegerProperty>("viewShape")->setAllNodeValue(4);
    graph->getProperty<IntegerProperty>("viewShape")->setAllEdgeValue(0);
    graph->getProperty<StringProperty>("viewFont")->setAllNodeValue(tlp::TulipBitmapDir + "font.ttf");
    graph->getProperty<StringProperty>("viewFont")->setAllEdgeValue(tlp::TulipBitmapDir + "font.ttf");
    graph->getProperty<IntegerProperty>("viewFontSize")->setAllNodeValue(18);
    graph->getProperty<IntegerProperty>("viewFontSize")->setAllEdgeValue(18);
    graph->getProperty<IntegerProperty>("viewSrcAnchorShape")->setAllEdgeValue(EdgeExtremityGlyphManager::NoEdgeExtremetiesId);
    graph->getProperty<SizeProperty>("viewSrcAnchorSize")->setAllEdgeValue(Size(0.25,0.25,0.25));
    graph->getProperty<IntegerProperty>("viewTgtAnchorShape")->setAllEdgeValue(50);
    graph->getProperty<SizeProperty>("viewTgtAnchorSize")->setAllEdgeValue(Size(0.25,0.25,0.25));
  }

  void Controller::terminated() {
//    if (QMessageBox::question(0, trUtf8("Exiting"),trUtf8("Do you want to save your graph?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
//      save();
  }

}
