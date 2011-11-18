#ifndef PANELSELECTIONWIZARD_H
#define PANELSELECTIONWIZARD_H

#include <QtGui/QWizard>

namespace tlp {
class Graph;
class PluginInformations;
}

namespace Ui {
class PanelSelectionWizard;
class PanelSelectionItem;
}

class FlattenedTreeModelDecorator;
class GraphHierarchiesModel;

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

public slots:
  void setFocus(bool);

protected:
  void focusInEvent(QFocusEvent *);
};

class PanelSelectionWizard : public QWizard {
  Q_OBJECT

  Ui::PanelSelectionWizard* _ui;
  GraphHierarchiesModel* _model;
  FlattenedTreeModelDecorator* _flattenedModel;
  tlp::Graph* _graph;
  PanelSelectionItem* _activeItem;
public:
  explicit PanelSelectionWizard(GraphHierarchiesModel* model, QWidget *parent = 0);
  virtual ~PanelSelectionWizard();

  tlp::Graph* graph() const;
  QString panelName() const;

protected slots:
  void graphSelected(int);
  void panelSelected();
};

#endif // PANELSELECTIONWIZARD_H
