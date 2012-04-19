#include "TableViewConfiguration.h"

#include <tulip/GraphPropertiesModel.h>
#include <tulip/BooleanProperty.h>
#include "ui_TableViewConfiguration.h"

using namespace tlp;

TableViewConfiguration::TableViewConfiguration(QWidget *parent): QWidget(parent), _ui(new Ui::TableViewConfiguration) {
  _ui->setupUi(this);
  connect(_ui->edgesCheck,SIGNAL(toggled(bool)),this,SIGNAL(settingsChanged()));
  connect(_ui->nodesCheck,SIGNAL(toggled(bool)),this,SIGNAL(settingsChanged()));
  connect(_ui->filteringPropertyCombo,SIGNAL(currentIndexChanged(int)),this,SIGNAL(settingsChanged()));
}

TableViewConfiguration::~TableViewConfiguration() {
  delete _ui;
}

void TableViewConfiguration::setGraph(tlp::Graph *g) {
  // Force creation of the viewSelection property
  BooleanProperty* viewSelection = g->getProperty<BooleanProperty>("viewSelection");
  GraphPropertiesModel<BooleanProperty>* model = new GraphPropertiesModel<BooleanProperty>(trUtf8("No filtering"),g);
  _ui->filteringPropertyCombo->setModel(model);
  _ui->filteringPropertyCombo->setCurrentIndex(model->rowOf(viewSelection));
}

bool TableViewConfiguration::isShowEdges() const {
  return _ui->edgesCheck->isChecked();
}

bool TableViewConfiguration::isShowNodes() const {
  return _ui->nodesCheck->isChecked();
}

BooleanProperty* TableViewConfiguration::filteringProperty() const {
  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  PropertyInterface* pi = model->data(model->index(_ui->filteringPropertyCombo->currentIndex(),0),TulipModel::PropertyRole).value<PropertyInterface*>();
  return dynamic_cast<BooleanProperty*>(pi);
}

void TableViewConfiguration::setShowNodes(bool f) {
  _ui->nodesCheck->setChecked(f);
}

void TableViewConfiguration::setShowEdges(bool f) {
  _ui->edgesCheck->setChecked(f);
}

void TableViewConfiguration::setFilteringProperty(QString name) {
  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  int r = 0;

  if (!name.isEmpty())
    r = model->rowOf(model->graph()->getProperty<BooleanProperty>(name.toStdString()));

  if (r < 0)
    r=0;

  _ui->filteringPropertyCombo->setCurrentIndex(r);
}
