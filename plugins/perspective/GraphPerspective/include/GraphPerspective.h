/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
class QDialog;

class GraphHierarchiesEditor;
class GraphPerspectiveLogger;

namespace tlp {
class GraphHierarchiesModel;
class View;
class BooleanProperty;
class ColorScaleConfigDialog;
} // namespace tlp

namespace Ui {
class GraphPerspectiveMainWindowData;
}

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
class PythonPanel;
namespace tlp {
class PythonIDE;
}
#endif

class GraphPerspective : public tlp::Perspective, tlp::Observable {
  Q_OBJECT
  Ui::GraphPerspectiveMainWindowData *_ui;
  tlp::GraphHierarchiesModel *_graphs;
  tlp::ColorScaleConfigDialog *_colorScalesDialog;

  void reserveDefaultProperties();
  QString _lastOpenLocation;

  void buildRecentDocumentsMenu();
  void addRecentDocument(const QString &path);

  void showStartPanels(tlp::Graph *);
  void applyDefaultLayout(tlp::Graph *);

  void buildPythonIDE();

public:
  GraphPerspectiveLogger *_logger;

  PLUGININFORMATION("Tulip", "Tulip Team", "2011/07/11",
                    "Analyze several graphs/subgraphs hierarchies\n(designed to import/explore "
                    "data from various graph or csv file formats)",
                    "1.0", "")
  std::string icon() const override {
    return ":/tulip/graphperspective/icons/32/desktop.png";
  }

  GraphPerspective(const tlp::PluginContext *c);
  ~GraphPerspective() override;
  void start(tlp::PluginProgress *) override;
  tlp::GraphHierarchiesModel *model() const;
  void copy(tlp::Graph *, bool deleteAfter = false);
  tlp::Graph *createSubGraph(tlp::Graph *);
  QAction *createPanelAction();
  QAction *exportAction();

  void treatEvent(const tlp::Event &) override;

  void logMessage(QtMsgType, const QMessageLogContext &, const QString &) override;

  bool terminated() override;

public slots:
  void importGraph();
  void exportGraph(tlp::Graph *g = nullptr);
  void saveGraphHierarchyInTlpFile(tlp::Graph *g = nullptr);
  void createPanel(tlp::Graph *g = nullptr);
  bool save();
  bool saveAs(const QString &path = "");
  void open(QString fileName = "");
  void openProjectFile(const QString &path) override;

  void displayLogMessages() override;
  void showAPIDocumentation();
  void showUserDocumentation();
  void showDevelDocumentation();
  void showPythonDocumentation();

  void redrawPanels(bool center = false) override;
  void centerPanelsForGraph(tlp::Graph *, bool graphChanged, bool onlyGlMainView);
  void centerPanelsForGraph(tlp::Graph *g) override {
    centerPanelsForGraph(g, false, false);
  }
  void closePanelsForGraph(tlp::Graph *g = nullptr);
  bool
  setGlMainViewPropertiesForGraph(tlp::Graph *g,
                                  const std::map<std::string, tlp::PropertyInterface *> &propsMap);
  void showSearchDialog(bool = true);
  void openPreferences();

  void setAutoCenterPanelsOnDraw(bool f);

  void pluginsListChanged();

  void showPythonIDE();

  void displayColorScalesDialog();

  void showAboutTulipPage();

protected slots:
  void currentGraphChanged(tlp::Graph *graph);
  void refreshDockExpandControls();
  void panelFocused(tlp::View *);
  void focusedPanelGraphSet(tlp::Graph *);
  void focusedPanelSynchronized();
  void clearGraph();
  void deleteSelectedElements(bool fromRoot = false);
  void deleteSelectedElementsFromRootGraph();
  void invertSelection();
  void reverseSelectedEdges();
  void cancelSelection();
  void make_graph();
  void selectAll(bool nodes = true, bool edges = true);
  void selectAllNodes();
  void selectAllEdges();
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
  void addNewGraph();
  void newProject();
  void openRecentFile();
  void changeSynchronization(bool);
  void openExternalFile();
  void showHideSideBar();
  void showStartMessage();
  void resetLoggerDialogPosition();
  void showHideLogger();
  void panelsEmpty();
  void showHideMenuBar();
  void updateLogIconsAndCounters();
  void initPythonIDE();
  void displayStatusMessage(const QString &s) override;
  void clearStatusMessage() override;

protected:
  bool eventFilter(QObject *, QEvent *) override;
  void importGraph(const std::string &module, tlp::DataSet &data);
  void destroyWorkspace();

  QDialog *_searchDialog;
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  PythonPanel *_pythonPanel;
  tlp::PythonIDE *_pythonIDE;
  QDialog *_pythonIDEDialog;
#endif
};

#endif // GRAPHPERSPECTIVE_H
