/*
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
///@cond DOXYGEN_HIDDEN

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include <QMap>
#include <QVector>

#include <tulip/tulipconf.h>

class PlaceHolderWidget;

namespace Ui {
class Workspace;
}

class QAbstractButton;
class QLabel;
class QMimeData;

namespace tlp {
class PluginProgress;
class View;
class WorkspacePanel;
class Graph;
class GraphHierarchiesModel;
class TulipProject;

class TLP_QT_SCOPE Workspace : public QWidget {
  Q_OBJECT

  Ui::Workspace *_ui;
  QList<WorkspacePanel *> _panels;
  int _currentPanelIndex;
  QWidget *_oldWorkspaceMode;
  WorkspacePanel *_focusedPanel;
  bool _focusedPanelHighlighting;

  QMap<QWidget *, QVector<PlaceHolderWidget *>> _modeToSlots;
  QMap<QWidget *, QWidget *> _modeSwitches;
  GraphHierarchiesModel *_model;
  QAbstractButton *_exposeButton;

  QString panelTitle(WorkspacePanel *) const;

  bool _autoCenterViews;

public:
  explicit Workspace(QWidget *parent = nullptr);
  ~Workspace() override;

  int addPanel(tlp::View *);
  inline bool empty() const {
    return _panels.empty();
  }
  QList<tlp::View *> panels() const;

public slots:
  void delView(tlp::View *view);
  void switchToStartupMode();
  void switchToSingleMode();
  void setSingleModeSwitch(QWidget *);
  void switchToSplitMode();
  void setSplitModeSwitch(QWidget *);
  void switchToSplitHorizontalMode();
  void setSplitHorizontalModeSwitch(QWidget *);
  void switchToSplit3Mode();
  void setSplit3ModeSwitch(QWidget *);
  void switchToSplit32Mode();
  void setSplit32ModeSwitch(QWidget *);
  void switchToSplit33Mode();
  void setSplit33ModeSwitch(QWidget *);
  void switchToGridMode();
  void setGridModeSwitch(QWidget *);
  void switchToSixMode();
  void setSixModeSwitch(QWidget *);

  void nextPage();
  void previousPage();
  void setExposeModeSwitch(QAbstractButton *);
  void expose(bool);
  void showExposeMode();
  void hideExposeMode();
  void closeAll();
  void setActivePanel(tlp::View *);
  void setModel(tlp::GraphHierarchiesModel *);
  tlp::GraphHierarchiesModel *graphModel() const;

  void writeProject(tlp::TulipProject *, QMap<tlp::Graph *, QString>, tlp::PluginProgress *);
  void readProject(tlp::TulipProject *, QMap<QString, tlp::Graph *>, tlp::PluginProgress *);

  void setPageCountLabel(QLabel *l);

  void redrawPanels(bool center = false);

  void setAutoCenterPanelsOnDraw(bool);

  void setFocusedPanelHighlighting(bool);

  void setGraphForFocusedPanel(tlp::Graph *);

signals:
  void panelFocused(tlp::View *);
  void focusedPanelSynchronized();
  void addPanelRequest(tlp::Graph *g = nullptr);
  void importGraphRequest();

protected slots:
  void viewNeedsDraw();
  void switchWorkspaceMode(QWidget *page);
  void panelDestroyed(QObject *);
  void updatePageCountLabel();
  void updateAvailableModes();
  void updatePanels();
  void uncheckExposeButton();
  QWidget *suitableMode(QWidget *oldMode);
  WorkspacePanel *panelForScene(QObject *obj);
  void swapPanelsRequested(WorkspacePanel *panel);
  void updateStartupMode();
  void changeFocusedPanelSynchronization(bool);

protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

  bool handleDragEnterEvent(QEvent *e, const QMimeData *mimedata);
  bool handleDropEvent(const QMimeData *mimedata);

  bool eventFilter(QObject *, QEvent *) override;

  QWidget *currentModeWidget() const;
  QVector<PlaceHolderWidget *> currentModeSlots() const;
  unsigned int currentSlotsCount() const;

  void setFocusedPanel(WorkspacePanel *panel);
};
} // namespace tlp

#endif // WORKSPACE_H
///@endcond
