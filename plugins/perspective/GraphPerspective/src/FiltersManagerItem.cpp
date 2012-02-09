#include "FiltersManagerItem.h"

#include "ui_FiltersManagerItem.h"
#include "ui_FiltersManagerAlgorithmItem.h"
#include "ui_FiltersManagerInvertItem.h"
#include "ui_FiltersManagerCompareItem.h"

#include <QtGui/QMenu>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/ParameterListModel.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/Perspective.h>
#include <tulip/TulipItemDelegate.h>
#include "GraphPerspective.h"

using namespace tlp;
using namespace std;

/*
  Helper classes
  */

// Abstract
void AbstractFiltersManagerItem::setGraph(Graph *g) {
  _graph = g;
  graphChanged();
}
void AbstractFiltersManagerItem::graphChanged() {
}

template<typename LISTER>
void updateGraphModel(QTableView* table,const QString& algName, tlp::Graph* g) {
  int h=0;
  if (!algName.isNull()) {
    ParameterList params = LISTER::getPluginParameters(algName.toStdString());
    table->setModel(new ParameterListModel(params,g));
    for (int i=0; i<table->model()->rowCount(); ++i)
      h += table->rowHeight(i);
  }
  else {
    table->setModel(NULL);
  }

  table->setMaximumHeight(h);
  table->setMinimumHeight(h);
}
// Algorithm item

FiltersManagerAlgorithmItem::FiltersManagerAlgorithmItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerAlgorithmItem) {
  _ui->setupUi(this);
  string s;
  forEach(s,BooleanPluginLister::availablePlugins()) {
    _ui->algorithmCombo->addItem(s.c_str());
  }
  connect(_ui->algorithmCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(algorithmSelected(int)));
  _ui->algorithmParams->setItemDelegate(new TulipItemDelegate);
  updateGraphModel<BooleanPluginLister>(_ui->algorithmParams,QString::null,_graph);
}
void FiltersManagerAlgorithmItem::algorithmSelected(int i) {
  _ui->algorithmParams->setEnabled(i != 0);
  QString algName = QString::null;
  if (i != 0)
    algName = _ui->algorithmCombo->itemText(i);
  updateGraphModel<BooleanPluginLister>(_ui->algorithmParams,algName,_graph);
  emit titleChanged();
}

void FiltersManagerAlgorithmItem::applyFilter() {
}

QString FiltersManagerAlgorithmItem::title() const {
  if (_ui->algorithmCombo->currentIndex() == 0)
    return trUtf8("Select filtering method");
  return _ui->algorithmCombo->currentText();
}

void FiltersManagerAlgorithmItem::graphChanged() {
  if (_ui->algorithmCombo->currentIndex() != 0)
    updateGraphModel<BooleanPluginLister>(_ui->algorithmParams,_ui->algorithmCombo->currentText(),_graph);
}

// Comparison item
FiltersManagerCompareItem::FiltersManagerCompareItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerCompareItem) {
  _ui->setupUi(this);
}
void FiltersManagerCompareItem::applyFilter() {
}
QString FiltersManagerCompareItem::title() const {
}

// Invert item
FiltersManagerInvertItem::FiltersManagerInvertItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerInvertItem) {
  _ui->setupUi(this);
  connect(_ui->elementCombo,SIGNAL(currentIndexChanged(int)),this,SIGNAL(titleChanged()));
}
void FiltersManagerInvertItem::applyFilter() {
}
QString FiltersManagerInvertItem::title() const {
  return trUtf8("Invert selection ") + _ui->elementCombo->currentText().toLower();
}
// ===========

FiltersManagerItem::FiltersManagerItem(QWidget *parent): QFrame(parent), _ui(new Ui::FiltersManagerItem) {
  connect(Perspective::typedInstance<GraphPerspective>()->model(),SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(graphChanged(tlp::Graph*)));

  _ui->setupUi(this);
  connect(_ui->delButton,SIGNAL(clicked()),this,SIGNAL(removed()));
  setMode(Invalid);
}

FiltersManagerItem::~FiltersManagerItem() {
  delete _ui;
}

void FiltersManagerItem::setCompareMode() {
  setMode(Compare);
}

void FiltersManagerItem::setAlgorithmMode() {
  setMode(Algorithm);
}

void FiltersManagerItem::setInvertMode() {
  setMode(Invert);
}

void FiltersManagerItem::setMode(Mode m) {
  _ui->delButton->setVisible(m != Invalid);
  _ui->addButton->setVisible(m == Invalid);
  _ui->dataBox->setVisible(m != Invalid);

  AbstractFiltersManagerItem* w = NULL;

  if (m == Compare)
    w = new FiltersManagerCompareItem;
  else if (m == Invert)
    w = new FiltersManagerInvertItem;
  else if (m == Algorithm)
    w = new FiltersManagerAlgorithmItem;

  _ui->dataBox->setWidget(w);
  if (w != NULL) {
    connect(w,SIGNAL(titleChanged()),this,SLOT(dataBoxTitleChanged()));
    _ui->dataBox->setTitle(w->title());

    w->setGraph(Perspective::typedInstance<GraphPerspective>()->model()->currentGraph());
  }

  emit modeChanged(m);
}

void FiltersManagerItem::addButtonClicked() {
  QMenu addMenu;
  addMenu.addAction(trUtf8("Invert selection"),this,SLOT(setInvertMode()));
  addMenu.addAction(trUtf8("Compare values"),this,SLOT(setCompareMode()));
  addMenu.addAction(trUtf8("Filtering algorithm"),this,SLOT(setAlgorithmMode()));
  addMenu.exec(QCursor().pos());
}

void FiltersManagerItem::dataBoxTitleChanged() {
  _ui->dataBox->setTitle(static_cast<AbstractFiltersManagerItem*>(sender())->title());
}

void FiltersManagerItem::graphChanged(Graph *g) {
  if (_ui->dataBox->widget() != NULL)
    static_cast<AbstractFiltersManagerItem*>(_ui->dataBox->widget())->setGraph(g);
}
