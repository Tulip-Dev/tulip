#ifndef CSVIMPORTDIALOG_H
#define CSVIMPORTDIALOG_H

#include <tulip/CSVContentHandler.h>
#include <tulip/CSVParser.h>
#include <tulip/CSVGraphImport.h>
#include <QtGui/QWidget>
#include <QtGui/QTableWidget>
#include <string>

namespace Ui {
    class CSVImportDialog;
}

namespace tlp{
    /**
      * @brief Dialog to import CSV data into graph.
      *
      * Allows user to configure all the data import steps from the file selection to the graph mapping.
      **/
class CSVImportDialog : public QDialog {
    Q_OBJECT
public:
    CSVImportDialog(QWidget *parent = 0);
    ~CSVImportDialog();

    /**
      * @brief Set the graph.
      **/
    void setGraph(tlp::Graph* graph);
    tlp::Graph* getGraph()const{
        return graph;
    }

    QSize sizeHint () const;
public slots:
    /**
      * @brief Set the file to open.
      *
      * Use this function is use if you know the file to parse.
      **/
    void setFileToOpen(const QString& fileToOpen);
    /**
      * @brief Import the CSV data into the graph.
      * @return True if there is no error during the import process.
      **/
    bool import();
protected:
    void changeEvent(QEvent *e);
    virtual CSVImportColumnToGraphPropertyMapping* buildColumnToPropertyMapping()const;


protected slots:

    void parsingParametersChanged();
    void importParametersChanged();
    void mappingParametersChanged();
    void checkImportParameters();

private:
    Ui::CSVImportDialog *ui;
    tlp::Graph* graph;
    QPushButton* importButton;
};
}
#endif // CSVIMPORTDIALOG_H
