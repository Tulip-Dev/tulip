#ifndef TULIPTABLEWIDGET_H
#define TULIPTABLEWIDGET_H
#include <QtGui/QTableView>
#include <tulip/Graph.h>

class GraphTableModel;
class TulipTableWidget : public QTableView
{
    Q_OBJECT
public:
    TulipTableWidget(QWidget* parent=NULL);
    virtual ~TulipTableWidget(){}

    void setGraph(tlp::Graph* graph,tlp::ElementType element);    

protected:
    void setModel(QAbstractItemModel *){}    

    tlp::Graph* _graph;
    tlp::ElementType _type;
    GraphTableModel* _tulipTableModel;

protected slots:

    void createNewProperties();
    /**
      * @brief Hide the selected columns or the column stored in the data of the action.
      *
      * If there is a selection
      **/
    void hideColumn();
    /**
      * @brief
      **/
    void setAllColumnValues();
    /**
      *
      **/
    void copyColumnToOther();
    /**
      * @brief Set all the column values to the default value.
      **/
    void resetColumn();
    /**
      * @brief Delete the selected columns.
      **/
    void deleteColumn();
    /**
      * @brief Delete the selected elements.
      **/
    void deleteElements();
    void deleteAllElements();

    void doDeleteElements(const QModelIndexList& elements,bool delAll);


    void updateHorizontalHeaderVisibility(bool checked);
    void showElementsContextMenu(const QPoint& position);
    void showPropertiesContextMenu(const QPoint& position);
    void showPropertiesListDialog();


};

#endif // TULIPTABLEWIDGET_H
