#ifndef GRAPHPROPERTIESEDITOR_H
#define GRAPHPROPERTIESEDITOR_H

#include <QtGui/QWidget>

namespace tlp {
class Graph;
class GraphHierarchiesModel;
}

namespace Ui {
class GraphPropertiesEditor;
}

class GraphPropertiesEditor : public QWidget {
  Q_OBJECT
  Ui::GraphPropertiesEditor* _ui;
public:
  explicit GraphPropertiesEditor(QWidget *parent = 0);

public slots:
  void setGraph(tlp::Graph*);
  void setModel(tlp::GraphHierarchiesModel*);
};

#endif // GRAPHPROPERTIESEDITOR_H
