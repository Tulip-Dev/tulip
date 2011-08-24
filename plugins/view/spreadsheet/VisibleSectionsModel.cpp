#include "VisibleSectionsModel.h"
#include "GraphTableWidget.h"
#include <QtGui/QHeaderView>
#include "TulipTableWidgetColumnSelectionModel.h"
#include <tulip/TlpQtTools.h>

using namespace std;
VisibleSectionsModel::VisibleSectionsModel(QWidget* parent): QComboBox(parent),_columnModel(NULL) {

}

void VisibleSectionsModel::setColumnModel(TulipTableWidgetColumnSelectionModel* columModel) {
  _columnModel = columModel;
  connect(_columnModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(propertiesDataChanged(QModelIndex,QModelIndex)));
  connect(_columnModel,SIGNAL(rowsInserted(QModelIndex,int,int)),this,SLOT(propertiesInserted(QModelIndex,int,int)));
  connect(_columnModel,SIGNAL(rowsRemoved(QModelIndex,int,int)),this,SLOT(propertiesRemoved(QModelIndex,int,int)));
  connect(_columnModel,SIGNAL(rowsMoved(QModelIndex , int , int , QModelIndex, int)),this,SLOT(propertiesMoved(QModelIndex,int,int,QModelIndex,int)));
  connect(_columnModel,SIGNAL(modelReset()),this,SLOT(propertiesReset()));
  initModel();

}

void VisibleSectionsModel::initModel(int selectedColumnIndex) {
  int index=0;
  clear();
  addItem(tr("All columns"),QVariant(-1));

  for(int i = 0 ; i < _columnModel->rowCount() ; ++i) {
    if(_columnModel->isColumnVisible(i)) {
      tlp::PropertyInterface* property = _columnModel->propertyForIndex(_columnModel->index(i));

      if(property != NULL) {
        addItem(tlp::tlpStringToQString(property->getName()),QVariant(i));

        //If the last added index match with the column to select
        if(i == selectedColumnIndex) {
          index = count()-1;
        }
      }
    }
  }

  setCurrentIndex(index);
}

void VisibleSectionsModel::propertiesInserted ( const QModelIndex &, int , int ) {
  initModel();
}

void VisibleSectionsModel::propertiesMoved ( const QModelIndex & , int , int , const QModelIndex & , int ) {
  initModel();
}

void VisibleSectionsModel::propertiesRemoved ( const QModelIndex & , int , int ) {
  initModel();
}

void VisibleSectionsModel::propertiesDataChanged(const QModelIndex &, const QModelIndex &) {
  initModel();
}

void VisibleSectionsModel::propertiesReset() {
  initModel();
}
