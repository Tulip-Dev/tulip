#ifndef CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H
#define CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H

#include <QWidget>
#include <tulip/tuliphash.h>
#include <tulip/CSVGraphImport.h>
class CSVGraphImport;
class QComboBox;
namespace Ui {
    class CSVGraphMappingConfigurationWidget;
}

namespace tlp{

    class Graph;

    /**
      * @brief Widget used to link a CSV column to a graph property.
      **/
class TLP_QT_SCOPE CSVColumnToGraphPropertyMappingWidget : public QWidget{

public:
    CSVColumnToGraphPropertyMappingWidget(QWidget* parent=NULL);

    /**
      * @brief Fill the graph property list and CSV column combo box. Erase previous selection.
      **/
      void setData(tlp::Graph* graph,const CSVImportParameters& csvProperties);
      /**
        * @brief Return the index of the selected CSV column.
        **/
      unsigned int getSelectedColumnIndex()const;
      /**
        * @brief Return the name of the selected graph property.
        **/
      std::string getSelectedGraphProperty()const;

private:
    QComboBox *graphPropertyComboBox;
    QComboBox *columnComboBox;    
};

/**
  * @brief Widget generating the CSVToGraphDataMapping object.
  *
  * This widget allow user to configure a CSVToGraphDataMapping object. This object is used during the CSV import process to map CSV columns to graph elements like nodes or edges.
  **/
class TLP_QT_SCOPE CSVGraphMappingConfigurationWidget : public QWidget {
    Q_OBJECT
public:
    CSVGraphMappingConfigurationWidget(QWidget *parent = 0);
    ~CSVGraphMappingConfigurationWidget();

    /**
      * @brief Configure the widget with the CSV import parameters.      
      **/
    void updateWidget(tlp::Graph* graph,const CSVImportParameters& importParameters);    
    /**
      * @brief Build the mapping object between the CSV columns and the graph elements.
      **/
    CSVToGraphDataMapping *buildMappingObject(const CSVImportParameters& importParameters)const;


protected:
    void changeEvent(QEvent *e);
    tlp::Graph* graph;

private:
    Ui::CSVGraphMappingConfigurationWidget *ui;
};
}
#endif // CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H
