#include "FiltersManagerInvertItem.h"

#include "ui_FiltersManagerInvertItem.h"

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>

using namespace tlp;

FiltersManagerInvertItem::FiltersManagerInvertItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerInvertItem) {
  _ui->setupUi(this);
  connect(_ui->elementCombo,SIGNAL(currentIndexChanged(int)),this,SIGNAL(titleChanged()));
}
void FiltersManagerInvertItem::applyFilter(BooleanProperty* prop) {
  int i = _ui->elementCombo->currentIndex();
  if (i == 0 || i == 2) {
    node n;
    forEach(n, _graph->getNodes())
        prop->setNodeValue(n,!(prop->getNodeValue(n)));
  }

  if (i == 1 || i == 2) {
    edge e;
    forEach(e, _graph->getEdges())
        prop->setEdgeValue(e,!(prop->getEdgeValue(e)));
  }
}
QString FiltersManagerInvertItem::title() const {
  return trUtf8("Invert selection ") + _ui->elementCombo->currentText().toLower();
}
