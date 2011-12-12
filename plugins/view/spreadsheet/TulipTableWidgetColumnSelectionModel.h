#ifndef TULIPTABLEWIDGETCOLUMNSELECTIONMODEL_H
#define TULIPTABLEWIDGETCOLUMNSELECTIONMODEL_H
#include <QtCore/QAbstractListModel>
class GraphTableWidget;
class GraphTableModel;

namespace tlp {
class PropertyInterface;
}
/**
  * @brief List model controlling the visibility of TulipTableWidget view columns.
  **/
class TulipTableWidgetColumnSelectionModel : public QAbstractListModel {
  Q_OBJECT
public:
  TulipTableWidgetColumnSelectionModel(GraphTableWidget* tableView ,QObject* parent=NULL);

  int rowCount( const QModelIndex & parent = QModelIndex() ) const;

  QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

  bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

  Qt::ItemFlags flags( const QModelIndex & index ) const;

  /**
    * @brief Set the visibility of the given column. The column index correspond to the index of the column.
    **/
  void setColumnVisible(int columnIndex,bool visible);

  bool isColumnVisible(int columnIndex)const;

  GraphTableModel* graphTableModel() {
    return _tableModel;
  }

  tlp::PropertyInterface* propertyForIndex(const QModelIndex& index);

private :
  GraphTableWidget* _tableView;
  GraphTableModel* _tableModel;

private slots:
  void columnsInserted ( const QModelIndex & parent, int start, int end );
  void columnsMoved ( const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationColumn );
  void columnsRemoved ( const QModelIndex & parent, int start, int end );
  void headerDataChanged ( Qt::Orientation orientation, int first, int last );
};

#endif // TULIPTABLEWIDGETCOLUMNSELECTIONMODEL_H
