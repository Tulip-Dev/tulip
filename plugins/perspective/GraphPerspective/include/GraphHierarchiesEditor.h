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
  GraphHierarchiesModel *_model;

public:
  explicit GraphHierarchiesEditor(GraphHierarchiesModel *model, QWidget *parent = 0);
  ~GraphHierarchiesEditor();

private:
};

#endif // GRAPHHIERARCHIESEDITOR_H
