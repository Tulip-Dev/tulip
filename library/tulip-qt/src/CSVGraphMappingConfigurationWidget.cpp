#include "CSVGraphMappingConfigurationWidget.h"
#include "CSVGraphMappingConfigurationWidgetData.h"
#include "tulip/CSVGraphImport.h"
#include <tulip/Graph.h>
#include <tulip/ForEach.h>
#include <tulip/TlpQtTools.h>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

using namespace tlp;
using namespace std;
CSVColumnToGraphPropertyMappingWidget::CSVColumnToGraphPropertyMappingWidget(QWidget* parent):QWidget(parent),graphPropertyComboBox(NULL),columnComboBox(NULL){
    QGridLayout *gridLayout = new QGridLayout();
    setLayout(gridLayout);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setColumnStretch(0,2);
    gridLayout->setColumnStretch(2,2);
    QLabel * CSVColumnlabel = new QLabel(tr("CSV column"),this);
    gridLayout->addWidget(CSVColumnlabel,0,0);
    columnComboBox = new QComboBox(this);
    gridLayout->addWidget(columnComboBox,1,0);
    QLabel * GraphPropertylabel = new QLabel(tr("Graph property"),this);
    gridLayout->addWidget(GraphPropertylabel,0,2);
    graphPropertyComboBox = new QComboBox(this);
    gridLayout->addWidget(graphPropertyComboBox,1,2);
    QLabel *arrow = new QLabel(tr("->"),this);
    arrow->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    arrow->setPixmap(QPixmap(":/right.png"));
    gridLayout->addWidget(arrow,1,1,Qt::AlignCenter);
}

void CSVColumnToGraphPropertyMappingWidget::setData(Graph* graph,const CSVImportParameters& csvProperties){
    QStringList labels;
    string propertyName;
    forEach(propertyName,graph->getProperties()){
        labels<<tlpStringToQString(propertyName);
    }
    graphPropertyComboBox->clear();
    graphPropertyComboBox->addItems(labels);

    columnComboBox->clear();    
    for(unsigned int i = 0 ; i< csvProperties.columnNumber(); ++i){
        columnComboBox->addItem(tlpStringToQString(csvProperties.getColumnName(i)),QVariant(i));
    }
}

unsigned int CSVColumnToGraphPropertyMappingWidget::getSelectedColumnIndex()const{
    return columnComboBox->itemData(columnComboBox->currentIndex()).toUInt();
}
string CSVColumnToGraphPropertyMappingWidget::getSelectedGraphProperty()const{
    return QStringToTlpString(graphPropertyComboBox->currentText());
}

CSVGraphMappingConfigurationWidget::CSVGraphMappingConfigurationWidget(QWidget *parent) :
        QWidget(parent),graph(NULL),
        ui(new Ui::CSVGraphMappingConfigurationWidget)
{
    ui->setupUi(this);
}

CSVGraphMappingConfigurationWidget::~CSVGraphMappingConfigurationWidget()
{
    delete ui;
}

void CSVGraphMappingConfigurationWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CSVGraphMappingConfigurationWidget::updateWidget(tlp::Graph* graph,const CSVImportParameters& importParameters){
    this->graph = graph;    
    //Init widgets.
    ui->nodeColumnToPropertyMappingwidget->setData(graph,importParameters);
    ui->EdgeSourceMappingwidget->setData(graph,importParameters);
    ui->EdgeTargetMappingwidget->setData(graph,importParameters);
    ui->edgeIdMappingwidget->setData(graph,importParameters);
}

CSVToGraphDataMapping *CSVGraphMappingConfigurationWidget::buildMappingObject(const CSVImportParameters& parameters)const{
    if(ui->mappingTypeTabWidget->currentWidget()==ui->nodesTab){
        //If checked user want to map it's csv column with csv param in graph
        if(ui->useNodeMappingGroupBox->isChecked()){
        return new CSVToGraphNodeIdMapping(graph,ui->nodeColumnToPropertyMappingwidget->getSelectedColumnIndex(),ui->nodeColumnToPropertyMappingwidget->getSelectedGraphProperty(),parameters.getFirstLineIndex(),parameters.getLastLineIndex(),ui->createMissingNodesCheckBox->checkState()==Qt::Checked);
        }else{
            //User d'ont have an id in this csv file create new nodes.
            return new CSVToNewNodeIdMapping(graph);
    }
    }
    else if(ui->mappingTypeTabWidget->currentWidget()==ui->edgesIdTab){
        return new CSVToGraphEdgeIdMapping(graph,ui->edgeIdMappingwidget->getSelectedColumnIndex(),ui->edgeIdMappingwidget->getSelectedGraphProperty(),parameters.getFirstLineIndex(),parameters.getLastLineIndex());
    }
    else if(ui->mappingTypeTabWidget->currentWidget()==ui->edgesSrcAndTgtTab){
        return new CSVToGraphEdgeSrcTgtMapping(graph,ui->EdgeSourceMappingwidget->getSelectedColumnIndex(),ui->EdgeTargetMappingwidget->getSelectedColumnIndex(),ui->EdgeSourceMappingwidget->getSelectedGraphProperty(),ui->EdgeTargetMappingwidget->getSelectedGraphProperty(),parameters.getFirstLineIndex(),parameters.getLastLineIndex());
    }else{
        return NULL;
    }
}
