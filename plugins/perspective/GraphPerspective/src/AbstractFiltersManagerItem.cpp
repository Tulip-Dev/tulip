#include "AbstractFiltersManagerItem.h"

using namespace tlp;

#include <QtGui/QStandardItem>
#include <QtGui/QComboBox>

QStandardItem* itemAt(QComboBox* combo, int i) {
  QStandardItemModel* model = qobject_cast<QStandardItemModel*>(combo->model());
  QModelIndex index = model->index(i,combo->modelColumn(),combo->rootModelIndex());
  return model->itemFromIndex(index);
}

QStandardItem* lastItem(QComboBox* combo) {
  return itemAt(combo,combo->count()-1);
}

void fillTitle(QComboBox* combo, const QString& s) {
  combo->addItem(s);
  QStandardItem* item = lastItem(combo);
  QFont f;
  f.setBold(true);
  item->setFont(f);
  item->setSelectable(false);
}

AbstractFiltersManagerItem::AbstractFiltersManagerItem(QWidget* parent): QWidget(parent), _graph(NULL) {}

void AbstractFiltersManagerItem::setGraph(Graph *g) {
  _graph = g;
  graphChanged();
}
void AbstractFiltersManagerItem::graphChanged() {
}
