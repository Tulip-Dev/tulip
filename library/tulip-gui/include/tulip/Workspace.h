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

class QLabel;
namespace tlp {
class PluginProgress;
class View;
class WorkspacePanel;
class Graph;
class GraphHierarchiesModel;
class TulipProject;

class TLP_QT_SCOPE Workspace: public QWidget {
  Q_OBJECT

  Q_PROPERTY(bool bottomFrameVisible READ isBottomFrameVisible WRITE setBottomFrameVisible)

  Ui::Workspace* _ui;
  QList<WorkspacePanel*> _panels;
  int _currentPanelIndex;
  QWidget* _oldWorkspaceMode;

  QMap<QWidget*,QVector<PlaceHolderWidget*> > _modeToSlots;
  QMap<QWidget*,QWidget*> _modeSwitches;
  tlp::GraphHierarchiesModel* _model;

  QString panelTitle(tlp::WorkspacePanel*) const;

  QLabel* _pageCountLabel;

public:
  explicit Workspace(QWidget *parent = NULL);
  virtual ~Workspace();

  int addPanel(tlp::View*);
  QList<tlp::View*> panels() const;

  bool isBottomFrameVisible() const;

public slots:
  void delView(tlp::View* view);
  void switchToStartupMode();
  void switchToSingleMode();
  void setSingleModeSwitch(QWidget*);
  void switchToSplitMode();
  void setSplitModeSwitch(QWidget*);
  void switchToSplit3Mode();
  void setSplit3ModeSwitch(QWidget*);
  void switchToSplit32Mode();
  void setSplit32ModeSwitch(QWidget*);
  void switchToGridMode();
  void setGridModeSwitch(QWidget*);
  void switchToSixMode();
  void setSixModeSwitch(QWidget*);

  void nextPage();
  void previousPage();
  void expose(bool);
  void showExposeMode();
  void hideExposeMode();

  void setActivePanel(tlp::View*);
  void setModel(tlp::GraphHierarchiesModel*);

  void writeProject(tlp::TulipProject*,QMap<tlp::Graph*,QString>, tlp::PluginProgress*);
  void readProject(tlp::TulipProject*,QMap<QString,tlp::Graph*>,tlp::PluginProgress*);

  void setBottomFrameVisible(bool);
  void setPageCountLabel(QLabel* l);

  void redrawPanels(bool center=false);

signals:
  void panelFocused(tlp::View*);
  void addPanelRequest(tlp::Graph* g = NULL);

protected slots:
  void viewNeedsDraw();
  void switchWorkspaceMode(QWidget* page);
  void panelDestroyed(QObject*);
  void updatePageCountLabel();
  void updateAvailableModes();
  void updatePanels();
  void uncheckExposeButton();
  QWidget* suitableMode(QWidget* oldMode);
  WorkspacePanel* panelForScene(QObject* obj);

protected:
  virtual void dragEnterEvent(QDragEnterEvent* event);
  virtual void dropEvent(QDropEvent* event);

  bool handleDragEnterEvent(QEvent* e, const QMimeData* mimedata);
  bool handleDropEvent(const QMimeData* mimedata, WorkspacePanel* panel = NULL);

  bool eventFilter(QObject *, QEvent *);

  QWidget* currentModeWidget() const;
  QVector<PlaceHolderWidget*> currentModeSlots() const;
  unsigned int currentSlotsCount() const;
};
}

#endif // WORKSPACE_H
