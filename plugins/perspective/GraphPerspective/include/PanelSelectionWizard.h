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
#ifndef PANELSELECTIONWIZARD_H
#define PANELSELECTIONWIZARD_H

#include <QtCore/QModelIndex>
#include <QtGui/QWizard>

namespace tlp {
class Graph;
class PluginInformations;
class GraphHierarchiesModel;
class View;
}

namespace Ui {
class PanelSelectionWizard;
class PanelSelectionItem;
}


// Helper class
class PanelSelectionItem: public QWidget {
  Q_OBJECT
  Ui::PanelSelectionItem* _ui;

  QString _viewName;
public:
  explicit PanelSelectionItem(tlp::PluginInformations* infos, QWidget* parent = 0);
  virtual ~PanelSelectionItem();

  QString viewName() const;

signals:
  void selected();
  void doubleClicked();

public slots:
  void setFocus(bool);

protected:
  void focusInEvent(QFocusEvent *);
  void mouseDoubleClickEvent(QMouseEvent *);
};

class PanelSelectionWizard : public QWizard {
  Q_OBJECT

  Ui::PanelSelectionWizard* _ui;
  tlp::GraphHierarchiesModel* _model;
  PanelSelectionItem* _activeItem;
  bool _canSelectGraph;
  tlp::View* _view;
public:
  explicit PanelSelectionWizard(tlp::GraphHierarchiesModel* model, QWidget *parent = 0, bool canSelectGraph = true);
  virtual ~PanelSelectionWizard();

  tlp::Graph* graph() const;
  QString panelName() const;
  tlp::View* panel() const;

  void setSelectedGraph(tlp::Graph*);

signals:
  void wizardFinished();

protected slots:
  void panelSelected();
  void panelDoubleClicked();

  void nextButtonClicked();
  void pageChanged(int);
};

#endif // PANELSELECTIONWIZARD_H
