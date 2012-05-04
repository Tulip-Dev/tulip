#include <tulip/Graph.h>

namespace tlp {

template<typename PROPTYPE>
GraphPropertiesModel<PROPTYPE>::GraphPropertiesModel(tlp::Graph* graph, bool checkable, bool hasNullProperty, QObject *parent): tlp::TulipModel(parent), _graph(graph), _placeholder(QString::null), _checkable(checkable), _hasNullProperty(hasNullProperty) {
  if (_graph != NULL)
    _graph->addListener(this);
}

template<typename PROPTYPE>
GraphPropertiesModel<PROPTYPE>::GraphPropertiesModel(QString placeholder, tlp::Graph* graph, bool checkable, bool hasNullProperty, QObject *parent): tlp::TulipModel(parent), _graph(graph), _placeholder(placeholder), _checkable(checkable), _hasNullProperty(hasNullProperty) {
  if (_graph != NULL)
    _graph->addListener(this);
}

template<typename PROPTYPE>
QModelIndex GraphPropertiesModel<PROPTYPE>::index(int row, int column,const QModelIndex &parent) const {
  if (!hasIndex(row,column,parent))
    return QModelIndex();

  int i=0;

  if (!_placeholder.isNull()) {
    i=1;

    if (row == 0)
      return createIndex(row,column);
  }

  if (_hasNullProperty) {
    if (row == 0)
      return createIndex(row, column);
    else i = 1;
  }

  std::string propName;

  forEach(propName,_graph->getInheritedProperties()) {
    if (dynamic_cast<PROPTYPE*>(_graph->getProperty(propName)) == NULL)
      continue;

    if (i++ == row)
      return createIndex(row,column,_graph->getProperty(propName));
  }
  forEach(propName,_graph->getLocalProperties()) {
    if (dynamic_cast<PROPTYPE*>(_graph->getProperty(propName)) == NULL)
      continue;

    if (i++ == row)
      return createIndex(row,column,_graph->getProperty(propName));
  }

  return QModelIndex();
}

template<typename PROPTYPE>
QModelIndex GraphPropertiesModel<PROPTYPE>::parent(const QModelIndex &) const {
  return QModelIndex();
}

template<typename PROPTYPE>
int GraphPropertiesModel<PROPTYPE>::rowCount(const QModelIndex &parent) const {
  if (parent.isValid() || _graph == NULL)
    return 0;

  int result = 0;

  if (!_placeholder.isNull())
    result = 1;

  std::string propName;
  forEach(propName, _graph->getProperties()) {
    if (dynamic_cast<PROPTYPE*>(_graph->getProperty(propName)) == NULL)
      continue;

    result++;
  }
  if (_hasNullProperty)
    ++result;
  return result;
}

template<typename PROPTYPE>
int GraphPropertiesModel<PROPTYPE>::columnCount(const QModelIndex &) const {
  return 3;
}

template<typename PROPTYPE>
QVariant GraphPropertiesModel<PROPTYPE>::data(const QModelIndex &index, int role) const {
  PropertyInterface* pi = NULL;

  if (index.internalPointer() == NULL && index.row() != 0)
    return QVariant();

  pi = (PropertyInterface*)(index.internalPointer());

  if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
    if (!_placeholder.isNull() && index.row() == 0)
      return _placeholder;

    if (pi == NULL)
      return QString();

    if (index.column() == 0)
      return QString(pi->getName().c_str());
    else if (index.column() == 1)
      return pi->getTypename().c_str();
    else if (index.column() == 2)
      return (_graph->existLocalProperty(pi->getName()) ? trUtf8("Local") : trUtf8("Inherited"));

  }
  else if (role == Qt::DecorationRole && index.column() == 0 && pi != NULL && !_graph->existLocalProperty(pi->getName()))
    return QIcon(":/tulip/gui/ui/inherited_properties.png");

  else if (role == Qt::FontRole) {
    QFont f;

    if (!_placeholder.isNull() && index.row() == 0)
      f.setItalic(true);

    return f;
  }
  else if (role == PropertyRole) {
    return QVariant::fromValue<PropertyInterface*>(pi);
  }
  else if (_checkable && role == Qt::CheckStateRole && index.column() == 0) {
    return (_unCheckedIndexes.contains(index.row()) ? Qt::Unchecked : Qt::Checked);
  }

  return QVariant();
}

template<typename PROPTYPE>
int GraphPropertiesModel<PROPTYPE>::rowOf(PropertyInterface* pi) const {
  int nbRows = rowCount();
  for (int i=0; i<nbRows; ++i) {
    QVariant v = data(index(i,0),PropertyRole);

    if (v.value<PropertyInterface*>() == pi)
      return i;
  }

  return 0;
}

}
