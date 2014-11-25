/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#ifndef GRAPHPERSPECTIVE_H
#define GRAPHPERSPECTIVE_H

#include <QModelIndex>
#include <QPoint>
#include <tulip/Perspective.h>
#include <tulip/Observable.h>

class QAction;
class QHeaderView;

class GraphHierarchiesEditor;
class GraphPerspectiveLogger;

namespace tlp {
class GraphHierarchiesModel;
class View;
class BooleanProperty;
}

namespace Ui {
class GraphPerspectiveMainWindowData;
}

#ifdef BUILD_PYTHON_COMPONENTS
class PythonPanel;
class PythonPluginsIDE;
#endif

class GraphPerspective : public tlp::Perspective, tlp::Observable {
  Q_OBJECT
  Ui::GraphPerspectiveMainWindowData *_ui;
  tlp::GraphHierarchiesModel *_graphs;

  void reserveDefaultProperties();
  QString _lastOpenLocation;
  QString _recentDocumentsSettingsKey;

  void buildRecentDocumentsMenu();
  void addRecentDocument(const QString &path);

  void showStartPanels(tlp::Graph*);
  void applyRandomLayout(tlp::Graph*);

public:
  GraphPerspectiveLogger* _logger;

  PLUGININFORMATION("Tulip", "Tulip Team", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0", "")
  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/desktop.png";
  }

  GraphPerspective(const tlp::PluginContext* c);
  virtual ~GraphPerspective();
  virtual void start(tlp::PluginProgress *);
  tlp::GraphHierarchiesModel* model() const;
  void copy(tlp::Graph*, bool deleteAfter=false);
  tlp::Graph* createSubGraph(tlp::Graph*);

  void treatEvent(const tlp::Event &);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  void log(QtMsgType, const QMessageLogContext &, const QString &);
#else
  void log(QtMsgType,const char*);
#endif

public slots:
  void importGraph();
  void exportGraph(tlp::Graph*g = NULL);
  void saveGraphHierarchyInTlpFile(tlp::Graph*g = NULL);
  void createPanel(tlp::Graph* g = NULL);
  bool save();
  bool saveAs(const QString& path=QString::null);
  void open(QString fileName = QString::null);
  virtual void openProjectFile(const QString &path);

  void showLogger();
  void showDocumentation();

  virtual void redrawPanels(bool center=false);
  void centerPanelsForGraph(tlp::Graph*, bool graphChanged, bool onlyGlMainView);
  void centerPanelsForGraph(tlp::Graph* g) {
    centerPanelsForGraph(g, false, false);
  }
  void closePanelsForGraph(tlp::Graph* g = NULL);
  bool setGlMainViewPropertiesForGraph(tlp::Graph* g, const std::map<std::string, tlp::PropertyInterface*>& propsMap);
  void setSearchOutput(bool);
  void setPythonPanel(bool);
  void openPreferences();

  void setAutoCenterPanelsOnDraw(bool f);

  void pluginsListChanged();

  void setWorkspaceMode();
  void setDevelopMode();

protected slots:
  void currentGraphChanged(tlp::Graph* graph);
  void refreshDockExpandControls();
  void panelFocused(tlp::View*);
  void focusedPanelGraphSet(tlp::Graph*);
  void focusedPanelSynchronized();
  void deleteSelectedElements();
  void invertSelection();
  void cancelSelection();
  void selectAll();
  void undo();
  void redo();
  void cut();
  void paste();
  void copy();
  void group();
  void createSubGraph();
  void cloneSubGraph();
  void addEmptySubGraph();
  void CSVImport();
  void logCleared();
  void findPlugins();
  void addNewGraph();
  void newProject();
  void openRecentFile();
  void changeSynchronization(bool);

protected:
  bool eventFilter(QObject *, QEvent *);
  void importGraph(const std::string& module, tlp::DataSet& data);


#ifdef BUILD_PYTHON_COMPONENTS
  PythonPanel *_pythonPanel;
  PythonPluginsIDE *_developFrame;
#endif

};

#endif // GRAPHPERSPECTIVE_H
