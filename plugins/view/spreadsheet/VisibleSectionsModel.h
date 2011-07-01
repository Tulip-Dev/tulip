#ifndef VISIBLESECTIONMSODEL_H
#define VISIBLESECTIONSMODEL_H
#include <QtGui/QSortFilterProxyModel>
#include <vector>
class TulipTableWidgetColumnSelectionModel;

class VisibleSectionsModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    VisibleSectionsModel(TulipTableWidgetColumnSelectionModel* columModel,QObject* parent= NULL);
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;


protected:
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;
private:    
    TulipTableWidgetColumnSelectionModel* _columnModel;
};

#endif // VISIBLESECTIONSMODEL_H
