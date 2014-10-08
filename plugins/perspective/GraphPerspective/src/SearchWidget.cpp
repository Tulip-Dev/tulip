/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include "SearchWidget.h"
#include "ui_SearchWidget.h"

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/GraphPropertiesModel.h>
#include <tulip/BooleanProperty.h>
#include <tulip/GraphTableItemDelegate.h>
#include <tulip/GraphModel.h>
#include <tulip/DoubleProperty.h>
#include <tulip/TulipMimes.h>
#include <tulip/StringProperty.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TlpQtTools.h>

#include <QDebug>
#include <QStandardItemModel>
#include <QDragEnterEvent>
#include <QRegExp>

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
  bool compare(tlp::node n) {
    return compareStrings(_a->getNodeStringValue(n).c_str(),_b->getNodeStringValue(n).c_str());
  }
  bool compare(tlp::edge e) {
    return compareStrings(_a->getEdgeStringValue(e).c_str(),_b->getEdgeStringValue(e).c_str());
  }
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
  bool compare(tlp::node n) {
    return compareDoubles(_numericA->getNodeValue(n),_numericB->getNodeValue(n));
  }
  bool compare(tlp::edge e) {
    return compareDoubles(_numericA->getEdgeValue(e),_numericB->getEdgeValue(e));
  }
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
STRING_CMP(NoCaseStringEqualsOperator, a.compare(b, Qt::CaseInsensitive) == 0)
STRING_CMP(NoCaseStringDifferentOperator, a.compare(b, Qt::CaseInsensitive) != 0)
STRING_CMP(NoCaseStartsWithOperator,a.startsWith(b, Qt::CaseInsensitive))
STRING_CMP(NoCaseEndsWithOperator,a.endsWith(b, Qt::CaseInsensitive))
STRING_CMP(NoCaseContainsOperator,a.contains(b, Qt::CaseInsensitive))

class MatchesOperator: public StringSearchOperator {
public:
  bool compareStrings(const QString &a, const QString &b) {
    QRegExp regexp(b);
    return regexp.exactMatch(a);
  }
};

class NoCaseMatchesOperator: public StringSearchOperator {
public:
  bool compareStrings(const QString &a, const QString &b) {
    QRegExp regexp(b, Qt::CaseInsensitive);
    return regexp.exactMatch(a);
  }
};

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
    << new ContainsOperator
    << new MatchesOperator;

QVector<SearchOperator*> SearchWidget::STRING_OPERATORS = QVector<SearchOperator*>()
    << new StringEqualsOperator
    << new StringDifferentOperator
    << NULL
    << NULL
    << NULL
    << NULL
    << new StartsWithOperator
    << new EndsWithOperator
    << new ContainsOperator
    << new MatchesOperator;

QVector<SearchOperator*> SearchWidget::NOCASE_STRING_OPERATORS = QVector<SearchOperator*>()
    << new NoCaseStringEqualsOperator
    << new NoCaseStringDifferentOperator
    << NULL
    << NULL
    << NULL
    << NULL
    << new NoCaseStartsWithOperator
    << new NoCaseEndsWithOperator
    << new NoCaseContainsOperator
    << new NoCaseMatchesOperator;

SearchWidget::SearchWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SearchWidget) {
  _ui->setupUi(this);
  _ui->tableWidget->hide();
  _ui->tableWidget->setItemDelegate(new TulipItemDelegate(_ui->tableWidget));

  _ui->resultsStorageCombo->setModel(new GraphPropertiesModel<BooleanProperty>(NULL,false,_ui->resultsStorageCombo));
  _ui->searchTermACombo->setModel(new GraphPropertiesModel<PropertyInterface>(NULL,false,_ui->searchTermACombo));
  _ui->searchTermBCombo->setModel(new GraphPropertiesModel<PropertyInterface>(trUtf8("Custom value"),NULL,false,_ui->searchTermBCombo));
  connect(_ui->graphCombo,SIGNAL(currentItemChanged()),this,SLOT(graphIndexChanged()));
  connect(_ui->selectionModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionModeChanged(int)));
}

SearchWidget::~SearchWidget() {
  delete _ui;
}

void SearchWidget::setModel(tlp::GraphHierarchiesModel *model) {
  _ui->graphCombo->setModel(model);
  setGraph(model->currentGraph());
}

void SearchWidget::currentGraphChanged(tlp::Graph *g) {
  GraphHierarchiesModel* graphsModel = static_cast<GraphHierarchiesModel*>(_ui->graphCombo->model());
  QModelIndex idx = graphsModel->indexOf(g);
  _ui->graphCombo->setRootModelIndex(idx.parent());
  _ui->graphCombo->setCurrentIndex(idx.row());
}

void searchForIndex(QComboBox* combo, const QString& s) {
  combo->setCurrentIndex(0);

  if (!s.isNull()) {
    QAbstractItemModel* model = combo->model();

    for(int i = 0; i < model->rowCount(); ++i) {
      if (model->index(i,0).data().toString() == s) {
        combo->setCurrentIndex(i);
        break;
      }
    }
  }
}

void SearchWidget::setGraph(Graph *g) {
  if (g != NULL) {
    // Force creation of viewSelection to ensure we have at least one boolean property exising in the graph
    g->getProperty<BooleanProperty>("viewSelection");
  }

  else {
    _ui->resultsCountLabel->setText("");
  }

  QString oldStorageName = QString::null;
  QString oldTermAName = QString::null;
  QString oldTermBName = QString::null;

  if (_ui->resultsStorageCombo->model() != NULL) {
    oldStorageName = _ui->resultsStorageCombo->currentText();
  }

  if (_ui->searchTermACombo->model() != NULL) {
    oldTermAName = _ui->searchTermACombo->currentText();
  }

  if (_ui->searchTermBCombo->model() != NULL) {
    oldTermBName = _ui->searchTermBCombo->currentText();
  }

  _ui->resultsStorageCombo->setModel(new GraphPropertiesModel<BooleanProperty>(g,false,_ui->resultsStorageCombo));
  _ui->searchTermACombo->setModel(new GraphPropertiesModel<PropertyInterface>(g,false,_ui->searchTermACombo));
  _ui->searchTermBCombo->setModel(new GraphPropertiesModel<PropertyInterface>(trUtf8("Custom value"),g,false,_ui->searchTermBCombo));

  if (!oldStorageName.isNull())
    searchForIndex(_ui->resultsStorageCombo,oldStorageName);
  else
    searchForIndex(_ui->resultsStorageCombo,"viewSelection");

  if (!oldTermAName.isNull())
    searchForIndex(_ui->searchTermACombo,oldTermAName);
  else
    searchForIndex(_ui->searchTermACombo,"viewMetric");

  if (!oldTermBName.isNull())
    searchForIndex(_ui->searchTermBCombo,oldTermBName);
  else
    searchForIndex(_ui->searchTermBCombo,trUtf8("Custom value"));
}

void SearchWidget::selectionModeChanged(int index) {
  _ui->resultsStorageCombo->setEnabled((index==3)?false:true);
}

void SearchWidget::search() {
  tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
  g->push();
  Observable::holdObservers();
  SearchOperator* op = searchOperator();

  tlp::PropertyInterface* a = term(_ui->searchTermACombo);
  tlp::PropertyInterface* b = NULL;
  bool deleteTermB = false;

  if (_ui->tableWidget->isVisible()) {
    deleteTermB = true;

    if (isNumericComparison()) {
      DoubleProperty* doubleProp = new DoubleProperty(g);
      doubleProp->setAllNodeValue(_ui->tableWidget->item(0, 0)->data(Qt::DisplayRole).toInt());
      doubleProp->setAllEdgeValue(_ui->tableWidget->item(0, 0)->data(Qt::DisplayRole).toInt());
      b = doubleProp;
    }
    else {
      StringProperty* stringProp = new StringProperty(g);
      DataType* tulipData = TulipMetaTypes::qVariantToDataType(_ui->tableWidget->item(0, 0)->data(Qt::DisplayRole));

      if(tulipData == NULL) {
        qCritical() << "could not convert this type correctly " << _ui->tableWidget->item(0, 0)->data(Qt::DisplayRole) << ", please report this as a bug";
#ifdef NDEBUG
        return;
#endif
      }

      DataTypeSerializer* serializer = DataSet::typenameToSerializer(tulipData->getTypeName());

      if(serializer == NULL) {
        qCritical() << QString::fromUtf8("no type serializer found for ") << tlp::tlpStringToQString(tulipData->getTypeName()) << ", please report this as a bug";
#ifdef NDEBUG
        return;
#endif
      }

      stringstream temp;
      serializer->writeData(temp, tulipData);
      QString serializedValue = temp.str().c_str();

      //Tulip serializers add quotes around the serialized value, remove them for comparison
      if(serializedValue.startsWith('"') && serializedValue.endsWith('"')) {
        serializedValue = serializedValue.mid(1, serializedValue.length()-2);
      }

      stringProp->setAllNodeValue(serializedValue.toStdString());
      stringProp->setAllEdgeValue(serializedValue.toStdString());
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

  QString searchOpDescription;

  if (_ui->selectionModeCombo->currentIndex() == 0) {// replace current selection
    output->copy(result);
    searchOpDescription = "found";
  }
  else if (_ui->selectionModeCombo->currentIndex() == 1) {   // add to current selection
    if (onNodes) {
      forEach(n,result->getNodesEqualTo(true)) {
        output->setNodeValue(n,true);
      }
    }

    if (onEdges) {
      forEach(e,result->getEdgesEqualTo(true)) {
        output->setEdgeValue(e,true);
      }
    }

    searchOpDescription = "added to selection";
  }
  else if (_ui->selectionModeCombo->currentIndex() == 2) { // remove from current selection
    if (onNodes) {
      forEach(n,result->getNodesEqualTo(true)) {
        output->setNodeValue(n,false);
      }
    }

    if (onEdges) {
      forEach(e,result->getEdgesEqualTo(true)) {
        output->setEdgeValue(e,false);
      }
    }

    searchOpDescription = "removed from selection";
  }
  else if (_ui->selectionModeCombo->currentIndex() == 3) { // no modification
    output = result;
    searchOpDescription = "found but not added to selection";
  }

  if (deleteTermB)
    delete b;

  unsigned int resultsCountNodes = 0, resultsCountEdges=0;
  forEach(n, result->getNodesEqualTo(true)) {
    resultsCountNodes++;
  }
  forEach(e, result->getEdgesEqualTo(true)) {
    resultsCountEdges++;
  }
  delete result;

  if(onNodes&&!onEdges)
    _ui->resultsCountLabel->setText(QString::number(resultsCountNodes) + " node(s) " + searchOpDescription);
  else if(!onNodes&&onEdges)
    _ui->resultsCountLabel->setText(QString::number(resultsCountEdges) +" edge(s) " + searchOpDescription);
  else
    _ui->resultsCountLabel->setText(QString::number(resultsCountNodes) + " node(s) and " + QString::number(resultsCountEdges) +" edge(s) " + searchOpDescription);

  Observable::unholdObservers();
}

void SearchWidget::graphIndexChanged() {
  tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
  setGraph(g);
}

void SearchWidget::termAChanged() {
  tlp::PropertyInterface* prop = term(_ui->searchTermACombo);

  //isHidden checks if the widget was set hidden; using isVisible would check if the widget is currently displayed
  if (!_ui->tableWidget->isHidden()) {
    updateEditorWidget();
    updateOperators(prop, _ui->tableWidget->item(0, 0)->data(Qt::DisplayRole).toString());
  }
  else {
    updateOperators(prop, term(_ui->searchTermBCombo));
  }
}

void SearchWidget::termBChanged() {
  if (_ui->searchTermBCombo->currentIndex() != 0) {
    _ui->tableWidget->hide();
    tlp::PropertyInterface* prop = term(_ui->searchTermBCombo);
    updateOperators(term(_ui->searchTermACombo),prop);
  }
  else {
    _ui->tableWidget->show();
    updateEditorWidget();
    updateOperators(term(_ui->searchTermACombo), _ui->tableWidget->item(0, 0)->data(Qt::DisplayRole).toString());
  }
}

void SearchWidget::updateOperators(tlp::PropertyInterface *a, tlp::PropertyInterface *b) {
  setNumericOperatorsEnabled(dynamic_cast<tlp::DoubleProperty*>(a) != NULL && dynamic_cast<tlp::DoubleProperty*>(b) != NULL);
}

void SearchWidget::updateOperators(PropertyInterface *a, const QString &b) {
  bool isCustomValueDouble = false;

  if (b.isEmpty())
    isCustomValueDouble = true;
  else
    b.toDouble(&isCustomValueDouble);

  setNumericOperatorsEnabled(dynamic_cast<tlp::DoubleProperty*>(a) != NULL && isCustomValueDouble);
}

void SearchWidget::setNumericOperatorsEnabled(bool e) {
  for(int i=2; i<=5; ++i) {
    static_cast<QStandardItemModel*>(_ui->operatorCombo->model())->item(i)->setEnabled(e);

    if (_ui->operatorCombo->currentIndex() == i && !e)
      _ui->operatorCombo->setCurrentIndex(0);
  }
}

void SearchWidget::updateEditorWidget() {
  QVariant defaultValue;
  tlp::PropertyInterface* prop = term(_ui->searchTermACombo);
  int scopeIndex = _ui->scopeCombo->currentIndex();

  if (scopeIndex == 1 || scopeIndex == 0)
    defaultValue = GraphModel::nodeDefaultValue(prop);
  else
    defaultValue = GraphModel::edgeDefaultValue(prop);

  // workaround for sf bug #716
  // I suspect that because 0 == false (when updating termA from viewMetric
  // from viewSelection) the table item was not properly refreshed.
  // So force the refresh of the table item with defaultValue
  // in first displaying a 'blank' value
  _ui->tableWidget->item(0, 0)->setData(Qt::DisplayRole, QVariant(QString()));
  _ui->tableWidget->item(0, 0)->setData(Qt::DisplayRole, defaultValue);
  _ui->tableWidget->item(0, 0)->setData(Qt::EditRole, defaultValue);
}

void SearchWidget::dragEnterEvent(QDragEnterEvent *dragEv) {
  const GraphMimeType* mimeType = dynamic_cast<const GraphMimeType*>(dragEv->mimeData());

  if (mimeType != NULL) {
    dragEv->accept();
  }
}

void SearchWidget::dropEvent(QDropEvent* dropEv) {
  const GraphMimeType* mimeType = dynamic_cast<const GraphMimeType*>(dropEv->mimeData());

  if (mimeType != NULL) {
    currentGraphChanged(mimeType->graph());
    dropEv->accept();
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
    op = _ui->caseSensitivityCheck->isChecked() ? STRING_OPERATORS[_ui->operatorCombo->currentIndex()] : NOCASE_STRING_OPERATORS[_ui->operatorCombo->currentIndex()];

  return op;
}

bool SearchWidget::isNumericComparison() {
  return static_cast<QStandardItemModel*>(_ui->operatorCombo->model())->item(2)->isEnabled();
}
