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
#include "GraphPerspective.h"

#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

#include <tulip/ImportModule.h>
#include <tulip/Graph.h>
#include <tulip/View.h>

#include "ui_GraphPerspectiveMainWindow.h"

#include "ImportWizard.h"
#include "PanelSelectionWizard.h"
#include "GraphHierarchiesEditor.h"
#include <tulip/GraphHierarchiesModel.h>

#ifndef NDEBUG
#include <modeltest.h>
#endif

#include <QtCore/QDebug>

using namespace tlp;

GraphPerspective::GraphPerspective(PerspectiveContext &c): Perspective(c), _ui(0), _graphs(new GraphHierarchiesModel(this)) {
#ifndef NDEBUG
  new ModelTest(_graphs,this);
#endif /* NDEBUG */
  Q_INIT_RESOURCE(GraphPerspective);
}

void GraphPerspective::construct(tlp::PluginProgress *progress) {
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
  connect(_ui->workspace,SIGNAL(addPanelAtStartupButtonClicked()),this,SLOT(createPanel()));

  // Connect actions
  connect(_ui->actionFull_screen,SIGNAL(triggered(bool)),this,SLOT(showFullScreen(bool)));
  connect(_ui->actionImport,SIGNAL(triggered()),this,SLOT(importGraph()));
  connect(_ui->workspace,SIGNAL(panelFocused(tlp::View*)),this,SLOT(panelFocused(tlp::View*)));
  connect(_ui->actionSave_Project,SIGNAL(triggered()),this,SLOT(save()));
  connect(_ui->actionSave_Project_as,SIGNAL(triggered()),this,SLOT(saveAs()));
  connect(_ui->actionOpen_Project,SIGNAL(triggered()),this,SLOT(open()));

  // D-BUS actions
  connect(_ui->actionPlugins_Center,SIGNAL(triggered()),this,SIGNAL(showTulipPluginsCenter()));
  connect(_ui->actionAbout_us,SIGNAL(triggered()),this,SIGNAL(showTulipAboutPage()));

  // Setting initial sizes for splitters
  _ui->workspaceSplitter->setSizes(QList<int>() << 200 << 1000);
  _ui->docksSplitter->setSizes(QList<int>() << 500 << 800);

  _mainWindow->show();
  // Open project with model
  _graphs->readProject(_project,progress);

  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->algorithmRunner->setModel(_graphs);
  _ui->workspace->setModel(_graphs);

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h,SIGNAL(expanded(bool)),this,SLOT(refreshDockExpandControls()));
  }

  //FIXME: alpha release code
  _ui->filtersManager->hide();
}

void GraphPerspective::refreshDockExpandControls() {
  QList<HeaderFrame *> expandedHeaders, collapsedHeaders;
  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    h->expandControl()->setEnabled(true);

    if (h->isExpanded())
      expandedHeaders.push_back(h);
    else
      collapsedHeaders.push_back(h);
  }

  if (expandedHeaders.size() == 1)
    expandedHeaders[0]->expandControl()->setEnabled(false);
}

void GraphPerspective::showFullScreen(bool f) {
  if (f) {
    _maximised = _mainWindow->isMaximized();
    _mainWindow->showFullScreen();
  }
  else {
    _mainWindow->showNormal();

    if (_maximised)
      _mainWindow->showMaximized();
  }
}

void GraphPerspective::importGraph() {
  ImportWizard wizard(_graphs,_mainWindow);

  if (wizard.exec() == QDialog::Accepted) {
    Graph* g = NULL;

    if (!wizard.algorithm().isNull()) {
      DataSet data = wizard.parameters();
      g = tlp::importGraph(wizard.algorithm().toStdString(),data);

      if (g == NULL) {
        QMessageBox::critical(_mainWindow,trUtf8("Import error"),wizard.algorithm() + trUtf8(" failed to import data"));
        return;
      }

      std::string name;

      if (!g->getAttribute<std::string>("name",name)) {
        name = (wizard.algorithm() + " - " + wizard.parameters().toString().c_str()).toStdString();
        g->setAttribute<std::string>("name",name);
      }
    }
    else {
      g = newGraph();
    }

    _graphs->addGraph(g);

    if (wizard.createPanel() && !wizard.panelName().isNull()) {
      _ui->workspace->setActivePanel(_ui->workspace->addPanel(wizard.panelName(),g));
    }
  }
}

void GraphPerspective::createPanel(tlp::Graph* g) {
  if (_graphs->size() == 0)
    return;

  PanelSelectionWizard wizard(_graphs,_mainWindow);

  if (g != NULL) {
    wizard.setSelectedGraph(g);
  }
  else {
    wizard.setSelectedGraph(_graphs->currentGraph());
  }

  if (wizard.exec() == QDialog::Accepted) {
    if (!wizard.panelName().isNull()) {
      _ui->workspace->setActivePanel(_ui->workspace->addPanel(wizard.panelName(),wizard.graph()));
    }
  }
}

void GraphPerspective::panelFocused(tlp::View* view) {
  if (view->graph() != NULL && _ui->graphHierarchiesEditor->synchronized())
    _graphs->setCurrentGraph(view->graph());
}

void GraphPerspective::save() {
  saveAs(_project->projectFile());
}

void GraphPerspective::saveAs(const QString& path) {
  if (path.isEmpty()) {
    QString path = QFileDialog::getSaveFileName(_mainWindow,trUtf8("Select the file you want to save your workspace in."),QString(),"*.tlpx");

    if (!path.isEmpty()) {
      saveAs(path);
    }

    return;
  }

  _graphs->writeProject(_project,0); // FIXME add progress here
  _project->write(path); // FIXME: add progress here
}

void GraphPerspective::open(const QString &path) {
}

PERSPECTIVEPLUGIN(GraphPerspective,"Graph hierarchy analysis", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0")
