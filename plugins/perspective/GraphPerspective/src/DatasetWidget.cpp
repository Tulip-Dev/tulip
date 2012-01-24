#include "DatasetWidget.h"
#include "ui_DatasetWidget.h"

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/DataSet.h>

using namespace tlp;

DatasetWidget::DatasetWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::DatasetWidget)
{
    _ui->setupUi(this);
    connect(_ui->propertieswidget,SIGNAL(tableChanged(tlp::ElementType)),SLOT(displayedElementTypeChanged(tlp::ElementType)));
}

DatasetWidget::~DatasetWidget()
{
    delete _ui;
}

void DatasetWidget::setModel(GraphHierarchiesModel * model){
    connect(model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(currentGraphChanged(tlp::Graph*)));
    _ui->hierarchyWidget->setModel(model);
    currentGraphChanged(newGraph());
}

void DatasetWidget::currentGraphChanged(Graph* newGraph){
    _ui->nodesTableWidget->setData(newGraph,DataSet(),NODE);
    _ui->edgesTableWidget->setData(newGraph,DataSet(),EDGE);
    displayedElementTypeChanged(_ui->propertieswidget->displayedType());
}

void DatasetWidget::displayedElementTypeChanged(ElementType type){
    _ui->propertieswidget->setTulipTableWidgetColumnSelectionModel(type==NODE?_ui->nodesTableWidget->columnEditionModel():_ui->edgesTableWidget->columnEditionModel());
    _ui->stackedWidget->setCurrentIndex(type==NODE?0:1);
}
