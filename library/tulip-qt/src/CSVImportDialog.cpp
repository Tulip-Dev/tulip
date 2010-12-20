#include "CSVImportDialog.h"
#include "CSVImportDialogData.h"
#include "tulip/CSVGraphImport.h"
#include <tulip/TlpQtTools.h>
#include "tulip/QtProgress.h"
#include <tulip/CSVParser.h>
#include <iostream>

using namespace tlp;
using namespace std;

CSVImportDialog::CSVImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSVImportDialog),importButton(NULL)
{
    ui->setupUi(this);
    importButton=ui->buttonBox->addButton(tr("Import"), QDialogButtonBox::AcceptRole );
    connect(ui->csvParserConfigurationwidget,SIGNAL(parserChanged()),this,SLOT(parsingParametersChanged()));
    connect(ui->csvConfigurationWidget,SIGNAL(fileInfoChanged()),this,SLOT(importParametersChanged()));
    connect(ui->graphMappingWidget,SIGNAL(mappingChanged()),this,SLOT(mappingParametersChanged()));
    checkImportParameters();
}

CSVImportDialog::~CSVImportDialog()
{
    delete ui;
}

void CSVImportDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void CSVImportDialog::setGraph(Graph* graph){
this->graph = graph;
}

void CSVImportDialog::setFileToOpen(const QString& fileToOpen){
ui->csvParserConfigurationwidget->setFileToOpen(fileToOpen);
}

void CSVImportDialog::parsingParametersChanged(){
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

void CSVImportDialog::importParametersChanged(){
//Get new import parameters.
CSVImportParameters importParam = ui->csvConfigurationWidget->getImportParameters();
//Update data mapping widget.
ui->graphMappingWidget->updateWidget(graph,importParam);
}

void CSVImportDialog::mappingParametersChanged(){
checkImportParameters();
}

CSVImportColumnToGraphPropertyMapping* CSVImportDialog::buildColumnToPropertyMapping()const{
return new CSVImportColumnToGraphPropertyMappingProxy(graph,ui->csvConfigurationWidget->getImportParameters(),const_cast<CSVImportDialog*>(this));
}
bool CSVImportDialog::import(){
if(graph != NULL){
    CSVParser* parser = ui->csvParserConfigurationwidget->buildParser();
    if(parser != NULL){
    CSVImportParameters importParam = ui->csvConfigurationWidget->getImportParameters();
    //Get row to graph element mapping
    CSVToGraphDataMapping *rowMapping = ui->graphMappingWidget->buildMappingObject(importParam);
    //Get column to graph properties mapping
    CSVImportColumnToGraphPropertyMapping *columnMapping = buildColumnToPropertyMapping();
    //Invalid mapping objects
    if(rowMapping== NULL || columnMapping==NULL){
        return false;
    }
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

QSize CSVImportDialog::sizeHint() const{
return QSize(500,700);
}
void CSVImportDialog::checkImportParameters(){
    if(ui->csvParserConfigurationwidget->isValid() && ui->graphMappingWidget->isValid()){
        importButton->setEnabled(true);
    }else{
        importButton->setEnabled(false);
    }
}
