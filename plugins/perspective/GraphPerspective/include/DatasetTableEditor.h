#ifndef DATASETTABLEEDITOR_H
#define DATASETTABLEEDITOR_H

#include <QtGui/QWidget>
#include <tulip/Graph.h>

namespace Ui {
class DatasetTableEditor;
}

class TulipTableWidgetColumnSelectionModel;

class DatasetTableEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit DatasetTableEditor(QWidget *parent = 0);
    ~DatasetTableEditor();

    tlp::ElementType displayedType()const;

    void setTulipTableWidgetColumnSelectionModel(TulipTableWidgetColumnSelectionModel* model);

signals:
    void tableChanged(tlp::ElementType type);
    
private:
    Ui::DatasetTableEditor *_ui;

private slots:
    void menuChanged(const QString& menu);
};

#endif // DATASETTABLEEDITOR_H
