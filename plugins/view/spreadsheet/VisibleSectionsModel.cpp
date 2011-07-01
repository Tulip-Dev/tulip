#include "VisibleSectionsModel.h"
#include "GraphTableWidget.h"
#include <QtGui/QHeaderView>
#include "TulipTableWidgetColumnSelectionModel.h"
#include <tulip/TlpQtTools.h>
VisibleSectionsModel::VisibleSectionsModel(TulipTableWidgetColumnSelectionModel* columnModel,QObject* parent): QSortFilterProxyModel(parent),_columnModel(columnModel)
{
    setSourceModel(columnModel);
}

QVariant VisibleSectionsModel::data( const QModelIndex & index, int role ) const{
    switch(role){
    case Qt::UserRole:
        return QVariant(mapToSource(index).row());
        break;
     case Qt::DisplayRole:
        //Display only the name of the property avoid a display bug when having text with more than one line in a QCombobox
        return QVariant(tlp::tlpStringToQString(_columnModel->propertyForIndex(mapToSource(index))->getName()));
        break;
    default:
        return QSortFilterProxyModel::data(index,role);
        break;
    }

}

bool VisibleSectionsModel::filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const{
    return _columnModel->isColumnVisible(source_row);
}

