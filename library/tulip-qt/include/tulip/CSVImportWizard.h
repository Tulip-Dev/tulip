#ifndef CSVIMPORTWIZARD_H
#define CSVIMPORTWIZARD_H

#include <QtGui/QWizard>
#include <QtGui/QWizardPage>
#include <QtGui/QTableWidget>
#include <QtGui/QHeaderView>
#include "tulip/CSVParserConfigurationWidget.h"
#include "tulip/CSVImportConfigurationWidget.h"
#include "tulip/CSVGraphMappingConfigurationWidget.h"
namespace Ui {
    class CSVImportWizard;
}
namespace tlp{

    /**
      * @brief QWIzardPage encapsulating a CSVParserConfigurationWidget and a preview.
      **/
    class CSVParsingConfigurationQWizardPage : public QWizardPage{
    Q_OBJECT
public:
    CSVParsingConfigurationQWizardPage ( QWidget * parent = NULL );
    bool isComplete() const;
    CSVParser* buildParser()const;

private:
    void updatePreview();
    CSVParserConfigurationWidget* parserConfigurationWidget;
    CSVTableWidget* previewTableWidget;

private slots:
    void parserChanged();
};

class CSVImportConfigurationQWizardPage : public QWizardPage{
    Q_OBJECT
public:
    CSVImportConfigurationQWizardPage ( QWidget * parent = NULL );
    void initializePage();
    CSVImportParameters getImportParameters()const{
        return importConfigurationWidget->getImportParameters();
    }
private:
    CSVImportConfigurationWidget* importConfigurationWidget;
};

class CSVGraphMappingConfigurationQWizardPage : public QWizardPage{
    Q_OBJECT
public:
    CSVGraphMappingConfigurationQWizardPage ( QWidget * parent = NULL );
    void initializePage();
    bool isComplete() const;
    CSVToGraphDataMapping* buildMappingObject()const{
        return graphMappingConfigurationWidget->buildMappingObject();
    }
private:
    CSVGraphMappingConfigurationWidget* graphMappingConfigurationWidget;
};

class CSVImportWizard : public QWizard
{
    Q_OBJECT

public:
    explicit CSVImportWizard(QWidget *parent = 0);
    ~CSVImportWizard();

    CSVParsingConfigurationQWizardPage* getParsingConfigurationPage()const;
    CSVImportConfigurationQWizardPage* getImportConfigurationPage()const;
    CSVGraphMappingConfigurationQWizardPage* getMappingConfigurationPage()const;

    void setGraph(Graph* graph){
        this->graph = graph;
    }

    Graph* getGraph()const{
        return graph;
    }

public slots:
    void accept();
private:
    Ui::CSVImportWizard *ui;
    tlp::Graph* graph;
};
}
#endif // CSVIMPORTWIZARD_H
