#ifndef CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H
#define CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <tulip/tuliphash.h>
#include <tulip/CSVGraphImport.h>
class CSVGraphImport;

namespace Ui {
    class CSVGraphMappingConfigurationWidget;
}

namespace tlp{

    class Graph;


    /**
      * @brief Combo box used to select a CSV column.
      **/
    class TLP_QT_SCOPE CSVColumnComboBox : public QComboBox{
    public:
        CSVColumnComboBox(QWidget* parent=NULL);
        /**
          * @brief Fill the combobox with the CSV parameters.
          *
          * The first item is not a valid choice it's just a label.
          **/
        void setCsvProperties(const CSVImportParameters& csvProperties);
        /**
          * @brief Get column selected by user or UINT_MAX if no valid column is selected.
          * @return The column selected by the user.
          *
          **/
        unsigned int getSelectedColumnIndex()const;
    };

    /**
      * @brief Combo box for property selection and creation in a graph.
      *
      * If the combo box is editable and if user enter the name of a property that doesn't exists this will create it.
      * A popup will querry user for the type of the property to create.
      * There is no way to delete a created property in this widget.
      **/
    class TLP_QT_SCOPE GraphPropertiesSelectionComboBox : public QComboBox{
        Q_OBJECT
    public:
        GraphPropertiesSelectionComboBox(QWidget* parent=NULL);
        /**
          * @brief Fill the combobox with the properties in the given graph.
          **/
        void setGraph(tlp::Graph*);
        /**
          * @brief return the name of the property selected by the user. If the label is selected an empty string will be returned.
          * @return The name of the selected property or an empty string if no valid property is selected.
          **/
        std::string getSelectedGraphProperty()const;
    protected:

        void keyPressEvent(QKeyEvent *e);
    private:
        tlp::Graph *currentGraph;

    private slots:
        void newGraphPropertySelected(const QString& propertyName);
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
    CSVToGraphDataMapping *buildMappingObject() const;

    /**
      * @brief Check if the values entered by user are valid.
      *
      * If return true you are sure that buildMappingObject never return a NULL object.
      **/
    bool isValid()const;

protected:
    void changeEvent(QEvent *e);
    tlp::Graph* graph;

private:
    CSVImportParameters importParameters;
    Ui::CSVGraphMappingConfigurationWidget *ui;

signals:
    void mappingChanged();
};
}
#endif // CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H
