#include <tulip/GraphElementModel.h>

#include <QtGui/QFont>

#include <tulip/ForEach.h>
#include <tulip/Graph.h>

using namespace tlp;
using namespace std;

GraphElementModel::GraphElementModel(Graph *graph,unsigned int id,QObject *parent):TulipModel(parent),_graph(graph),_id(id) {

}

int GraphElementModel::rowCount(const QModelIndex &parent) const {
  if (_graph == NULL || parent.isValid())
    return 0;

  string propertyName;
  int result=0;
  forEach(propertyName,_graph->getProperties()) {
    ++result;
  }
  return result;
}

int GraphElementModel::columnCount(const QModelIndex &parent) const {
  if (_graph == NULL || parent.isValid())
    return 0;

  return 1;
}

QModelIndex GraphElementModel::parent(const QModelIndex &/*child*/) const {
  return QModelIndex();
}

QVariant GraphElementModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
      return headerText(_id);
    else if (role == Qt::TextAlignmentRole)
      return Qt::AlignCenter;
  }
  else if(role==Qt::DisplayRole) {
    string propertyName;
    int result=0;
    forEach(propertyName,_graph->getProperties()) {
      if(section==result)
        return propertyName.c_str();

      ++result;
    }
  }

  return TulipModel::headerData(section,orientation,role);
}

QModelIndex GraphElementModel::index(int row, int column,const QModelIndex &parent) const {
  if (!hasIndex(row,column,parent))
    return QModelIndex();

  string propertyName;
  PropertyInterface* prop;
  int result=0;
  forEach(propertyName,_graph->getProperties()) {
    if(result==row)
      prop=_graph->getProperty(propertyName);

    ++result;
  }
  return createIndex(row,column,prop);
}

QVariant GraphElementModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole)
    return value(_id,(PropertyInterface*)(index.internalPointer()));

  return QVariant();
}

bool GraphNodeElementModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if(role==Qt::EditRole) {
    string propertyName;
    PropertyInterface* prop;
    int result=0;
    forEach(propertyName,_graph->getProperties()) {
      if(result==index.row())
        prop=_graph->getProperty(propertyName);

      ++result;
    }

    return GraphModel::setNodeValue(_id,prop,value);
  }

  return false;
}

bool GraphEdgeElementModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if(role==Qt::EditRole) {
    string propertyName;
    PropertyInterface* prop;
    int result=0;
    forEach(propertyName,_graph->getProperties()) {
      if(result==index.row())
        prop=_graph->getProperty(propertyName);

      ++result;
    }

    return GraphModel::setEdgeValue(_id,prop,value);
  }

  return false;
}

