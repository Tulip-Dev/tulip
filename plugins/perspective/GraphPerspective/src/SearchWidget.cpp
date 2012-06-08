#include "SearchWidget.h"

#include "ui_SearchWidget.h"
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/GraphPropertiesModel.h>
#include <tulip/BooleanProperty.h>

using namespace tlp;
using namespace std;

SearchWidget::SearchWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SearchWidget) {
  _ui->setupUi(this);
  _ui->resultsFrame->hide();
  _ui->customValueEdit->hide();
}

SearchWidget::~SearchWidget() {
  delete _ui;
}

void SearchWidget::setModel(tlp::GraphHierarchiesModel *model) {
  _ui->graphCombo->setModel(model);
  connect(model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(currentGraphChanged(tlp::Graph*)));
  currentGraphChanged(model->currentGraph());
}

void SearchWidget::currentGraphChanged(tlp::Graph *g) {
  GraphHierarchiesModel* graphsModel = static_cast<GraphHierarchiesModel*>(_ui->graphCombo->model());
  _ui->graphCombo->setCurrentIndex(graphsModel->indexOf(g).row());
}

void SearchWidget::setGraph(Graph *g) {
  _ui->resultsStorageCombo->clear();
  _ui->searchTermACombo->clear();
  _ui->searchTermBCombo->clear();
  _ui->resultsTable->setModel(NULL);
  _ui->resultsFrame->hide();
  _ui->resultsCountLabel->setText("");
  setEnabled(g != NULL);
  if (g != NULL) {
    // Force creation of viewSelection to ensure we have at least one boolean property exisint in the graph
    g->getProperty<BooleanProperty>("viewSelection");
    // Storage model
    GraphPropertiesModel<BooleanProperty>* storageModel = new GraphPropertiesModel<BooleanProperty>(g,false,_ui->resultsStorageCombo);
    _ui->resultsStorageCombo->setModel(storageModel);
    // Term A
    GraphPropertiesModel<PropertyInterface>* termAModel = new GraphPropertiesModel<PropertyInterface>(g,false,_ui->searchTermACombo);
    _ui->searchTermACombo->setModel(termAModel);
    // Term B
    GraphPropertiesModel<PropertyInterface>* termBModel = new GraphPropertiesModel<PropertyInterface>(trUtf8("Custom value"), g,false,_ui->searchTermBCombo);
    _ui->searchTermBCombo->setModel(termBModel);
  }
}

void SearchWidget::graphIndexChanged(int) {
  tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
  setGraph(g);
}
