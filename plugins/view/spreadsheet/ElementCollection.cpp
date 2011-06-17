#include "ElementCollection.h"
#include <iostream>
using namespace std;


/**
    Constructs a string list model with the given \a parent.
*/

ElementCollection::ElementCollection(QObject *parent)
    : QAbstractListModel(parent)
{
}

ElementCollection::ElementCollection(const ElementCollection& other):QAbstractListModel(NULL),_data(other._data){

}

ElementCollection & ElementCollection::operator=( const ElementCollection & other){
    if(this != &other){
        _data= other._data;
    }
    return *this;
}

/**
    Constructs a string list model containing the specified \a strings
    with the given \a parent.
*/

ElementCollection::ElementCollection(const QStringList &strings, QObject *parent)
    : QAbstractListModel(parent)
{
    for(QStringList::const_iterator it = strings.begin();it != strings.end();++it){
        addElement(*it);
    }
}

void ElementCollection::addElement(const QString& label){
    QMap<int,QVariant> element;
    element.insert(Qt::DisplayRole,label);
    _data.push_back(element);
}

/**
    Returns the number of rows in the model. This value corresponds to the
    number of items in the model's internal string list.

    The optional \a parent argument is in most models used to specify
    the parent of the rows to be counted. Because this is a list if a
    valid parent is specified, the result will always be 0.

    \sa insertRows(), removeRows(), QAbstractItemModel::rowCount()
*/

int ElementCollection::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _data.count();
}

/**
    Returns data for the specified \a role, from the item with the
    given \a index.

    If the view requests an invalid index, an invalid variant is returned.

    \sa setData()
*/

QVariant ElementCollection::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _data.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        if(_data.at(index.row()).contains(Qt::DisplayRole)){
            return _data.at(index.row()).value(Qt::DisplayRole);
        }else{
            return QVariant();
        }
    }else{
        if(_data.at(index.row()).contains(role)){
            return _data.at(index.row()).value(role);
        }else{
            return QVariant();
        }
    }

    return QVariant();
}

/**
    Returns the flags for the item with the given \a index.

    Valid items are enabled, selectable, and editable.

    \sa QAbstractItemModel::flags()
*/

Qt::ItemFlags ElementCollection::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable;
}

/**
    Sets the data for the specifed \a role in the item with the given
    \a index in the model, to the provided \a value.

    The dataChanged() signal is emitted if the item is changed.

    \sa Qt::ItemDataRole, data()
*/

bool ElementCollection::setData(const QModelIndex &index, const QVariant &value, int role)
{    
    if (index.row() >= 0 && index.row() < _data.size()){ 
        if(role == Qt::EditRole || role == Qt::DisplayRole) {
            _data[index.row()].insert(Qt::DisplayRole,  value);
        }else{            
            _data[index.row()].insert(role, value);
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

/**
    Inserts \a count rows into the model, beginning at the given \a row.

    The \a parent index of the rows is optional and is only used for
    consistency with QAbstractItemModel. By default, a null index is
    specified, indicating that the rows are inserted in the top level of
    the model.

    \sa QAbstractItemModel::insertRows()
*/

bool ElementCollection::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount(parent))
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        _data.insert(row, QMap<int,QVariant>() );

    endInsertRows();

    return true;
}

/**
    Removes \a count rows from the model, beginning at the given \a row.

    The \a parent index of the rows is optional and is only used for
    consistency with QAbstractItemModel. By default, a null index is
    specified, indicating that the rows are removed in the top level of
    the model.

    \sa QAbstractItemModel::removeRows()
*/

bool ElementCollection::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        _data.removeAt(row);

    endRemoveRows();

    return true;
}

void ElementCollection::setAllElementSelection(bool selected){
    for(int i = 0 ; i < rowCount(); ++i){
        setData(index(i,0),QVariant(selected),SelectionType);
    }
}

void ElementCollection::setElementSelection(int element,bool selected){
    setData(index(element,0),QVariant(selected),SelectionType);
}

QList<int> ElementCollection::selectedElement()const{
    QList<int> selectedElements;
    for(int i = 0 ; i < rowCount(); ++i){
        QVariant value = data(index(i,0),SelectionType);
        if(value.isValid() && value.toBool()){
            selectedElements.push_back(i);
        }
    }
    return selectedElements;
}

/**
  \reimp
*/
//void ElementCollection::sort(int, Qt::SortOrder order)
//{
//    emit layoutAboutToBeChanged();
//    if (order == Qt::AscendingOrder)
//        qSort(_data.begin(), _data.end(), ElementCollection::sortElements);
//    else
//        qSort(_data.begin(), _data.end(), ElementCollection::sortElements);
//    emit layoutChanged();
//}
