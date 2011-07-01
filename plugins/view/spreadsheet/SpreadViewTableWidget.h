#ifndef SPREADVIEWTABLEWIDGET_H
#define SPREADVIEWTABLEWIDGET_H

#include <QWidget>
#include <tulip/Graph.h>
#include <tulip/MutableContainer.h>
#include <tulip/ObservableProperty.h>

#include <QtCore/QModelIndex>
#include <QtGui/QMenu>


namespace Ui {
    class SpreadViewTableWidget;
}
class TulipTableWidgetColumnSelectionWidget;
class GraphTableWidget;
class TulipTableWidgetColumnSelectionModel;
class VisibleSectionsModel;

class SpreadViewTableWidget : public QWidget, public tlp::PropertyObserver, public tlp::Observable
{
    Q_OBJECT

public:
    explicit SpreadViewTableWidget(QWidget *parent = 0);
    ~SpreadViewTableWidget();

    void setData(tlp::Graph* graph,const tlp::DataSet& data,tlp::ElementType type);
    tlp::DataSet getData()const;

    TulipTableWidgetColumnSelectionWidget* columnEditionWidget();

    GraphTableWidget* graphTableWidget();

    void update();



    void treatEvent(const tlp::Event &);    

public slots:

    /**
      * @brief Delete the highligted elements from the graph.
      **/
    void deleteHighlightedElements();

private slots:

    //Menu slots
    /**
    * @brief Show property menu
    **/
    void showPropertiesContextMenu(const QPoint& position);
    /**
    * @brief Show the context menu when user click on the table view.
    **/
    void showTableContextMenu(const QPoint& position);
    /**
    * @brief Called when user request the context menu on the elements header view.
    */
    void showElementsContextMenu(const QPoint& position);

    void updateElementVisibility(int state);
    void showOnlySelectedElements(bool show);

    void filterColumnChanged(int index);

    void filterElements();
    void filterElements(const QString& pattern);

    //Columns operations
    void hideColumn();
    void createNewProperties();
    void copyColumnToOther();
    void setAllColumnValues();
    void resetColumn();
    void deleteColumn();

    //Rows operations
    void selectElements();
    void addToSelection();
    void removeFromSelection();
    void highlightElements();

    //Nodes operations
    void copyNodes();
    void group();
    void ungroup();

    //Model observation
    void columnsInserted(const QModelIndex & parent, int first, int last);
    void columnsDeleted(const QModelIndex & parent, int first, int last);
    void rowsInserted(const QModelIndex & parent, int first, int last);
    void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void modelReset();

private:

    void showPropertiesContextMenu(GraphTableWidget* tableWidget,int clickedColumn,const QPoint& position);
    void fillPropertiesContextMenu(QMenu& menu,GraphTableWidget* tableWidget,int clickedColumn);
    /**
      * @brief Execute the context menu for the selected row.
      **/
    void showElementsContextMenu(GraphTableWidget* tableWidget,int clickedRowIndex,const QPoint& position);
    void fillElementsContextMenu(QMenu& menu,GraphTableWidget* tableWidget,int clickedRowIndex);

    void deleteHighlightedElements( const QModelIndexList& elements,GraphTableWidget *tableWidget ,bool delAll);

    bool displayOnlySelectedElements()const;
    QRegExp elementValuesFilter()const;

    void updateSelectionForHighlightedElements(GraphTableWidget* tableWidget,bool select,bool clearOldSelection=false);

    void updateFilters();

    Ui::SpreadViewTableWidget *ui;

    TulipTableWidgetColumnSelectionModel *_tableColumnModel;
    VisibleSectionsModel *_currentVisibleSectionModel;
    tlp::MutableContainer<bool> _updatedElements;
    tlp::BooleanProperty* _selectionProperty;
    bool _reloadSelectionProperty;    
};

#endif // SPREADVIEWTABLEWIDGET_H
