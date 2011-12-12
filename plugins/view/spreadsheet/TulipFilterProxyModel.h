#ifndef TULIPFILTERPROXYMODEL_H
#define TULIPFILTERPROXYMODEL_H
#include <QtGui/QSortFilterProxyModel>
#include <tulip/Observable.h>
class GraphTableModel;

namespace tlp {
class BooleanProperty;
}
class TulipFilterProxyModel : public QSortFilterProxyModel , public tlp::Observable {
  Q_OBJECT
public:
  TulipFilterProxyModel(QObject* parent=NULL);

  void setGraphTableModel(GraphTableModel* tableModel);

  void sort(int column, Qt::SortOrder order);

  void setShowOnlySelectedElement(bool showOnlySelectedElements);

  void treatEvent(const tlp::Event &);

  void update();

protected:

  bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;
  bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;

private:
  void setSourceModel ( QAbstractItemModel * sourceModel );

  GraphTableModel* _tableModel;
  bool _showOnlyVisibleElements;

  mutable tlp::BooleanProperty* _selectionProperty;
  mutable bool _reloadSelectionProperty;

  mutable bool _needToReload;
};

#endif // TULIPFILTERPROXYMODEL_H
