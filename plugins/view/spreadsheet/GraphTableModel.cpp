#include "GraphTableModel.h"

#include <tulip/Graph.h>
#include <tulip/TlpQtTools.h>
#include "TulipQVariantBuilder.h"


using namespace tlp;
using namespace std;


struct PropertyValueComparator {
public:
    PropertyValueComparator(Qt::SortOrder sortOrder,ElementType type,PropertyInterface* property):_sortOrder(sortOrder),_type(type),_property(property){}
    PropertyValueComparator(const PropertyValueComparator& other):_sortOrder(other._sortOrder),_type(other._type),_property(other._property){}
    bool operator()(unsigned int elt1,unsigned int elt2){
        if(_type == NODE){            
            if(_sortOrder == Qt::AscendingOrder){
                return _property->compare(node(elt1),node(elt2)) < 0 ;
            }else{
                return _property->compare(node(elt1),node(elt2)) > 0 ;
            }
        }else{
            if(_sortOrder == Qt::AscendingOrder){
                return _property->compare(edge(elt1),edge(elt2)) < 0 ;
            }else{
                return _property->compare(edge(elt1),edge(elt2)) > 0 ;
            }
        }
    }
private:
    Qt::SortOrder _sortOrder;
    ElementType _type;
    PropertyInterface* _property;
};

struct PropertyComparator {
public:
    bool operator()(PropertyInterface* p1,PropertyInterface* p2){
        return p1->getName().compare(p2->getName())<0;
    }
};


GraphTableModelIndex::GraphTableModelIndex(unsigned int element,PropertyInterface* property):_element(element),_property(property){

}

GraphTableModel::GraphTableModel(Graph* graph,ElementType displayType,QObject* parent ):QAbstractTableModel(parent),_graph(NULL),_displayedType(displayType),_orientation(Qt::Vertical),_sortColum(-1),_order(Qt::AscendingOrder)
{
    setGraph(graph);
}

void GraphTableModel::setGraph(Graph* newGraph){
    if(_graph != NULL){        
        _graph->removeObserver(this);
        _graph->removeGraphObserver(this);
    }
    _graph = newGraph;
    if(_graph != NULL){
        _graph->addObserver(this);
        _graph->addGraphObserver(this);
    }
    updateElementsTable();
    updatePropertyTable();
    reset();
}

void GraphTableModel::setElementType(ElementType type){
    _displayedType = type;
    updateElementsTable();
    reset();
}

void GraphTableModel::setOrientation(Qt::Orientation orientation){
    _orientation = orientation;
    reset();
}
unsigned int GraphTableModel::idForIndex(int index,const QModelIndex&) const{
    if(index > -1 && (unsigned int)index < _idTable.size()){
        return _idTable[index];
    }else{
        return UINT_MAX;
    }
}
PropertyInterface* GraphTableModel::propertyForIndex(int index,const QModelIndex&) const{
    if(index > -1 && (unsigned int)index < _propertiesTable.size()){
        return _propertiesTable[index];
    }else{
        return NULL;
    }
}

void GraphTableModel::updateElementsTable(){
    _idTable.clear();
    if(_graph != NULL){
        if(_displayedType == NODE){
            node n;
            forEach(n,_graph->getNodes()){
                _idTable.push_back(n.id);
            }
        }else{
            edge e;
            forEach(e,_graph->getEdges()){
                _idTable.push_back(e.id);
            }
        }
    }
}

void GraphTableModel::updatePropertyTable(){
    _propertiesTable.clear();
    if(_graph != NULL){
        PropertyInterface* property;
        forEach(property,_graph->getObjectProperties()){         
            property->removePropertyObserver(this);            
            property->removeObserver(this);
            _propertiesTable.push_back(property);            
            property->getGraph()->addGraphObserver(this);
            property->addObserver(this);            
        }
        PropertyComparator comparator;        
        std::stable_sort(_propertiesTable.begin(),_propertiesTable.end(),comparator);
    }
}

int GraphTableModel::columnCount(const QModelIndex& ) const{
    return _orientation == Qt::Vertical?_propertiesTable.size():_idTable.size();
}
int GraphTableModel::rowCount(const QModelIndex& ) const{
    return _orientation == Qt::Vertical?_idTable.size():_propertiesTable.size();
}

QVariant GraphTableModel::data(const QModelIndex& index, int role ) const{
    switch(role){
    case DataTypeRole:
        {
            GraphTableModelIndex tableIndex = element(index);
            if(tableIndex.isValid()){
                TulipQVariantBuilder helper;
                return QVariant(helper.getPropertyType(_displayedType,tableIndex.property()));
            }
        }
        break;
    default:
        GraphTableModelIndex tableIndex = element(index);
        if(tableIndex.isValid()){
            TulipQVariantBuilder helper;
            return helper.data(role,_displayedType,tableIndex.element(),tableIndex.property());
        }
    }
    return QVariant();
}
bool GraphTableModel::setData( const QModelIndex & index, const QVariant & value, int role ){
    switch(role){
    case Qt::EditRole:
        GraphTableModelIndex tableIndex = element(index);
        if(tableIndex.isValid()){
            TulipQVariantBuilder helper;
            return helper.setData(value,_displayedType,tableIndex.element(),tableIndex.property());
        }
        break;
    }
    return false;
}

QVariant GraphTableModel::headerData(int section, Qt::Orientation orientation, int role ) const{
    switch(role){
    case Qt::DisplayRole:
        if(orientation == Qt::Vertical){
            return QVariant(QString::number(_idTable[section]));
        }else{
            QString name = QString::fromStdString(_propertiesTable[section]->getName());
            name.append("\n");
            name.append("( ");
            name.append(tlpStringToQString(_propertiesTable[section]->getTypename()));
            name.append(" )");

            PropertyInterface* property = _propertiesTable[section];
            if(property->getGraph() != _graph){
                name.prepend("*");
            }

            return QVariant(name);
        }

        break;
    case Qt::ToolTipRole:
        if(orientation == Qt::Horizontal){
            PropertyInterface* property = _propertiesTable[section];
            QString toolTip;
            toolTip.append("Property : ");
            toolTip.append(tlpStringToQString(property->getName()));
            toolTip.append("\n");
            toolTip.append("Type : ");
            toolTip.append(tlpStringToQString(property->getTypename()));
            toolTip.append("\n");
            Graph* propertyGraph = property->getGraph();
            if(propertyGraph != _graph){
                toolTip.append("Inherited property from graph : ");
                toolTip.append(tlpStringToQString(propertyGraph->getAttribute<string>("name")));
                toolTip.append(" ( ");
                toolTip.append(QString::number(propertyGraph->getId()));
                toolTip.append(" )");
            }else{
                toolTip.append("Local property");
            }
            return QVariant(toolTip);
        }
        break;       
    }
    return QVariant();
}

Qt::ItemFlags GraphTableModel::flags( const QModelIndex & index ) const{
    GraphTableModelIndex tableIndex = element(index);
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    if(tableIndex.isValid()){
        TulipQVariantBuilder helper;
        return helper.flags(defaultFlags,_displayedType,tableIndex.element(),tableIndex.property());
    }else{
        return defaultFlags;
    }

}
void GraphTableModel::sort( int column, Qt::SortOrder order ){
    if(_orientation == Qt::Vertical){
        if(column < columnCount()){
            _sortColum = column;
            _order = order;
            PropertyInterface* propertyName = _propertiesTable[column];            
            assert(propertyName!=NULL);
            PropertyValueComparator comparator(order,_displayedType,propertyName);
            std::stable_sort(_idTable.begin(),_idTable.end(),comparator);
            emit dataChanged(index(0,0),index(rowCount(),columnCount()));
            emit headerDataChanged ( Qt::Vertical, 0, rowCount() );
        }
    }
}

bool GraphTableModel::removeColumns( int column, int count, const QModelIndex & parent ){
    if(column > -1 && column+count <columnCount(parent)){
        if(_orientation == Qt::Vertical){
            return removeProperties(column,column+count-1,parent);
        }else{
            return removeElements(column,column+count-1,parent);
        }
    }else{
        return false;
    }
}

bool GraphTableModel::removeRows(int row, int count, const QModelIndex &parent){
    if(row > -1 && row+count <rowCount(parent)){
        if(_orientation == Qt::Vertical){
            return removeElements(row,row+count-1,parent);
        }else{
            return removeProperties(row,row+count-1,parent);
        }
    }else{
        return false;
    }
}

bool GraphTableModel::removeElements(int first,int last,const QModelIndex& parent){
    if(_graph != NULL){
        for(int i = first ; i <= last ; ++i){
            if(_displayedType == NODE){
                _graph->delNode(node(idForIndex(i,parent)));
            }else{
                _graph->delEdge(edge(idForIndex(i,parent)));
            }
        }
        return true;
    }else{
        return false;
    }
}

bool GraphTableModel::removeProperties(int first,int last,const QModelIndex& parent){
    if(_graph != NULL){
        for(int i = first ; i <= last ; ++i){
            PropertyInterface* propertyInterface = propertyForIndex(i,parent);
            propertyInterface->getGraph()->delLocalProperty(propertyInterface->getName());
        }
        return true;
    }else{
        return false;
    }
}

bool GraphTableModel::removeColumns( const QModelIndexList& toRemove){    
    set<int> columns;
    for(QModelIndexList::const_iterator it = toRemove.begin() ; it != toRemove.end() ; ++it){
        int column = (*it).column();
        if(columns.find(column) == columns.end()){
            columns.insert(column);            
        }
    }
    for(set<int>::reverse_iterator it = columns.rbegin();it != columns.rend();++it){
        if(!removeColumns((*it),1)){
            return false;
        }
    }
    return true;
}

bool GraphTableModel::removeRows( const QModelIndexList& toRemove){
    set<int> rows;
    for(QModelIndexList::const_iterator it = toRemove.begin() ; it != toRemove.end() ; ++it){
        int row = (*it).row();
        if(rows.find(row) == rows.end()){
            rows.insert(row);            
        }
    }
    for(set<int>::reverse_iterator it = rows.rbegin();it != rows.rend();++it){
        if(!removeRows((*it),1)){
            return false;
        }
    }
    return true;
}


void GraphTableModel::addNode(Graph *, const node n){
    if(_displayedType == NODE){
        _idsToAdd.insert(n.id);
    }
}

void GraphTableModel::addEdge(Graph *, const edge e){
    if(_displayedType == EDGE){
        _idsToAdd.insert(e.id);
    }
}

void GraphTableModel::delNode(Graph *,const node n){    
    if(_displayedType == NODE){
        _idsToDelete.insert(n.id);
    }
}

void GraphTableModel::delEdge(Graph *,const edge e){
    if(_displayedType == EDGE){
        _idsToDelete.insert(e.id);
    }
}
void GraphTableModel::addLocalProperty(Graph* g, const string& propertyName){    
    //    if(_propertiesTable.find(g->getProperty(propertyName)))
    //If there is an existing inherited property with the same name hide the inherited property.
    bool found = false;
    for(unsigned int i = 0;i < _propertiesTable.size(); ++i){
        //Find an existing property hide them
        if(_propertiesTable[i]->getName().compare(propertyName)==0){
            _propertiesTable[i]->removePropertyObserver(this);
            _propertiesTable[i]->removeObserver(this);
            _propertiesTable[i]=g->getProperty(propertyName);
            _propertiesTable[i]->addPropertyObserver(this);
            _propertiesTable[i]->addObserver(this);
            found = true;
            _propertiesUpdated.insert(_propertiesTable[i]);
            break;
        }
    }
    //If no existing properties add new one.
    if(!found){
        _propertiesToAdd.insert(g->getProperty(propertyName));
    }
}

void GraphTableModel::delLocalProperty(tlp::Graph *graph, const std::string & name){
    delProperty(graph->getProperty(name));
}

void GraphTableModel::addInheritedProperty(Graph* g, const string& propertyName){    
    _propertiesToAdd.insert(g->getProperty(propertyName));
}

void GraphTableModel::delInheritedProperty(Graph *graph, const std::string &name){
    delProperty(graph->getProperty(name));
}

void GraphTableModel::delProperty(PropertyInterface* property){    
    //Need to check if there is a property with the same name in the upper hierachy
    Graph *current = property->getGraph();
    Graph* parent = current->getSuperGraph();
    //If there is a super property replace the deleted one by the super
    if(parent != current && parent->existProperty(property->getName())){
        for(unsigned int i = 0;i < _propertiesTable.size(); ++i){
            if(_propertiesTable[i] == property){
                _propertiesTable[i]->removePropertyObserver(this);
                _propertiesTable[i]->removeObserver(this);
                _propertiesTable[i] = parent->getProperty(property->getName());
                _propertiesTable[i]->addPropertyObserver(this);
                _propertiesTable[i]->addObserver(this);
                _propertiesUpdated.insert(_propertiesTable[i]);
            }
        }
    }else{
        _propertiesToDelete.insert(property);
    }
}

void GraphTableModel::afterSetNodeValue(PropertyInterface* property, const node n){
    if(_displayedType == NODE){
        _dataUpdated.push_back(GraphTableModelIndex(n.id,property));
    }
}

void GraphTableModel::afterSetEdgeValue(PropertyInterface* property, const edge e){
    if(_displayedType == EDGE){
        _dataUpdated.push_back(GraphTableModelIndex(e.id,property));
    }
}

void GraphTableModel::afterSetAllNodeValue(PropertyInterface* property){
    if(_displayedType == NODE){
        _propertiesUpdated.insert(property);
    }
}

void GraphTableModel::afterSetAllEdgeValue(PropertyInterface* property){
    if(_displayedType == EDGE){
        _propertiesUpdated.insert(property);
    }
}



void GraphTableModel::treatEvents(const  vector<Event> &){
    if(!_idsToDelete.empty()){                
        removeFromVector<unsigned int>(_idsToDelete,_idTable,_orientation==Qt::Vertical);        
        _idsToDelete.clear();
    }
    if(!_propertiesToDelete.empty()){        
        removeFromVector<PropertyInterface*>(_propertiesToDelete,_propertiesTable,_orientation==Qt::Horizontal);
        _propertiesToDelete.clear();
    }
    if(!_idsToAdd.empty()){            
        if(_sortColum != -1){
            PropertyInterface* propertyName = _propertiesTable[_sortColum];
            PropertyValueComparator comparator(_order,_displayedType,propertyName);
            addToVector<unsigned int,PropertyValueComparator>(_idsToAdd,_idTable,_orientation==Qt::Vertical,&comparator);
        }else{
            addToVector<unsigned int,PropertyValueComparator>(_idsToAdd,_idTable,_orientation==Qt::Vertical,NULL);
        }
        _idsToAdd.clear();
    }    
    if(!_propertiesToAdd.empty()){        
        for(set<PropertyInterface*>::iterator it = _propertiesToAdd.begin();it!= _propertiesToAdd.end();++it){            
        }
        PropertyComparator comparator;
        addToVector<PropertyInterface*,PropertyComparator>(_propertiesToAdd,_propertiesTable,_orientation==Qt::Horizontal,&comparator);
        _propertiesToAdd.clear();
    }
    if(!_propertiesUpdated.empty() || !_dataUpdated.empty()){
        if(_sortColum != -1){
            bool needToSort = false;
            if(_orientation == Qt::Vertical){
                PropertyInterface* sortProperty = _propertiesTable[_sortColum];
                for(set<PropertyInterface*>::iterator it = _propertiesUpdated.begin();it != _propertiesUpdated.end();++it){
                    if(sortProperty == *it){
                        needToSort = true;
                        break;
                    }
                }
                if(!needToSort){
                    for(vector<GraphTableModelIndex>::iterator it = _dataUpdated.begin();it != _dataUpdated.end();++it){
                        if((*it).property()==sortProperty){
                            needToSort = true;
                            break;
                        }
                    }
                }
            }
            if(needToSort){
                sort(_sortColum,_order);
            }else{
                emit dataChanged(index(0,0),index(rowCount(),columnCount()));
            }
        }else{
            emit dataChanged(index(0,0),index(rowCount(),columnCount()));
        }
        _propertiesUpdated.clear();
        _dataUpdated.clear();
    }
}
