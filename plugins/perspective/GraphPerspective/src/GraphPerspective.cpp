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
#include "ShadowFilter.h"
#include <tulip/GraphHierarchiesModel.h>

#ifndef NDEBUG
#include <modeltest.h>
#endif

#include <QtCore/QDebug>

using namespace tlp;

GraphPerspective::GraphPerspective(const tlp::PluginContext* c): Perspective(c), _ui(0), _graphs(new GraphHierarchiesModel(this)) {
#ifndef NDEBUG
  new ModelTest(_graphs,this);
#endif /* NDEBUG */
  Q_INIT_RESOURCE(GraphPerspective);
}

void GraphPerspective::construct(tlp::PluginProgress *progress) {
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
  _mainWindow->installEventFilter(new ShadowFilter(this));
  connect(_ui->workspace,SIGNAL(addPanelRequest(tlp::Graph*)),this,SLOT(createPanel(tlp::Graph*)));

  connect(_graphs,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(currentGraphChanged(tlp::Graph*)));

  // Connect actions
  connect(_ui->actionFull_screen,SIGNAL(triggered(bool)),this,SLOT(showFullScreen(bool)));
  connect(_ui->actionImport,SIGNAL(triggered()),this,SLOT(importGraph()));
  connect(_ui->workspace,SIGNAL(panelFocused(tlp::View*)),this,SLOT(panelFocused(tlp::View*)));
  connect(_ui->actionSave_Project,SIGNAL(triggered()),this,SLOT(save()));
  connect(_ui->actionSave_Project_as,SIGNAL(triggered()),this,SLOT(saveAs()));
  connect(_ui->actionOpen_Project,SIGNAL(triggered()),this,SLOT(open()));
  connect(_ui->actionAnalyze,SIGNAL(triggered()),this,SLOT(modeSwitch()));
  connect(_ui->actionCharts,SIGNAL(triggered()),this,SLOT(modeSwitch()));
  connect(_ui->actionDelete,SIGNAL(triggered()),this,SLOT(deleteSelectedElements()));
  connect(_ui->actionInvert_selection,SIGNAL(triggered()),this,SLOT(invertSelection()));
  connect(_ui->actionCancel_selection,SIGNAL(triggered()),this,SLOT(cancelSelection()));
  connect(_ui->actionSelect_All,SIGNAL(triggered()),this,SLOT(selectAll()));
  connect(_ui->actionUndo,SIGNAL(triggered()),this,SLOT(undo()));
  connect(_ui->actionRedo,SIGNAL(triggered()),this,SLOT(redo()));
  connect(_ui->actionCut,SIGNAL(triggered()),this,SLOT(cut()));
  connect(_ui->actionPaste,SIGNAL(triggered()),this,SLOT(paste()));
  connect(_ui->actionCopy,SIGNAL(triggered()),this,SLOT(copy()));
  connect(_ui->actionGroup_elements,SIGNAL(triggered()),this,SLOT(group()));
  connect(_ui->actionCreate_sub_graph,SIGNAL(triggered()),this,SLOT(createSubGraph()));

  // D-BUS actions
  connect(_ui->actionPlugins_Center,SIGNAL(triggered()),this,SIGNAL(showTulipPluginsCenter()));
  connect(_ui->actionAbout_us,SIGNAL(triggered()),this,SIGNAL(showTulipAboutPage()));

  // Setting initial sizes for splitters
  _ui->workspaceSplitter->setSizes(QList<int>() << 200 << 1000);

  _mainWindow->show();
  // Open project with model
  _graphs->readProject(_project,progress);

  if (!_externalFile.isEmpty()) {
    QFileInfo externalFileInfo(_externalFile);

    if (externalFileInfo.exists()) {
      progress->setComment((trUtf8("Loading ") + externalFileInfo.fileName()).toStdString());
      DataSet dataSet;
      dataSet.set("file::filename", externalFileInfo.absoluteFilePath().toStdString());
      Graph *externalGraph = tlp::importGraph("tlp", dataSet, progress);
      _graphs->addGraph(externalGraph);
    }
  }

  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->algorithmRunner->setModel(_graphs);
  _ui->workspace->setModel(_graphs);
  _ui->datasetWidget->setModel(_graphs);

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h,SIGNAL(expanded(bool)),this,SLOT(refreshDockExpandControls()));
  }
}

tlp::GraphHierarchiesModel* GraphPerspective::model() const {
  return _graphs;
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
  ImportWizard wizard(_mainWindow);

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
    _ui->graphHierarchiesEditor->repackHeaders();
  }
}

void GraphPerspective::createPanel(tlp::Graph* g) {
  if (_graphs->size() == 0)
    return;

  PanelSelectionWizard wizard(_graphs,_mainWindow);

  if (g != NULL)
    wizard.setSelectedGraph(g);
  else
    wizard.setSelectedGraph(_graphs->currentGraph());


  int result = wizard.exec();

  if (result == QDialog::Accepted && wizard.panel() != NULL) {
    _ui->workspace->addPanel(wizard.panel(),wizard.panelName());
    _ui->workspace->setActivePanel(wizard.panel());
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
    QString path = QFileDialog::getSaveFileName(_mainWindow,trUtf8("Save file"),QString(),"Tulip Project (*.tlpx)");

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

void GraphPerspective::centerPanels(tlp::PropertyInterface* pi) {
  foreach(tlp::View* view, _ui->workspace->panels()) {
    if (view->isLayoutProperty(pi))
      view->centerView();
  }
}

void GraphPerspective::modeSwitch() {
  QWidget* mode = NULL;

  if (sender() == _ui->actionCharts)
    mode = _ui->datasetModePage;
  else if (sender() == _ui->actionAnalyze)
    mode = _ui->visualizeModePage;

  if (mode != NULL)
    _ui->centralWidget->setCurrentWidget(mode);
}

void GraphPerspective::deleteSelectedElements() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");

  tlp::Iterator<node>* itNodes = selection->getNodesEqualTo(true);
  graph->delNodes(itNodes, false);
  delete itNodes;

  tlp::Iterator<edge>* itEdges = selection->getEdgesEqualTo(true);
  graph->delEdges(itEdges, false);
  delete itEdges;

  Observable::unholdObservers();
}

void GraphPerspective::invertSelection() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  selection->reverse();
  Observable::unholdObservers();
}

void GraphPerspective::cancelSelection() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  selection->setAllEdgeValue(false);
  selection->setAllNodeValue(false);
  Observable::unholdObservers();
}

void GraphPerspective::selectAll() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  selection->setAllEdgeValue(true);
  selection->setAllNodeValue(true);
  Observable::unholdObservers();
}

void GraphPerspective::undo() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  graph->pop();
  Observable::unholdObservers();
}

void GraphPerspective::redo() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  graph->unpop();
  Observable::unholdObservers();
}

void GraphPerspective::cut() {
  //TODO implement me
}

void GraphPerspective::paste() {
  //TODO implement me
}

void GraphPerspective::copy() {
  //TODO implement me
}

void GraphPerspective::group() {
  //TODO implement me
}

void GraphPerspective::createSubGraph() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->addSubGraph(selection);
  Observable::unholdObservers();
}

void GraphPerspective::currentGraphChanged(Graph *graph) {
  bool enabled(graph != NULL);
  _ui->actionUndo->setEnabled(enabled);
  _ui->actionRedo->setEnabled(enabled);
  _ui->actionCut->setEnabled(enabled);
  _ui->actionCopy->setEnabled(enabled);
  _ui->actionPaste->setEnabled(enabled);
  _ui->actionDelete->setEnabled(enabled);
  _ui->actionInvert_selection->setEnabled(enabled);
  _ui->actionSelect_All->setEnabled(enabled);
  _ui->actionCancel_selection->setEnabled(enabled);
  _ui->actionGroup_elements->setEnabled(enabled);
  _ui->actionCreate_sub_graph->setEnabled(enabled);
}

PLUGIN(GraphPerspective)
