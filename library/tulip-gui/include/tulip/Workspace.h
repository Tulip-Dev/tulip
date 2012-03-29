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
#include <QtCore/QAbstractItemModel>
#include <tulip/tulipconf.h>
#include <tulip/DataSet.h>
#include <tulip/WorkspaceExposeWidget.h>

class PlaceHolderWidget;

namespace Ui {
class Workspace;
}

namespace tlp {
class PluginProgress;
class View;
class WorkspacePanel;
class Graph;
class GraphHierarchiesModel;
class TulipProject;

class TLP_QT_SCOPE Workspace: public QWidget {
  Q_OBJECT

  // Helper class
  class PanelsStorage: public QAbstractItemModel {
    QList<tlp::WorkspacePanel*> _storage;
  public:
    PanelsStorage(const PanelsStorage&);
    PanelsStorage(QObject* parent = 0);
    void push_back(tlp::WorkspacePanel*);
    int removeAll(tlp::WorkspacePanel *);
    void clear();
    // Allows the model to behave like a list and to be iterable
    typedef QList<tlp::WorkspacePanel *>::iterator iterator;
    typedef QList<tlp::WorkspacePanel *>::const_iterator const_iterator;
    tlp::WorkspacePanel *operator[](int i) const;
    tlp::WorkspacePanel *operator[](int i);
    int size() const;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    // inherited from QAbstractItemModel
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  };

  Ui::Workspace* _ui;
  PanelsStorage _panels;
  int _currentPanelIndex;
  QWidget* _oldWorkspaceMode;

  QMap<QWidget*,QVector<PlaceHolderWidget*> > _modeToSlots;
  QMap<QWidget*,QWidget*> _modeSwitches;
  tlp::GraphHierarchiesModel* _model;

  QString panelTitle(tlp::WorkspacePanel*) const;

public:
  explicit Workspace(QWidget *parent = 0);
  virtual ~Workspace();

  void addPanel(tlp::View*);
  QList<tlp::View*> panels() const;

public slots:
  void delView(tlp::View* view);
  void switchToStartupMode();
  void switchToSingleMode();
  void switchToSplitMode();
  void switchToSplit3Mode();
  void switchToGridMode();

  void nextPage();
  void previousPage();
  void expose(bool);
  void showExposeMode();
  void hideExposeMode();

  void setActivePanel(tlp::View*);
  void setModel(tlp::GraphHierarchiesModel*);

  void writeProject(tlp::TulipProject*,QMap<tlp::Graph*,QString>, tlp::PluginProgress*);
  void readProject(tlp::TulipProject*,QMap<QString,tlp::Graph*>,tlp::PluginProgress*);

signals:
  void panelFocused(tlp::View*);
  void addPanelRequest(tlp::Graph* g = NULL);

protected slots:
  void addPanelFromDropAction(const QMimeData* data);
  void viewNeedsDraw();
  void switchWorkspaceMode(QWidget* page);
  void panelDestroyed(QObject*);
  void updatePageCountLabel();
  void updateAvailableModes();
  void updatePanels();
  void uncheckExposeButton();
  QWidget* suitableMode(QWidget* oldMode);

protected:
  bool eventFilter(QObject *, QEvent *);
  bool event(QEvent *);

  QWidget* currentModeWidget() const;
  QVector<PlaceHolderWidget*> currentModeSlots() const;
  unsigned int currentSlotsCount() const;
};
}

#endif // WORKSPACE_H
