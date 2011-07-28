#ifndef GRAPHHIERARCHIESEDITOR_H
#define GRAPHHIERARCHIESEDITOR_H

#include <QtGui/QWidget>

class GraphHierarchiesModel;

namespace Ui {
class GraphHierarchiesEditorData;
}

class GraphHierarchiesEditor : public QWidget {
  Q_OBJECT
  Ui::GraphHierarchiesEditorData *_ui;

public:
  explicit GraphHierarchiesEditor(QWidget *parent = 0);
  void setModel(GraphHierarchiesModel *model);
  ~GraphHierarchiesEditor();

private:
};

#endif // GRAPHHIERARCHIESEDITOR_H
