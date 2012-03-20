#include "FiltersManagerAlgorithmItem.h"
#include "ui_FiltersManagerAlgorithmItem.h"

#include <QtGui/QStandardItem>
#include <QtGui/QComboBox>

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/TulipItemDelegate.h>

using namespace tlp;
using namespace std;

FiltersManagerAlgorithmItem::FiltersManagerAlgorithmItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerAlgorithmItem) {
  _ui->setupUi(this);

  fillTitle(_ui->algorithmCombo,trUtf8("Select filter"));
  list<string> booleanAlgorithms = PluginLister::instance()->availablePlugins<BooleanAlgorithm>();

  for(list<string>::const_iterator it = booleanAlgorithms.begin(); it != booleanAlgorithms.end(); ++it) {
    string s(*it);
    _ui->algorithmCombo->addItem(s.c_str());
  }

  connect(_ui->algorithmCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(algorithmSelected(int)));
  _ui->algorithmParams->setItemDelegate(new TulipItemDelegate);
  connect(_ui->algorithmParams, SIGNAL(destroyed()), _ui->algorithmParams->itemDelegate(), SLOT(deleteLater()));
  
  updateGraphModel(_ui->algorithmParams,QString::null,_graph);
}
void FiltersManagerAlgorithmItem::algorithmSelected(int i) {
  _ui->algorithmParams->setEnabled(i != 0);
  QString algName = QString::null;

  if (i != 0)
    algName = _ui->algorithmCombo->itemText(i);

  updateGraphModel(_ui->algorithmParams,algName,_graph);
  emit titleChanged();
}

void FiltersManagerAlgorithmItem::applyFilter(BooleanProperty* prop) {
  if (_ui->algorithmCombo->currentIndex() == 0)
    return;

  ParameterListModel* model = dynamic_cast<ParameterListModel*>(_ui->algorithmParams->model());
  string msg;

  if (model != NULL) {
    DataSet data = model->parametersValues();
    _graph->computeProperty<BooleanProperty>(_ui->algorithmCombo->currentText().toStdString(),
        prop,
        msg,
        0,
        &data);
  }
  else {
    _graph->computeProperty<BooleanProperty>(_ui->algorithmCombo->currentText().toStdString(),
        prop,
        msg);
  }
}

QString FiltersManagerAlgorithmItem::title() const {
  if (_ui->algorithmCombo->currentIndex() == 0)
    return trUtf8("Select filtering method");

  return _ui->algorithmCombo->currentText();
}

void FiltersManagerAlgorithmItem::graphChanged() {
  if (_ui->algorithmCombo->currentIndex() != 0)
    updateGraphModel(_ui->algorithmParams,_ui->algorithmCombo->currentText(),_graph);
}
