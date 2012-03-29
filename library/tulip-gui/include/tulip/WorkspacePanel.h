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
#ifndef WORKSPACEPANEL_H
#define WORKSPACEPANEL_H

#include <QtCore/QAbstractItemModel>
#include <QtGui/QWidget>
#include <tulip/tulipconf.h>
#include <tulip/DataSet.h>
#include <tulip/SimplePluginProgress.h>

class QGraphicsObject;
class QPropertyAnimation;
class QGraphicsProxyWidget;

namespace Ui {
class WorkspacePanel;
}

namespace tlp {
class Graph;
class View;
class Interactor;
class GraphHierarchiesModel;

class TLP_QT_SCOPE WorkspacePanel : public QWidget, public tlp::SimplePluginProgress {
  Q_OBJECT
  Q_PROPERTY(bool progressMode READ isProgressMode WRITE toggleProgressMode)

  Ui::WorkspacePanel* _ui;
  tlp::View* _view;
  QString _viewName;
  QMap<QAction*, QWidget*> _actionTriggers;

  QGraphicsProxyWidget* _viewConfigurationWidgets;
  bool _viewConfigurationExpanded;
  QGraphicsProxyWidget* _currentInteractorConfigurationItem;
  QGraphicsObject* _progressItem;

  QPointF configurationTabPosition(bool expanded) const;
  void setConfigurationTabExpanded(bool expanded, bool animate=true);

public:
  explicit WorkspacePanel(tlp::View* view, QWidget* parent=0);
  virtual ~WorkspacePanel();

  tlp::View* view() const;
  QString viewName() const;
  bool isProgressMode() const;

public slots:
  void toggleProgressMode(bool p);
  void setView(tlp::View* view);
  void setCurrentInteractor(tlp::Interactor*);
  void setGraphsModel(tlp::GraphHierarchiesModel*);
  void viewGraphSet(tlp::Graph*);

signals:
  void drawNeeded();

protected slots:
  void hideConfigurationTab();
  void interactorActionTriggered();
  void viewDestroyed();
  void graphComboIndexChanged();
  void setCurrentInteractorConfigurationVisible(bool);
  void actionChanged();

protected:
  void progress_handler(int step, int max_step);
  void refreshInteractorsToolbar();
  bool eventFilter(QObject *, QEvent *);
  void resizeEvent(QResizeEvent *);
};

}

#endif // WORKSPACEPANEL_H
