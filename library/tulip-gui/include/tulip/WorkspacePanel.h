/*
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
///@cond DOXYGEN_HIDDEN

#ifndef WORKSPACEPANEL_H
#define WORKSPACEPANEL_H

#include <QFrame>
#include <QAction>

#include <tulip/tulipconf.h>

class QDragEnterEvent;
class QGraphicsObject;
class QPropertyAnimation;
class QGraphicsProxyWidget;
class QGraphicsRectItem;
class QMimeData;

namespace Ui {
class WorkspacePanel;
}

namespace tlp {
class Graph;
class View;
class Interactor;
class GraphHierarchiesModel;

class TLP_QT_SCOPE WorkspacePanel : public QFrame {
  Q_OBJECT

  Ui::WorkspacePanel* _ui;
  tlp::View* _view;
  QString _viewName;
  QMap<QAction*, QWidget*> _actionTriggers;
  QGraphicsRectItem* _overlayRect;

  QGraphicsProxyWidget* _viewConfigurationWidgets;
  bool _viewConfigurationExpanded;
  QGraphicsProxyWidget* _currentInteractorConfigurationItem;

  QPointF configurationTabPosition(bool expanded) const;
  void setConfigurationTabExpanded(bool expanded, bool animate=true);

public:
  explicit WorkspacePanel(tlp::View* view, QWidget* parent=NULL);
  virtual ~WorkspacePanel();

  bool eventFilter(QObject *, QEvent *);

  tlp::View* view() const;
  QString viewName() const;

public slots:
  void setView(tlp::View* view);
  void setCurrentInteractor(tlp::Interactor*);
  void setGraphsModel(tlp::GraphHierarchiesModel*);
  void viewGraphSet(tlp::Graph*);
  void setOverlayMode(bool);
  void setHighlightMode(bool);
  bool isGraphSynchronized() const;

signals:
  void drawNeeded();
  void swapWithPanels(WorkspacePanel* panel);
  void changeGraphSynchronization(bool);

protected slots:
  void hideConfigurationTab();
  void interactorActionTriggered();
  void viewDestroyed();
  void graphComboIndexChanged();
  void setCurrentInteractorConfigurationVisible(bool);
  void actionChanged();

  void scrollInteractorsRight();
  void scrollInteractorsLeft();
  void resetInteractorsScrollButtonsVisibility();
  void refreshInteractorsToolbar();
  void toggleSynchronization(bool);

protected:

  void resizeEvent(QResizeEvent *);

  void dragEnterEvent(QDragEnterEvent *);
  void dropEvent(QDropEvent *);
  void dragLeaveEvent(QDragLeaveEvent *);

  bool handleDragEnterEvent(QEvent* e, const QMimeData* mimedata);
  bool handleDropEvent(const QMimeData* mimedata);

};

}

#endif // WORKSPACEPANEL_H
///@endcond
