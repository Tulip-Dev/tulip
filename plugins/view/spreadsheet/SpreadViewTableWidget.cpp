#include "SpreadViewTableWidget.h"
#include "ui_SpreadViewTableWidget.h"
#include "GraphTableModel.h"
#include "GraphTableWidget.h"
#include "VisibleSectionsModel.h"

#include <tulip/PropertyCreationDialog.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/BooleanProperty.h>

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QSortFilterProxyModel>

#include "TulipTableWidgetColumnSelectionModel.h"

#include <set>

using namespace std;
using namespace tlp;

SpreadViewTableWidget::SpreadViewTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpreadViewTableWidget),_tableColumnModel(NULL),_selectionProperty(NULL),_reloadSelectionProperty(false)
{
    ui->setupUi(this);
    //Edges table
    ui->tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView->horizontalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showPropertiesContextMenu(QPoint)));
    ui->tableView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView->verticalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showElementsContextMenu(QPoint)));
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showTableContextMenu(QPoint)));

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(filterColumnChanged(int)));

    //Column selection widgets
    ui->columnEditionWidget->setVisible(false);
    ui->columnEditionWidget->setEnabled(false);

    connect(ui->showOnlySelectedElementsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateElementVisibility(int)));
    connect(ui->filterPatternLineEdit,SIGNAL(returnPressed()),this,SLOT(filterElements()));
    connect(ui->filterPushButton,SIGNAL(clicked()),this,SLOT(filterElements()));

    ui->filterProgressBar->setVisible(false);
}

SpreadViewTableWidget::~SpreadViewTableWidget()
{
    delete ui;
}

void SpreadViewTableWidget::setData(tlp::Graph* graph,tlp::ElementType type){
    ui->tableView->setGraph(graph,type);    
    TulipTableWidgetColumnSelectionModel* oldColumnModel = _tableColumnModel;
    _tableColumnModel = new TulipTableWidgetColumnSelectionModel(ui->tableView,this);
    ui->columnEditionWidget->setColumnSelectionModel(_tableColumnModel);
    ui->columnEditionWidget->setEnabled(true);

    if(oldColumnModel != NULL){
        oldColumnModel->deleteLater();
    }

    VisibleSectionsModel *visibleColumnsModel = new VisibleSectionsModel(_tableColumnModel,this);
    visibleColumnsModel->setDynamicSortFilter(true);
    ui->comboBox->setModel(visibleColumnsModel);

    //Connect data modification events to ensure filtering il always up to date.
    connect(ui->tableView->graphModel(),SIGNAL(columnsAboutToBeInserted ( QModelIndex , int , int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
    connect(ui->tableView->graphModel(),SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),this,SLOT(rowsInserted(QModelIndex,int,int)));
    connect(ui->tableView->graphModel(),SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),this,SLOT(rowsInserted(QModelIndex,int,int)));
    connect(ui->tableView->graphModel(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(dataChanged(QModelIndex,QModelIndex)));
    connect(ui->tableView->graphModel(),SIGNAL(modelReset()),this,SLOT(modelReset()));

}

TulipTableWidgetColumnSelectionWidget* SpreadViewTableWidget::columnEditionWidget(){
    return ui->columnEditionWidget;
}

GraphTableWidget* SpreadViewTableWidget::graphTableWidget(){
    return ui->tableView;
}

void SpreadViewTableWidget::update(){
    ui->tableView->graphModel()->update();
}



void SpreadViewTableWidget::showElementsContextMenu(GraphTableWidget* tableWidget,int clickedRowIndex,const QPoint& position){
    QMenu contextMenu(tableWidget);
    fillElementsContextMenu(contextMenu,tableWidget,clickedRowIndex);
    contextMenu.exec(tableWidget->mapToGlobal(position));
}
void SpreadViewTableWidget::fillElementsContextMenu(QMenu& menu,GraphTableWidget* tableWidget,int clickedRowIndex){
    QAbstractItemModel* tulipTableModel = tableWidget->model();
    //Ensure the clicked index is always selected.
    if(!tableWidget->selectionModel()->isRowSelected(clickedRowIndex,QModelIndex())){
        tableWidget->selectionModel()->select(tulipTableModel->index(clickedRowIndex,0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
    QModelIndexList rows = tableWidget->selectedRows(0);
    std::cout<<__PRETTY_FUNCTION__<<" "<<__LINE__<<" "<<rows.size()<<std::endl;
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    QAction* selectOnGraph = menu.addAction(tr("Select on graph"),this,SLOT(selectElements()));
    selectOnGraph->setToolTip(tr("Replace the current graph selection by the elements highlighted in the table."));
    GraphTableWidget::SelectionStatus status = tableWidget->selectionStatus(rows);
    QAction* addToSelection = menu.addAction(tr("Add to graph selection"),this,SLOT(addToSelection()));
    addToSelection->setToolTip(tr("Add the elements highlighted in the table to the graph selection."));
    QAction* removeFromSelection = menu.addAction(tr("Remove from graph selection"),this,SLOT(removeFromSelection()));
    removeFromSelection->setToolTip(tr("Remove the elements highlighted in the table from the graph selection."));
    if(status == GraphTableWidget::Selected){
        addToSelection->setEnabled(false);
    }else if(status == GraphTableWidget::Unselected){
        removeFromSelection->setEnabled(false);
    }
    menu.addAction(tr("Highlight graph selection in the table"),this,SLOT(highlightElements()));

    if(tableWidget->elementType() == NODE){

        menu.addAction(tr("Copy"),this,SLOT(copyNodes()));

        //Group only available if there is more than one node selected.
        QAction *group = menu.addAction(tr("Group "),this,SLOT(group()));
        group->setEnabled(rows.size()>1);

        QAction *ungroup = menu.addAction(tr("Ungroup "),this,SLOT(ungroup()));
        //If only one node is not a meta node cannot ungroup.
        for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
            if(!ui->tableView->graph()->isMetaNode(node(*it))){
                ungroup->setEnabled(false);
                break;
            }
        }
    }

    menu.addAction(QIcon(":/i_del.png"),tr("Delete"),this,SLOT(deleteHighlightedElements()));
}


void SpreadViewTableWidget::showPropertiesContextMenu(GraphTableWidget* tableWidget,int clickedColumn,const QPoint& position){
    QMenu contextMenu(tableWidget);
    fillPropertiesContextMenu(contextMenu,tableWidget,clickedColumn);
    contextMenu.exec(tableWidget->mapToGlobal(position));
}
void SpreadViewTableWidget::fillPropertiesContextMenu(QMenu& menu,GraphTableWidget* tulipTableWidget,int clickedColumn){
    //Properties operations
    QAction *hideColumnAction = menu.addAction(tr("Hide column"),this,SLOT(hideColumn()));
    hideColumnAction->setData(QVariant(clickedColumn));

    menu.addAction(tr("Create new column"),this,SLOT(createNewProperties()));

    QAction *copyToColumnAction = menu.addAction(tr("Copy to"),this,SLOT(copyColumnToOther()));
    copyToColumnAction->setData(QVariant(clickedColumn));

    QAction *setAllValuesAction = menu.addAction(tr("Set all values"),this,SLOT(setAllColumnValues()));
    setAllValuesAction->setData(QVariant(clickedColumn));

    QAction *clearColumnAction =menu.addAction(tr("Reset column"),this,SLOT(resetColumn()));
    clearColumnAction->setData(QVariant(clickedColumn));


    QAction *deleteColumnAction =menu.addAction(tr("Delete column"),this,SLOT(deleteColumn()));
    deleteColumnAction->setData(QVariant(clickedColumn));
    //Avoid to delete inherited properties
    if(tulipTableWidget->graphModel()->propertyForIndex(clickedColumn)->getGraph() != ui->tableView->graph()){
        deleteColumnAction->setEnabled(false);
    }
}

void SpreadViewTableWidget::showElementsContextMenu(const QPoint& position){
    QHeaderView* headerView = qobject_cast<QHeaderView*>(sender());
    showElementsContextMenu(ui->tableView,headerView->logicalIndexAt(position),position);
}

void SpreadViewTableWidget::showPropertiesContextMenu(const QPoint& position){
    QHeaderView* headerView = qobject_cast<QHeaderView*>(sender());
    showPropertiesContextMenu(ui->tableView,headerView->logicalIndexAt(position),position);
}

void SpreadViewTableWidget::showTableContextMenu(const QPoint& position){
    GraphTableWidget* tableWidget = qobject_cast<GraphTableWidget*>(sender());
    QMenu contextMenu(tableWidget);
    fillElementsContextMenu(contextMenu,tableWidget,tableWidget->indexAt(position).row());
    contextMenu.exec(tableWidget->mapToGlobal(position));
}



void SpreadViewTableWidget::hideColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        ui->tableView->horizontalHeader()->setSectionHidden(index,true);
    }
}

void SpreadViewTableWidget::createNewProperties(){
    PropertyCreationDialog::createNewProperty(ui->tableView->graph(),ui->tableView);
}

void SpreadViewTableWidget::copyColumnToOther(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        GraphTableWidget* table = ui->tableView;
        PropertyInterface* property = table->graphModel()->propertyForIndex(index);
        CopyPropertyDialog::copyProperty(ui->tableView->graph(),property,true,table);
    }
}

void SpreadViewTableWidget::setAllColumnValues(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        GraphTableWidget* tableWidget = ui->tableView;
        GraphTableModel* model = tableWidget->graphModel();
        QDialog dialog(tableWidget);
        dialog.setLayout(new QVBoxLayout(&dialog));
        //Take the value of the first element.
        QWidget *editorWidget = tableWidget->itemDelegate()->createEditor(tableWidget,QStyleOptionViewItem(),model->index(0,index,QModelIndex()));
        dialog.layout()->addWidget(editorWidget);
        QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        dialog.layout()->addWidget(buttonBox);
        if(dialog.exec()== QDialog::Accepted){
            Observable::holdObservers();
            for(int j = 0 ; j < model->rowCount() ; ++j){
                if(!tableWidget->isRowHidden(j)){
                    tableWidget->itemDelegate()->setModelData(editorWidget,model,model->index(j,index));
                }
            }
            Observable::unholdObservers();
        }
    }
}

void SpreadViewTableWidget::resetColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        GraphTableWidget* tableWidget = ui->tableView;
        GraphTableModel* model = tableWidget->graphModel();
        PropertyInterface* property = model->propertyForIndex(index);
        if(model->elementType() == NODE){
            property->setAllNodeStringValue(property->getNodeDefaultStringValue());
        }else{
            property->setAllEdgeStringValue(property->getEdgeDefaultStringValue());
        }
    }
}

void SpreadViewTableWidget::deleteColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        GraphTableModel* model = ui->tableView->graphModel();
        PropertyInterface* property = model->propertyForIndex(index);
        string propertyName = property->getName();
        Observable::holdObservers();
        property->getGraph()->delLocalProperty(propertyName);
        Observable::unholdObservers();
    }
}

void SpreadViewTableWidget::selectElements(){
    GraphTableWidget *tableWidget = ui->tableView;
    updateSelectionForHighlightedElements(tableWidget,true,true);
}

void SpreadViewTableWidget::addToSelection(){
    GraphTableWidget *tableWidget = ui->tableView;
    updateSelectionForHighlightedElements(tableWidget,true);
}

void SpreadViewTableWidget::removeFromSelection(){
    GraphTableWidget *tableWidget = ui->tableView;
    updateSelectionForHighlightedElements(tableWidget,false);
}

void SpreadViewTableWidget::updateSelectionForHighlightedElements(GraphTableWidget* tableWidget,bool select,bool clearOldSelection){
    BooleanProperty* selectionProperty = ui->tableView->graph()->getProperty<BooleanProperty>("viewSelection");
    set<unsigned int> elements = tableWidget->indexListToIds(tableWidget->selectedRows(0));
    Observable::holdObservers();
    if(clearOldSelection){
        if(tableWidget->elementType() == NODE){
            selectionProperty->setAllNodeValue(false);
        }else{
            selectionProperty->setAllEdgeValue(false);
        }
    }
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        if(tableWidget->elementType() == NODE){
            selectionProperty->setNodeValue(node(*it),select);
        }else{
            selectionProperty->setEdgeValue(edge(*it),select);
        }
    }
    Observable::unholdObservers();
}

void SpreadViewTableWidget::highlightElements(){
    GraphTableWidget *tableWidget = ui->tableView;
    BooleanProperty* selectionProperty = ui->tableView->graph()->getProperty<BooleanProperty>("viewSelection");
    set<unsigned int> ids;
    if(tableWidget->elementType()==NODE){
        node n;
        forEach(n,selectionProperty->getNodesEqualTo(true,ui->tableView->graph())){
            ids.insert(n.id);
        }
    }else{
        edge e;
        forEach(e,selectionProperty->getEdgesEqualTo(true,ui->tableView->graph())){
            ids.insert(e.id);
        }
    }
    tableWidget->highlightAndDisplayElements(ids);
}

void SpreadViewTableWidget::deleteHighlightedElements(){
    GraphTableWidget *tableWidget = ui->tableView;
    QModelIndexList indexes = tableWidget->selectedRows(0);
    Observable::holdObservers();
    deleteHighlightedElements(indexes,tableWidget,false);
    Observable::unholdObservers();
}

void SpreadViewTableWidget::deleteHighlightedElements(const QModelIndexList& elements,GraphTableWidget *tableWidget ,bool delAll){
    //Get the elements list
    set<unsigned int> elementsList = tableWidget->indexListToIds(elements);
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elementsList.begin(); it != elementsList.end(); ++it){
        if(*it != UINT_MAX){
            if(tableWidget->elementType() == NODE){
                ui->tableView->graph()->delNode(node(*it),delAll);
            }else{
                ui->tableView->graph()->delEdge(edge(*it),delAll);
            }
        }
    }
    Observable::unholdObservers();
}

void SpreadViewTableWidget::copyNodes(){
    GraphTableWidget *tableWidget = ui->tableView;
    QModelIndexList rows = tableWidget->selectedRows(0);
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    set<unsigned int> createdElements;
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        node orig = node(*it);
        node dest = ui->tableView->graph()->addNode();
        createdElements.insert(dest.id);
        PropertyInterface* property;
        forEach(property,ui->tableView->graph()->getObjectProperties()){
            property->setNodeStringValue(dest,property->getNodeStringValue(orig));
        }
    }
    Observable::unholdObservers();
    tableWidget->highlightAndDisplayElements(createdElements);
}

void SpreadViewTableWidget::group(){
    GraphTableWidget *tableWidget = ui->tableView;
    QModelIndexList rows = tableWidget->selectedRows(0);
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    set<node> nodes;
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        nodes.insert(node(*it));
    }
    Observable::holdObservers();
    node createdMetanode = ui->tableView->graph()->createMetaNode(nodes);
    Observable::unholdObservers();
    set<unsigned int> metanodeId;
    metanodeId.insert(createdMetanode.id);
    tableWidget->highlightAndDisplayElements(metanodeId);
}

void SpreadViewTableWidget::ungroup(){
    GraphTableWidget *tableWidget = ui->tableView;
    QModelIndexList rows = tableWidget->selectedRows(0);
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    set<unsigned int> metanodeNodes;
    set<unsigned int> metanodeEdges;
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        Graph* metanodeGraph = ui->tableView->graph()->getNodeMetaInfo(node(*it));
        node n;
        forEach(n,metanodeGraph->getNodes()){
            metanodeNodes.insert(n.id);
        }
        edge e;
        forEach(e,metanodeGraph->getEdges()){
            metanodeEdges.insert(e.id);
        }
        ui->tableView->graph()->openMetaNode(node(*it));
    }
    Observable::unholdObservers();
    ui->tableView->highlightAndDisplayElements(metanodeNodes);
}

void SpreadViewTableWidget::updateElementVisibility(int state){
    showOnlySelectedElements(state == Qt::Checked);
}

void SpreadViewTableWidget::showOnlySelectedElements(bool show){
    assert(ui->tableView->graph() != NULL);
    if(show){
        //Listen for property creation destruction.
        ui->tableView->graph()->addListener(this);
        if(ui->tableView->graph()->existProperty("viewSelection")){
            _selectionProperty = ui->tableView->graph()->getProperty<BooleanProperty>("viewSelection");
            _selectionProperty->addListener(this);
        }
    }else{
        //No need to listen graph anymore.
        ui->tableView->graph()->removeListener(this);
        if(_selectionProperty != NULL){
            _selectionProperty->removeListener(this);
            _selectionProperty = NULL;
        }
    }
    //Need to update filter.
    _updatedElements.setAll(true);
    updateFilters();
}

void SpreadViewTableWidget::updateFilters(){
    GraphTableModel* tableModel = ui->tableView->graphModel();

    bool displayOnlySelected = displayOnlySelectedElements();
    QRegExp regExp = elementValuesFilter();
    ui->filterProgressBar->setVisible(true);
    int totalOfElements = tableModel->rowCount();
    ui->filterProgressBar->setRange(0,totalOfElements);
    int searchColumn = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
    for(int i= 0; i < tableModel->rowCount(); ++i){
        unsigned int id =tableModel->idForIndex(i);
        if(_updatedElements.get(id)){
            bool display = true;
            if(displayOnlySelected && _selectionProperty!=NULL){
                display =  tableModel->elementType() == NODE ? _selectionProperty->getNodeValue(node(id)):_selectionProperty->getEdgeValue(edge(id));
            }
            bool match = false;
            if(regExp.isEmpty() ){
                match = true;
            }else{                                
                match |= regExp.exactMatch(tableModel->data(tableModel->index(i,searchColumn)).toString());
            }
            ui->tableView->setRowHidden(i,!(display && match));
        }
        if(i%200 == 0){
            ui->filterProgressBar->setValue(ui->filterProgressBar->value()+200);
        }
    }
    _updatedElements.setAll(false);
    ui->filterProgressBar->setVisible(false);
}

void SpreadViewTableWidget::treatEvent(const Event &ev){
    const PropertyEvent* propEvt = dynamic_cast<const PropertyEvent*>(&ev);
    if (propEvt) {
        switch(propEvt->getType()) {
        case PropertyEvent::TLP_AFTER_SET_NODE_VALUE:
            if(ui->tableView->graphModel()->elementType() == NODE){
                _updatedElements.set(propEvt->getNode(),true);
            }
            break;
        case PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE:
            if(ui->tableView->graphModel()->elementType() == NODE){
                _updatedElements.setAll(true);
            }
            break;
        case PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE:
            if(ui->tableView->graphModel()->elementType() == EDGE){
                _updatedElements.setAll(true);
            }
            break;
        case PropertyEvent::TLP_AFTER_SET_EDGE_VALUE:
            if(ui->tableView->graphModel()->elementType() == EDGE){
                _updatedElements.set(propEvt->getEdge(),true);
            }
            break;
        default:
            break;
        }
    } else {
        const GraphEvent* graphEvt = dynamic_cast<const GraphEvent*>(&ev);
        if(graphEvt){
            switch(graphEvt->getType()) {
            case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
            case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
            case GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY:
            case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY:{
                string propertyName = graphEvt->getPropertyName();
                //A new selection property was created or the current one will be destructed.
                if(propertyName.compare("viewSelection")==0){
                    if(_selectionProperty != NULL){
                        _selectionProperty->removeListener(this);
                        _selectionProperty = NULL;
                    }
                    _reloadSelectionProperty = true;
                }
            }
                break;
            default:
                break;
            }
        }
    }
}

void SpreadViewTableWidget::treatEvents(const vector<Event> &){
    //Need to reload selection property
    if (_reloadSelectionProperty) {
        assert(_selectionProperty == NULL);
        if(ui->tableView->graph()->existProperty("viewSelection")){
            _selectionProperty = ui->tableView->graph()->getProperty<BooleanProperty>("viewSelection");
            _selectionProperty->addListener(this);
            _updatedElements.setAll(true);
        }
        _reloadSelectionProperty = false;
    }
    updateFilters();
}

void SpreadViewTableWidget::filterElements(){
    filterElements(ui->filterPatternLineEdit->text());
}

void SpreadViewTableWidget::filterElements(const QString&){
    _updatedElements.setAll(true);
    updateFilters();
}

bool SpreadViewTableWidget::displayOnlySelectedElements()const{
    return ui->showOnlySelectedElementsCheckBox->checkState()==Qt::Checked;
}

QRegExp SpreadViewTableWidget::elementValuesFilter()const{
    return QRegExp(ui->filterPatternLineEdit->text());
}

void SpreadViewTableWidget::columnsInserted(const QModelIndex & , int , int ){
    //When adding a column we need to sort all the elements
    modelReset();
}

void SpreadViewTableWidget::columnsDeleted(const QModelIndex &, int , int ){
    //When deleting a column we need to sort all the elements
    modelReset();
}

void SpreadViewTableWidget::rowsInserted(const QModelIndex &, int first, int last){
    GraphTableModel* model = static_cast<GraphTableModel*>(sender());
    for(int i = first ; i <= last ; ++i){
        _updatedElements.set(model->idForIndex(i),true);
    }
}

void SpreadViewTableWidget::modelReset(){
    _updatedElements.setAll(true);
    //Needed to force redraw here as the model can be updated even if the graph structure don't change.
    updateFilters();
}
void SpreadViewTableWidget::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight){
    GraphTableModel* model = static_cast<GraphTableModel*>(sender());
    for(int i = topLeft.row() ; i <= bottomRight.row() ; ++i){
        _updatedElements.set(model->idForIndex(i),true);
    }
    //Needed to force redraw here as the model can be updated even if the graph structure don't change.
    updateFilters();
}

void SpreadViewTableWidget::filterColumnChanged(int){
    _updatedElements.setAll(true);
    updateFilters();
}
