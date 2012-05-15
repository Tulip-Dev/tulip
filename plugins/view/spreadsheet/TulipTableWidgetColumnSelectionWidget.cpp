#include "TulipTableWidgetColumnSelectionWidget.h"
#include "ui_TulipTableWidgetColumnSelectionWidget.h"
#include "TulipTableWidgetColumnSelectionModel.h"
#include "GraphTableModel.h"
#include <QtGui/QMenu>
#include <QtGui/QStyledItemDelegate>
#include <tulip/PropertyCreationDialog.h>
#include <tulip/CopyPropertyDialog.h>
#include <cassert>
#include <string>


/**
  * @brief Internal delegate cobjec to draw the icon on the left instead of the right.
  **/
class TulipTableWidgetColumnSelectionWidgetItemDelegate : public QStyledItemDelegate {
public :
  TulipTableWidgetColumnSelectionWidgetItemDelegate(QObject * parent) : QStyledItemDelegate(parent) {
  }

  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);
    opt.decorationPosition= QStyleOptionViewItem::Right;
    const QWidget *widget = NULL;

    if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option)) {
      widget = v3->widget;
    }

    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
  }
};

TulipTableWidgetColumnSelectionWidget::TulipTableWidgetColumnSelectionWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TableColumnSelectionWidget),_tableColumnModel(NULL) {
  ui->setupUi(this);
  connect(ui->checkUncheckAllCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUncheckAllStateChanged(int)));
  //Predefined filters
  connect(ui->dataPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showDataProperties()));
  connect(ui->visualPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showVisualProperties()));

  //User pattern
  connect(ui->customPatternLineEdit,SIGNAL(textChanged( QString)),this,SLOT(showCustomPatternProperties(QString)));

  ui->listView->setItemDelegate(new TulipTableWidgetColumnSelectionWidgetItemDelegate(ui->listView));

  ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->listView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
  ui->customPatternLineEdit->setPlaceholderText(QApplication::translate("TableColumnSelectionWidget", "Input a custom filter", 0, QApplication::UnicodeUTF8));
#endif
}

TulipTableWidgetColumnSelectionWidget::~TulipTableWidgetColumnSelectionWidget() {
  delete ui;
}

void TulipTableWidgetColumnSelectionWidget::setColumnSelectionModel(TulipTableWidgetColumnSelectionModel* model) {
  assert(model != NULL);
  QAbstractItemModel* oldModel = ui->listView->model();

  if(oldModel) {
    disconnect(oldModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(updateCheckUncheckAllButtonState()));
  }

  _tableColumnModel = model;
  ui->listView->setModel(_tableColumnModel);
  updateCheckUncheckAllButtonState();
  connect(_tableColumnModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(updateCheckUncheckAllButtonState()));
}

void TulipTableWidgetColumnSelectionWidget::showVisualProperties() {
  selectColumns(QRegExp("^view"),true);
}

void TulipTableWidgetColumnSelectionWidget::showDataProperties() {
  selectColumns(QRegExp("^view"),false);
}

void TulipTableWidgetColumnSelectionWidget::showCustomPatternProperties(const QString& pattern) {
  selectColumns(QRegExp(pattern),true);
}

void TulipTableWidgetColumnSelectionWidget::selectColumns(const QRegExp& regExp,bool match) {
  assert(_tableColumnModel != NULL);

  for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i) {
    _tableColumnModel->setColumnVisible(i,match == (regExp.indexIn(_tableColumnModel->data(_tableColumnModel->index(i)).toString())!= -1));
  }
}

void TulipTableWidgetColumnSelectionWidget::checkUncheckAllStateChanged(int state) {
  bool showColumn = state == Qt::Unchecked?false:true;

  for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i) {
    _tableColumnModel->setColumnVisible(i,showColumn);
  }
}

void TulipTableWidgetColumnSelectionWidget::updateCheckUncheckAllButtonState() {
  Qt::CheckState state=Qt::Checked;

  for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i) {
    Qt::CheckState columnCheckState = static_cast<Qt::CheckState>(_tableColumnModel->data(_tableColumnModel->index(i),Qt::CheckStateRole).toInt());

    if(i==0 ) {
      //If the first element is not checked so uncheck it.
      if(columnCheckState == Qt::Unchecked) {
        state = Qt::Unchecked;
      }
    }
    else {
      if(columnCheckState != state) {
        state = Qt::PartiallyChecked;
        break;
      }
    }
  }

  ui->checkUncheckAllCheckBox->blockSignals(true);
  ui->checkUncheckAllCheckBox->setCheckState(state);
  ui->checkUncheckAllCheckBox->blockSignals(false);
}


void TulipTableWidgetColumnSelectionWidget::showContextMenu(const QPoint& position) {

  QMenu menu(ui->listView);
  QModelIndex index = ui->listView->indexAt(position);

  //Ensure the clicked index is always selected.
  if(!ui->listView->selectionModel()->isRowSelected(index.row(),QModelIndex())) {
    ui->listView->selectionModel()->select(_tableColumnModel->index(index.row(),0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  }

  QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);

  //Properties operations
  menu.addAction(tr("Show"),this,SLOT(showSelectedColumns()));
  menu.addAction(tr("Show selected only"),this,SLOT(showOnlySelectedColumns()));
  menu.addAction(tr("Hide"),this,SLOT(hideSelectedColumns()));

  menu.addAction(tr("Create new property"),this,SLOT(createNewColumn()));

  QAction *copyToColumnAction = menu.addAction(tr("Copy to"),this,SLOT(copyColumn()));

  if(rows.size()!=1) {
    copyToColumnAction->setEnabled(false);
  }

  QAction *deleteColumnAction =menu.addAction(tr("Delete"));
  connect(deleteColumnAction,SIGNAL(triggered()),this,SLOT(deleteSelectedColumns()),Qt::QueuedConnection);

  //Avoid to delete inherited properties
  for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it) {
    //If the property is an inherited property don't erase it
    if(_tableColumnModel->propertyForIndex(*it)->getGraph() != _tableColumnModel->graphTableModel()->graph()) {
      deleteColumnAction->setEnabled(false);
    }
  }

  menu.exec(ui->listView->mapToGlobal(position));

}
void TulipTableWidgetColumnSelectionWidget::showSelectedColumns() {
  QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);

  if(!rows.isEmpty()) {
    for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it) {
      _tableColumnModel->setColumnVisible((*it).row(),true);
    }
  }
}

void TulipTableWidgetColumnSelectionWidget::showOnlySelectedColumns() {
  QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);
  std::set<int> columnsToShow;

  for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it) {
    columnsToShow.insert((*it).row());
  }

  for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i) {
    _tableColumnModel->setColumnVisible(i,columnsToShow.find(i)!= columnsToShow.end());
  }
}

void TulipTableWidgetColumnSelectionWidget::hideSelectedColumns() {
  QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);

  if(!rows.isEmpty()) {
    for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it) {
      _tableColumnModel->setColumnVisible((*it).row(),false);
    }
  }
}

void TulipTableWidgetColumnSelectionWidget::createNewColumn() {
  tlp::PropertyCreationDialog::createNewProperty(_tableColumnModel->graphTableModel()->graph(),this);
}


void TulipTableWidgetColumnSelectionWidget::copyColumn() {
  QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);

  if(rows.size()==1) {
    tlp::PropertyInterface* property = _tableColumnModel->propertyForIndex(rows.front());
    tlp::Observable::holdObservers();
    tlp::CopyPropertyDialog::copyProperty(_tableColumnModel->graphTableModel()->graph(),property,true,this);
    tlp::Observable::unholdObservers();
  }
}


void TulipTableWidgetColumnSelectionWidget::deleteSelectedColumns() {
  QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);
  ui->listView->selectionModel()->clearSelection();
  tlp::Observable::holdObservers();
  std::vector<tlp::PropertyInterface*> toDelete;

  for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it) {
    tlp::PropertyInterface* property = _tableColumnModel->propertyForIndex(*it);

    //Check if the property exist in the graph (avoid to delete a function two time).
    if(property!= NULL && property->getGraph()->existProperty(property->getName())) {
      toDelete.push_back(property);
    }
  }

  //Need to store the elements to delete as the indexes can be updated during the deletion.
  for(std::vector<tlp::PropertyInterface*>::iterator it = toDelete.begin() ; it != toDelete.end() ; ++it) {
    (*it)->getGraph()->delLocalProperty(std::string((*it)->getName()));
  }

  tlp::Observable::unholdObservers();
}
