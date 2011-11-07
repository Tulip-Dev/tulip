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

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>
#include <tulip/DataSet.h>

namespace Ui {
class Workspace;
}

namespace tlp {
class View;
class WorkspacePanel;
class Graph;

class TLP_QT_SCOPE Workspace: public QWidget {
  Q_OBJECT

  Ui::Workspace* _ui;
  QList<tlp::WorkspacePanel*> _panels;

public:
  explicit Workspace(QWidget *parent = 0);
  virtual ~Workspace();

  tlp::View* addView(const QString& viewName, tlp::Graph* g, const tlp::DataSet& data=tlp::DataSet());
  QList<tlp::View*> views() const;

public slots:
  void delView(tlp::View* view);

  void switchToFullPage();
  void switchToSplitPage();
  void switchToGridPage();
  void switchToExposePage();
  void showViewSwitcher();

  void nextPage();
  void previousPage();

protected slots:
  void viewNeedsDraw();
  void panelClosed();

};
}

#endif // WORKSPACE_H
