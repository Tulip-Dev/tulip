#include "TulipFilterProxyModel.h"
#include "GraphTableModel.h"

#include <tulip/BooleanProperty.h>
#include <tulip/Graph.h>
using namespace tlp;
using namespace std;
TulipFilterProxyModel::TulipFilterProxyModel(QObject* parent):QSortFilterProxyModel(parent),_tableModel(NULL),_showOnlyVisibleElements(false),_selectionProperty(NULL),_reloadSelectionProperty(false),_needToReload(false) {
}

void TulipFilterProxyModel::sort(int column, Qt::SortOrder order) {
  //Disable sorting
  if(_tableModel) {
    _tableModel->sort(column,order);
  }
}

void TulipFilterProxyModel::setSourceModel ( QAbstractItemModel * sourceModel ) {
  QSortFilterProxyModel::setSourceModel(sourceModel);
}

void TulipFilterProxyModel::setGraphTableModel(GraphTableModel* tableModel) {
  _tableModel = tableModel;
  setSourceModel(tableModel);
}

void TulipFilterProxyModel::setShowOnlySelectedElement(bool showOnlySelectedElements) {
  if(_showOnlyVisibleElements != showOnlySelectedElements) {
    //Stop listening viewSelection
    if(_showOnlyVisibleElements) {
      _selectionProperty->removeListener(this);
      _tableModel->graph()->removeListener(this);
    }
    else {
      //Begin listening view selection property
      if(_tableModel->graph()->existProperty("viewSelection")) {
        _selectionProperty = _tableModel->graph()->getProperty<BooleanProperty>("viewSelection");
        _selectionProperty->addListener(const_cast<TulipFilterProxyModel*>(this));
      }

      _tableModel->graph()->addListener(const_cast<TulipFilterProxyModel*>(this));
    }

    _showOnlyVisibleElements = showOnlySelectedElements;
  }

}

bool TulipFilterProxyModel::filterAcceptsRow ( int source_row, const QModelIndex &) const {
  //Avoid another invalidate filter when calling update
  _needToReload = false;

  //Need to reload selection property
  if (_reloadSelectionProperty) {
    assert(_selectionProperty == NULL);

    if(_tableModel->graph()->existProperty("viewSelection")) {
      _selectionProperty = _tableModel->graph()->getProperty<BooleanProperty>("viewSelection");
      _selectionProperty->addListener(const_cast<TulipFilterProxyModel*>(this));
      _tableModel->graph()->addListener(const_cast<TulipFilterProxyModel*>(this));
    }

    _reloadSelectionProperty = false;
  }

  unsigned int id =_tableModel->idForIndex(source_row);
  bool display = true;

  if(_showOnlyVisibleElements && _selectionProperty!=NULL) {
    display =  _tableModel->elementType() == NODE ? _selectionProperty->getNodeValue(node(id)):_selectionProperty->getEdgeValue(edge(id));
  }

  bool match = false;
  QRegExp regExp= filterRegExp();

  if(regExp.isEmpty() ) {
    match = true;
  }
  else {
    //Search in a specific column
    if(filterKeyColumn()!= -1) {
      match |= regExp.exactMatch(_tableModel->data(_tableModel->index(source_row,filterKeyColumn())).toString());
    }
    else {
      //Search in all columns
      bool foundPattern = false;

      for(int j =0 ; j < _tableModel->columnCount(); ++j) {
        //Filter only visible columns.
        //                        if(!ui->tableView->isColumnHidden(j)){
        foundPattern = regExp.exactMatch(_tableModel->data(_tableModel->index(source_row,j)).toString());

        //If we find the pattern stop the search
        if(foundPattern)
          break;

        //                        }
      }

      match |= foundPattern;
    }
  }

  return (display && match);
}

bool TulipFilterProxyModel::filterAcceptsColumn ( int, const QModelIndex &) const {
  return true;
}

void TulipFilterProxyModel::treatEvent(const Event &ev) {
  const PropertyEvent* propEvt = dynamic_cast<const PropertyEvent*>(&ev);

  if (propEvt) {
    switch(propEvt->getType()) {
    case PropertyEvent::TLP_AFTER_SET_NODE_VALUE:

      if(_tableModel->elementType() == NODE) {
        _needToReload = true;
      }

      break;

    case PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE:

      if(_tableModel->elementType() == NODE) {
        _needToReload = true;
      }

      break;

    case PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE:

      if(_tableModel->elementType() == EDGE) {
        _needToReload = true;
      }

      break;

    case PropertyEvent::TLP_AFTER_SET_EDGE_VALUE:

      if(_tableModel->elementType() == EDGE) {
        _needToReload = true;
      }

      break;

    default:
      break;
    }
  }
  else {
    const GraphEvent* graphEvt = dynamic_cast<const GraphEvent*>(&ev);

    if(graphEvt) {
      switch(graphEvt->getType()) {
      case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
      case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
      case GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY:
      case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY: {
        string propertyName = graphEvt->getPropertyName();

        //A new selection property was created or the current one will be destructed.
        if(propertyName.compare("viewSelection")==0) {
          _tableModel->graph()->removeListener(this);

          if(_selectionProperty != NULL) {
            _selectionProperty->removeListener(this);
            _selectionProperty = NULL;
          }

          _reloadSelectionProperty = true;
          _needToReload = true;
        }
      }
      break;

      default:
        break;
      }
    }
  }
}

void TulipFilterProxyModel::update() {
  if(_needToReload) {
    invalidateFilter();
    _needToReload = false;
  }
}
