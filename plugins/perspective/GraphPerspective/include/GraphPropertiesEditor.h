#ifndef GRAPHPROPERTIESEDITOR_H
#define GRAPHPROPERTIESEDITOR_H

#include <QtGui/QWidget>

namespace tlp {
class Graph;
class GraphHierarchiesModel;
class PropertyInterface;
class TulipItemDelegate;
}

namespace Ui {
class GraphPropertiesEditor;
}

class GraphPropertiesEditor : public QWidget {
  Q_OBJECT
  Ui::GraphPropertiesEditor* _ui;
  tlp::PropertyInterface* _contextProperty;
  tlp::Graph* _graph;
  tlp::TulipItemDelegate* _delegate;
public:
  explicit GraphPropertiesEditor(QWidget *parent = 0);

public slots:
  void setGraph(tlp::Graph*);
  void setModel(tlp::GraphHierarchiesModel*);

protected slots:
  void showCustomContextMenu(const QPoint&);
  void setAllNodes();
  void setAllEdges();
  void copyProperty();
  void delProperty();
  void toLabels();
  void setPropertyFilter(QString);
};

#endif // GRAPHPROPERTIESEDITOR_H
