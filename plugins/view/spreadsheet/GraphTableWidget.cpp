#include "GraphTableWidget.h"
#include <tulip/PropertyCreationDialog.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/BooleanProperty.h>

#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QKeyEvent>

#include "GraphTableModel.h"
#include "TulipItemDelegate.h"
#include "TulipFilterProxyModel.h"

using namespace tlp;
using namespace std;
GraphTableWidget::GraphTableWidget(QWidget* parent):QTableView(parent),_graph(NULL),_type(NODE),_tulipTableModel(NULL),_filterModel(NULL) {
  horizontalHeader()->setMovable(true);
  setItemDelegate(new TulipItemDelegate(this));
}

void GraphTableWidget::setGraph(Graph* graph,ElementType element) {
  _graph = graph;
  _type = element;
  GraphTableModel* oldModel = _tulipTableModel;

  _tulipTableModel = new GraphTableModel(graph,element,this);

  if(_filterModel) {
    _filterModel->setGraphTableModel(_tulipTableModel);
    QTableView::setModel(_filterModel);
  }
  else {
    QTableView::setModel(_tulipTableModel);
  }

  if(oldModel) {
    oldModel->deleteLater();
  }
}


GraphTableWidget::SelectionStatus GraphTableWidget::selectionStatus(const QModelIndexList& elementsIndexes)const {
  BooleanProperty* selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
  set<unsigned int> elements = indexListToIds(elementsIndexes);
  bool allSelected = true;
  bool allUnselected = true;

  for(set<unsigned int>::iterator it = elements.begin(); it != elements.end(); ++it) {
    if(_type == NODE) {
      if(selectionProperty->getNodeValue(node(*it))) {
        allUnselected = false;
      }
      else {
        allSelected = false;
      }
    }
    else {
      if(selectionProperty->getEdgeValue(edge(*it))) {
        allUnselected = false;
      }
      else {
        allSelected = false;

      }
    }
  }

  return allSelected?Selected:(allUnselected?Unselected:PartiallySelected);
}

set<unsigned int> GraphTableWidget::indexListToIds(const QModelIndexList& elementsIndexes)const {
  set<unsigned int> elements;

  for(QModelIndexList::const_iterator it = elementsIndexes.begin(); it != elementsIndexes.end(); ++it) {
    elements.insert(_tulipTableModel->idForIndex(*it));
  }

  return elements;
}


void GraphTableWidget::highlightElements(const std::set<unsigned int>& elementsToHighligh) {
  QItemSelectionModel *itemSelectionModel = new QItemSelectionModel(_tulipTableModel);

  for(int i = 0 ; i < _tulipTableModel->rowCount() ; ++i) {
    if(elementsToHighligh.find(_tulipTableModel->idForIndex(i))!=elementsToHighligh.end()) {
      itemSelectionModel->select(_tulipTableModel->index(i,0),QItemSelectionModel::Select| QItemSelectionModel::Rows);
    }
  }

  QItemSelectionModel *oldSelectionModel = selectionModel();
  setSelectionModel(itemSelectionModel);
  oldSelectionModel->deleteLater();
}

void GraphTableWidget::scrollToFirstOf(const QModelIndexList& indexes) {
  int xMin=INT_MAX;
  int yMin=INT_MAX;
  int rowMin = -1;
  int colMin = -1;

  for(QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); ++it) {
    QRect indexRect = visualRect(*it);

    if(indexRect.left() < xMin) {
      xMin = indexRect.left();
      rowMin =( *it).row();
    }

    if(indexRect.top() < yMin) {
      yMin = indexRect.top();
      colMin = (*it).column();
    }
  }

  if(rowMin != -1 && colMin != -1) {
    QTableView::scrollTo(_tulipTableModel->index(rowMin,colMin),QAbstractItemView::PositionAtTop);
  }
}

void GraphTableWidget::highlightAndDisplayElements(const std::set<unsigned int>& elements) {
  highlightElements(elements);
  scrollToFirstOf(selectionModel()->selectedIndexes());
}
void GraphTableWidget::highlightAllElements() {
  selectionModel()->select(QItemSelection(model()->index(0,0),model()->index(model()->rowCount()-1,model()->columnCount()-1)),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

QModelIndexList GraphTableWidget::selectedRows(int columns)const {
  QModelIndexList modelSelected = selectionModel()->selectedRows(columns);
  QModelIndexList viewSelected;

  for (int i = 0; i < modelSelected.count(); ++i) {
    QModelIndex index = modelSelected.at(i);
    viewSelected.append(_filterModel == NULL?index:_filterModel->mapToSource(index));
  }

  return viewSelected;
}

void GraphTableWidget::setRowFilter(QRegExp regExp,bool showOnlySelectedElements,int columnFilter) {
  //Unset the filter
  if(regExp.isEmpty() && !showOnlySelectedElements) {
    unsetRowFilter();
  }
  else {
    if(_filterModel == NULL) {
      _filterModel = new TulipFilterProxyModel(this);
      _filterModel->setDynamicSortFilter(true);
      _filterModel->setGraphTableModel(_tulipTableModel);
      _filterModel->setShowOnlySelectedElement(showOnlySelectedElements);
      _filterModel->setFilterKeyColumn(columnFilter);
      _filterModel->setFilterRegExp(regExp);
      QTableView::setModel(_filterModel);
    }
    else {
      _filterModel->setShowOnlySelectedElement(showOnlySelectedElements);
      _filterModel->setFilterKeyColumn(columnFilter);
      _filterModel->setFilterRegExp(regExp);
    }
  }
}

void GraphTableWidget::unsetRowFilter() {
  if(_filterModel != NULL) {
    QTableView::setModel(_tulipTableModel);
    _filterModel->deleteLater();
    _filterModel = NULL;
  }
}

void GraphTableWidget::update() {
  _tulipTableModel->update();

  if(_filterModel)
    _filterModel->update();
}
