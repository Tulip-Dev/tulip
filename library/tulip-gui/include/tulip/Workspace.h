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
#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtGui/QWidget>
#include <tulip/tulipconf.h>
#include <tulip/DataSet.h>

class PlaceHolderWidget;

namespace Ui {
class Workspace;
}

namespace tlp {
class View;
class WorkspacePanel;
class Graph;
class GraphHierarchiesModel;

class TLP_QT_SCOPE Workspace: public QWidget {
  Q_OBJECT

  Ui::Workspace* _ui;
  QList<tlp::WorkspacePanel*> _panels;
  int _currentPanelIndex;

  QMap<QWidget*,QVector<PlaceHolderWidget*> > _modeToSlots;
  QMap<QWidget*,QWidget*> _modeSwitches;
  tlp::GraphHierarchiesModel* _model;

public:
  explicit Workspace(QWidget *parent = 0);
  virtual ~Workspace();

  tlp::View* addPanel(const QString& viewName, tlp::Graph* g, const tlp::DataSet& data=tlp::DataSet());
  QList<tlp::View*> panels() const;

public slots:
  void delView(tlp::View* view);
  void switchToStartupMode();
  void switchToSingleMode();
  void switchToSplitMode();
  void switchToGridMode();

  void nextPage();
  void previousPage();

  void setActivePanel(tlp::View*);
  void setModel(tlp::GraphHierarchiesModel*);

signals:
  void addPanelAtStartupButtonClicked();
  void panelFocused(tlp::View*);

protected slots:
  void viewNeedsDraw();
  void switchWorkspaceMode(QWidget* page);
  void updatePageCountLabel();
  void updateAvailableModes();
  void removePanel(tlp::WorkspacePanel*);
  void updatePanels();

  QWidget* currentModeWidget() const;
  QVector<PlaceHolderWidget*> currentModeSlots() const;
  unsigned int currentSlotsCount() const;

protected:
  bool eventFilter(QObject *, QEvent *);

};
}

#endif // WORKSPACE_H
