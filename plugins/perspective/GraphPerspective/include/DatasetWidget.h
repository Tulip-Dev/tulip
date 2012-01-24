#ifndef DATASETWIDGET_H
#define DATASETWIDGET_H

#include <QtGui/QWidget>
#include <tulip/Graph.h>
namespace Ui {
class DatasetWidget;
}

namespace tlp {
class GraphHierarchiesModel;
}

class DatasetWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DatasetWidget(QWidget *parent = 0);
    ~DatasetWidget();
    void setModel(tlp::GraphHierarchiesModel * model);
    
private:
    Ui::DatasetWidget *_ui;

private slots:
    void currentGraphChanged(tlp::Graph* newGraph);
    void displayedElementTypeChanged(tlp::ElementType type);
};

#endif // DATASETWIDGET_H
