#include "SpreadViewTableWidget.h"
#include "ui_SpreadViewTableWidget.h"
#include "GraphTableModel.h"
#include "GraphTableWidget.h"

#include <tulip/PropertyCreationDialog.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/BooleanProperty.h>

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QSortFilterProxyModel>
#include <QtCore/QTime>

#include "TulipTableWidgetColumnSelectionModel.h"

#include <set>

using namespace std;
using namespace tlp;

SpreadViewTableWidget::SpreadViewTableWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SpreadViewTableWidget),_tableColumnModel(NULL) {
  ui->setupUi(this);
  //Edges table
  ui->tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->tableView->horizontalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showPropertiesContextMenu(QPoint)));
  ui->tableView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->tableView->verticalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showElementsContextMenu(QPoint)));
  ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showTableContextMenu(QPoint)));

  //Column selection widgets
  ui->columnEditionWidget->setVisible(false);
  ui->columnEditionWidget->setEnabled(false);


  //Filtering
  connect(ui->showOnlySelectedElementsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(filterElements()));
  connect(ui->filterPatternLineEdit,SIGNAL(returnPressed()),this,SLOT(filterElements()));
  connect(ui->filterPushButton,SIGNAL(clicked()),this,SLOT(filterElements()));
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
  ui->filterPatternLineEdit->setPlaceholderText(QApplication::translate("SpreadViewTableWidget", "Input a filter pattern", 0, QApplication::UnicodeUTF8));
#endif

}

SpreadViewTableWidget::~SpreadViewTableWidget() {
  delete ui;
}

DataSet SpreadViewTableWidget::getData()const {
  //Saving state of header view.
  DataSet data;
  QHeaderView *headerView = ui->tableView->horizontalHeader();

  for(int i = 0 ;  i < headerView->count() ; ++i) {
    DataSet columnData;
    columnData.set("hidden",headerView->isSectionHidden(i));
    data.set(QString::number(i).toStdString(),columnData);
  }

  return data;
}

void SpreadViewTableWidget::setData(Graph* graph,const DataSet &data,ElementType type) {
  ui->tableView->setGraph(graph,type);
  QHeaderView *headerView = ui->tableView->horizontalHeader();
  //Reload header view state
  pair<string, DataType*> columnDataSet;
  forEach(columnDataSet,data.getValues()) {
    bool conversionWork = false;
    QString value = QString::fromUtf8(columnDataSet.first.c_str());
    int column = value.toInt(&conversionWork);

    if(conversionWork) {
      DataSet columnData = *static_cast<DataSet*>(columnDataSet.second->value);

      if(columnData.exist("hidden")) {
        bool hidden=false;
        columnData.get("hidden",hidden);
        headerView->setSectionHidden(column,hidden);
      }
    }
  }

  TulipTableWidgetColumnSelectionModel* oldColumnModel = _tableColumnModel;
  _tableColumnModel = new TulipTableWidgetColumnSelectionModel(ui->tableView,this);
  ui->columnEditionWidget->setColumnSelectionModel(_tableColumnModel);
  ui->columnEditionWidget->setEnabled(true);

  if(oldColumnModel != NULL) {
    oldColumnModel->deleteLater();
  }

  //Search in column combobox.
  ui->comboBox->blockSignals(true);
  ui->comboBox->setColumnModel(_tableColumnModel);
  ui->comboBox->blockSignals(false);

}

TulipTableWidgetColumnSelectionWidget* SpreadViewTableWidget::columnEditionWidget() {
  return ui->columnEditionWidget;
}

GraphTableWidget* SpreadViewTableWidget::graphTableWidget() {
  return ui->tableView;
}

void SpreadViewTableWidget::update() {
  setEnabled(false);
  ui->tableView->update();
  setEnabled(true);
}



void SpreadViewTableWidget::showElementsContextMenu(GraphTableWidget* tableWidget,int clickedRowIndex,const QPoint& position) {
  QMenu contextMenu(tableWidget);
  fillElementsContextMenu(contextMenu,tableWidget,clickedRowIndex);
  contextMenu.exec(tableWidget->mapToGlobal(position));
}
void SpreadViewTableWidget::fillElementsContextMenu(QMenu& menu,GraphTableWidget* tableWidget,int clickedRowIndex) {
  QAbstractItemModel* tulipTableModel = tableWidget->model();

  //Ensure the clicked index is always selected.
  if(!tableWidget->selectionModel()->isRowSelected(clickedRowIndex,QModelIndex())) {
    tableWidget->selectionModel()->select(tulipTableModel->index(clickedRowIndex,0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  }

  QString elementTypeLabel = tableWidget->graphModel()->elementType()==NODE?tr("node(s)"):tr("edge(s)");
  QModelIndexList rows = tableWidget->selectedRows(0);
  set<unsigned int> elements = tableWidget->indexListToIds(rows);
  QAction* selectOnGraph = menu.addAction(tr("Select"),this,SLOT(selectElements()));
  selectOnGraph->setToolTip(tr("Replace the graph selection by the ")+ elementTypeLabel + tr(" highlighted in the table."));
  GraphTableWidget::SelectionStatus status = tableWidget->selectionStatus(rows);
  QAction* addToSelection = menu.addAction(tr("Add to the graph selection"),this,SLOT(addToSelection()));
  addToSelection->setToolTip(tr("Add the ")+elementTypeLabel+tr(" highlighted in the table to the graph selection."));
  QAction* removeFromSelection = menu.addAction(tr("Remove from the graph selection"),this,SLOT(removeFromSelection()));
  removeFromSelection->setToolTip(tr("Remove the ") + elementTypeLabel + tr(" highlighted in the table from the graph selection."));

  if(status == GraphTableWidget::Selected) {
    addToSelection->setEnabled(false);
  }
  else if(status == GraphTableWidget::Unselected) {
    removeFromSelection->setEnabled(false);
  }

  QAction* highlightAction = menu.addAction(tr("Highlight selection"),this,SLOT(highlightElements()));
  highlightAction->setToolTip(tr("Highlight the ")+elementTypeLabel+tr(" selected in the graph"));

  if(tableWidget->elementType() == NODE) {

    QAction* copyAction=menu.addAction(tr("Clone"),this,SLOT(copyNodes()));
    copyAction->setToolTip(tr("Clone the ")+elementTypeLabel+tr(" highlighted"));

    //Group only available if there is more than one node selected.
    QAction *group = menu.addAction(tr("Group "),this,SLOT(group()));
    group->setToolTip(tr("Create a meta node with the highlighted nodes"));
    //Can only create a metanode if we have more than one node and if the current graph is not the root graph.
    group->setEnabled(rows.size()>1 && ui->tableView->graph()->getRoot() != ui->tableView->graph());

    QAction *ungroup = menu.addAction(tr("Ungroup "),this,SLOT(ungroup()));

    //If only one node is not a meta node cannot ungroup.
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end(); ++it) {
      if(!ui->tableView->graph()->isMetaNode(node(*it))) {
        ungroup->setEnabled(false);
        break;
      }
    }
  }

  menu.addAction(tr("Delete"),this,SLOT(deleteHighlightedElements()));
}


void SpreadViewTableWidget::showPropertiesContextMenu(GraphTableWidget* tableWidget,int clickedColumn,const QPoint& position) {
  QMenu contextMenu(tableWidget);
  fillPropertiesContextMenu(contextMenu,tableWidget,clickedColumn);
  contextMenu.exec(tableWidget->mapToGlobal(position));
}
void SpreadViewTableWidget::fillPropertiesContextMenu(QMenu& menu,GraphTableWidget* tulipTableWidget,int clickedColumn) {
  PropertyInterface* property = tulipTableWidget->graphModel()->propertyForIndex(clickedColumn);

  //Properties operations
  QAction *hideColumnAction = menu.addAction(tr("Hide"),this,SLOT(hideColumn()));
  hideColumnAction->setData(QVariant(clickedColumn));

  menu.addAction(tr("Create new property"),this,SLOT(createNewProperties()));

  QAction *copyToColumnAction = menu.addAction(tr("Copy to"),this,SLOT(copyColumnToOther()));
  copyToColumnAction->setData(QVariant(clickedColumn));

  QAction *setAllValuesAction = menu.addAction(tr("Set all values"),this,SLOT(setAllColumnValues()));
  setAllValuesAction->setData(QVariant(clickedColumn));

  QAction *clearColumnAction =menu.addAction(tr("Reset"),this,SLOT(resetColumn()));
  clearColumnAction->setData(QVariant(clickedColumn));


  QAction *deleteColumnAction =menu.addAction(tr("Delete"),this,SLOT(deleteColumn()));
  deleteColumnAction->setData(QVariant(clickedColumn));

  //Avoid to delete inherited properties
  if(property == NULL ) {
    hideColumnAction->setEnabled(false);
    copyToColumnAction->setEnabled(false);
    setAllValuesAction->setEnabled(false);
    clearColumnAction->setEnabled(false);
    deleteColumnAction->setEnabled(false);
  }
  else {
    if(property->getGraph() != ui->tableView->graph()) {
      deleteColumnAction->setEnabled(false);
    }
  }
}

void SpreadViewTableWidget::showElementsContextMenu(const QPoint& position) {
  QHeaderView* headerView = qobject_cast<QHeaderView*>(sender());
  int logicalIndex = headerView->logicalIndexAt(position);

  //If there is no row under the pointer don't show the menu.
  if(logicalIndex != -1) {
    showElementsContextMenu(ui->tableView,logicalIndex,position);
  }
}

void SpreadViewTableWidget::showPropertiesContextMenu(const QPoint& position) {
  QHeaderView* headerView = qobject_cast<QHeaderView*>(sender());
  showPropertiesContextMenu(ui->tableView,headerView->logicalIndexAt(position),position);
}

void SpreadViewTableWidget::showTableContextMenu(const QPoint& position) {
  GraphTableWidget* tableWidget = qobject_cast<GraphTableWidget*>(sender());
  int logicalIndex = tableWidget->indexAt(position).row();

  //If there is no row under the pointer don't show the menu.
  if(logicalIndex != -1) {
    QMenu contextMenu(tableWidget);
    fillElementsContextMenu(contextMenu,tableWidget,logicalIndex);
    contextMenu.exec(tableWidget->mapToGlobal(position));
  }
}



void SpreadViewTableWidget::hideColumn() {
  QAction* action = qobject_cast<QAction*>(sender());

  if(action!=NULL) {
    int index = action->data().toInt();
    ui->tableView->horizontalHeader()->setSectionHidden(index,true);
  }
}

void SpreadViewTableWidget::createNewProperties() {
  PropertyCreationDialog::createNewProperty(ui->tableView->graph(),ui->tableView);
}

void SpreadViewTableWidget::copyColumnToOther() {
  QAction* action = qobject_cast<QAction*>(sender());

  if(action!=NULL) {
    int index = action->data().toInt();
    GraphTableWidget* table = ui->tableView;
    PropertyInterface* property = table->graphModel()->propertyForIndex(index);
    Observable::holdObservers();
    CopyPropertyDialog::copyProperty(ui->tableView->graph(),property,true,table);
    Observable::unholdObservers();
  }
}

void SpreadViewTableWidget::setAllColumnValues() {
  QAction* action = qobject_cast<QAction*>(sender());

  if(action!=NULL) {
    int index = action->data().toInt();
    GraphTableWidget* tableWidget = ui->tableView;
    QAbstractItemModel* model = tableWidget->model();
    QDialog dialog(tableWidget);
    dialog.setLayout(new QVBoxLayout(&dialog));
    //Take the value of the first element.
    QWidget *editorWidget = tableWidget->itemDelegate()->createEditor(tableWidget,QStyleOptionViewItem(),model->index(0,index,QModelIndex()));
    dialog.layout()->addWidget(editorWidget);
    QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    dialog.layout()->addWidget(buttonBox);

    if(dialog.exec()== QDialog::Accepted) {
      Observable::holdObservers();

      for(int j = 0 ; j < model->rowCount() ; ++j) {
        if(!tableWidget->isRowHidden(j)) {
          tableWidget->itemDelegate()->setModelData(editorWidget,model,model->index(j,index));
        }
      }

      Observable::unholdObservers();
    }
  }
}

void SpreadViewTableWidget::resetColumn() {
  QAction* action = qobject_cast<QAction*>(sender());

  if(action!=NULL) {
    int index = action->data().toInt();
    GraphTableWidget* tableWidget = ui->tableView;
    GraphTableModel* model = tableWidget->graphModel();
    PropertyInterface* property = model->propertyForIndex(index);

    if(model->elementType() == NODE) {
      property->setAllNodeStringValue(property->getNodeDefaultStringValue());
    }
    else {
      property->setAllEdgeStringValue(property->getEdgeDefaultStringValue());
    }
  }
}

void SpreadViewTableWidget::deleteColumn() {
  QAction* action = qobject_cast<QAction*>(sender());

  if(action!=NULL) {
    int index = action->data().toInt();
    GraphTableModel* model = ui->tableView->graphModel();
    PropertyInterface* property = model->propertyForIndex(index);
    string propertyName = property->getName();
    Observable::holdObservers();
    property->getGraph()->delLocalProperty(propertyName);
    Observable::unholdObservers();
  }
}

void SpreadViewTableWidget::selectElements() {
  GraphTableWidget *tableWidget = ui->tableView;
  updateSelectionForHighlightedElements(tableWidget,true,true);
}

void SpreadViewTableWidget::addToSelection() {
  GraphTableWidget *tableWidget = ui->tableView;
  updateSelectionForHighlightedElements(tableWidget,true);
}

void SpreadViewTableWidget::removeFromSelection() {
  GraphTableWidget *tableWidget = ui->tableView;
  updateSelectionForHighlightedElements(tableWidget,false);
}

void SpreadViewTableWidget::updateSelectionForHighlightedElements(GraphTableWidget* tableWidget,bool select,bool clearOldSelection) {
  BooleanProperty* selectionProperty = ui->tableView->graph()->getProperty<BooleanProperty>("viewSelection");
  set<unsigned int> elements = tableWidget->indexListToIds(tableWidget->selectedRows(0));
  Observable::holdObservers();

  if(clearOldSelection) {
    if(tableWidget->elementType() == NODE) {
      selectionProperty->setAllNodeValue(false);
    }
    else {
      selectionProperty->setAllEdgeValue(false);
    }
  }

  for(set<unsigned int>::iterator it = elements.begin(); it != elements.end(); ++it) {
    if(tableWidget->elementType() == NODE) {
      selectionProperty->setNodeValue(node(*it),select);
    }
    else {
      selectionProperty->setEdgeValue(edge(*it),select);
    }
  }

  Observable::unholdObservers();
}

void SpreadViewTableWidget::highlightElements() {
  GraphTableWidget *tableWidget = ui->tableView;
  BooleanProperty* selectionProperty = ui->tableView->graph()->getProperty<BooleanProperty>("viewSelection");
  set<unsigned int> ids;

  if(tableWidget->elementType()==NODE) {
    node n;
    forEach(n,selectionProperty->getNodesEqualTo(true,ui->tableView->graph())) {
      ids.insert(n.id);
    }
  }
  else {
    edge e;
    forEach(e,selectionProperty->getEdgesEqualTo(true,ui->tableView->graph())) {
      ids.insert(e.id);
    }
  }

  tableWidget->highlightAndDisplayElements(ids);
}

void SpreadViewTableWidget::deleteHighlightedElements() {
  GraphTableWidget *tableWidget = ui->tableView;
  QModelIndexList indexes = tableWidget->selectedRows(0);
  Observable::holdObservers();
  deleteHighlightedElements(indexes,tableWidget,false);
  Observable::unholdObservers();
}

void SpreadViewTableWidget::deleteHighlightedElements(const QModelIndexList& elements,GraphTableWidget *tableWidget ,bool delAll) {
  //Get the elements list
  set<unsigned int> elementsList = tableWidget->indexListToIds(elements);
  Observable::holdObservers();

  for(set<unsigned int>::iterator it = elementsList.begin(); it != elementsList.end(); ++it) {
    if(*it != UINT_MAX) {
      if(tableWidget->elementType() == NODE) {
        ui->tableView->graph()->delNode(node(*it),delAll);
      }
      else {
        ui->tableView->graph()->delEdge(edge(*it),delAll);
      }
    }
  }

  Observable::unholdObservers();
}

void SpreadViewTableWidget::copyNodes() {
  GraphTableWidget *tableWidget = ui->tableView;
  QModelIndexList rows = tableWidget->selectedRows(0);
  set<unsigned int> elements = tableWidget->indexListToIds(rows);
  set<unsigned int> createdElements;
  Observable::holdObservers();

  for(set<unsigned int>::iterator it = elements.begin(); it != elements.end(); ++it) {
    node orig = node(*it);
    node dest = ui->tableView->graph()->addNode();
    createdElements.insert(dest.id);
    PropertyInterface* property;
    forEach(property,ui->tableView->graph()->getObjectProperties()) {
      property->setNodeStringValue(dest,property->getNodeStringValue(orig));
    }
  }

  Observable::unholdObservers();
  tableWidget->highlightAndDisplayElements(createdElements);
}

void SpreadViewTableWidget::group() {
  GraphTableWidget *tableWidget = ui->tableView;
  QModelIndexList rows = tableWidget->selectedRows(0);
  set<unsigned int> elements = tableWidget->indexListToIds(rows);
  set<node> nodes;

  for(set<unsigned int>::iterator it = elements.begin(); it != elements.end(); ++it) {
    nodes.insert(node(*it));
  }

  Observable::holdObservers();
  node createdMetanode = ui->tableView->graph()->createMetaNode(nodes);
  Observable::unholdObservers();
  set<unsigned int> metanodeId;
  metanodeId.insert(createdMetanode.id);
  tableWidget->highlightAndDisplayElements(metanodeId);
}

void SpreadViewTableWidget::ungroup() {
  GraphTableWidget *tableWidget = ui->tableView;
  QModelIndexList rows = tableWidget->selectedRows(0);
  set<unsigned int> elements = tableWidget->indexListToIds(rows);
  set<unsigned int> metanodeNodes;
  set<unsigned int> metanodeEdges;
  Observable::holdObservers();

  for(set<unsigned int>::iterator it = elements.begin(); it != elements.end(); ++it) {
    Graph* metanodeGraph = ui->tableView->graph()->getNodeMetaInfo(node(*it));
    node n;
    forEach(n,metanodeGraph->getNodes()) {
      metanodeNodes.insert(n.id);
    }
    edge e;
    forEach(e,metanodeGraph->getEdges()) {
      metanodeEdges.insert(e.id);
    }
    ui->tableView->graph()->openMetaNode(node(*it));
  }

  Observable::unholdObservers();
  ui->tableView->highlightAndDisplayElements(metanodeNodes);
}

void SpreadViewTableWidget::updateElementVisibility(int state) {
  showOnlySelectedElements(state == Qt::Checked);
}

void SpreadViewTableWidget::showOnlySelectedElements(bool) {
  assert(ui->tableView->graph() != NULL);

}

void SpreadViewTableWidget::filterElements() {
  setEnabled(false);
  QApplication::processEvents();
  ui->tableView->setRowFilter(QRegExp(ui->filterPatternLineEdit->text()),(ui->showOnlySelectedElementsCheckBox->checkState() == Qt::Checked),ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt());
  setEnabled(true);
  QApplication::processEvents();
}

bool SpreadViewTableWidget::displayOnlySelectedElements()const {
  return ui->showOnlySelectedElementsCheckBox->checkState()==Qt::Checked;
}

QRegExp SpreadViewTableWidget::elementValuesFilter()const {
  return QRegExp(ui->filterPatternLineEdit->text());
}

void SpreadViewTableWidget::filterColumnChanged(int) {
}

void SpreadViewTableWidget::invalidateFilter() {

}
