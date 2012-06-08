#include "SearchWidget.h"

#include "ui_SearchWidget.h"
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/GraphPropertiesModel.h>
#include <tulip/BooleanProperty.h>
#include <QtGui/QStandardItemModel>

using namespace tlp;
using namespace std;

class SearchOperator {
protected:
  tlp::PropertyInterface* _a;
  tlp::PropertyInterface* _b;
public:
  virtual void setProperties(tlp::PropertyInterface* a, tlp::PropertyInterface* b) {
    _a = a;
    _b = b;
  }

  virtual bool compare(tlp::node n)=0;
  virtual bool compare(tlp::edge e)=0;

  tlp::BooleanProperty* run(tlp::Graph* g, bool onNodes, bool onEdges) {
    tlp::BooleanProperty* prop = new BooleanProperty(g);
    if (onNodes) {
      node n;
      forEach(n,g->getNodes())
          prop->setNodeValue(n,compare(n));
    }
    if (onEdges) {
      edge e;
      forEach(e,g->getEdges())
          prop->setEdgeValue(e,compare(e));
    }
    return prop;
  }
};
class StringSearchOperator: public SearchOperator {
public:
  virtual bool compareStrings(const QString& a, const QString& b)=0;
  bool compare(tlp::node n) { return compareStrings(_a->getNodeStringValue(n).c_str(),_b->getNodeStringValue(n).c_str()); }
  bool compare(tlp::edge e) { return compareStrings(_a->getEdgeStringValue(e).c_str(),_b->getEdgeStringValue(e).c_str()); }
};
class NumericSearchOperator: public SearchOperator {
  tlp::DoubleProperty* _numericA;
  tlp::DoubleProperty* _numericB;
public:
  virtual void setProperties(PropertyInterface *a, PropertyInterface *b) {
    SearchOperator::setProperties(a,b);
    _numericA = static_cast<DoubleProperty*>(_a);
    _numericB = static_cast<DoubleProperty*>(_b);
  }
  virtual bool compareDoubles(double a, double b)=0;
  bool compare(tlp::node n) { return compareDoubles(_numericA->getNodeValue(n),_numericB->getNodeValue(n)); }
  bool compare(tlp::edge e) { return compareDoubles(_numericA->getEdgeValue(e),_numericB->getEdgeValue(e)); }
};

#define STRING_CMP(NAME,CMP) class NAME : public StringSearchOperator { \
public:\
  bool compareStrings(const QString &a, const QString &b) { return CMP ; }\
};
STRING_CMP(StringEqualsOperator,a == b)
STRING_CMP(StringDifferentOperator,a != b)
STRING_CMP(StartsWithOperator,a.startsWith(b))
STRING_CMP(EndsWithOperator,a.endsWith(b))
STRING_CMP(ContainsOperator,a.contains(b))

#define NUM_CMP(NAME,CMP) class NAME : public NumericSearchOperator { \
public:\
  bool compareDoubles(double a, double b) { return a CMP b; }\
};
NUM_CMP(DoubleEqualsOperator,==)
NUM_CMP(DoubleDifferentOperator,!=)
NUM_CMP(GreaterOperator,>)
NUM_CMP(GreaterEqualOperator,>=)
NUM_CMP(LesserOperator,<)
NUM_CMP(LesserEqualOperator,<=)

QVector<SearchOperator*> SearchWidget::NUMERIC_OPERATORS = QVector<SearchOperator*>()
    << new DoubleEqualsOperator
    << new DoubleDifferentOperator
    << new GreaterOperator
    << new GreaterEqualOperator
    << new LesserOperator
    << new LesserEqualOperator
    << new StartsWithOperator
    << new EndsWithOperator
    << new ContainsOperator;

QVector<SearchOperator*> SearchWidget::STRING_OPERATORS = QVector<SearchOperator*>()
    << new StringEqualsOperator
    << new StringDifferentOperator
    << NULL
    << NULL
    << NULL
    << NULL
    << new StartsWithOperator
    << new EndsWithOperator
    << new ContainsOperator;

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

void SearchWidget::search() {
  tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
  g->push();
  Observable::holdObservers();
  SearchOperator* op = searchOperator();

  tlp::PropertyInterface* a = term(_ui->searchTermACombo);
  tlp::PropertyInterface* b = NULL;
  bool deleteTermB = false;
  if (_ui->customValueEdit->isVisible()) {
    deleteTermB = true;
    if (isNumericComparison()) {
      DoubleProperty* doubleProp = new DoubleProperty(g);
      doubleProp->setAllNodeValue(_ui->customValueEdit->text().toInt());
      doubleProp->setAllEdgeValue(_ui->customValueEdit->text().toInt());
      b = doubleProp;
    }
    else {
      StringProperty* stringProp = new StringProperty(g);
      stringProp->setAllNodeValue(_ui->customValueEdit->text().toStdString());
      stringProp->setAllEdgeValue(_ui->customValueEdit->text().toStdString());
      b = stringProp;
    }
  }
  else {
    b = term(_ui->searchTermBCombo);
  }

  op->setProperties(a,b);
  int scopeIndex = _ui->scopeCombo->currentIndex();
  bool onNodes = scopeIndex == 0 || scopeIndex == 1;
  bool onEdges = scopeIndex == 0 || scopeIndex == 2;
  BooleanProperty* result = op->run(g, onNodes, onEdges);

  PropertyInterface* outputInterface = _ui->resultsStorageCombo->model()->data(_ui->resultsStorageCombo->model()->index(_ui->resultsStorageCombo->currentIndex(),0),TulipModel::PropertyRole).value<PropertyInterface*>();
  BooleanProperty* output = static_cast<BooleanProperty*>(outputInterface);

  node n;
  edge e;
  if (_ui->selectionModeCombo->currentIndex() == 0) // replace current selection
    *output = *result;
  else if (_ui->selectionModeCombo->currentIndex() == 1) { // add to current selection
    if (onNodes) {
      forEach(n,g->getNodes()) {
        if (result->getNodeValue(n))
          output->setNodeValue(n,true);
      }
    }
    if (onEdges) {
      forEach(e,g->getEdges()) {
        if (result->getEdgeValue(e))
          output->setEdgeValue(e,true);
      }
    }
  }
  else if (_ui->selectionModeCombo->currentIndex() == 2) { // remove from current selection
    if (onNodes) {
      forEach(n,g->getNodes()) {
        if (result->getNodeValue(n))
          output->setNodeValue(n,false);
      }
    }
    if (onEdges) {
      forEach(e,g->getEdges()) {
        if (result->getEdgeValue(e))
          output->setEdgeValue(e,false);
      }
    }
  }

  delete result;
  if (deleteTermB)
    delete b;
  Observable::unholdObservers();
}

void SearchWidget::graphIndexChanged(int) {
  tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
  setGraph(g);
}

void SearchWidget::termAChanged() {
  tlp::PropertyInterface* prop = term(_ui->searchTermACombo);
  updateOperators(prop, term(_ui->searchTermBCombo));
}

void SearchWidget::termBChanged() {
  if (_ui->searchTermBCombo->currentIndex() != 0) {
    _ui->customValueEdit->hide();
    tlp::PropertyInterface* prop = term(_ui->searchTermBCombo);
    updateOperators(term(_ui->searchTermACombo),prop);
  }
  else {
    _ui->customValueEdit->show();
    updateOperators(term(_ui->searchTermACombo),_ui->customValueEdit->text());
  }
}

void SearchWidget::updateOperators(tlp::PropertyInterface *a, tlp::PropertyInterface *b) {
  setNumericOperatorsEnabled(dynamic_cast<tlp::DoubleProperty*>(a) != NULL && dynamic_cast<tlp::DoubleProperty*>(b) != NULL);
}

void SearchWidget::updateOperators(PropertyInterface *a, const QString &b) {
  bool isCustomValueInt = false;
  b.toInt(&isCustomValueInt);
  setNumericOperatorsEnabled(dynamic_cast<tlp::DoubleProperty*>(a) != NULL && isCustomValueInt);
}

void SearchWidget::setNumericOperatorsEnabled(bool e) {
  for(int i=2;i<=5;++i) {
    static_cast<QStandardItemModel*>(_ui->operatorCombo->model())->item(i)->setEnabled(e);
    if (_ui->operatorCombo->currentIndex() == i && !e)
      _ui->operatorCombo->setCurrentIndex(0);
  }
}

PropertyInterface *SearchWidget::term(QComboBox *combo) {
  GraphPropertiesModel<PropertyInterface>* model= static_cast<GraphPropertiesModel<PropertyInterface>*>(combo->model());
  return model->data(model->index(combo->currentIndex(),0),TulipModel::PropertyRole).value<PropertyInterface*>();
}

SearchOperator *SearchWidget::searchOperator() {
  SearchOperator* op = NULL;
  if (isNumericComparison())
    op = NUMERIC_OPERATORS[_ui->operatorCombo->currentIndex()];
  else
    op = STRING_OPERATORS[_ui->operatorCombo->currentIndex()];
  return op;
}

bool SearchWidget::isNumericComparison() {
  return static_cast<QStandardItemModel*>(_ui->operatorCombo->model())->item(2)->isEnabled();
}
