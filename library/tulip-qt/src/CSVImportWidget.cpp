#include "tulip/CSVImportWidget.h"
#include "tulip/CSVImportWidgetData.h"
#include "tulip/CSVGraphImport.h"
#include <tulip/TlpQtTools.h>
#include "tulip/QtProgress.h"


#include <tulip/CSVParser.h>
#include <iostream>

using namespace tlp;
using namespace std;



CSVImportWidget::CSVImportWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CSVImportWidget),graph(NULL)
{
    ui->setupUi(this);
    connect(ui->csvParserConfigurationwidget,SIGNAL(parserChanged()),this,SLOT(parsingParametersChanged()));
    connect(ui->csvConfigurationWidget,SIGNAL(fileInfoChanged()),this,SLOT(importParametersChanged()));
}

CSVImportWidget::~CSVImportWidget()
{
    delete ui;
}

void CSVImportWidget::setGraph(Graph* graph){
    this->graph = graph;
}

void CSVImportWidget::changeEvent(QEvent *e)
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


void CSVImportWidget::setFileToOpen(const QString& fileToOpen){
    ui->csvParserConfigurationwidget->setFileToOpen(fileToOpen);
}

void CSVImportWidget::parsingParametersChanged(){        
        CSVParser* parser = ui->csvParserConfigurationwidget->buildParser();
        if(parser!=NULL){
            ui->groupBox_4->setEnabled(true);
            ui->groupBox_3->setEnabled(true);

        //Update import configuration widget.
        ui->csvConfigurationWidget->setNewParser(parser);        
        CSVImportParameters importParam = ui->csvConfigurationWidget->getImportParameters();
        //Update data mapping widget.
        ui->graphMappingWidget->updateWidget(graph,importParam);
    }else{
        ui->groupBox_4->setEnabled(false);
        ui->groupBox_3->setEnabled(false);
    }
}

void CSVImportWidget::importParametersChanged(){    
    //Get new import parameters.
    CSVImportParameters importParam = ui->csvConfigurationWidget->getImportParameters();
    //Update data mapping widget.
    ui->graphMappingWidget->updateWidget(graph,importParam);
}

CSVImportColumnToGraphPropertyMapping* CSVImportWidget::buildColumnToPropertyMapping()const{
    return new CSVImportColumnToGraphPropertyMappingProxy(graph,ui->csvConfigurationWidget->getImportParameters(),const_cast<CSVImportWidget*>(this));
}
bool CSVImportWidget::import(){
    if(graph != NULL){
        CSVParser* parser = ui->csvParserConfigurationwidget->buildParser();
        if(parser != NULL){
        CSVImportParameters importParam = ui->csvConfigurationWidget->getImportParameters();
        //Get row to graph element mapping
        CSVToGraphDataMapping *rowMapping = ui->graphMappingWidget->buildMappingObject(importParam);
        //Get column to graph properties mapping
        CSVImportColumnToGraphPropertyMapping *columnMapping = buildColumnToPropertyMapping();

        //Launch the import process
        QtProgress progress(this,"Importing CSV data on graph");
        progress.setComment("Building index");
        //Build the mapping index
        bool result = parser->parse(rowMapping,&progress);
        if(result){
        //Build import object
        CSVGraphImport csvToGraph(rowMapping,columnMapping,ui->csvConfigurationWidget->getImportParameters());
        progress.setComment("Importing data");
        result = parser->parse(&csvToGraph,&progress);
        }
        //Release objects
        delete rowMapping;
        delete columnMapping;
        delete parser;
        return result;
        }
        return false;
    }
    return true;
}

QSize CSVImportWidget::sizeHint() const{
return QSize(500,700);
}
