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
#ifndef GRAPHPERSPECTIVE_H
#define GRAPHPERSPECTIVE_H

#include <QtCore/QModelIndex>
#include <QtCore/QPoint>
#include <tulip/Perspective.h>


class QAction;
class QHeaderView;

class GraphHierarchiesEditor;
class GraphPerspectiveLogger;

namespace tlp {
class GraphHierarchiesModel;
class View;
}

namespace Ui {
class GraphPerspectiveMainWindowData;
}

class GraphPerspective : public tlp::Perspective {
  Q_OBJECT
  Ui::GraphPerspectiveMainWindowData *_ui;
  tlp::GraphHierarchiesModel *_graphs;
  bool _maximised;

  void reserveDefaultProperties();

public:
  PLUGININFORMATIONS("Tulip", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0", "Hierarchy")
  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/desktop.png";
  }

  GraphPerspective(const tlp::PluginContext* c);
  virtual ~GraphPerspective();
  virtual void start(tlp::PluginProgress *);
  tlp::GraphHierarchiesModel* model() const;

  GraphPerspectiveLogger* _logger;

public slots:
  void showFullScreen(bool);
  void importGraph();
  void exportGraph(tlp::Graph*g = NULL);
  void createPanel(tlp::Graph* g = NULL);
  void save();
  void saveAs(const QString& path=QString::null);
  void open();

  void log(QtMsgType,const char*);
  void showLogger();

  void centerPanelsForGraph(tlp::Graph*);
  void closePanelsForGraph(tlp::Graph* g = NULL);
  void setPythonOutput(bool);
  void setSearchOutput(bool);

protected slots:
  void currentGraphChanged(tlp::Graph* graph);
  void refreshDockExpandControls();
  void panelFocused(tlp::View*);
  void focusedPanelGraphSet(tlp::Graph*);
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
  void CSVImport();
  void logCleared();
  void findPlugins();
  void addNewGraph();

protected:
  bool eventFilter(QObject *, QEvent *);
};

#endif // GRAPHPERSPECTIVE_H
