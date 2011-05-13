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

    /**
      * @brief Check if all the elements are selected .
      **/
    bool areAllElementsSelected(const QModelIndexList& elements)const;

    /**
      * @brief Convert the index list to ids.
      **/
    std::set<unsigned int> indexListToIds(const QModelIndexList& elementsIndexes)const;

    /**
      * @brief Execute the context menu for the selected row.
      **/
    void showElementContextMenu(int clickedRowIndex,const QPoint& position);

    void keyPressedEvent(QKeyEvent * event);

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
    //////////////////////////////////////////////
    /**
      * @brief Delete the selected elements.
      **/
    void deleteElements();
    void deleteAllElements();
    void doDeleteElements(const QModelIndexList& elements,bool delAll);

    void selectElements();

    void group();
    void ungroup();

    void copyNodes();

    void highlightElements();


    void updateHorizontalHeaderVisibility(bool checked);
    /**
      * @brief Show the context menu when user click on the vertical header view.
      */
    void showHorizontalHeaderViewContextMenu(const QPoint& position);
    /**
      * @brief Show the context menu when user click on the table view.
      **/
    void showTableContextMenu(const QPoint& position);



    void showPropertiesContextMenu(const QPoint& position);
    void showPropertiesListDialog();
    void showTableContextMenu();


};

#endif // TULIPTABLEWIDGET_H
