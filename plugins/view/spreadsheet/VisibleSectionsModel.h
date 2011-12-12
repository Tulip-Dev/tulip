#ifndef VISIBLESECTIONMSODEL_H
#define VISIBLESECTIONSMODEL_H

#include <QtGui/QComboBox>

class TulipTableWidgetColumnSelectionModel;

class VisibleSectionsModel : public QComboBox {
  Q_OBJECT
public:
  VisibleSectionsModel(QWidget* parent= NULL);

  void setColumnModel(TulipTableWidgetColumnSelectionModel* columModel);

private slots:
  void propertiesInserted ( const QModelIndex & parent, int start, int end );
  void propertiesMoved ( const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationColumn );
  void propertiesRemoved ( const QModelIndex & parent, int start, int end );
  void propertiesDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
  void propertiesReset();
private:

  void initModel(int selectedColumnIndex = -1);
  TulipTableWidgetColumnSelectionModel* _columnModel;
};

#endif // VISIBLESECTIONSMODEL_H
