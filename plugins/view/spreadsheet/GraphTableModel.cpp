#include "GraphTableModel.h"

#include <tulip/Graph.h>
#include <tulip/TlpQtTools.h>
#include "TulipQVariantBuilder.h"


using namespace tlp;
using namespace std;


/**
  * @brief Sort elements in function of the given property values.
  **/
struct PropertyValueComparator {
public:
  PropertyValueComparator(Qt::SortOrder sortOrder,ElementType type,PropertyInterface* property):_sortOrder(sortOrder),_elementType(type),_property(property) {}
  PropertyValueComparator(const PropertyValueComparator& other):_sortOrder(other._sortOrder),_elementType(other._elementType),_property(other._property) {}
  bool operator()(unsigned int elt1,unsigned int elt2) {
    if(_elementType == NODE) {
      if(_sortOrder == Qt::AscendingOrder) {
        return _property->compare(node(elt1),node(elt2)) <= -1 ;
      }
      else {
        return _property->compare(node(elt1),node(elt2)) >= 1 ;
      }
    }
    else {
      if(_sortOrder == Qt::AscendingOrder) {
        return _property->compare(edge(elt1),edge(elt2)) <= -1 ;
      }
      else {
        return _property->compare(edge(elt1),edge(elt2)) >= 1 ;
      }
    }
  }
private:
  Qt::SortOrder _sortOrder;
  ElementType _elementType;
  PropertyInterface* _property;
};

/**
  * @brief Sort properties by name.
  **/
struct PropertyComparator {
public:
  bool operator()(PropertyInterface* p1,PropertyInterface* p2) {
    return p1->getName().compare(p2->getName())<0;
  }
};


GraphTableModelIndex::GraphTableModelIndex(unsigned int element,PropertyInterface* property):_element(element),_property(property) {

}

GraphTableModel::GraphTableModel(Graph* graph,ElementType displayType,QObject* parent ):QAbstractTableModel(parent),_graph(NULL),_elementType(displayType),_orientation(Qt::Vertical),_sortingProperty(NULL),_order(Qt::AscendingOrder) {
  setGraph(graph);
}

void GraphTableModel::setGraph(Graph* newGraph) {
  if(_graph != NULL) {
    _graph->removeObserver(this);
    _graph->removeGraphObserver(this);
  }

  _graph = newGraph;

  if(_graph != NULL) {
    _graph->addObserver(this);
    _graph->addGraphObserver(this);
  }

  updateElementsTable();
  updatePropertyTable();
  reset();
}

void GraphTableModel::setElementType(ElementType type) {
  _elementType = type;
  updateElementsTable();
  reset();
}

void GraphTableModel::setOrientation(Qt::Orientation orientation) {
  _orientation = orientation;
  reset();
}
unsigned int GraphTableModel::idForIndex(int index,const QModelIndex&) const {
  if(index > -1 && (unsigned int)index < _idTable.size()) {
    return _idTable[index];
  }
  else {
    return UINT_MAX;
  }
}

QList<int> GraphTableModel::indexesForIds(const set<unsigned int>& ids)const {
  QList<int> indexes;

  for(int i = 0 ; (unsigned int)i< _idTable.size(); ++i) {
    if(ids.find(idForIndex(i))!=ids.end()) {
      indexes.push_back(i);
    }
  }

  return indexes;
}

PropertyInterface* GraphTableModel::propertyForIndex(int index,const QModelIndex&) const {
  if(index > -1 && (unsigned int)index < _propertiesTable.size()) {
    //If the property is going to be deleted.
    if(_propertiesToDelete.find(_propertiesTable[index]) == _propertiesToDelete.end()) {
      return _propertiesTable[index];
    }
    else {
      return NULL;
    }
  }
  else {
    return NULL;
  }
}

QList<int> GraphTableModel::indexesForProperties(const std::set<tlp::PropertyInterface*>& properties)const {
  QList<int> indexes;

  for(int i = 0 ; (unsigned int)i< _propertiesTable.size(); ++i) {
    if(properties.find(propertyForIndex(i))!=properties.end()) {
      indexes.push_back(i);
    }
  }

  return indexes;
}

pair<QModelIndex,QModelIndex> GraphTableModel::computeElementsArea(const set<unsigned int>& elementsIds)const {
  pair<unsigned int,unsigned int> fromTo = computeArea<unsigned int>(elementsIds,_idTable,_idToIndex);

  if(_orientation == Qt::Vertical) {
    return make_pair(index(0,fromTo.first),index(columnCount()-1,fromTo.second));
  }
  else {
    return make_pair(index(fromTo.first,0),index(rowCount()-1,fromTo.second));
  }
}


pair<QModelIndex,QModelIndex> GraphTableModel::computePropertiesArea(const set<tlp::PropertyInterface*>& properties)const {
  pair<unsigned int,unsigned int> fromTo = computeArea<PropertyInterface*>(properties,_propertiesTable,_propertyToIndex);

  if(_orientation == Qt::Vertical) {
    return make_pair(index(fromTo.first,0),index(rowCount()-1,fromTo.second));
  }
  else {
    return make_pair(index(0,fromTo.first),index(columnCount()-1,fromTo.second));
  }
}

void GraphTableModel::updateElementsTable() {
  _idTable.clear();

  if(_graph != NULL) {
    if(_elementType == NODE) {
      node n;
      forEach(n,_graph->getNodes()) {
        _idTable.push_back(n.id);
      }
    }
    else {
      edge e;
      forEach(e,_graph->getEdges()) {
        _idTable.push_back(e.id);
      }
    }
  }

  updateIndexMap<unsigned int>(_idTable,_idToIndex);
}

void GraphTableModel::updatePropertyTable() {
  _propertiesTable.clear();

  if(_graph != NULL) {
    PropertyInterface* property;
    forEach(property,_graph->getObjectProperties()) {
      if(useProperty(property)) {
        property->removePropertyObserver(this);
        property->removeObserver(this);
        _propertiesTable.push_back(property);
        property->addPropertyObserver(this);
        property->addObserver(this);
      }
    }
    PropertyComparator comparator;
    std::stable_sort(_propertiesTable.begin(),_propertiesTable.end(),comparator);
  }

  updateIndexMap<PropertyInterface*>(_propertiesTable,_propertyToIndex);
}

bool GraphTableModel::useProperty(PropertyInterface* property) const {
  TulipQVariantBuilder qvariantBuilder;

  switch(qvariantBuilder.getPropertyType(_elementType,property)) {
  case INVALID_PROPERTY_RTTI:
    return false;
    break;

  default:
    return true;
  }
}

int GraphTableModel::columnCount(const QModelIndex& ) const {
  return _orientation == Qt::Vertical?_propertiesTable.size():_idTable.size();
}
int GraphTableModel::rowCount(const QModelIndex& ) const {
  return _orientation == Qt::Vertical?_idTable.size():_propertiesTable.size();
}

QVariant GraphTableModel::data(const QModelIndex& index, int role ) const {
  switch(role) {
  default:
    GraphTableModelIndex tableIndex = element(index);

    //Check if the cell is valid : the node and and the property must be defined
    //If the property is going to be deleted don't display data
    if(tableIndex.isValid() && _propertiesToDelete.find(tableIndex.property())==_propertiesToDelete.end()) {
      TulipQVariantBuilder helper;
      return helper.data(_graph,role,_elementType,tableIndex.element(),tableIndex.property());
    }
  }

  return QVariant();
}

bool GraphTableModel::setAllElementsData(unsigned int propertyIndex,const QVariant & value, int role) {
  switch(role) {
  case Qt::EditRole:
    TulipQVariantBuilder helper;
    PropertyInterface* property = propertyForIndex(propertyIndex);
    return helper.setAllElementData(value,_elementType,property);
    break;
  }

  return false;
}

bool GraphTableModel::setData( const QModelIndex & index, const QVariant & value, int role ) {
  switch(role) {
  case Qt::EditRole:
    GraphTableModelIndex tableIndex = element(index);

    if(tableIndex.isValid()) {
      TulipQVariantBuilder helper;
      return helper.setData(value,_elementType,tableIndex.element(),tableIndex.property());
    }

    break;
  }

  return false;
}

QVariant GraphTableModel::headerData(int section, Qt::Orientation orientation, int role ) const {
  switch(role) {
  case Qt::DisplayRole:

    if(orientation == Qt::Vertical) {
      return QVariant(QString::number(_idTable[section]));
    }
    else {
      PropertyInterface* property = _propertiesTable[section];

      if(_propertiesToDelete.find(property)==_propertiesToDelete.end()) {
        QString name = QString::fromUtf8(property->getName().c_str());
        name.append("\n");
        name.append("( ");
        name.append(propertyInterfaceToPropertyTypeLabel(_propertiesTable[section]));
        name.append(" )");
        return QVariant(name);
      }
    }

    break;

  case Qt::ToolTipRole:

    if(orientation == Qt::Horizontal) {
      PropertyInterface* property = _propertiesTable[section];

      if(_propertiesToDelete.find(property)==_propertiesToDelete.end()) {
        QString toolTip;
        toolTip.append("Property : ");
        toolTip.append(tlpStringToQString(property->getName()));
        toolTip.append("\n");
        toolTip.append("Type : ");
        toolTip.append(propertyInterfaceToPropertyTypeLabel(property));
        toolTip.append("\n");
        Graph* propertyGraph = property->getGraph();

        if(propertyGraph != _graph) {
          string name;
          propertyGraph->getAttribute<string>("name", name);
          toolTip.append("Inherited property from graph : ");
          toolTip.append(tlpStringToQString(name));
          toolTip.append(" ( ");
          toolTip.append(QString::number(propertyGraph->getId()));
          toolTip.append(" )");
        }
        else {
          toolTip.append("Local property");
        }

        return QVariant(toolTip);
      }
    }

    break;

  case Qt::DecorationRole: {
    if(orientation == Qt::Horizontal) {
      PropertyInterface* property = _propertiesTable[section];

      if(_propertiesToDelete.find(property)==_propertiesToDelete.end()) {
        return property->getGraph() != _graph?QVariant(QIcon(":/spreadsheet/inherited_properties.png")):QVariant();
      }
    }
  }
  break;
  }

  return QVariant();
}

Qt::ItemFlags GraphTableModel::flags( const QModelIndex & index ) const {
  GraphTableModelIndex tableIndex = element(index);
  Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

  if(tableIndex.isValid() && _propertiesToDelete.find(tableIndex.property())==_propertiesToDelete.end()) {
    TulipQVariantBuilder helper;
    return helper.flags(defaultFlags,_elementType,tableIndex.element(),tableIndex.property());
  }
  else {
    return defaultFlags;
  }

}
void GraphTableModel::sort( int column, Qt::SortOrder order ) {
  if(_orientation == Qt::Vertical) {
    if(column >-1 && column < columnCount()) {
      sortElements(_propertiesTable[column],order);
    }
  }
}

void GraphTableModel::sortElements(tlp::PropertyInterface* property, Qt::SortOrder order) {
  assert(property!=NULL);
  _order = order;
  _sortingProperty = property;
  PropertyValueComparator comparator(order,_elementType,_sortingProperty);
  std::stable_sort(_idTable.begin(),_idTable.end(),comparator);
  updateIndexMap<unsigned int>(_idTable,_idToIndex);
  emit dataChanged(index(0,0),index(rowCount()-1,columnCount()-1));

  if(_orientation == Qt::Vertical) {
    emit headerDataChanged ( Qt::Vertical, 0, rowCount()-1 );
  }
  else {
    emit headerDataChanged ( Qt::Horizontal, 0, columnCount()-1 );
  }
}

bool GraphTableModel::removeColumns( int column, int count, const QModelIndex & parent ) {
  if(column > -1 && column+count <columnCount(parent)) {
    if(_orientation == Qt::Vertical) {
      return removeProperties(column,column+count-1,parent);
    }
    else {
      return removeElements(column,column+count-1,parent);
    }
  }
  else {
    return false;
  }
}

bool GraphTableModel::removeRows(int row, int count, const QModelIndex &parent) {
  if(row > -1 && row+count <rowCount(parent)) {
    if(_orientation == Qt::Vertical) {
      return removeElements(row,row+count-1,parent);
    }
    else {
      return removeProperties(row,row+count-1,parent);
    }
  }
  else {
    return false;
  }
}

bool GraphTableModel::removeElements(int first,int last,const QModelIndex& parent) {
  if(_graph != NULL) {
    for(int i = first ; i <= last ; ++i) {
      if(_elementType == NODE) {
        _graph->delNode(node(idForIndex(i,parent)));
      }
      else {
        _graph->delEdge(edge(idForIndex(i,parent)));
      }
    }

    return true;
  }
  else {
    return false;
  }
}

bool GraphTableModel::removeProperties(int first,int last,const QModelIndex& parent) {
  if(_graph != NULL) {
    for(int i = first ; i <= last ; ++i) {
      PropertyInterface* propertyInterface = propertyForIndex(i,parent);
      string name = propertyInterface->getName();
      propertyInterface->getGraph()->delLocalProperty(name);
    }

    return true;
  }
  else {
    return false;
  }
}

bool GraphTableModel::removeColumns( const QModelIndexList& toRemove) {
  set<int> columns;

  for(QModelIndexList::const_iterator it = toRemove.begin() ; it != toRemove.end() ; ++it) {
    int column = (*it).column();

    if(columns.find(column) == columns.end()) {
      columns.insert(column);
    }
  }

  for(set<int>::reverse_iterator it = columns.rbegin(); it != columns.rend(); ++it) {
    if(!removeColumns((*it),1)) {
      return false;
    }
  }

  return true;
}

bool GraphTableModel::removeRows( const QModelIndexList& toRemove) {
  set<int> rows;

  for(QModelIndexList::const_iterator it = toRemove.begin() ; it != toRemove.end() ; ++it) {
    int row = (*it).row();

    if(rows.find(row) == rows.end()) {
      rows.insert(row);
    }
  }

  for(set<int>::reverse_iterator it = rows.rbegin(); it != rows.rend(); ++it) {
    if(!removeRows((*it),1)) {
      return false;
    }
  }

  return true;
}


void GraphTableModel::addNode(Graph *, const node n) {
  if(_elementType == NODE) {
    _idsToAdd.insert(n.id);
  }
}

void GraphTableModel::addEdge(Graph *, const edge e) {
  if(_elementType == EDGE) {
    _idsToAdd.insert(e.id);
  }
}

void GraphTableModel::delNode(Graph *,const node n) {
  if(_elementType == NODE) {
    _idsToDelete.insert(n.id);
  }
}

void GraphTableModel::delEdge(Graph *,const edge e) {
  if(_elementType == EDGE) {
    _idsToDelete.insert(e.id);
  }
}
void GraphTableModel::addLocalProperty(Graph* g, const string& propertyName) {
  PropertyInterface* property = g->getProperty(propertyName);

  //Check if we need to add the property
  if(useProperty(property)) {
    //If there is an existing inherited property with the same name hide the inherited property.
    for(unsigned int i = 0; i < _propertiesTable.size(); ++i) {
      //Find an existing property override them
      //Check if the property is marked for deletion.
      if( _propertiesToDelete.find(_propertiesTable[i])== _propertiesToDelete.end() && _propertiesTable[i]->getName().compare(propertyName)==0) {
        _propertiesToDelete.insert(_propertiesTable[i]);
        break;
      }
    }

    _propertiesToAdd.insert(property);
  }
}

void GraphTableModel::beforeDelLocalProperty(tlp::Graph *graph, const std::string & name) {
  PropertyInterface* property = graph->getProperty(name);

  //If the property was added then deleted before an update of the table
  if(_propertiesToAdd.find(property) != _propertiesToAdd.end()) {
    _propertiesToAdd.erase(property);
  }
  else {
    _propertiesToDelete.insert(property);
    removeFromVector<PropertyInterface*>(_propertiesToDelete,_propertiesTable,_propertyToIndex,_orientation==Qt::Horizontal);
    _propertiesToDelete.clear();
    property->removePropertyObserver(this);
    property->removeObserver(this);
  }
}

void GraphTableModel::addInheritedProperty(Graph* g, const string& propertyName) {
  _propertiesToAdd.insert(g->getProperty(propertyName));
}

void GraphTableModel::beforeDelInheritedProperty(Graph *graph, const std::string &name) {
  PropertyInterface* property = graph->getProperty(name);

  //If the property was added then deleted before an update of the table
  if(_propertiesToAdd.find(property) != _propertiesToAdd.end()) {
    _propertiesToAdd.erase(property);
  }
  else {
    _propertiesToDelete.insert(property);
    removeFromVector<PropertyInterface*>(_propertiesToDelete,_propertiesTable,_propertyToIndex,_orientation==Qt::Horizontal);
    _propertiesToDelete.clear();
    property->removePropertyObserver(this);
    property->removeObserver(this);

  }

}

void GraphTableModel::afterSetNodeValue(PropertyInterface* property, const node n) {
  if(_elementType == NODE) {
    //If the element was not marked for deletion
    if(_idsToDelete.find(n.id) == _idsToDelete.end()) {
      _dataUpdated.push_back(GraphTableModelIndex(n.id,property));
    }
  }
}

void GraphTableModel::afterSetEdgeValue(PropertyInterface* property, const edge e) {
  if(_elementType == EDGE) {
    //If the element was not marked for deletion
    if(_idsToDelete.find(e.id) == _idsToDelete.end()) {
      _dataUpdated.push_back(GraphTableModelIndex(e.id,property));
    }
  }
}

void GraphTableModel::afterSetAllNodeValue(PropertyInterface* property) {
  if(_elementType == NODE) {
    _propertiesUpdated.insert(property);
  }
}

void GraphTableModel::afterSetAllEdgeValue(PropertyInterface* property) {
  if(_elementType == EDGE) {
    _propertiesUpdated.insert(property);
  }
}

void GraphTableModel::update() {
  //Remove deleted properties and elements from vector.
  if(!_idsToDelete.empty()) {
    removeFromVector<unsigned int>(_idsToDelete,_idTable,_idToIndex,_orientation==Qt::Vertical);
    _idsToDelete.clear();
  }

  if(!_propertiesToDelete.empty()) {
    removeFromVector<PropertyInterface*>(_propertiesToDelete,_propertiesTable,_propertyToIndex,_orientation==Qt::Horizontal);

    //Check if the sorting property is deleted
    if(_propertiesToDelete.find(_sortingProperty)!= _propertiesToDelete.end()) {
      //No sort property
      _sortingProperty = NULL;
    }

    _propertiesToDelete.clear();
  }

  if(!_idsToAdd.empty()) {
    PropertyValueComparator *comparator = _sortingProperty != NULL?(new PropertyValueComparator(_order,_elementType,_sortingProperty)):(NULL);
    addToVector<unsigned int,PropertyValueComparator>(_idsToAdd,_idTable,_idToIndex,_orientation==Qt::Vertical,comparator);
    delete comparator;
    _idsToAdd.clear();
  }

  if(!_propertiesToAdd.empty()) {
    PropertyComparator comparator;
    addToVector<PropertyInterface*,PropertyComparator>(_propertiesToAdd,_propertiesTable,_propertyToIndex,_orientation==Qt::Horizontal,&comparator);

    for(set<PropertyInterface*>::iterator it = _propertiesToAdd.begin() ; it != _propertiesToAdd.end(); ++it) {
      (*it)->addPropertyObserver(this);
      (*it)->addObserver(this);
    }

    _propertiesToAdd.clear();
  }

  //Send modification events.
  if(!_propertiesUpdated.empty() || !_dataUpdated.empty()) {
    bool needToSort = false;

    if(_sortingProperty != NULL) {
      //If the sorting property is updated we need to sort elements again.
      //Check if we have a value updated in the sorting property

      if(_orientation == Qt::Vertical) {
        //Check if a set all node/edge value occur on the sorting property
        if(_propertiesUpdated.find(_sortingProperty) != _propertiesUpdated.end()) {
          needToSort = true;
        }

        //Check if a node/edge value modification occur on the sorting property
        if(!needToSort) {
          for(vector<GraphTableModelIndex>::iterator it = _dataUpdated.begin(); it != _dataUpdated.end(); ++it) {
            if((*it).property()==_sortingProperty) {
              needToSort = true;
              break;
            }
          }
        }
      }
    }

    if(needToSort) {
      //Sort elements update all the data
      sortElements(_sortingProperty,_order);
    }
    else {
      //Compute the area to update.
      int first = 0;
      int left = 0;
      int last = rowCount()-1;
      int right = columnCount()-1;
      //If properties where updated we need to update all the elements so there os no need to compute the area for updated elements.
      set<PropertyInterface*> properties = _propertiesUpdated;
      set<unsigned int> elements;
      //If a set all node/edge value was called need to update all properties
      bool updateAllElements  = !_propertiesUpdated.empty();

      for( vector<GraphTableModelIndex>::iterator it = _dataUpdated.begin(); it != _dataUpdated.end() ; ++it) {
        properties.insert((*it).property());
        elements.insert((*it).element());
      }

      pair<unsigned int,unsigned int> propertiesArea = computeArea<PropertyInterface*>(properties,_propertiesTable,_propertyToIndex);
      left = propertiesArea.first;
      right = propertiesArea.second;

      if(!updateAllElements) {
        pair<unsigned int,unsigned int> elementsArea = computeArea<unsigned int>(elements,_idTable,_idToIndex);
        first = elementsArea.first;
        last = elementsArea.second;
      }

      emit dataChanged(index(first,left),index(last,right));
    }

    _propertiesUpdated.clear();
    _dataUpdated.clear();
  }
}

