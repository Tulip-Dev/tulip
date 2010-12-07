#ifndef CSVIMPORTWIDGET_H
#define CSVIMPORTWIDGET_H

#include <tulip/CSVContentHandler.h>
#include <tulip/CSVParser.h>
#include <tulip/CSVGraphImport.h>
#include <QWidget>
#include <QtGui/QTableWidget>
#include <string>
namespace Ui {
    class CSVImportWidget;
}

namespace tlp{
class TLP_QT_SCOPE CSVImportWidget : public QWidget {
    Q_OBJECT
public:
    CSVImportWidget(QWidget *parent = 0);
    ~CSVImportWidget();
    void setGraph(tlp::Graph* graph);
    tlp::Graph* getGraph()const{
        return graph;
    }

    QSize sizeHint () const;
public slots:
    void setFileToOpen(const QString& fileToOpen);
    bool import();
protected:
    void changeEvent(QEvent *e);
    virtual CSVImportColumnToGraphPropertyMapping* buildColumnToPropertyMapping()const;


protected slots:

    void parsingParametersChanged();
    void importParametersChanged();

private:
    Ui::CSVImportWidget *ui;
    tlp::Graph* graph;
};
}
#endif // CSVIMPORTWIDGET_H
